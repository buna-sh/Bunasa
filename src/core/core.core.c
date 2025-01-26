#include <stdio.h>
#include <stdlib.h>
#include <libvirt/libvirt.h>
#include <string.h>
#include "../controllers/commands.con.h" 

// Function to initialize libvirt connection
virConnectPtr initialize_libvirt() {
    virConnectPtr conn = virConnectOpen(NULL);  // Open the default connection (local)
    if (conn == NULL) {
        fprintf(stderr, "Failed to open connection to libvirt.\n");
        exit(1);
    }
    return conn;
}

// Function to display usage instructions
void display_usage() {
    printf("Usage:\n");
    printf("  list             List all active VMs\n");
    printf("  start <vm_name>  Start the VM with the given name\n");
    printf("  stop <vm_name>   Stop the VM with the given name\n");
    printf("  create <vm_name> Create a new VM with the given name\n");
}

int main(int argc, char *argv[]) {
    // Ensure we have at least one argument
    if (argc < 2) {
        display_usage();
        return 1;
    }
    
    // Initialize libvirt connection
    virConnectPtr conn = initialize_libvirt();
    
    // Handle commands based on the arguments passed
    if (strcmp(argv[1], "list") == 0) {
        list_vms(conn);
    } else if (strcmp(argv[1], "start") == 0 && argc == 3) {
        start_vm(conn, argv[2]);  // Start VM
    } else if (strcmp(argv[1], "stop") == 0 && argc == 3) {
        stop_vm(conn, argv[2]);   // Stop VM
    } else if (strcmp(argv[1], "create") == 0 && argc == 3) {
        create_vm(conn, argv[2]); // Create new VM
    } else {
        display_usage();
    }
    
    // Cleanup
    cleanup_libvirt(conn);
    
    return 0;
}
