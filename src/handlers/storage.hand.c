#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libvirt/libvirt.h>
#include <libvirt/libvirt-storage.h>

#include "storage.hand.h"

// Function to create a new virtual disk within a storage pool
int create_disk(virConnectPtr conn, const char *disk_name, const char *size, const char *format) {
    // Define storage pool name (you can use any existing pool)
    const char *pool_name = "images";
    // Get the storage pool
    virStoragePoolPtr pool = virStoragePoolLookupByName(conn, pool_name);
    if (pool == NULL) {
        fprintf(stderr, "Failed to find storage pool %s.\n", pool_name);
        return -1;
    }

    // Prepare the XML for creating a disk volume
    char disk_xml[1024];
    snprintf(disk_xml, sizeof(disk_xml),
        "<volume>"
        "  <name>%s</name>"
        "  <capacity unit='bytes'>%s</capacity>"
        "  <allocation unit='bytes'>%s</allocation>"
        "  <target>"
        "    <path>/var/lib/libvirt/images/%s</path>"
        "    <format type='%s'/>"
        "  </target>"
        "</volume>", disk_name, size, size, disk_name, format);

    // Create the disk volume
    virStorageVolPtr result = virStorageVolCreateXML(pool, disk_xml, 0);
    if (result == NULL) {
        fprintf(stderr, "Failed to create disk %s.\n", disk_name);
        return -1;
    }

    printf("Disk %s created successfully.\n", disk_name);
    return 0;
}

// Function to list disks from the storage pool
int list_disks(virConnectPtr conn, const char *vm_name) {
        const char *pool_name = "images"; 

    // Get the storage pool
    virStoragePoolPtr pool = virStoragePoolLookupByName(conn, pool_name);
    if (pool == NULL) {
        fprintf(stderr, "Failed to find storage pool %s.\n", pool_name);
        return -1;
    }

    // List the storage volumes (disks) in the pool
    virStorageVolPtr *volumes = NULL;
    int volume_count = virStoragePoolListVolumes(pool, &volumes, 0);
    if (volume_count < 0) {
        fprintf(stderr, "Failed to list volumes in storage pool %s.\n", pool_name);
        return -1;
    }

    // Print out the names of the volumes (disks)
    for (int i = 0; i < volume_count; i++) {
        if (volumes[i]) {
            char *disk_name = virStorageVolGetName(volumes[i]);
            printf("Disk %d: %s\n", i + 1, disk_name);
            free(disk_name); // Make sure to free the allocated memory
        }
    }

    // Free the volume list
    free(volumes);
    return 0;
}

// Function to attach a disk to a VM
int attach_disk(virConnectPtr conn, const char *vm_name, const char *disk_name) {
    virDomainPtr vm = virDomainLookupByName(conn, vm_name);
    if (vm == NULL) {
        fprintf(stderr, "Failed to find VM %s.\n", vm_name);
        return -1;
    }

    // Prepare XML to attach disk
    char attach_disk_xml[1024];
    snprintf(attach_disk_xml, sizeof(attach_disk_xml),
        "<disk type='file' device='disk'>"
        "  <driver name='qemu' type='qcow2'/>"
        "  <source file='/var/lib/libvirt/images/%s'/>"
        "  <target dev='vda' bus='virtio'/>"
        "</disk>", disk_name);

    // Attach the disk to the VM
    int result = virDomainAttachDevice(vm, attach_disk_xml);
    if (result < 0) {
        fprintf(stderr, "Failed to attach disk %s to VM %s.\n", disk_name, vm_name);
        return -1;
    }

    printf("Disk %s attached to VM %s successfully.\n", disk_name, vm_name);
    return 0;
}

// Function to detach a disk from a VM
int detach_disk(virConnectPtr conn, const char *vm_name, const char *disk_name) {
    virDomainPtr vm = virDomainLookupByName(conn, vm_name);
    if (vm == NULL) {
        fprintf(stderr, "Failed to find VM %s.\n", vm_name);
        return -1;
    }

    // Prepare XML to detach disk
    char detach_disk_xml[1024];
    snprintf(detach_disk_xml, sizeof(detach_disk_xml),
        "<disk type='file' device='disk'>"
        "  <source file='/var/lib/libvirt/images/%s'/>"
        "</disk>", disk_name);

    // Detach the disk from the VM
    int result = virDomainDetachDevice(vm, detach_disk_xml);
    if (result < 0) {
        fprintf(stderr, "Failed to detach disk %s from VM %s.\n", disk_name, vm_name);
        return -1;
    }

    printf("Disk %s detached from VM %s successfully.\n", disk_name, vm_name);
    return 0;
}

// Function to delete a disk from the storage pool
int delete_disk(virConnectPtr conn, const char *disk_name) {
    // Define storage pool name (you can use any existing pool)
    const char *pool_name = "images"; // Example pool name, replace with your pool's name

    // Get the storage pool
    virStoragePoolPtr pool = virStoragePoolLookupByName(conn, pool_name);
    if (pool == NULL) {
        fprintf(stderr, "Failed to find storage pool %s.\n", pool_name);
        return -1;
    }

    // Get the volume
    virStorageVolPtr vol = virStorageVolLookupByName(pool, disk_name);
    if (vol == NULL) {
        fprintf(stderr, "Failed to find disk volume %s.\n", disk_name);
        return -1;
    }

    // Delete the disk volume
    int result = virStorageVolDelete(vol, 0);
    if (result < 0) {
        fprintf(stderr, "Failed to delete disk %s.\n", disk_name);
        return -1;
    }

    printf("Disk %s deleted successfully.\n", disk_name);
    return 0;
}
