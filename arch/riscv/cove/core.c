// SPDX-License-Identifier: GPL-2.0
/*
 * Confidential Computing Platform Capability checks
 *
 * Copyright (c) 2023 Rivos Inc.
 *
 * Authors:
 *     Rajnesh Kanwal <rkanwal@rivosinc.com>
 *     Wojciech Ozga <woz@zurich.ibm.com>
 */

#include <linux/export.h>
#include <linux/cc_platform.h>
#include <asm/sbi.h>
#include <asm/cove.h>

static bool is_tvm;

bool is_cove_guest(void)
{
	return is_tvm;
}
EXPORT_SYMBOL_GPL(is_cove_guest);

bool cc_platform_has(enum cc_attr attr)
{
	switch (attr) {
	case CC_ATTR_GUEST_MEM_ENCRYPT:
	case CC_ATTR_MEM_ENCRYPT:
		return is_cove_guest();
	default:
		return false;
	}
}
EXPORT_SYMBOL_GPL(cc_platform_has);

void riscv_cove_sbi_init(void)
{
	if (sbi_probe_extension(SBI_EXT_COVG) > 0)
		is_tvm = true;
}

int promote_to_cove_guest(char *boot_command_line, unsigned long fdt_address)
{
	struct sbiret ret;
	int rc = 0;
	unsigned long tap_addr = 0;

	if (strstr(boot_command_line, "promote_to_tvm")) {
		ret = sbi_ecall(SBI_EXT_COVH, SBI_EXT_COVH_PROMOTE_TO_TVM, fdt_address,
					tap_addr, 0, 0, 0, 0);
		if (ret.error) {
				rc = sbi_err_map_linux_errno(ret.error);
				goto done;
		}
	}
	pr_info("Promotion to CoVE guest succeeded\n");

	return rc;
done:
	pr_err("Promotion to CoVE guest failed %d\n", rc);

	return rc;
}
