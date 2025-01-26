// storage.hand.h
#ifndef STORAGE_HAND_H
#define STORAGE_HAND_H

#include <libvirt/libvirt.h>

// Function to create a new virtual disk
int create_disk(virConnectPtr conn, const char *disk_name, const char *size, const char *format);

// Function to list all disks of a VM
int list_disks(virConnectPtr conn, const char *vm_name);

// Function to attach a disk to a VM
int attach_disk(virConnectPtr conn, const char *vm_name, const char *disk_name);

// Function to detach a disk from a VM
int detach_disk(virConnectPtr conn, const char *vm_name, const char *disk_name);

// Function to delete a disk
int delete_disk(virConnectPtr conn, const char *disk_name);

#endif
