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
#include <asm/kvm_cove.h>
#include <asm/kvm_nacl.h>
#include <asm/kvm_cove_sbi.h>

static int kvm_riscv_cove_promote_to_tvm(struct kvm_vcpu *vcpu,
					 unsigned long fdt_address,
					 unsigned long tap_addr) {
	struct kvm_cove_tvm_context *tvmc;
	struct kvm_cpu_context *cntx;
	struct kvm_vcpu *target_vcpu;
	unsigned long target_vcpuid;
	void *nshmem = nacl_shmem();
	struct kvm_guest_timer *gt;
	int rc, gpr_id, offset;

	rc = kvm_riscv_cove_vm_single_step_init(vcpu->kvm);
	if (rc)
		goto exit;

	tvmc = vcpu->kvm->arch.tvmc;
	cntx = &vcpu->arch.guest_context;
	gt = &vcpu->kvm->arch.timer;

	/* Reset all but boot vcpu and preload VM's pages */
	kvm_for_each_vcpu(target_vcpuid, target_vcpu, vcpu->kvm) {
		kvm_arch_vcpu_postcreate(target_vcpu);
		target_vcpu->requests = 0;
	}

	for (gpr_id = 1; gpr_id < 32; gpr_id++) {
		offset = KVM_ARCH_GUEST_ZERO + gpr_id * sizeof(unsigned long);
		nacl_shmem_gpr_write_cove(nshmem, offset,
					  ((unsigned long *)cntx)[gpr_id]);
	}
	kvm_arch_vcpu_load(vcpu, smp_processor_id());
	rc = sbi_covh_tsm_promote_to_tvm(fdt_address, tap_addr, cntx->sepc+4,
					 &tvmc->tvm_guest_id);
	if (rc)
		goto vcpus_allocated;

	tvmc->finalized_done = true;
	gt->time_delta = nacl_shmem_csr_read(nshmem, CSR_HTIMEDELTA);
	kvm_info("CoVE Guest creation successful with guest id %lx\n", tvmc->tvm_guest_id);
	return 0;

vcpus_allocated:
	kvm_for_each_vcpu(target_vcpuid, target_vcpu, vcpu->kvm)
		kvm_riscv_cove_vcpu_destroy(vcpu);
	kvm_riscv_cove_vm_destroy(vcpu->kvm);

exit:
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
		return ret;

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
