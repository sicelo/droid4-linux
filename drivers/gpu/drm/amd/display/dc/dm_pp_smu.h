/*
 * Copyright 2017 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: AMD
 *
 */

#ifndef DM_PP_SMU_IF__H
#define DM_PP_SMU_IF__H

/*
 * interface to PPLIB/SMU to setup clocks and pstate requirements on SoC
 */

typedef bool BOOLEAN;

enum pp_smu_ver {
	/*
	 * PP_SMU_INTERFACE_X should be interpreted as the interface defined
	 * starting from X, where X is some family of ASICs.  This is as
	 * opposed to interfaces used only for X.  There will be some degree
	 * of interface sharing between families of ASIcs.
	 */
	PP_SMU_UNSUPPORTED,
	PP_SMU_VER_RV,

	PP_SMU_VER_MAX
};

struct pp_smu {
	enum pp_smu_ver ver;
	const void *pp;

	/*
	 * interim extra handle for backwards compatibility
	 * as some existing functionality not yet implemented
	 * by ppsmu
	 */
	const void *dm;
};

enum pp_smu_status {
	PP_SMU_RESULT_UNDEFINED = 0,
	PP_SMU_RESULT_OK = 1,
	PP_SMU_RESULT_FAIL,
	PP_SMU_RESULT_UNSUPPORTED
};


#define PP_SMU_WM_SET_RANGE_CLK_UNCONSTRAINED_MIN 0x0
#define PP_SMU_WM_SET_RANGE_CLK_UNCONSTRAINED_MAX 0xFFFF

enum wm_type {
	WM_TYPE_PSTATE_CHG = 0,
	WM_TYPE_RETRAINING = 1,
};

/* This structure is a copy of WatermarkRowGeneric_t defined by smuxx_driver_if.h*/
struct pp_smu_wm_set_range {
	uint16_t min_fill_clk_mhz;
	uint16_t max_fill_clk_mhz;
	uint16_t min_drain_clk_mhz;
	uint16_t max_drain_clk_mhz;

	uint8_t wm_inst;
	uint8_t wm_type;
};

#define MAX_WATERMARK_SETS 4

struct pp_smu_wm_range_sets {
	unsigned int num_reader_wm_sets;
	struct pp_smu_wm_set_range reader_wm_sets[MAX_WATERMARK_SETS];

	unsigned int num_writer_wm_sets;
	struct pp_smu_wm_set_range writer_wm_sets[MAX_WATERMARK_SETS];
};

struct pp_smu_funcs_rv {
	struct pp_smu pp_smu;

	/* PPSMC_MSG_SetDisplayCount
	 * 0 triggers S0i2 optimization
	 */

	void (*set_display_count)(struct pp_smu *pp, int count);

	/* reader and writer WM's are sent together as part of one table*/
	/*
	 * PPSMC_MSG_SetDriverDramAddrHigh
	 * PPSMC_MSG_SetDriverDramAddrLow
	 * PPSMC_MSG_TransferTableDram2Smu
	 *
	 * */
	void (*set_wm_ranges)(struct pp_smu *pp,
			struct pp_smu_wm_range_sets *ranges);

	/* PPSMC_MSG_SetHardMinDcfclkByFreq
	 * fixed clock at requested freq, either from FCH bypass or DFS
	 */
	void (*set_hard_min_dcfclk_by_freq)(struct pp_smu *pp, int mhz);

	/* PPSMC_MSG_SetMinDeepSleepDcfclk
	 * when DF is in cstate, dcf clock is further divided down
	 * to just above given frequency
	 */
	void (*set_min_deep_sleep_dcfclk)(struct pp_smu *pp, int mhz);

	/* PPSMC_MSG_SetHardMinFclkByFreq
	 * FCLK will vary with DPM, but never below requested hard min
	 */
	void (*set_hard_min_fclk_by_freq)(struct pp_smu *pp, int mhz);

	/* PPSMC_MSG_SetHardMinSocclkByFreq
	 * Needed for DWB support
	 */
	void (*set_hard_min_socclk_by_freq)(struct pp_smu *pp, int mhz);

	/* PME w/a */
	void (*set_pme_wa_enable)(struct pp_smu *pp);
};

struct pp_smu_funcs {
	struct pp_smu ctx;
	union {
		struct pp_smu_funcs_rv rv_funcs;

	};
};

#endif /* DM_PP_SMU_IF__H */
