#include <stdio.h>
#include <stdlib.h>
#include <libvirt/libvirt.h>
#include <string.h>
#include "../controllers/commands.con.h" 

int main(int argc, char *argv[]) {
    // Ensure we have the correct number of arguments
    if (argc < 5) {
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
    } else if (strcmp(argv[1], "create") == 0 && argc == 5) {
        const char *vm_name = argv[2];
        int ram = atoi(argv[3]);  // RAM in MB // Which is the third argument.
        int cpu = atoi(argv[4]);  // CPU cores // Which is the fourth argument.
        create_vm(conn, vm_name, ram, cpu, "/path/to/iso.iso"); // Add path the ISO.
    } else {
        display_usage();
    }
    
    // Cleanup
    cleanup_libvirt(conn);
    
    return 0;
}
