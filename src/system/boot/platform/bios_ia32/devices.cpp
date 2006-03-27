/*
 * Copyright 2003-2006, Axel Dörfler, axeld@pinc-software.de.
 * Distributed under the terms of the MIT License.
 */


#include "bios.h"

#include <KernelExport.h>
#include <boot/platform.h>
#include <boot/partitions.h>
#include <boot/stdio.h>
#include <boot/stage2.h>

#include <string.h>

#define TRACE_DEVICES
#ifdef TRACE_DEVICES
#	define TRACE(x) dprintf x
#else
#	define TRACE(x) ;
#endif


// exported from shell.S
extern uint8 gBootedFromImage;
extern uint8 gBootDriveID;
extern uint32 gBootPartitionOffset;

// int 0x13 definitions
#define BIOS_RESET_DISK_SYSTEM			0x0000
#define BIOS_READ						0x0200
#define BIOS_GET_DRIVE_PARAMETERS		0x0800
#define BIOS_IS_EXT_PRESENT				0x4100
#define BIOS_EXT_READ					0x4200
#define BIOS_GET_EXT_DRIVE_PARAMETERS	0x4800
#define BIOS_BOOT_CD_GET_STATUS			0x4b01

struct real_addr {
	uint16	offset;
	uint16	segment;
};

struct disk_address_packet {
	uint8		size;
	uint8		reserved;
	uint16		number_of_blocks;
	uint32		buffer;
	uint64		lba;
	uint64		flat_buffer;
};

static const uint16 kParametersSizeVersion1 = 0x1a;
static const uint16 kParametersSizeVersion2 = 0x1e;
static const uint16 kParametersSizeVersion3 = 0x42;

static const uint16 kDevicePathSignature = 0xbedd;

struct drive_parameters {
	uint16		parameters_size;
	uint16		flags;
	uint32		cylinders;
	uint32		heads;
	uint32		sectors_per_track;
	uint64		sectors;
	uint16		bytes_per_sector;
	/* edd 2.0 */
	real_addr	device_table;
	/* edd 3.0 */
	uint16		device_path_signature;
	uint8		device_path_size;
	uint8		reserved1[3];
	char		host_bus[4];
	char		interface_type[8];
	union {
		struct {
			uint16	base_address;
		} legacy;
		struct {
			uint8	bus;
			uint8	slot;
			uint8	function;
		} pci;
		uint8		reserved[8];
	} interface;
	union {
		struct {
			uint8	slave;
		} ata;
		struct {
			uint8	slave;
			uint8	logical_unit;
		} atapi;
		struct {
			uint8	logical_unit;
		} scsi;
		struct {
			uint8	tbd;
		} usb;
		struct {
			uint64	guid;
		} firewire;
		struct {
			uint64	wwd;
		} fibre;
	} device;
	uint8		reserved2;
	uint8		checksum;
} _PACKED;

struct device_table {
	uint16	base_address;
	uint16	control_port_address;
	uint8	_reserved1 : 4;
	uint8	is_slave : 1;
	uint8	_reserved2 : 1;
	uint8	lba_enabled : 1;
} _PACKED;

struct specification_packet {
	uint8	size;
	uint8	media_type;
	uint8	drive_number;
	uint8	controller_index;
	uint32	start_emulation;
	uint16	device_specification;
	uint8	_more_[9];
} _PACKED;

class BIOSDrive : public Node {
	public:
		BIOSDrive(uint8 driveID);
		virtual ~BIOSDrive();

		status_t InitCheck() const;

		virtual ssize_t ReadAt(void *cookie, off_t pos, void *buffer, size_t bufferSize);
		virtual ssize_t WriteAt(void *cookie, off_t pos, const void *buffer, size_t bufferSize);

		virtual off_t Size() const;

		uint32 BlockSize() const { return fBlockSize; }

		status_t FillIdentifier();

		bool HasParameters() const { return fHasParameters; }
		const drive_parameters &Parameters() const { return fParameters; }

		disk_identifier &Identifier() { return fIdentifier; }
		uint8 DriveID() const { return fDriveID; }

	protected:
		uint8	fDriveID;
		bool	fLBA;
		uint64	fSize;
		uint32	fBlockSize;
		bool	fHasParameters;
		drive_parameters fParameters;
		disk_identifier fIdentifier;
};


static void
check_cd_boot(BIOSDrive *drive)
{
	gKernelArgs.boot_disk.cd = false;

	if (drive->DriveID() != 0)
		return;

	struct bios_regs regs;
	regs.eax = BIOS_BOOT_CD_GET_STATUS;
	regs.edx = 0;
	regs.esi = kDataSegmentScratch;
	call_bios(0x13, &regs);

	if ((regs.flags & CARRY_FLAG) != 0)
		return;

	// we obviously were booted from CD!

	specification_packet *packet = (specification_packet *)kDataSegmentScratch;
	gKernelArgs.boot_disk.cd = packet->media_type != 0;

#if 0
	dprintf("got CD boot spec:\n");
	dprintf("  size: %#x\n", packet->size);
	dprintf("  media type: %u\n", packet->media_type);
	dprintf("  drive_number: %u\n", packet->drive_number);
	dprintf("  controller index: %u\n", packet->controller_index);
	dprintf("  start emulation: %lu\n", packet->start_emulation);
	dprintf("  device_specification: %u\n", packet->device_specification);
#endif
}


static status_t
get_ext_drive_parameters(uint8 drive, drive_parameters *targetParameters)
{
	drive_parameters *parameter = (drive_parameters *)kDataSegmentScratch;

	memset(parameter, 0, sizeof(drive_parameters));
	parameter->parameters_size = sizeof(drive_parameters);

	struct bios_regs regs;
	regs.eax = BIOS_GET_EXT_DRIVE_PARAMETERS;
	regs.edx = drive;
	regs.esi = (addr_t)parameter - kDataSegmentBase;
	call_bios(0x13, &regs);

	// filter out faulty BIOS return codes
	if ((regs.flags & CARRY_FLAG) != 0
		|| parameter->sectors == 0)
		return B_ERROR;

	memcpy(targetParameters, parameter, sizeof(drive_parameters));
	return B_OK;
}


static status_t
get_drive_parameters(uint8 drive, drive_parameters *parameters)
{
	struct bios_regs regs;
	regs.eax = BIOS_GET_DRIVE_PARAMETERS;
	regs.edx = drive;
	regs.es = 0;
	regs.edi = 0;	// guard against faulty BIOS, see Ralf Brown's interrupt list
	call_bios(0x13, &regs);

	if ((regs.flags & CARRY_FLAG) != 0 || (regs.ecx & 0x3f) == 0)
		return B_ERROR;

	// fill drive_parameters structure with useful values
	parameters->parameters_size = kParametersSizeVersion1;
	parameters->flags = 0;
	parameters->cylinders = (((regs.ecx & 0xc0) << 2) | ((regs.ecx >> 8) & 0xff)) + 1;
	parameters->heads = ((regs.edx >> 8) & 0xff) + 1;
		// heads and cylinders start counting from 0
	parameters->sectors_per_track = regs.ecx & 0x3f;
	parameters->sectors = parameters->cylinders * parameters->heads
		* parameters->sectors_per_track;
	parameters->bytes_per_sector = 512;

	return B_OK;
}


static status_t
get_number_of_drives(uint8 *_count)
{
	struct bios_regs regs;
	regs.eax = BIOS_GET_DRIVE_PARAMETERS;
	regs.edx = 0x80;
	regs.es = 0;
	regs.edi = 0;
	call_bios(0x13, &regs);

	if (regs.flags & CARRY_FLAG)
		return B_ERROR;

	*_count = regs.edx & 0xff;
	return B_OK;
}


/** parse EDD 3.0 drive path information */

static status_t
fill_disk_identifier_v3(disk_identifier &disk, const drive_parameters &parameters)
{
	if (parameters.parameters_size < kParametersSizeVersion3
		|| parameters.device_path_signature != kDevicePathSignature)
		return B_BAD_TYPE;

	// parse host bus

	if (!strncmp(parameters.host_bus, "PCI", 3)) {
		disk.bus_type = PCI_BUS;

		disk.bus.pci.bus = parameters.interface.pci.bus;
		disk.bus.pci.slot = parameters.interface.pci.slot;
		disk.bus.pci.function = parameters.interface.pci.function;
	} else if (!strncmp(parameters.host_bus, "ISA", 3)) {
		disk.bus_type = LEGACY_BUS;

		disk.bus.legacy.base_address = parameters.interface.legacy.base_address;
		dprintf("legacy base address %x\n", disk.bus.legacy.base_address);
	} else {
		dprintf("unknown host bus \"%s\"\n", parameters.host_bus);
		return B_BAD_DATA;
	}

	// parse interface

	if (!strncmp(parameters.interface_type, "ATA", 3)) {
		disk.device_type = ATA_DEVICE;
		disk.device.ata.master = !parameters.device.ata.slave;
		dprintf("ATA device, %s\n", disk.device.ata.master ? "master" : "slave");
	} else if (!strncmp(parameters.interface_type, "ATAPI", 3)) {
		disk.device_type = ATAPI_DEVICE;
		disk.device.atapi.master = !parameters.device.ata.slave;
		disk.device.atapi.logical_unit = parameters.device.atapi.logical_unit;
	} else if (!strncmp(parameters.interface_type, "SCSI", 3)) {
		disk.device_type = SCSI_DEVICE;
		disk.device.scsi.logical_unit = parameters.device.scsi.logical_unit;
	} else if (!strncmp(parameters.interface_type, "USB", 3)) {
		disk.device_type = USB_DEVICE;
		disk.device.usb.tbd = parameters.device.usb.tbd;
	} else if (!strncmp(parameters.interface_type, "1394", 3)) {
		disk.device_type = FIREWIRE_DEVICE;
		disk.device.firewire.guid = parameters.device.firewire.guid;
	} else if (!strncmp(parameters.interface_type, "FIBRE", 3)) {
		disk.device_type = FIBRE_DEVICE;
		disk.device.fibre.wwd = parameters.device.fibre.wwd;
	} else {
		dprintf("unknown interface type \"%s\"\n", parameters.interface_type);
		return B_BAD_DATA;
	}

	return B_OK;
}


/** EDD 2.0 drive table information */

static status_t
fill_disk_identifier_v2(disk_identifier &disk, const drive_parameters &parameters)
{
	if (parameters.device_table.segment == 0xffff
		&& parameters.device_table.offset == 0xffff)
		return B_BAD_TYPE;

	device_table *table = (device_table *)LINEAR_ADDRESS(parameters.device_table.segment,
		parameters.device_table.offset);

	disk.bus_type = LEGACY_BUS;
	disk.bus.legacy.base_address = table->base_address;

	disk.device_type = ATA_DEVICE;
	disk.device.ata.master = !table->is_slave;

	return B_OK;
}


static off_t
get_next_check_sum_offset(int32 index, off_t maxSize)
{
	// The boot block often contains the disk super block, and should be
	// unique enough for most cases
	if (index < 2)
		return index * 512;

	// Try some data in the first part of the drive
	if (index < 4)
		return (maxSize >> 10) + index * 2048;

	// Some random value might do
	return ((system_time() + index) % (maxSize >> 9)) * 512;
}


/**	Computes a check sum for the specified block.
 *	The check sum is the sum of all data in that block interpreted as an
 *	array of uint32 values.
 */

static uint32
compute_check_sum(BIOSDrive *drive, off_t offset)
{
	char buffer[512];
	ssize_t bytesRead = drive->ReadAt(NULL, offset, buffer, sizeof(buffer));
	if (bytesRead < B_OK)
		return 0;

	if (bytesRead < (ssize_t)sizeof(buffer))
		memset(buffer + bytesRead, 0, sizeof(buffer) - bytesRead);

	uint32 *array = (uint32 *)buffer;
	uint32 sum = 0;

	for (uint32 i = 0; i < (bytesRead + sizeof(uint32) - 1) / sizeof(uint32); i++) {
		sum += array[i];
	}

	return sum;
}


static void
find_unique_check_sums(NodeList *devices)
{
	NodeIterator iterator = devices->GetIterator();
	Node *device;
	int32 index = 0;
	off_t minSize = 0;
	const int32 kMaxTries = 200;

	while (index < kMaxTries) {
		bool clash = false;

		iterator.Rewind();

		while ((device = iterator.Next()) != NULL) {
			BIOSDrive *drive = (BIOSDrive *)device;
#if 0
			// there is no RTTI in the boot loader...
			BIOSDrive *drive = dynamic_cast<BIOSDrive *>(device);
			if (drive == NULL)
				continue;
#endif

			// TODO: currently, we assume that the BIOS provided us with unique
			//	disk identifiers... hopefully this is a good idea
			if (drive->Identifier().device_type != UNKNOWN_DEVICE)
				continue;

			if (minSize == 0 || drive->Size() < minSize)
				minSize = drive->Size();

			// check for clashes

			NodeIterator compareIterator = devices->GetIterator();
			while ((device = compareIterator.Next()) != NULL) {
				BIOSDrive *compareDrive = (BIOSDrive *)device;

				if (compareDrive == drive
					|| compareDrive->Identifier().device_type != UNKNOWN_DEVICE)
					continue;

				if (!memcmp(&drive->Identifier(), &compareDrive->Identifier(),
						sizeof(disk_identifier))) {
					clash = true;
					break;
				}
			}

			if (clash)
				break;
		}

		if (!clash) {
			// our work here is done.
			return;
		}

		// add a new block to the check sums

		off_t offset = get_next_check_sum_offset(index, minSize);
		int32 i = index % NUM_DISK_CHECK_SUMS;
		iterator.Rewind();

		while ((device = iterator.Next()) != NULL) {
			BIOSDrive *drive = (BIOSDrive *)device;

			disk_identifier& disk = drive->Identifier();
			disk.device.unknown.check_sums[i].offset = offset;
			disk.device.unknown.check_sums[i].sum = compute_check_sum(drive, offset);

			TRACE(("disk %x, offset %Ld, sum %lu\n", drive->DriveID(), offset,
				disk.device.unknown.check_sums[i].sum));
		}

		index++;
	}

	// If we get here, we couldn't find a way to differentiate all disks from each other.
	// It's very likely that one disk is an exact copy of the other, so there is nothing
	// we could do, anyway.

	dprintf("Could not make BIOS drives unique! Might boot from the wrong disk...\n");
}


#define DUMPED_BLOCK_SIZE 16

void
dumpBlock(const char *buffer, int size, const char *prefix)
{
	int i;
	
	for (i = 0; i < size;) {
		int start = i;

		dprintf(prefix);
		for (; i < start+DUMPED_BLOCK_SIZE; i++) {
			if (!(i % 4))
				dprintf(" ");

			if (i >= size)
				dprintf("  ");
			else
				dprintf("%02x", *(unsigned char *)(buffer + i));
		}
		dprintf("  ");

		for (i = start; i < start + DUMPED_BLOCK_SIZE; i++) {
			if (i < size) {
				char c = buffer[i];

				if (c < 30)
					dprintf(".");
				else
					dprintf("%c", c);
			} else
				break;
		}
		dprintf("\n");
	}
}


//	#pragma mark -


BIOSDrive::BIOSDrive(uint8 driveID)
	:
	fDriveID(driveID)
{
	TRACE(("drive ID %u\n", driveID));

	if (get_ext_drive_parameters(driveID, &fParameters) != B_OK) {
		// old style CHS support

		if (get_drive_parameters(driveID, &fParameters) != B_OK) {
			dprintf("getting drive parameters for: %u failed!\n", fDriveID);
			return;
		}

		TRACE(("  cylinders: %lu, heads: %lu, sectors: %lu, bytes_per_sector: %u\n",
			fParameters.cylinders, fParameters.heads, fParameters.sectors_per_track,
			fParameters.bytes_per_sector));
		TRACE(("  total sectors: %Ld\n", fParameters.sectors));

		fBlockSize = 512;
		fSize = fParameters.sectors * fBlockSize;
		fLBA = false;
		fHasParameters = false;
	} else {
		TRACE(("size: %x\n", fParameters.parameters_size));
		TRACE(("drive_path_signature: %x\n", fParameters.device_path_signature));
		TRACE(("host bus: \"%s\", interface: \"%s\"\n", fParameters.host_bus,
			fParameters.interface_type));
		TRACE(("cylinders: %lu, heads: %lu, sectors: %lu, bytes_per_sector: %u\n",
			fParameters.cylinders, fParameters.heads, fParameters.sectors_per_track,
			fParameters.bytes_per_sector));
		TRACE(("total sectors: %Ld\n", fParameters.sectors));

		fBlockSize = fParameters.bytes_per_sector;
		fSize = fParameters.sectors * fBlockSize;
		fLBA = true;
		fHasParameters = true;
	}
}


BIOSDrive::~BIOSDrive()
{
}


status_t 
BIOSDrive::InitCheck() const
{
	return fSize > 0 ? B_OK : B_ERROR;
}


ssize_t 
BIOSDrive::ReadAt(void *cookie, off_t pos, void *buffer, size_t bufferSize)
{
	uint32 offset = pos % fBlockSize;
	pos /= fBlockSize;

	uint32 blocksLeft = (bufferSize + offset + fBlockSize - 1) / fBlockSize;
	int32 totalBytesRead = 0;

	//TRACE(("BIOS reads %lu bytes from %Ld (offset = %lu), drive %u\n",
	//	blocksLeft * fBlockSize, pos * fBlockSize, offset, fDriveID));

	uint32 scratchSize = 24 * 1024 / fBlockSize;
		// maximum value allowed by Phoenix BIOS is 0x7f

	while (blocksLeft > 0) {
		uint32 blocksRead = blocksLeft;
		if (blocksRead > scratchSize)
			blocksRead = scratchSize;

		if (fLBA) {
			struct disk_address_packet *packet = (disk_address_packet *)kDataSegmentScratch;
			memset(packet, 0, sizeof(disk_address_packet));

			packet->size = sizeof(disk_address_packet);
			packet->number_of_blocks = blocksRead;
			packet->buffer = kExtraSegmentScratch;
			packet->lba = pos;

			struct bios_regs regs;
			regs.eax = BIOS_EXT_READ;
			regs.edx = fDriveID;
			regs.esi = (addr_t)packet - kDataSegmentBase;
			call_bios(0x13, &regs);

			if (regs.flags & CARRY_FLAG)
				goto chs_read;
		} else {
	chs_read:
			// Old style CHS read routine

			// We can only read up to 64 kB this way, but since scratchSize
			// is actually lower than this value, we don't have to take care
			// of this here.

			uint32 sector = pos % fParameters.sectors_per_track + 1;
				// sectors start countint at 1 (unlike head and cylinder)
			uint32 head = pos / fParameters.sectors_per_track;
			uint32 cylinder = head / fParameters.heads;
			head %= fParameters.heads;

			if (cylinder >= fParameters.cylinders)
				return B_BAD_VALUE;

			// try to read from the device more than once, just to make sure it'll work
			struct bios_regs regs;
			int32 tries = 3;

			while (tries-- > 0) {
				regs.eax = BIOS_READ | blocksRead;
				regs.edx = fDriveID | (head << 8);
				regs.ecx = sector | ((cylinder >> 2) & 0xc0) | ((cylinder & 0xff) << 8);
				regs.es = 0;
				regs.ebx = kExtraSegmentScratch;
				call_bios(0x13, &regs);

				if ((regs.flags & CARRY_FLAG) == 0)
					break;

				if (tries < 2) {
					// reset disk system
					regs.eax = BIOS_RESET_DISK_SYSTEM;
					regs.edx = fDriveID;
					call_bios(0x13, &regs);
				}
	
				// wait a bit between the retries (1/20 sec)
				spin(50000);
			}

			if (regs.flags & CARRY_FLAG) {
				dprintf("reading %ld bytes from drive %u failed at %Ld\n",
					blocksRead, fDriveID, pos);
				return B_ERROR;
			}
		}

		uint32 bytesRead = fBlockSize * blocksRead - offset;
		// copy no more than bufferSize bytes
		if (bytesRead > bufferSize)
			bytesRead = bufferSize;

		memcpy(buffer, (void *)(kExtraSegmentScratch + offset), bytesRead);
		pos += blocksRead;
		offset = 0;
		blocksLeft -= blocksRead;
		bufferSize -= bytesRead;
		buffer = (void *)((addr_t)buffer + bytesRead);
		totalBytesRead += bytesRead;
	}

	return totalBytesRead;
}


ssize_t 
BIOSDrive::WriteAt(void *cookie, off_t pos, const void *buffer, size_t bufferSize)
{
	// we don't have to know how to write
	return B_NOT_ALLOWED;
}


off_t 
BIOSDrive::Size() const
{
	return fSize;
}


status_t
BIOSDrive::FillIdentifier()
{
	if (HasParameters()) {
		// try all drive_parameters versions, beginning from the most informative

		if (fill_disk_identifier_v3(fIdentifier, fParameters) == B_OK)
			return B_OK;

		if (fill_disk_identifier_v2(fIdentifier, fParameters) == B_OK)
			return B_OK;

		// no interesting information, we have to fall back to the default
		// unknown interface/device type identifier
	}

	fIdentifier.bus_type = UNKNOWN_BUS;
	fIdentifier.device_type = UNKNOWN_DEVICE;
	fIdentifier.device.unknown.size = Size();

	for (int32 i = 0; i < NUM_DISK_CHECK_SUMS; i++) {
		fIdentifier.device.unknown.check_sums[i].offset = -1;
		fIdentifier.device.unknown.check_sums[i].sum = 0;
	}

	return B_ERROR;
}


//	#pragma mark -


status_t 
platform_get_boot_device(struct stage2_args *args, Node **_device)
{
	TRACE(("boot drive ID: %x\n", gBootDriveID));

	BIOSDrive *drive = new BIOSDrive(gBootDriveID);
	if (drive->InitCheck() != B_OK) {
		dprintf("no boot drive!\n");
		return B_ERROR;
	}

	if (drive->FillIdentifier() != B_OK) {
		// TODO: we need to add all block devices!
	}

	TRACE(("drive size: %Ld bytes\n", drive->Size()));
	gKernelArgs.boot_disk.booted_from_image = gBootedFromImage;

	*_device = drive;
	return B_OK;
}


status_t
platform_get_boot_partition(struct stage2_args *args, Node *bootDevice,
	NodeList *list, boot::Partition **_partition)
{
	BIOSDrive *drive = static_cast<BIOSDrive *>(bootDevice);
	off_t offset = (off_t)gBootPartitionOffset * drive->BlockSize();

	dprintf("boot partition offset: %Ld\n", offset);

	NodeIterator iterator = list->GetIterator();
	boot::Partition *partition = NULL;
	while ((partition = (boot::Partition *)iterator.Next()) != NULL) {
		TRACE(("partition offset = %Ld, size = %Ld\n", partition->offset, partition->size));
		// search for the partition that contains the partition
		// offset as reported by the BFS boot block
		if (offset >= partition->offset
			&& offset < partition->offset + partition->size) {
			*_partition = partition;
			return B_OK;
		}
	}

	return B_ENTRY_NOT_FOUND;
}


status_t
platform_add_block_devices(stage2_args *args, NodeList *devicesList)
{
	uint8 driveCount;
	if (get_number_of_drives(&driveCount) != B_OK)
		return B_ERROR;

	dprintf("number of drives: %d\n", driveCount);

	bool identifierMissing = false;

	for (int32 i = 0; i < driveCount; i++) {
		uint8 driveID = i + 0x80;
		if (driveID == gBootDriveID)
			continue;

		BIOSDrive *drive = new BIOSDrive(driveID);
		if (drive->InitCheck() != B_OK) {
			dprintf("could not add drive %u\n", driveID);
			delete drive;
			continue;
		}

		devicesList->Add(drive);

		if (drive->FillIdentifier() != B_OK)
			identifierMissing = true;
	}

	if (identifierMissing) {
		// we cannot distinguish between all drives by identifier, we need
		// compute checksums for them
		find_unique_check_sums(devicesList);
	}

	return B_OK; 
}


status_t 
platform_register_boot_device(Node *device)
{
	BIOSDrive *drive = (BIOSDrive *)device;

	check_cd_boot(drive);

	gKernelArgs.platform_args.boot_drive_number = drive->DriveID();
	gKernelArgs.boot_disk.identifier = drive->Identifier();

	return B_OK;
}

