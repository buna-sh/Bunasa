#include <stdio.h>
#include <libvirt/libvirt.h>
#include <stdlib.h>
#include <string.h>
#include "network.con.h"

// Function to create a virtual network
#include <libvirt/libvirt.h>
#include <stdio.h>
#include <stdlib.h>

// Function to create a virtual network
int create_network(virConnectPtr conn, const char *network_name, const char *ip_address) {
    char network_xml[2048];

    // If IP is "dhcp", we will set up DHCP
    if (strcmp(ip_address, "dhcp") == 0) {
        snprintf(network_xml, sizeof(network_xml),
                 "<network>"
                 "  <name>%s</name>"
                 "  <bridge name='virbr0' stp='on' delay='0'/>"
                 "  <ip address='0.0.0.0' netmask='0.0.0.0'>"
                 "    <dhcp>"
                 "      <range start='192.168.100.100' end='192.168.100.200'/>"
                 "    </dhcp>"
                 "  </ip>"
                 "</network>", network_name);
    } else {
        // If a static IP is provided, use the given IP address
        snprintf(network_xml, sizeof(network_xml),
                 "<network>"
                 "  <name>%s</name>"
                 "  <bridge name='virbr0' stp='on' delay='0'/>"
                 "  <ip address='%s' netmask='255.255.255.0'>"
                 "    <dhcp>"
                 "      <range start='192.168.100.100' end='192.168.100.200'/>"
                 "    </dhcp>"
                 "  </ip>"
                 "</network>", network_name, ip_address);
    }

    // Create the network using virNetworkCreateXML
    virNetworkPtr network = virNetworkCreateXML(conn, network_xml);  // No flag required
    if (network == NULL) {
        fprintf(stderr, "Failed to create network %s.\n", network_name);
        return -1;
    }

    printf("Network %s created successfully.\n", network_name);
    return 0;
}
// Function to list all active virtual networks
void list_networks(virConnectPtr conn) {
    virNetworkPtr *networks;
    int num_networks = virConnectListAllNetworks(conn, &networks, 0);

    if (num_networks < 0) {
        fprintf(stderr, "Failed to list networks.\n");
        exit(1);
    }

    printf("Active Networks:\n");
    for (int i = 0; i < num_networks; i++) {
        const char *name = virNetworkGetName(networks[i]);
        if (name != NULL) {
            printf("%s\n", name);
        } else {
            fprintf(stderr, "Failed to get the name of network %d.\n", i);
        }
    }

    // Cleanup
    free(networks);
}

// Function to attach a VM to a virtual network
int attach_network_to_vm(virConnectPtr conn, const char *vm_name, const char *network_name) {
    virDomainPtr domain = virDomainLookupByName(conn, vm_name);
    if (domain == NULL) {
        fprintf(stderr, "VM %s not found.\n", vm_name);
        return -1;
    }

    virNetworkPtr network = virNetworkLookupByName(conn, network_name);
    if (network == NULL) {
        fprintf(stderr, "Network %s not found.\n", network_name);
        return -1;
    }

    // Attach the VM to the network
    if (virDomainAttachDevice(domain, network_name) != 0) {
        fprintf(stderr, "Failed to attach VM %s to network %s.\n", vm_name, network_name);
        return -1;
    }

    printf("VM %s successfully attached to network %s.\n", vm_name, network_name);
    return 0;
}

// Function to delete a virtual network
int delete_network(virConnectPtr conn, const char *network_name) {
    virNetworkPtr network = virNetworkLookupByName(conn, network_name);
    if (network == NULL) {
        fprintf(stderr, "Network %s not found.\n", network_name);
        return -1;
    }

    if (virNetworkDestroy(network) != 0) {
        fprintf(stderr, "Failed to destroy network %s.\n", network_name);
        return -1;
    }

    if (virNetworkUndefine(network) != 0) {
        fprintf(stderr, "Failed to undefine network %s.\n", network_name);
        return -1;
    }

    printf("Network %s deleted successfully.\n", network_name);
    return 0;
}
