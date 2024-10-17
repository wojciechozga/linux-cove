/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2015 Regents of the University of California
 * Copyright (c) 2020 Western Digital Corporation or its affiliates.
 */

#ifndef _ASM_RISCV_SBI_H
#define _ASM_RISCV_SBI_H

#include <linux/types.h>
#include <linux/cpumask.h>

#ifdef CONFIG_RISCV_SBI
enum sbi_ext_id {
#ifdef CONFIG_RISCV_SBI_V01
	SBI_EXT_0_1_SET_TIMER = 0x0,
	SBI_EXT_0_1_CONSOLE_PUTCHAR = 0x1,
	SBI_EXT_0_1_CONSOLE_GETCHAR = 0x2,
	SBI_EXT_0_1_CLEAR_IPI = 0x3,
	SBI_EXT_0_1_SEND_IPI = 0x4,
	SBI_EXT_0_1_REMOTE_FENCE_I = 0x5,
	SBI_EXT_0_1_REMOTE_SFENCE_VMA = 0x6,
	SBI_EXT_0_1_REMOTE_SFENCE_VMA_ASID = 0x7,
	SBI_EXT_0_1_SHUTDOWN = 0x8,
#endif
	SBI_EXT_BASE = 0x10,
	SBI_EXT_TIME = 0x54494D45,
	SBI_EXT_IPI = 0x735049,
	SBI_EXT_RFENCE = 0x52464E43,
	SBI_EXT_HSM = 0x48534D,
	SBI_EXT_SRST = 0x53525354,
	SBI_EXT_PMU = 0x504D55,
	SBI_EXT_DBCN = 0x4442434E,
	SBI_EXT_NACL = 0x4E41434C,
	SBI_EXT_COVH = 0x434F5648,
	SBI_EXT_COVI = 0x434F5649,
	SBI_EXT_COVG = 0x434F5647,

	/* Experimentals extensions must lie within this range */
	SBI_EXT_EXPERIMENTAL_START = 0x08000000,
	SBI_EXT_EXPERIMENTAL_END = 0x08FFFFFF,

	/* Vendor extensions must lie within this range */
	SBI_EXT_VENDOR_START = 0x09000000,
	SBI_EXT_VENDOR_END = 0x09FFFFFF,
};

enum sbi_ext_base_fid {
	SBI_EXT_BASE_GET_SPEC_VERSION = 0,
	SBI_EXT_BASE_GET_IMP_ID,
	SBI_EXT_BASE_GET_IMP_VERSION,
	SBI_EXT_BASE_PROBE_EXT,
	SBI_EXT_BASE_GET_MVENDORID,
	SBI_EXT_BASE_GET_MARCHID,
	SBI_EXT_BASE_GET_MIMPID,
};

enum sbi_ext_time_fid {
	SBI_EXT_TIME_SET_TIMER = 0,
};

enum sbi_ext_ipi_fid {
	SBI_EXT_IPI_SEND_IPI = 0,
};

enum sbi_ext_rfence_fid {
	SBI_EXT_RFENCE_REMOTE_FENCE_I = 0,
	SBI_EXT_RFENCE_REMOTE_SFENCE_VMA,
	SBI_EXT_RFENCE_REMOTE_SFENCE_VMA_ASID,
	SBI_EXT_RFENCE_REMOTE_HFENCE_GVMA_VMID,
	SBI_EXT_RFENCE_REMOTE_HFENCE_GVMA,
	SBI_EXT_RFENCE_REMOTE_HFENCE_VVMA_ASID,
	SBI_EXT_RFENCE_REMOTE_HFENCE_VVMA,
};

enum sbi_ext_hsm_fid {
	SBI_EXT_HSM_HART_START = 0,
	SBI_EXT_HSM_HART_STOP,
	SBI_EXT_HSM_HART_STATUS,
	SBI_EXT_HSM_HART_SUSPEND,
};

enum sbi_hsm_hart_state {
	SBI_HSM_STATE_STARTED = 0,
	SBI_HSM_STATE_STOPPED,
	SBI_HSM_STATE_START_PENDING,
	SBI_HSM_STATE_STOP_PENDING,
	SBI_HSM_STATE_SUSPENDED,
	SBI_HSM_STATE_SUSPEND_PENDING,
	SBI_HSM_STATE_RESUME_PENDING,
};

#define SBI_HSM_SUSP_BASE_MASK			0x7fffffff
#define SBI_HSM_SUSP_NON_RET_BIT		0x80000000
#define SBI_HSM_SUSP_PLAT_BASE			0x10000000

#define SBI_HSM_SUSPEND_RET_DEFAULT		0x00000000
#define SBI_HSM_SUSPEND_RET_PLATFORM		SBI_HSM_SUSP_PLAT_BASE
#define SBI_HSM_SUSPEND_RET_LAST		SBI_HSM_SUSP_BASE_MASK
#define SBI_HSM_SUSPEND_NON_RET_DEFAULT		SBI_HSM_SUSP_NON_RET_BIT
#define SBI_HSM_SUSPEND_NON_RET_PLATFORM	(SBI_HSM_SUSP_NON_RET_BIT | \
						 SBI_HSM_SUSP_PLAT_BASE)
#define SBI_HSM_SUSPEND_NON_RET_LAST		(SBI_HSM_SUSP_NON_RET_BIT | \
						 SBI_HSM_SUSP_BASE_MASK)

enum sbi_ext_srst_fid {
	SBI_EXT_SRST_RESET = 0,
};

enum sbi_srst_reset_type {
	SBI_SRST_RESET_TYPE_SHUTDOWN = 0,
	SBI_SRST_RESET_TYPE_COLD_REBOOT,
	SBI_SRST_RESET_TYPE_WARM_REBOOT,
};

enum sbi_srst_reset_reason {
	SBI_SRST_RESET_REASON_NONE = 0,
	SBI_SRST_RESET_REASON_SYS_FAILURE,
};

enum sbi_ext_pmu_fid {
	SBI_EXT_PMU_NUM_COUNTERS = 0,
	SBI_EXT_PMU_COUNTER_GET_INFO,
	SBI_EXT_PMU_COUNTER_CFG_MATCH,
	SBI_EXT_PMU_COUNTER_START,
	SBI_EXT_PMU_COUNTER_STOP,
	SBI_EXT_PMU_COUNTER_FW_READ,
};

union sbi_pmu_ctr_info {
	unsigned long value;
	struct {
		unsigned long csr:12;
		unsigned long width:6;
#if __riscv_xlen == 32
		unsigned long reserved:13;
#else
		unsigned long reserved:45;
#endif
		unsigned long type:1;
	};
};

#define RISCV_PMU_RAW_EVENT_MASK GENMASK_ULL(47, 0)
#define RISCV_PMU_RAW_EVENT_IDX 0x20000

/** General pmu event codes specified in SBI PMU extension */
enum sbi_pmu_hw_generic_events_t {
	SBI_PMU_HW_NO_EVENT			= 0,
	SBI_PMU_HW_CPU_CYCLES			= 1,
	SBI_PMU_HW_INSTRUCTIONS			= 2,
	SBI_PMU_HW_CACHE_REFERENCES		= 3,
	SBI_PMU_HW_CACHE_MISSES			= 4,
	SBI_PMU_HW_BRANCH_INSTRUCTIONS		= 5,
	SBI_PMU_HW_BRANCH_MISSES		= 6,
	SBI_PMU_HW_BUS_CYCLES			= 7,
	SBI_PMU_HW_STALLED_CYCLES_FRONTEND	= 8,
	SBI_PMU_HW_STALLED_CYCLES_BACKEND	= 9,
	SBI_PMU_HW_REF_CPU_CYCLES		= 10,

	SBI_PMU_HW_GENERAL_MAX,
};

/**
 * Special "firmware" events provided by the firmware, even if the hardware
 * does not support performance events. These events are encoded as a raw
 * event type in Linux kernel perf framework.
 */
enum sbi_pmu_fw_generic_events_t {
	SBI_PMU_FW_MISALIGNED_LOAD	= 0,
	SBI_PMU_FW_MISALIGNED_STORE	= 1,
	SBI_PMU_FW_ACCESS_LOAD		= 2,
	SBI_PMU_FW_ACCESS_STORE		= 3,
	SBI_PMU_FW_ILLEGAL_INSN		= 4,
	SBI_PMU_FW_SET_TIMER		= 5,
	SBI_PMU_FW_IPI_SENT		= 6,
	SBI_PMU_FW_IPI_RCVD		= 7,
	SBI_PMU_FW_FENCE_I_SENT		= 8,
	SBI_PMU_FW_FENCE_I_RCVD		= 9,
	SBI_PMU_FW_SFENCE_VMA_SENT	= 10,
	SBI_PMU_FW_SFENCE_VMA_RCVD	= 11,
	SBI_PMU_FW_SFENCE_VMA_ASID_SENT	= 12,
	SBI_PMU_FW_SFENCE_VMA_ASID_RCVD	= 13,

	SBI_PMU_FW_HFENCE_GVMA_SENT	= 14,
	SBI_PMU_FW_HFENCE_GVMA_RCVD	= 15,
	SBI_PMU_FW_HFENCE_GVMA_VMID_SENT = 16,
	SBI_PMU_FW_HFENCE_GVMA_VMID_RCVD = 17,

	SBI_PMU_FW_HFENCE_VVMA_SENT	= 18,
	SBI_PMU_FW_HFENCE_VVMA_RCVD	= 19,
	SBI_PMU_FW_HFENCE_VVMA_ASID_SENT = 20,
	SBI_PMU_FW_HFENCE_VVMA_ASID_RCVD = 21,
	SBI_PMU_FW_MAX,
};

/* SBI PMU event types */
enum sbi_pmu_event_type {
	SBI_PMU_EVENT_TYPE_HW = 0x0,
	SBI_PMU_EVENT_TYPE_CACHE = 0x1,
	SBI_PMU_EVENT_TYPE_RAW = 0x2,
	SBI_PMU_EVENT_TYPE_FW = 0xf,
};

/* SBI PMU event types */
enum sbi_pmu_ctr_type {
	SBI_PMU_CTR_TYPE_HW = 0x0,
	SBI_PMU_CTR_TYPE_FW,
};

/* Helper macros to decode event idx */
#define SBI_PMU_EVENT_IDX_OFFSET 20
#define SBI_PMU_EVENT_IDX_MASK 0xFFFFF
#define SBI_PMU_EVENT_IDX_CODE_MASK 0xFFFF
#define SBI_PMU_EVENT_IDX_TYPE_MASK 0xF0000
#define SBI_PMU_EVENT_RAW_IDX 0x20000
#define SBI_PMU_FIXED_CTR_MASK 0x07

#define SBI_PMU_EVENT_CACHE_ID_CODE_MASK 0xFFF8
#define SBI_PMU_EVENT_CACHE_OP_ID_CODE_MASK 0x06
#define SBI_PMU_EVENT_CACHE_RESULT_ID_CODE_MASK 0x01

#define SBI_PMU_EVENT_CACHE_ID_SHIFT 3
#define SBI_PMU_EVENT_CACHE_OP_SHIFT 1

#define SBI_PMU_EVENT_IDX_INVALID 0xFFFFFFFF

/* Flags defined for config matching function */
#define SBI_PMU_CFG_FLAG_SKIP_MATCH	(1 << 0)
#define SBI_PMU_CFG_FLAG_CLEAR_VALUE	(1 << 1)
#define SBI_PMU_CFG_FLAG_AUTO_START	(1 << 2)
#define SBI_PMU_CFG_FLAG_SET_VUINH	(1 << 3)
#define SBI_PMU_CFG_FLAG_SET_VSINH	(1 << 4)
#define SBI_PMU_CFG_FLAG_SET_UINH	(1 << 5)
#define SBI_PMU_CFG_FLAG_SET_SINH	(1 << 6)
#define SBI_PMU_CFG_FLAG_SET_MINH	(1 << 7)

/* Flags defined for counter start function */
#define SBI_PMU_START_FLAG_SET_INIT_VALUE (1 << 0)

/* Flags defined for counter stop function */
#define SBI_PMU_STOP_FLAG_RESET (1 << 0)

enum sbi_ext_dbcn_fid {
	SBI_EXT_DBCN_CONSOLE_WRITE = 0,
	SBI_EXT_DBCN_CONSOLE_READ = 1,
	SBI_EXT_DBCN_CONSOLE_WRITE_BYTE = 2,
};

enum sbi_ext_nacl_fid {
	SBI_EXT_NACL_PROBE_FEATURE = 0x0,
	SBI_EXT_NACL_SETUP_SHMEM = 0x1,
	SBI_EXT_NACL_SYNC_CSR = 0x2,
	SBI_EXT_NACL_SYNC_HFENCE = 0x3,
	SBI_EXT_NACL_SYNC_SRET = 0x4,
};

enum sbi_ext_nacl_feature {
	SBI_NACL_FEAT_SYNC_CSR = 0x0,
	SBI_NACL_FEAT_SYNC_HFENCE = 0x1,
	SBI_NACL_FEAT_SYNC_SRET = 0x2,
	SBI_NACL_FEAT_AUTOSWAP_CSR = 0x3,
};

#define SBI_NACL_SHMEM_ADDR_SHIFT	12
#define SBI_NACL_SHMEM_SCRATCH_OFFSET	0x0000
#define SBI_NACL_SHMEM_SCRATCH_SIZE	0x1000
#define SBI_NACL_SHMEM_SRET_OFFSET	0x0000
#define SBI_NACL_SHMEM_SRET_SIZE	0x0200
#define SBI_NACL_SHMEM_AUTOSWAP_OFFSET	(SBI_NACL_SHMEM_SRET_OFFSET + \
					 SBI_NACL_SHMEM_SRET_SIZE)
#define SBI_NACL_SHMEM_AUTOSWAP_SIZE	0x0080
#define SBI_NACL_SHMEM_UNUSED_OFFSET	(SBI_NACL_SHMEM_AUTOSWAP_OFFSET + \
					 SBI_NACL_SHMEM_AUTOSWAP_SIZE)
#define SBI_NACL_SHMEM_UNUSED_SIZE	0x0580
#define SBI_NACL_SHMEM_HFENCE_OFFSET	(SBI_NACL_SHMEM_UNUSED_OFFSET + \
					 SBI_NACL_SHMEM_UNUSED_SIZE)
#define SBI_NACL_SHMEM_HFENCE_SIZE	0x0780
#define SBI_NACL_SHMEM_DBITMAP_OFFSET	(SBI_NACL_SHMEM_HFENCE_OFFSET + \
					 SBI_NACL_SHMEM_HFENCE_SIZE)
#define SBI_NACL_SHMEM_DBITMAP_SIZE	0x0080
#define SBI_NACL_SHMEM_CSR_OFFSET	(SBI_NACL_SHMEM_DBITMAP_OFFSET + \
					 SBI_NACL_SHMEM_DBITMAP_SIZE)
#define SBI_NACL_SHMEM_CSR_SIZE		((__riscv_xlen / 8) * 1024)
#define SBI_NACL_SHMEM_SIZE		(SBI_NACL_SHMEM_CSR_OFFSET + \
					 SBI_NACL_SHMEM_CSR_SIZE)

#define SBI_NACL_SHMEM_CSR_INDEX(__csr_num)	\
		((((__csr_num) & 0xc00) >> 2) | ((__csr_num) & 0xff))

#define SBI_NACL_SHMEM_HFENCE_ENTRY_SZ		((__riscv_xlen / 8) * 4)
#define SBI_NACL_SHMEM_HFENCE_ENTRY_MAX		\
		(SBI_NACL_SHMEM_HFENCE_SIZE /	\
		 SBI_NACL_SHMEM_HFENCE_ENTRY_SZ)
#define SBI_NACL_SHMEM_HFENCE_ENTRY(__num)	\
		(SBI_NACL_SHMEM_HFENCE_OFFSET +	\
		 (__num) * SBI_NACL_SHMEM_HFENCE_ENTRY_SZ)
#define SBI_NACL_SHMEM_HFENCE_ENTRY_CTRL(__num)	\
		SBI_NACL_SHMEM_HFENCE_ENTRY(__num)
#define SBI_NACL_SHMEM_HFENCE_ENTRY_PNUM(__num)\
		(SBI_NACL_SHMEM_HFENCE_ENTRY(__num) + (__riscv_xlen / 8))
#define SBI_NACL_SHMEM_HFENCE_ENTRY_PCOUNT(__num)\
		(SBI_NACL_SHMEM_HFENCE_ENTRY(__num) + \
		 ((__riscv_xlen / 8) * 3))

#if __riscv_xlen == 32
#define SBI_NACL_SHMEM_HFENCE_CTRL_ASID_BITS	9
#define SBI_NACL_SHMEM_HFENCE_CTRL_VMID_BITS	7
#else
#define SBI_NACL_SHMEM_HFENCE_CTRL_ASID_BITS	16
#define SBI_NACL_SHMEM_HFENCE_CTRL_VMID_BITS	14
#endif
#define SBI_NACL_SHMEM_HFENCE_CTRL_VMID_SHIFT	\
				SBI_NACL_SHMEM_HFENCE_CTRL_ASID_BITS
#define SBI_NACL_SHMEM_HFENCE_CTRL_ASID_MASK	\
		((1UL << SBI_NACL_SHMEM_HFENCE_CTRL_ASID_BITS) - 1)
#define SBI_NACL_SHMEM_HFENCE_CTRL_VMID_MASK	\
		((1UL << SBI_NACL_SHMEM_HFENCE_CTRL_VMID_BITS) - 1)

#define SBI_NACL_SHMEM_HFENCE_CTRL_ORDER_BITS	7
#define SBI_NACL_SHMEM_HFENCE_CTRL_ORDER_SHIFT	(__riscv_xlen - 16)
#define SBI_NACL_SHMEM_HFENCE_CTRL_ORDER_MASK	\
		((1UL << SBI_NACL_SHMEM_HFENCE_CTRL_ORDER_BITS) - 1)
#define SBI_NACL_SHMEM_HFENCE_ORDER_BASE	12

#define SBI_NACL_SHMEM_HFENCE_CTRL_TYPE_BITS	4
#define SBI_NACL_SHMEM_HFENCE_CTRL_TYPE_SHIFT	(__riscv_xlen - 8)
#define SBI_NACL_SHMEM_HFENCE_CTRL_TYPE_MASK	\
		((1UL << SBI_NACL_SHMEM_HFENCE_CTRL_TYPE_BITS) - 1)

#define SBI_NACL_SHMEM_HFENCE_TYPE_GVMA		0x0
#define SBI_NACL_SHMEM_HFENCE_TYPE_GVMA_ALL	0x1
#define SBI_NACL_SHMEM_HFENCE_TYPE_GVMA_VMID	0x2
#define SBI_NACL_SHMEM_HFENCE_TYPE_GVMA_VMID_ALL 0x3
#define SBI_NACL_SHMEM_HFENCE_TYPE_VVMA		0x4
#define SBI_NACL_SHMEM_HFENCE_TYPE_VVMA_ALL	0x5
#define SBI_NACL_SHMEM_HFENCE_TYPE_VVMA_ASID	0x6
#define SBI_NACL_SHMEM_HFENCE_TYPE_VVMA_ASID_ALL 0x7

#define SBI_NACL_SHMEM_HFENCE_CTRL_PEND_BITS	1
#define SBI_NACL_SHMEM_HFENCE_CTRL_PEND_SHIFT	(__riscv_xlen - 1)
#define SBI_NACL_SHMEM_HFENCE_CTRL_PEND_MASK	\
		((1UL << SBI_NACL_SHMEM_HFENCE_CTRL_PEND_BITS) - 1)
#define SBI_NACL_SHMEM_HFENCE_CTRL_PEND		\
		(SBI_NACL_SHMEM_HFENCE_CTRL_PEND_MASK << \
		 SBI_NACL_SHMEM_HFENCE_CTRL_PEND_SHIFT)

#define SBI_NACL_SHMEM_AUTOSWAP_FLAG_HSTATUS	(1 << 0)
#define SBI_NACL_SHMEM_AUTOSWAP_HSTATUS		((__riscv_xlen / 8) * 1)

#define SBI_NACL_SHMEM_SRET_X(__i)		((__riscv_xlen / 8) * (__i))
#define SBI_NACL_SHMEM_SRET_X_LAST		31

/* SBI COVH extension data structures */
enum sbi_ext_covh_fid {
	SBI_EXT_COVH_TSM_GET_INFO = 0,
	SBI_EXT_COVH_TSM_CONVERT_PAGES,
	SBI_EXT_COVH_TSM_RECLAIM_PAGES,
	SBI_EXT_COVH_TSM_INITIATE_FENCE,
	SBI_EXT_COVH_TSM_LOCAL_FENCE,
	SBI_EXT_COVH_CREATE_TVM,
	SBI_EXT_COVH_FINALIZE_TVM,
	SBI_EXT_COVH_DESTROY_TVM,
	SBI_EXT_COVH_TVM_ADD_MEMORY_REGION,
	SBI_EXT_COVH_TVM_ADD_PGT_PAGES,
	SBI_EXT_COVH_TVM_ADD_MEASURED_PAGES,
	SBI_EXT_COVH_TVM_ADD_ZERO_PAGES,
	SBI_EXT_COVH_TVM_ADD_SHARED_PAGES,
	SBI_EXT_COVH_TVM_CREATE_VCPU,
	SBI_EXT_COVH_TVM_VCPU_RUN,
	SBI_EXT_COVH_TVM_INITIATE_FENCE,
	SBI_EXT_COVH_TVM_INVALIDATE_PAGES,
	SBI_EXT_COVH_TVM_VALIDATE_PAGES,
	SBI_EXT_COVH_TVM_PROMOTE_PAGE,
	SBI_EXT_COVH_TVM_DEMOTE_PAGE,
	SBI_EXT_COVH_TVM_REMOVE_PAGES,
	SBI_EXT_COVH_PROMOTE_TO_TVM,
};

enum sbi_ext_covi_fid {
	SBI_EXT_COVI_TVM_AIA_INIT,
	SBI_EXT_COVI_TVM_CPU_SET_IMSIC_ADDR,
	SBI_EXT_COVI_TVM_CONVERT_IMSIC,
	SBI_EXT_COVI_TVM_RECLAIM_IMSIC,
	SBI_EXT_COVI_TVM_CPU_BIND_IMSIC,
	SBI_EXT_COVI_TVM_CPU_UNBIND_IMSIC_BEGIN,
	SBI_EXT_COVI_TVM_CPU_UNBIND_IMSIC_END,
	SBI_EXT_COVI_TVM_CPU_INJECT_EXT_INTERRUPT,
	SBI_EXT_COVI_TVM_REBIND_IMSIC_BEGIN,
	SBI_EXT_COVI_TVM_REBIND_IMSIC_CLONE,
	SBI_EXT_COVI_TVM_REBIND_IMSIC_END,
};

enum sbi_cove_page_type {
	SBI_COVE_PAGE_4K,
	SBI_COVE_PAGE_2MB,
	SBI_COVE_PAGE_1GB,
	SBI_COVE_PAGE_512GB,
};

enum sbi_cove_tsm_state {
	/* TSM has not been loaded yet */
	TSM_NOT_LOADED,
	/* TSM has been loaded but not initialized yet */
	TSM_LOADED,
	/* TSM has been initialized and ready to run */
	TSM_READY,
};

struct sbi_cove_tsm_info {
	/* Current state of the TSM */
	enum sbi_cove_tsm_state tstate;

	/* TSM implementation identifier */
	uint32_t impl_id;

	/* Version of the loaded TSM */
	uint32_t version;

	/* Capabilities of the TSM */
	unsigned long capabilities;

	/* Number of 4K pages required per TVM */
	unsigned long tvm_pages_needed;

	/* Maximum VCPUs supported per TVM */
	unsigned long tvm_max_vcpus;

	/* Number of 4K pages each vcpu per TVM */
	unsigned long tvcpu_pages_needed;
};

struct sbi_cove_tvm_create_params {
	/* Root page directory for TVM's page table management */
	unsigned long tvm_page_directory_addr;
	/* Confidential memory address used to store TVM state information. Must be page aligned */
	unsigned long tvm_state_addr;
};

struct sbi_cove_tvm_aia_params {
	/* The base address is the address of the IMSIC with group ID, hart ID, and guest ID of 0 */
	uint64_t imsic_base_addr;
	/* The number of group index bits in an IMSIC address */
	uint32_t group_index_bits;
	/* The location of the group index in an IMSIC address. Must be >= 24i. */
	uint32_t group_index_shift;
	/* The number of hart index bits in an IMSIC address */
	uint32_t hart_index_bits;
	/* The number of guest index bits in an IMSIC address. Must be >= log2(guests/hart + 1) */
	uint32_t guest_index_bits;
	/* The number of guest interrupt files to be implemented per vCPU */
	uint32_t guests_per_hart;
};

/* SBI COVG extension data structures */
enum sbi_ext_covg_fid {
	SBI_EXT_COVG_ADD_MMIO_REGION,
	SBI_EXT_COVG_REMOVE_MMIO_REGION,
	SBI_EXT_COVG_SHARE_MEMORY,
	SBI_EXT_COVG_UNSHARE_MEMORY,
	SBI_EXT_COVG_ALLOW_EXT_INTERRUPT,
	SBI_EXT_COVG_DENY_EXT_INTERRUPT,
};

#define SBI_SPEC_VERSION_DEFAULT	0x1
#define SBI_SPEC_VERSION_MAJOR_SHIFT	24
#define SBI_SPEC_VERSION_MAJOR_MASK	0x7f
#define SBI_SPEC_VERSION_MINOR_MASK	0xffffff

/* SBI return error codes */
#define SBI_SUCCESS		0
#define SBI_ERR_FAILURE		-1
#define SBI_ERR_NOT_SUPPORTED	-2
#define SBI_ERR_INVALID_PARAM	-3
#define SBI_ERR_DENIED		-4
#define SBI_ERR_INVALID_ADDRESS	-5
#define SBI_ERR_ALREADY_AVAILABLE -6
#define SBI_ERR_ALREADY_STARTED -7
#define SBI_ERR_ALREADY_STOPPED -8

extern unsigned long sbi_spec_version;
struct sbiret {
	long error;
	long value;
};

void sbi_init(void);
struct sbiret sbi_ecall(int ext, int fid, unsigned long arg0,
			unsigned long arg1, unsigned long arg2,
			unsigned long arg3, unsigned long arg4,
			unsigned long arg5);

void sbi_console_putchar(int ch);
int sbi_console_getchar(void);
long sbi_get_mvendorid(void);
long sbi_get_marchid(void);
long sbi_get_mimpid(void);
void sbi_set_timer(uint64_t stime_value);
void sbi_shutdown(void);
void sbi_send_ipi(unsigned int cpu);
int sbi_remote_fence_i(const struct cpumask *cpu_mask);
int sbi_remote_sfence_vma(const struct cpumask *cpu_mask,
			   unsigned long start,
			   unsigned long size);

int sbi_remote_sfence_vma_asid(const struct cpumask *cpu_mask,
				unsigned long start,
				unsigned long size,
				unsigned long asid);
int sbi_remote_hfence_gvma(const struct cpumask *cpu_mask,
			   unsigned long start,
			   unsigned long size);
int sbi_remote_hfence_gvma_vmid(const struct cpumask *cpu_mask,
				unsigned long start,
				unsigned long size,
				unsigned long vmid);
int sbi_remote_hfence_vvma(const struct cpumask *cpu_mask,
			   unsigned long start,
			   unsigned long size);
int sbi_remote_hfence_vvma_asid(const struct cpumask *cpu_mask,
				unsigned long start,
				unsigned long size,
				unsigned long asid);
int sbi_probe_extension(int ext);

/* Check if current SBI specification version is 0.1 or not */
static inline int sbi_spec_is_0_1(void)
{
	return (sbi_spec_version == SBI_SPEC_VERSION_DEFAULT) ? 1 : 0;
}

/* Get the major version of SBI */
static inline unsigned long sbi_major_version(void)
{
	return (sbi_spec_version >> SBI_SPEC_VERSION_MAJOR_SHIFT) &
		SBI_SPEC_VERSION_MAJOR_MASK;
}

/* Get the minor version of SBI */
static inline unsigned long sbi_minor_version(void)
{
	return sbi_spec_version & SBI_SPEC_VERSION_MINOR_MASK;
}

/* Make SBI version */
static inline unsigned long sbi_mk_version(unsigned long major,
					    unsigned long minor)
{
	return ((major & SBI_SPEC_VERSION_MAJOR_MASK) <<
		SBI_SPEC_VERSION_MAJOR_SHIFT) | minor;
}

int sbi_err_map_linux_errno(int err);
#else /* CONFIG_RISCV_SBI */
static inline int sbi_remote_fence_i(const struct cpumask *cpu_mask) { return -1; }
static inline void sbi_init(void) {}
#endif /* CONFIG_RISCV_SBI */

unsigned long riscv_cached_mvendorid(unsigned int cpu_id);
unsigned long riscv_cached_marchid(unsigned int cpu_id);
unsigned long riscv_cached_mimpid(unsigned int cpu_id);

#if IS_ENABLED(CONFIG_SMP) && IS_ENABLED(CONFIG_RISCV_SBI)
void sbi_ipi_init(void);
#else
static inline void sbi_ipi_init(void) { }
#endif

#endif /* _ASM_RISCV_SBI_H */
