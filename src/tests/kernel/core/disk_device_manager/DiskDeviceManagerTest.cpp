//----------------------------------------------------------------------
//  This software is part of the OpenBeOS distribution and is covered 
//  by the OpenBeOS license.
//---------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include <OS.h>

#include <KDiskDevice.h>
#include <KDiskDeviceManager.h>
#include <KDiskDeviceUtils.h>

const char *kTestFileDevice = "/boot/home/tmp/test-file-device";

// main
int
main()
{
	KDiskDeviceManager::CreateDefault();
	KDiskDeviceManager *manager = KDiskDeviceManager::Default();
	manager->InitialDeviceScan();
	partition_id id = manager->CreateFileDevice(kTestFileDevice);
	if (id < B_OK)
		printf("creating the file device failed: %s\n", strerror(id));
	// wait for scanning jobs to finish
	for (;;) {
		if (ManagerLocker locker = manager) {
			if (manager->CountJobs() == 0)
				break;
		}
	}
	// print devices
	for (int32 cookie = 0;
		 KDiskDevice *device = manager->RegisterNextDevice(&cookie); ) {
		PartitionRegistrar _(device, true);
		if (DeviceReadLocker locker = device) {
			device->Dump();
			printf("\n");
		}
	}

//	error = manager->DeleteFileDevice(kTestFileDevice);
//	if (error != B_OK)
//		printf("deleting the file device failed: %s\n", strerror(error));


	KDiskDeviceManager::DeleteDefault();
	return 0;
}

