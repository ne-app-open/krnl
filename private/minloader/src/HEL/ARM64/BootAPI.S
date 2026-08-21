# // SPDX-License-Identifier: Apache-2.0
# // Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
# // Licensed under the Apache License, Version 2.0 (see LICENSE file)
# // Official repository: https://github.com/ne-app-eu/krnl

.global rt_jump_to_address

.text

/**
 @brief this function setups a stack and then jumps to
 a function */
rt_jump_to_address:
    mov sp, x1
    br x0
    ret

