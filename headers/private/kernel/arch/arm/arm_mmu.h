/*
 * Copyright 2010-2012 Haiku, Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Francois Revol
 *		Ithamar R. Adema, ithamar.adema@team-embedded.nl
 *		Alexander von Gluck, kallisti5@unixzen.com
 */
#ifndef _ARCH_ARM_ARM_MMU_H
#define _ARCH_ARM_ARM_MMU_H


/*
 * generic arm mmu definitions
 */

/*
 * L1 defines for the page directory (page table walk methods)
 */
#define ARM_MMU_L1_TYPE_FAULT			0x0
	// MMU Fault
	// 31                                                  2 10
	// |                                                    |00|
#define ARM_MMU_L1_TYPE_SECTION			0x2
	// Single step table walk, 4096 entries
	// 1024K pages, 16K consumed
	// 31                   20 19   12 11  10 9 8      5 432 10
	// | page table address   |  0?   |  AP  |0| domain |1CB|10|
#define ARM_MMU_L1_TYPE_FINE			0x3
	// Three(?) step table walk, 1024 entries
	// 1K, 4K, 64K pages, 4K consumed
	// 31                           12 11     9 8      5 432 10
	// | page table address           |   0?   | domain |100|11|
#define ARM_MMU_L1_TYPE_COARSE			0x1
	// Two step table walk, 256 entries
	// 4K(Haiku), 64K pages, 1K consumed
	// 31                                  10 9 8      5 432 10
	// | page table address                  |0| domain |000|01|


// the domain is not used so and the ? is implementation specified... have not
// found it in the cortex A8 reference... so I set t to 0
// page table must obviously be on multiple of 1KB

#define ARM_MMU_L2_TYPE_LARGE			0x1
#define ARM_MMU_L2_TYPE_SMALLNEW		0x2
#define ARM_MMU_L2_TYPE_SMALLEXT		0x3

#define ARM_MMU_L2_FLAG_XN				0x001
#define ARM_MMU_L2_FLAG_B				0x004
#define ARM_MMU_L2_FLAG_C				0x008
#define ARM_MMU_L2_FLAG_AP0				0x010
#define ARM_MMU_L2_FLAG_AP1				0x020
#define ARM_MMU_L2_FLAG_TEX0			0x040
#define ARM_MMU_L2_FLAG_TEX1			0x080
#define ARM_MMU_L2_FLAG_TEX2			0x100
#define ARM_MMU_L2_FLAG_AP2				0x200
#define ARM_MMU_L2_FLAG_S				0x400
#define ARM_MMU_L2_FLAG_NG				0x800

#define ARM_MMU_L2_FLAG_AP_KRW			0x010
	// allow read and write for kernel only

#define ARM_MMU_L2_FLAG_AP_RW			0x030
	// allow read and write for user and system

#define ARM_MMU_L1_TABLE_ENTRY_COUNT	4096
#define ARM_MMU_L1_TABLE_SIZE			(ARM_MMU_L1_TABLE_ENTRY_COUNT \
											* sizeof(uint32))

#define ARM_MMU_L2_COARSE_ENTRY_COUNT	256
#define ARM_MMU_L2_COARSE_TABLE_SIZE	(ARM_MMU_L2_COARSE_ENTRY_COUNT \
											* sizeof(uint32))

#define ARM_MMU_L2_FINE_ENTRY_COUNT		1024
#define ARM_MMU_L2_FINE_TABLE_SIZE		(ARM_MMU_L2_FINE_ENTRY_COUNT \
											* sizeof(uint32))

/*
 * definitions for CP15 r1
 */

#define CR_R1_MMU						0x1		// enable MMU
#define CP_R1_XP						0x800000
	// if XP=0 then use backwards comaptible translation tables


#define VADDR_TO_PDENT(va)				((va) >> 20)
#define VADDR_TO_PTENT(va)				(((va) & 0xff000) >> 12)
#define VADDR_TO_PGOFF(va)				((va) & 0x0fff)

#define ARM_PDE_ADDRESS_MASK			0xfffffc00
#define ARM_PDE_TYPE_MASK				0x00000003

#define ARM_PTE_ADDRESS_MASK			0xfffff000
#define ARM_PTE_TYPE_MASK				0x00000003

#define ARM_PTE_PROTECTION_MASK			0x00000231	// AP[2:0], XN
#define ARM_PTE_MEMORY_TYPE_MASK		0x000001cc	// TEX, B, C

#endif /* _ARCH_ARM_ARM_MMU_H */
