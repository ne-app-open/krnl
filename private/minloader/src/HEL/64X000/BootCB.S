# // SPDX-License-Identifier: Apache-2.0
# // Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
# // Licensed under the Apache License, Version 2.0 (see LICENSE file)
# // Official repository: https://github.com/ne-app-eu/krnl

.section .boot_hdr
.align 4

/* BootZ boot header begin for a 64x000 Kernel. */

boot_hdr_mag:
        .ascii "CB"
boot_hdr_name:
        // it has to match ten bytes.
        .asciz "minloader\0\0\0"
boot_hdr_ver:
        .word 0x104
boot_hdr_proc:
        .long bootloader_start

/* BootZ boot header end */

.extern bootloader_main
.extern bootloader_stack

.globl bootloader_start
bootloader_start:
    psh	4			/* real address of .Laddr */
    ldi     0,(bootloader_stack-bootloader_start)(4)	/* stack address location */
    mv	19,0			/* use user defined stack */
    jrl

    bl bootloader_main
    blr
