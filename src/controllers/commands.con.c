#include <stdio.h>
#include <libvirt/libvirt.h>
#include <string.h>

// Function to start a virtual machine
void start_vm(virConnectPtr conn, const char *vm_name) {
    virDomainPtr domain = virDomainLookupByName(conn, vm_name);
    if (domain == NULL) {
        fprintf(stderr, "VM %s not found.\n", vm_name);
        return;
    }

    if (virDomainCreate(domain) != 0) {
        fprintf(stderr, "Failed to start VM %s.\n", vm_name);
    } else {
        printf("VM %s started.\n", vm_name);
    }
}

// Function to stop a virtual machine
void stop_vm(virConnectPtr conn, const char *vm_name) {
    virDomainPtr domain = virDomainLookupByName(conn, vm_name);
    if (domain == NULL) {
        fprintf(stderr, "VM %s not found.\n", vm_name);
        return;
    }

    if (virDomainShutdown(domain) != 0) {
        fprintf(stderr, "Failed to stop VM %s.\n", vm_name);
    } else {
        printf("VM %s stopped.\n", vm_name);
    }
}
