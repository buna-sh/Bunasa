#ifndef NETWORK_CONTROLLER_H
#define NETWORK_CONTROLLER_H

#include <libvirt/libvirt.h>

// Function to create a virtual network
int create_network(virConnectPtr conn, const char *network_name, const char *ip_address);

// Function to list all active virtual networks
void list_networks(virConnectPtr conn);

// Function to attach a VM to a virtual network
int attach_network_to_vm(virConnectPtr conn, const char *vm_name, const char *network_name);

// Function to delete a virtual network
int delete_network(virConnectPtr conn, const char *network_name);

// Function to start a virtual network
int start_network(virConnectPtr conn, const char *network_name);

// Function to stop a virtual network
int stop_network(virConnectPtr conn, const char *network_name);

#endif // NETWORK_CONTROLLER_H
