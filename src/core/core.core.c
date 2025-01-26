#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libvirt/libvirt.h>

#include "../controllers/commands.con.h"  
#include "../controllers/network.con.h"


#include "../handlers/iso.hand.h"
#include "../handlers/log.hand.h"

int main(int argc, char *argv[]) {
    // Initialize logging
    initialize_log();

    if (argc < 2) {
        log_message(ERROR, "Invalid arguments. Usage: ./vm_manager <command> <args>");
        display_usage();
        cleanup_log();
        return 1;
    }

    // Initialize libvirt connection
    virConnectPtr conn = initialize_libvirt();
    if (!conn) {
        log_message(ERROR, "Failed to initialize libvirt connection.");
        cleanup_log();
        return 1;
    }

    // Command handling
    if (strcmp(argv[1], "list") == 0) {
        log_message(INFO, "Listing active VMs.");
        list_vms(conn);
    } else if (strcmp(argv[1], "start") == 0 && argc == 3) {
        log_message(INFO, "Starting VM: %s", argv[2]);
        start_vm(conn, argv[2]);
    } else if (strcmp(argv[1], "stop") == 0 && argc == 3) {
        log_message(INFO, "Stopping VM: %s", argv[2]);
        stop_vm(conn, argv[2]);
    } else if (strcmp(argv[1], "create") == 0 && argc == 3) {
        log_message(INFO, "Creating VM: %s", argv[2]);
        create_vm(conn, argv[2], 1024, 1, "/path/to/iso");
    } else if (strcmp(argv[1], "create_network") == 0 && argc == 4) {
        log_message(INFO, "Creating network: %s with IP: %s", argv[2], argv[3]);
        create_network(conn, argv[2], argv[3]);
    } else if (strcmp(argv[1], "list_networks") == 0) {
        log_message(INFO, "Listing active networks.");
        list_networks(conn);
    } else if (strcmp(argv[1], "delete_network") == 0 && argc == 3) {
        log_message(INFO, "Deleting network: %s", argv[2]);
        delete_network(conn, argv[2]);
    } else if (strcmp(argv[1], "attach_network") == 0 && argc == 4) {
        log_message(INFO, "Attaching VM: %s to network: %s", argv[2], argv[3]);
        attach_network_to_vm(conn, argv[2], argv[3]);
    } else {
        log_message(ERROR, "Invalid command. Usage: ./vm_manager <command> <args>");
        display_usage();
        cleanup_log();
        return 1;
    }

    // Cleanup libvirt connection
    cleanup_libvirt(conn);

    // Cleanup logging
    cleanup_log();

    return 0;
}
