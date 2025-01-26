#ifndef VM_MANAGER_H
#define VM_MANAGER_H

#include <libvirt/libvirt.h>

// Function to start a virtual machine
void start_vm(virConnectPtr conn, const char *vm_name);

// Function to stop a virtual machine
void stop_vm(virConnectPtr conn, const char *vm_name);

// Function to create a virtual machine from XML configuration
void create_vm(virConnectPtr conn, const char *vm_name);

// Function to list all active virtual machines
void list_vms(virConnectPtr conn);

// Function to clean up and close the libvirt connection
void cleanup_libvirt(virConnectPtr conn);

#endif
