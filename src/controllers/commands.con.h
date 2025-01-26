#ifndef COMMANDS_CON_H
#define COMMANDS_CON_H

#include <libvirt/libvirt.h>

// Function to list active VMs
void list_vms(virConnectPtr conn);

// Function to start a virtual machine
void start_vm(virConnectPtr conn, const char *vm_name);

// Function to stop a virtual machine
void stop_vm(virConnectPtr conn, const char *vm_name);

// Function to create a virtual machine
void create_vm(virConnectPtr conn, const char *vm_name, const char *iso_path);

// Function to cleanup libvirt connection
void cleanup_libvirt(virConnectPtr conn);

// Function to initialize libvirt connection
virConnectPtr initialize_libvirt();

// Function to display usage instructions
void display_usage();

#endif // COMMANDS_CON_H
