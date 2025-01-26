#include <stdio.h>
#include <libvirt/libvirt.h>
#include <string.h>

// Function to list active VMs
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

// Function to cleanup libvirt connection
void cleanup_libvirt(virConnectPtr conn) {
    if (conn) {
        virConnectClose(conn);
    }
}

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

// Function to create a virtual machine from XML configuration
void create_vm(virConnectPtr conn, const char *vm_name) {
    // Minimal XML configuration for the VM
    const char *xml = 
        "<domain type='kvm'>"
        "  <name>%s</name>"
        "  <memory unit='KiB'>1048576</memory>"  // 1 GB memory
        "  <vcpu placement='static'>1</vcpu>"     // 1 CPU core
        "  <os>"
        "    <type arch='x86_64' machine='pc-i440fx-2.9'>hvm</type>"
        "  </os>"
        "  <devices>"
        "    <disk type='file' device='disk'>"
        "      <driver name='qemu' type='qcow2'/>"
        "      <source file='/var/lib/libvirt/images/%s.qcow2'/>"
        "      <target dev='vda' bus='virtio'/>"
        "    </disk>"
        "    <interface type='network'>"
        "      <mac address='52:54:00:dd:cb:cd'/>"
        "      <source network='default'/>"
        "      <model type='virtio'/>"
        "    </interface>"
        "  </devices>"
        "</domain>";

    // Prepare the XML string with the VM name
    char vm_xml[1024];
    snprintf(vm_xml, sizeof(vm_xml), xml, vm_name, vm_name);  // Using VM name in XML

    // Create the VM from the XML configuration
    virDomainPtr domain = virDomainCreateXML(conn, vm_xml, 0);
    if (domain == NULL) {
        fprintf(stderr, "Failed to create VM %s.\n", vm_name);
        return;
    }

    printf("VM %s created.\n", vm_name);
}
