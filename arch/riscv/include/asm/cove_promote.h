/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Definitions for promotion of VM to TVM.
 *
 * Copyright (c) 2024 IBM Corp.
 *
 * Authors:
 *     Wojciech Ozga <woz@zurich.ibm.com>
 */

#ifndef __RISCV_COVE_PROMOTE_H__
#define __RISCV_COVE_PROMOTE_H__

#ifdef CONFIG_RISCV_COVE_GUEST_PROMOTE
#define COVE_PROMOTE_SBI_EXT_ID 0x434F5648
#define COVE_PROMOTE_SBI_FID 0x15
#endif /* CONFIG_RISCV_COVE_GUEST_PROMOTE */

#endif /* __RISCV_COVE_PROMOTE_H__ */
