// ddm_userland_interface.h

#ifndef _DISK_DEVICE_MANAGER_USERLAND_INTERFACE_H
#define _DISK_DEVICE_MANAGER_USERLAND_INTERFACE_H

#include <DiskDeviceDefs.h>
#include <OS.h>

#ifdef __cplusplus
extern "C" {
#endif

// userland partition representation
struct user_partition_data {
	partition_id		id;
	partition_id		shadow_id;
	off_t				offset;
	off_t				size;
	uint32				block_size;
	uint32				status;
	uint32				flags;
	dev_t				volume;
	int32				index;
	int32				change_counter;	// needed?
	disk_system_id		disk_system;
	char				*name;
	char				*content_name;
	char				*type;
	char				*content_type;
	char				*parameters;
	char				*content_parameters;
	void				*user_data;
	int32				child_count;
	user_partition_data	*children[1];
};

// userland disk device representation
struct user_disk_device_data {
	uint32				device_flags;
	char				*path;
	user_partition_data	device_partition_data;
};

// userland partitionable space representation
struct user_partitionable_space_data {
	off_t	offset;
	off_t	size;
};

// userland partitionable space representation
struct user_disk_system_info {
	disk_system_id	id;
	char			name[B_FILE_NAME_LENGTH];	// better B_PATH_NAME_LENGTH?
	char			pretty_name[B_OS_NAME_LENGTH];
	bool			file_system;
};

// userland disk device job representation
struct user_disk_device_job_info {
	disk_job_id		id;
	uint32			type;
	partition_id	partition;
	char			desription[256];
};

// iterating, retrieving device/partition data
partition_id _kern_get_next_disk_device_id(int32 *cookie,
										   size_t *neededSize = NULL);
partition_id _kern_find_disk_device(const char *filename,
									size_t *neededSize = NULL);
partition_id _kern_find_partition(const char *filename,
								  size_t *neededSize = NULL);
status_t _kern_get_disk_device_data(partition_id deviceID, bool deviceOnly,
									bool shadow, user_disk_device_data *buffer,
									size_t bufferSize, size_t *neededSize);
status_t _kern_get_partition_data(partition_id partitionID, bool shadow,
								  user_partition_data *buffer,
								  size_t bufferSize, size_t *neededSize);
	// Dangerous?!
status_t _kern_get_partitionable_spaces(partition_id partitionID, bool shadow,
										user_partitionable_space_data *buffer,
										size_t bufferSize, size_t *neededSize);
	// Pass the partition change counter? If GetPartitionInfo() is only
	// allowed, when the device is locked, then we wouldn't need it.

partition_id _kern_register_file_device(const char *filename);
status_t _kern_unregister_file_device(partition_id deviceID,
									  const char *filename);
	// Only a valid deviceID or filename need to be passed. The other one
	// is -1/NULL. If both is given only filename is ignored.

// disk systems
status_t _kern_get_disk_system_info(disk_system_id id,
									user_disk_system_info *info);
status_t _kern_get_next_disk_system_info(int32 *cookie,
										 user_disk_system_info *info);
status_t _kern_find_disk_system(const char *name, user_disk_system_info *info);

bool _kern_supports_defragmenting_partition(disk_system_id diskSystemID,
											partition_id partitionID,
											bool *whileMounted);
bool _kern_supports_repairing_partition(disk_system_id diskSystemID,
										partition_id partitionID,
										bool checkOnly, bool *whileMounted);
bool _kern_supports_resizing_partition(disk_system_id diskSystemID,
									   partition_id partitionID,
									   bool *whileMounted);
bool _kern_supports_resizing_child_partition(disk_system_id diskSystemID,
											 partition_id partitionID);
bool _kern_supports_moving_partition(disk_system_id diskSystemID,
									 partition_id partitionID,
									 bool *whileMounted);
bool _kern_supports_moving_child_partition(disk_system_id diskSystemID,
										   partition_id partitionID);
bool _kern_supports_setting_partition_name(disk_system_id diskSystemID,
										   partition_id partitionID);
bool _kern_supports_setting_partition_content_name(disk_system_id diskSystemID,
												   partition_id partitionID,
												   bool *whileMounted);
bool _kern_supports_setting_partition_type(disk_system_id diskSystemID,
										   partition_id partitionID);
bool _kern_supports_creating_child_partition(disk_system_id diskSystemID,
											 partition_id partitionID);
bool _kern_supports_deleting_child_partition(disk_system_id diskSystemID,
											 partition_id partitionID);
bool _kern_supports_initializing_partition(disk_system_id diskSystemID,
										   partition_id partitionID);
bool _kern_supports_initializing_child_partition(disk_system_id diskSystemID,
												 partition_id partitionID,
												 const char *childSystem);
bool _kern_is_sub_disk_system_for(disk_system_id diskSystemID,
								  partition_id partitionID);

status_t _kern_validate_resize_partition(disk_system_id diskSystemID,
										 partition_id partitionID,
										 off_t *size);
status_t _kern_validate_resize_child_partition(disk_system_id diskSystemID,
											   partition_id partitionID,
											   off_t *size);
status_t _kern_validate_move_partition(disk_system_id diskSystemID,
									   partition_id partitionID,
									   off_t *offset);
status_t _kern_validate_move_child_partition(disk_system_id diskSystemID,
											 partition_id partitionID,
											 off_t *offset);
status_t _kern_validate_set_partition_name(disk_system_id diskSystemID,
										   partition_id partitionID,
										   char *name);
status_t _kern_validate_set_partition_content_name(disk_system_id diskSystemID,
												   partition_id partitionID,
												   char *name);
status_t _kern_validate_set_partition_type(disk_system_id diskSystemID,
										   partition_id partitionID,
										   const char *type);
status_t _kern_validate_initialize_partition(disk_system_id diskSystemID,
											 partition_id partitionID,
											 char *name,
											 const char *parameters);
status_t _kern_validate_create_child_partition(disk_system_id diskSystemID,
											   partition_id partitionID,
											   off_t *offset, off_t *size,
											   const char *type,
											   const char *parameters);
status_t _kern_get_next_supported_partition_type(disk_system_id diskSystemID,
												 partition_id partitionID,
												 int32 *cookie, char *type);
status_t _kern_get_partition_type_for_content_type(disk_system_id diskSystemID,
												   const char *contentType,
												   char *type);

// disk device modification
status_t _kern_prepare_disk_device_modifications(partition_id deviceID);
status_t _kern_commit_disk_device_modifications(partition_id deviceID,
												port_id port, int32 token,
												bool completeProgress);
status_t _kern_cancel_disk_device_modifications(partition_id deviceID);
bool _kern_is_disk_device_modified(partition_id deviceID);

#if 0

status_t defragment_partition(partition_id partition);
status_t repair_partition(partition_id partition, bool checkOnly);
status_t resize_partition(partition_id partition, off_t size);
status_t move_partition(partition_id partition, off_t offset);
status_t set_partition_parameters(partition_id partition,
								  const char *parameters,
								  const char *contentParameters);
status_t initialize_partition(partition_id partition, const char *diskSystem,
							  const char *parameters);
	// Note: There is also fs_initialize_volume(), which is not compatible
	// with this function, for it is more general with respect to how the
	// volume to be initialized is specified (though this might be solved
	// by providing an API for registering files as disk devices), and more
	// specific regarding the other parameters (flags and volumeName).
status_t create_child_partition(partition_id partition, off_t offset,
								off_t size, const char *parameters,
								partition_id *child);
status_t delete_partition(partition_id partition);

// jobs
status_t get_next_disk_device_job_info(user_disk_device_job_info *info,
									   int32 *cookie);
status_t get_disk_device_job_info(disk_job_id id,
								  user_disk_device_job_info *info);
status_t get_disk_device_job_status(disk_job_id id, uint32 *status,
									float *progress);

// watching
status_t start_disk_device_watching(port_id, int32 token, uint32 flags);
status_t start_disk_device_job_watching(disk_job_id job, port_id, int32 token,
										uint32 flags);
status_t stop_disk_device_watching(port_id, int32 token);

#endif	// 0

#ifdef __cplusplus
}
#endif

#endif	// _DISK_DEVICE_MANAGER_USERLAND_INTERFACE_H
