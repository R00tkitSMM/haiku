/*
 * Copyright 2007, François Revol, revol@free.fr.
 * Distributed under the terms of the MIT License.
 *
 * Copyright 2003-2005, Axel Dörfler, axeld@pinc-software.de.
 * Distributed under the terms of the MIT License.
 *
 * Copyright 2001, Travis Geiselbrecht. All rights reserved.
 * Distributed under the terms of the NewOS License.
 */


#include <KernelExport.h>

#include <kernel.h>
#include <boot/kernel_args.h>

#include <vm/vm.h>
#include <vm/VMAddressSpace.h>
#include <vm/vm_types.h>
#include <arch/vm.h>
//#include <arch_mmu.h>


//#define TRACE_ARCH_VM
#ifdef TRACE_ARCH_VM
#	define TRACE(x) dprintf x
#else
#	define TRACE(x) ;
#endif


status_t
arch_vm_init(kernel_args *args)
{
	TRACE(("arch_vm_init: entry\n"));
	return B_OK;
}


status_t
arch_vm_init2(kernel_args *args)
{
	return B_OK;
}


status_t
arch_vm_init_post_area(kernel_args *args)
{
	TRACE(("arch_vm_init_post_area: entry\n"));
	return B_OK;
}


status_t
arch_vm_init_end(kernel_args *args)
{
	TRACE(("arch_vm_init_end(): %" B_PRIu32 " virtual ranges to keep:\n",
		args->arch_args.num_virtual_ranges_to_keep));

	for (int i = 0; i < (int)args->arch_args.num_virtual_ranges_to_keep; i++) {
		addr_range &range = args->arch_args.virtual_ranges_to_keep[i];

		TRACE(("  start: %p, size: %#" B_PRIxSIZE "\n", (void*)range.start, range.size));

		// skip ranges outside the kernel address space
		if (!IS_KERNEL_ADDRESS(range.start)) {
			TRACE(("    no kernel address, skipping...\n"));
			continue;
		}

		phys_addr_t physicalAddress;
		void *address = (void*)range.start;
		if (vm_get_page_mapping(VMAddressSpace::KernelID(), range.start,
			&physicalAddress) != B_OK)
			panic("arch_vm_init_end(): No page mapping for %p\n", address);

		area_id area = vm_map_physical_memory(VMAddressSpace::KernelID(),
			"boot loader reserved area", &address,
			B_EXACT_ADDRESS, range.size,
			B_KERNEL_READ_AREA | B_KERNEL_WRITE_AREA,
			physicalAddress, true);

		if (area < 0) {
			panic("arch_vm_init_end(): Failed to create area for boot loader "
				"reserved area: %p - %p\n", (void*)range.start,
				(void*)(range.start + range.size));
		}
	}

	// Throw away all mappings that are unused by the kernel
	vm_free_unused_boot_loader_range(KERNEL_LOAD_BASE, KERNEL_SIZE);

	return B_OK;
}


status_t
arch_vm_init_post_modules(kernel_args *args)
{
	return B_OK;
}


void
arch_vm_aspace_swap(struct VMAddressSpace *from, struct VMAddressSpace *to)
{
	// This functions is only invoked when a userland thread is in the process
	// of dying. It switches to the kernel team and does whatever cleanup is
	// necessary (in case it is the team's main thread, it will delete the
	// team).
	// It is however not necessary to change the page directory. Userland team's
	// page directories include all kernel mappings as well. Furthermore our
	// arch specific translation map data objects are ref-counted, so they won't
	// go away as long as they are still used on any CPU.
}


bool
arch_vm_supports_protection(uint32 protection)
{
	// TODO check ARM protection possibilities
	return true;
}


void
arch_vm_unset_memory_type(VMArea *area)
{
}


status_t
arch_vm_set_memory_type(VMArea *area, phys_addr_t physicalBase, uint32 type)
{
	return B_OK;
}
