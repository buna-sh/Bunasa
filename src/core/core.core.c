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

void list_vms(virConnectPtr conn) {
    virDomainPtr *domains;
    int num_domains = virConnectListAllDomains(conn, &domains, VIR_CONNECT_LIST_DOMAINS_ACTIVE);
    
    if (num_domains < 0) {
        fprintf(stderr, "Failed to list domains.\n");
        exit(1);
    }
    
    printf("Active VMs:\n");
    for (int i = 0; i < num_domains; i++) {
        const char *name = virDomainGetName(domains[i]);
        if (name != NULL) {
            printf("%s\n", name);
        } else {
            fprintf(stderr, "Failed to get the name of domain %d.\n", i);
        }
    }
    
    // Cleanup
    free(domains);
}

// Cleanup function
void cleanup_libvirt(virConnectPtr conn) {
    if (conn) {
        virConnectClose(conn);
    }
}

// Function to display usage instructions
void display_usage() {
    printf("Usage:\n");
    printf("  list             List all active VMs\n");
    printf("  start <vm_name>  Start the VM with the given name\n");
    printf("  stop <vm_name>   Stop the VM with the given name\n");
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
        start_vm(conn, argv[2]);  // Call the start_vm function from vm_manager.c
    } else if (strcmp(argv[1], "stop") == 0 && argc == 3) {
        stop_vm(conn, argv[2]);   // Call the stop_vm function from vm_manager.c
    } else {
        display_usage();
    }
    
    // Cleanup
    cleanup_libvirt(conn);
    
    return 0;
}
