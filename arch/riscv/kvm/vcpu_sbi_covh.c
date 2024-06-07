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
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/spinlock.h>
#include <asm/csr.h>
#include <asm/sbi.h>
#include <asm/kvm_vcpu_sbi.h>
#include <asm/kvm_cove.h>
#include <asm/kvm_cove_sbi.h>
#include <asm/kvm_nacl.h>
#include <linux/rbtree.h>
#include <linux/pgtable.h>

static int preload_pages(struct kvm_vcpu *vcpu) {
	unsigned long hva, fault_addr, page;
	struct kvm_memory_slot *memslot;
	bool writable;
	int bkt;

	kvm_for_each_memslot(memslot, bkt, kvm_memslots(vcpu->kvm)) {
                for (page = 0; page < memslot->npages; page++) {
                        fault_addr = gfn_to_gpa(memslot->base_gfn) +
                                                page * PAGE_SIZE;
			hva = gfn_to_hva_memslot_prot(memslot,
						      gpa_to_gfn(fault_addr),
						      &writable);
			if (!kvm_is_error_hva(hva))
				kvm_riscv_gstage_map(vcpu, memslot, fault_addr,
						     hva, NULL);
		}
	}

	return 0;
}

static int kvm_riscv_cove_promote_to_tvm(struct kvm_vcpu *vcpu,
					 unsigned long fdt_address,
					 unsigned long tap_addr) {
	int rc;

	preload_pages(vcpu);
	rc = kvm_riscv_cove_vm_single_step_init(vcpu, fdt_address, tap_addr);
	if (rc)
		goto done;

	vcpu->kvm->arch.vm_type = KVM_VM_TYPE_RISCV_COVE;
done:
	return rc;
}

static int kvm_sbi_ext_covh_handler(struct kvm_vcpu *vcpu, struct kvm_run *run,
				    struct kvm_vcpu_sbi_return *retdata)
{
	struct kvm_cpu_context *cp = &vcpu->arch.guest_context;
	unsigned long funcid = cp->a6;
	int ret;

	switch (funcid) {
	case SBI_EXT_COVH_PROMOTE_TO_TVM:
		ret = kvm_riscv_cove_promote_to_tvm(vcpu, cp->a0, cp->a1);
		return 0;

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
