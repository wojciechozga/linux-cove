// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2024 IBM.
 *
 * Authors:
 *     Wojciech Ozga <woz@zurich.ibm.com>
 */
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/kvm_host.h>
#include <asm/sbi.h>
static int kvm_sbi_ext_covh_handler(struct kvm_vcpu *vcpu, struct kvm_run *run,
				    struct kvm_vcpu_sbi_return *retdata)
{
	struct kvm_cpu_context *cp = &vcpu->arch.guest_context;
	unsigned long funcid = cp->a6;
	switch (funcid) {
	default:
		kvm_err("%s: Unsupported guest SBI %ld.\n", __func__, funcid);
		retdata->err_val = SBI_ERR_NOT_SUPPORTED;
		return -EOPNOTSUPP;
	}
}
const struct kvm_vcpu_sbi_extension vcpu_sbi_ext_covh = {
	.extid_start = SBI_EXT_COVH,
	.extid_end = SBI_EXT_COVH,
	.handler = kvm_sbi_ext_covh_handler,
};