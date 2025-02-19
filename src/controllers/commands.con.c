#include <stdio.h>
#include <libvirt/libvirt.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "commands.con.h"  
#include "../handlers/iso.hand.h"

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

// Function to create a virtual machine with specified resources
void create_vm(virConnectPtr conn, const char *vm_name, int ram, int cpu, const char *iso_path) {
    // Validate ISO file before proceeding
    if (!validate_iso(iso_path)) {
        return;  
    }

    // Path to the VM disk image
    const char *disk_path = "/var/lib/libvirt/images/";
    char vm_image_path[1024];
    snprintf(vm_image_path, sizeof(vm_image_path), "%s%s.qcow2", disk_path, vm_name);

    // Ensure the directory exists
    struct stat st = {0};
    if (stat(disk_path, &st) == -1) {
        if (mkdir(disk_path, 0755) == -1) {
            fprintf(stderr, "Failed to create directory %s.\n", disk_path);
            return;
        }
    }

    // Create the disk image file if it doesn't exist
    if (access(vm_image_path, F_OK) == -1) {
        char cmd[1024];
        snprintf(cmd, sizeof(cmd), "qemu-img create -f qcow2 %s 10G", vm_image_path);
        if (system(cmd) != 0) {
            fprintf(stderr, "Failed to create disk image for VM %s.\n", vm_name);
            return;
        }
    }

    // Updated XML configuration to accept RAM and CPU
    const char *xml =
        "<domain type='kvm'>"
        "  <name>%s</name>"
        "  <memory unit='KiB'>%d</memory>"  // Dynamically set memory
        "  <vcpu placement='static'>%d</vcpu>"     // Dynamically set CPU cores
        "  <os>"
        "    <type arch='x86_64' machine='pc-i440fx-2.9'>hvm</type>"
        "    <boot dev='hd'/>"
        "    <boot dev='cdrom'/>" // Boot from CD-ROM (ISO)
        "  </os>"
        "  <devices>"
        "    <disk type='file' device='disk'>"
        "      <driver name='qemu' type='qcow2'/>"
        "      <source file='%s'/>"
        "      <target dev='vda' bus='virtio'/>"
        "    </disk>"
        "    <disk type='file' device='cdrom'>"
        "      <driver name='qemu' type='raw'/>"
        "      <source file='%s'/>"  // ISO path
        "      <target dev='hdc' bus='ide'/>"
        "      <address type='pci' domain='0x0000' bus='0x00' slot='0x03' function='0x0'/>"
        "    </disk>"
        "    <interface type='network'>"
        "      <mac address='52:54:00:dd:cb:cd'/>"
        "      <source network='default'/>"
        "      <model type='virtio'/>"
        "    </interface>"
        "  </devices>"
        "</domain>";

    // Prepare the XML string with the VM name, RAM, CPU, disk path, and ISO path
    char vm_xml[1024];
    snprintf(vm_xml, sizeof(vm_xml), xml, vm_name, ram * 1024, cpu, vm_image_path, iso_path);  // Dynamically using RAM and CPU values

    // Create the VM from the XML configuration
    virDomainPtr domain = virDomainCreateXML(conn, vm_xml, 0);
    if (domain == NULL) {
        fprintf(stderr, "Failed to create VM %s.\n", vm_name);
        return;
    }

    printf("VM %s created with %d MB RAM, %d CPU(s), and ISO %s.\n", vm_name, ram, cpu, iso_path);
}


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
    printf("  list_networks  Displays the current available networks\n");
    printf("  create_network <name> <ip> {you can enter dhcp for IP.} Creates a network bridge.  \n");
    printf("  attach_network_to_vm <vm_name> <bridge_name>> Connects the bridge to the VM.  \n");
    printf("  delete_network <network_name> Deletes a network / bridge.  \n");




}
