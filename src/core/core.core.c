#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libvirt/libvirt.h>

#include "../controllers/commands.con.h"  
#include "../controllers/network.con.h"

#include "../handlers/iso.hand.h"
#include "../handlers/log.hand.h"
#include "../handlers/storage.hand.h"

void cleanup_libvirt(virConnectPtr conn);  // Declare the cleanup function

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

    // Command handling for VM management
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
    } else if (strcmp(argv[1], "create_disk") == 0 && argc == 4) {
        log_message(INFO, "Creating disk: %s with size: %s", argv[2], argv[3]);
        create_disk(conn, argv[2], argv[3], "qcow2");
    } else if (strcmp(argv[1], "list_disks") == 0 && argc == 3) {
        log_message(INFO, "Listing disks for VM: %s", argv[2]);
        list_disks(conn, argv[2]);
    } else if (strcmp(argv[1], "attach_disk") == 0 && argc == 4) {
        log_message(INFO, "Attaching disk: %s to VM: %s", argv[2], argv[3]);
        attach_disk(conn, argv[3], argv[2]);
    } else if (strcmp(argv[1], "detach_disk") == 0 && argc == 4) {
        log_message(INFO, "Detaching disk: %s from VM: %s", argv[2], argv[3]);
        detach_disk(conn, argv[3], argv[2]);
    } else if (strcmp(argv[1], "delete_disk") == 0 && argc == 3) {
        log_message(INFO, "Deleting disk: %s", argv[2]);
        delete_disk(conn, argv[2]);
    } else {
        log_message(ERROR, "Unknown command.");
        display_usage();
    }

    // Cleanup libvirt connection
    cleanup_libvirt(conn);

    // Cleanup log
    cleanup_log();

    return 0;
}


