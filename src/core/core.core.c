#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libvirt/libvirt.h>

#include "../controllers/commands.con.h"  


#include "../handlers/iso.hand.h"
#include "../controllers/network.con.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        display_usage();
        return 1;
    }

    // Initialize libvirt connection
    virConnectPtr conn = initialize_libvirt();

    if (strcmp(argv[1], "list") == 0) {
        // List active VMs
        list_vms(conn);
    } else if (strcmp(argv[1], "start") == 0 && argc == 3) {
        // Start VM
        start_vm(conn, argv[2]);
    } else if (strcmp(argv[1], "stop") == 0 && argc == 3) {
        // Stop VM
        stop_vm(conn, argv[2]);
    } else if (strcmp(argv[1], "create") == 0 && argc == 3) {
        // Create VM
        create_vm(conn, argv[2], 1024, 1, "/path/to/iso");
    } else if (strcmp(argv[1], "create_network") == 0 && argc == 4) {
        // Create Network
        create_network(conn, argv[2], argv[3]);
    } else if (strcmp(argv[1], "list_networks") == 0) {
        // List active networks
        list_networks(conn);
    } else if (strcmp(argv[1], "delete_network") == 0 && argc == 3) {
        // Delete a virtual network
        delete_network(conn, argv[2]);
    } else if (strcmp(argv[1], "attach_network") == 0 && argc == 4) {
        // Attach VM to a network
        attach_network_to_vm(conn, argv[2], argv[3]);
    } else {
        // Invalid command
        display_usage();
        return 1;
    }

    // Cleanup libvirt connection
    cleanup_libvirt(conn);

    return 0;
}

