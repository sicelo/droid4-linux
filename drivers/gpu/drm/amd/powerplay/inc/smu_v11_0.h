/*
 * Copyright 2019 Advanced Micro Devices, Inc.
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
 */
#ifndef __SMU_V11_0_H__
#define __SMU_V11_0_H__

#include "amdgpu_smu.h"

/* MP Apertures */
#define MP0_Public			0x03800000
#define MP0_SRAM			0x03900000
#define MP1_Public			0x03b00000
#define MP1_SRAM			0x03c00004

/* address block */
#define smnMP1_FIRMWARE_FLAGS		0x3010024
#define smnMP0_FW_INTF			0x30101c0
#define smnMP1_PUB_CTRL			0x3010b14

#define TEMP_RANGE_MIN			(0)
#define TEMP_RANGE_MAX			(80 * 1000)

struct smu_11_0_max_sustainable_clocks {
	uint32_t display_clock;
	uint32_t phy_clock;
	uint32_t pixel_clock;
	uint32_t uclock;
	uint32_t dcef_clock;
	uint32_t soc_clock;
};

struct smu_11_0_dpm_table {
	uint32_t    min;        /* MHz */
	uint32_t    max;        /* MHz */
};

struct smu_11_0_dpm_tables {
	struct smu_11_0_dpm_table        soc_table;
	struct smu_11_0_dpm_table        gfx_table;
	struct smu_11_0_dpm_table        uclk_table;
	struct smu_11_0_dpm_table        eclk_table;
	struct smu_11_0_dpm_table        vclk_table;
	struct smu_11_0_dpm_table        dclk_table;
	struct smu_11_0_dpm_table        dcef_table;
	struct smu_11_0_dpm_table        pixel_table;
	struct smu_11_0_dpm_table        display_table;
	struct smu_11_0_dpm_table        phy_table;
	struct smu_11_0_dpm_table        fclk_table;
};

struct smu_11_0_dpm_context {
	struct smu_11_0_dpm_tables  dpm_tables;
	uint32_t                    workload_policy_mask;
	uint32_t                    dcef_min_ds_clk;
};

enum smu_11_0_power_state {
	SMU_11_0_POWER_STATE__D0 = 0,
	SMU_11_0_POWER_STATE__D1,
	SMU_11_0_POWER_STATE__D3, /* Sleep*/
	SMU_11_0_POWER_STATE__D4, /* Hibernate*/
	SMU_11_0_POWER_STATE__D5, /* Power off*/
};

struct smu_11_0_power_context {
	uint32_t	power_source;
	uint8_t		in_power_limit_boost_mode;
	enum smu_11_0_power_state power_state;
};

void smu_v11_0_set_smu_funcs(struct smu_context *smu);

#endif
