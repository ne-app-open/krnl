// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl


.globl __ImageStart
.extern hal_init_platform
.align 4
.text

__ImageStart:
	bl hal_init_platform
	blr
