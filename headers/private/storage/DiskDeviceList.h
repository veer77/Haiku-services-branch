//----------------------------------------------------------------------
//  This software is part of the OpenBeOS distribution and is covered 
//  by the OpenBeOS license.
//---------------------------------------------------------------------

#ifndef _DISK_DEVICE_LIST_H
#define _DISK_DEVICE_LIST_H

#include <DiskDeviceVisitor.h>
#include <Handler.h>
#include <ObjectList.h>

class BDiskDevice;
class BPartition;
class BSession;

class BDiskDeviceList : public BHandler {
public:
	BDiskDeviceList(bool useOwnLocker);
	virtual ~BDiskDeviceList();

	virtual void MessageReceived(BMessage *message);
	virtual void SetNextHandler(BHandler *handler);

	status_t Fetch();
	void Unset();

	bool Lock();
	void Unlock();

	uint32 CountDevices() const;
	BDiskDevice *DeviceAt(uint32 index) const;

	BDiskDevice *VisitEachDevice(BDiskDeviceVisitor *visitor);
	BPartition *VisitEachPartition(BDiskDeviceVisitor *visitor);
	bool Traverse(BDiskDeviceVisitor *visitor);

	BPartition *VisitEachMountedPartition(BDiskDeviceVisitor *visitor);
	BPartition *VisitEachMountablePartition(BDiskDeviceVisitor *visitor);
	BPartition *VisitEachInitializablePartition(BDiskDeviceVisitor *visitor);

	BDiskDevice *DeviceWithID(uint32 id) const;
	BPartition *PartitionWithID(uint32 id) const;

	virtual void MountPointMoved(BPartition *partition);
	virtual void PartitionMounted(BPartition *partition);
	virtual void PartitionUnmounted(BPartition *partition);
	virtual void PartitionResized(BPartition *partition);
	virtual void PartitionMoved(BPartition *partition);
	virtual void PartitionCreated(BPartition *partition);
	virtual void PartitionDeleted(BPartition *partition);
	virtual void PartitionDefragmented(BPartition *partition);
	virtual void PartitionRepaired(BPartition *partition);
	virtual void MediaChanged(BDiskDevice *device);
	virtual void DeviceAdded(BDiskDevice *device);
	virtual void DeviceRemoved(BDiskDevice *device);

private:
	void _MountPointMoved(BMessage *message);
	void _PartitionMounted(BMessage *message);
	void _PartitionUnmounted(BMessage *message);
	void _PartitionInitialized(BMessage *message);
	void _PartitionResized(BMessage *message);
	void _PartitionMoved(BMessage *message);
	void _PartitionCreated(BMessage *message);
	void _PartitionDeleted(BMessage *message);
	void _PartitionDefragmented(BMessage *message);
	void _PartitionRepaired(BMessage *message);
	void _MediaChanged(BMessage *message);
	void _DeviceAdded(BMessage *message);
	void _DeviceRemoved(BMessage *message);

	BDiskDevice *_FindDevice(BMessage *message);
	BPartition *_FindPartition(BMessage *message);

	BDiskDevice *_UpdateDevice(BMessage *message);

private:
	BLocker						*fLocker;
	BObjectList<BDiskDevice>	fDevices;
	bool						fSubscribed;
};

#endif	// _DISK_DEVICE_LIST_H
