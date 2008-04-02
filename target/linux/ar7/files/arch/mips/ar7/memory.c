/*
 * Based on arch/mips/mm/init.c
 * Copyright (C) 1994 - 2000 Ralf Baechle
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
 * Kevin D. Kissell, kevink@mips.com and Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 2000 MIPS Technologies, Inc.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <linux/bootmem.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/pfn.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/swap.h>

#include <asm/bootinfo.h>
#include <asm/page.h>
#include <asm/sections.h>

#include <asm/mips-boards/prom.h>

static int __init memsize(void)
{
	u32 size = (64 << 20);
	volatile u32 *addr = (u32 *)KSEG1ADDR(0x14000000 + size - 4);
	u32 *kernel_end = (u32 *)KSEG1ADDR(CPHYSADDR((u32)&_end));

	while (addr > kernel_end) {
		*addr = (u32)addr;
		size >>= 1;
		addr -= size >> 2;
	}

	do {
		addr += size >> 2;
		if (*addr != (u32)addr)
			break;
		size <<= 1;
	} while (size < (64 << 20));

	return size;
}

void __init prom_meminit(void)
{
	unsigned long pages;

	pages = memsize() >> PAGE_SHIFT;
	add_memory_region(PHYS_OFFSET, pages << PAGE_SHIFT,
			  BOOT_MEM_RAM);
}

void __init prom_free_prom_memory(void)
{
	return;
}
