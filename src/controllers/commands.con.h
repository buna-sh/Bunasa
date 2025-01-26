#ifndef VM_MANAGER_H
#define VM_MANAGER_H

#include <libvirt/libvirt.h>

void start_vm(virConnectPtr conn, const char *vm_name);
void stop_vm(virConnectPtr conn, const char *vm_name);

#endif
