/*
 * Copyright (c) 2013-2016, NVIDIA CORPORATION.
 *
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef _MINION_H 
#define _MINION_H

#include <linux/sizes.h>

#include "tegra186-common.h"

/* High-level configuration options */
#define CONFIG_TEGRA_BOARD_STRING	"ONT MinION_1C"

/* I2C */
#define CONFIG_SYS_I2C_TEGRA

/* SD/MMC */
#define CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_TEGRA_MMC

#define CONFIG_BOOTCOMMAND "run fpga_file_load; run fpga_load; run led_white; run distro_bootcmd"

/* Environment in eMMC, at the end of 2nd "boot sector" */
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV		0
#define CONFIG_SYS_MMC_ENV_PART		2
#define CONFIG_ENV_OFFSET		(-CONFIG_ENV_SIZE)

/* PCI host support */
#define CONFIG_PCI
#define CONFIG_PCI_PNP
#define CONFIG_CMD_PCI

#define BOARD_EXTRA_ENV_SETTINGS \
	"calculated_vars=kernel_addr_r fdt_addr_r scriptaddr pxefile_addr_r " \
		"ramdisk_addr_r fpgadata\0" \
	"kernel_addr_r_align=00200000\0" \
	"kernel_addr_r_offset=00080000\0" \
	"kernel_addr_r_size=02000000\0" \
	"kernel_addr_r_aliases=loadaddr\0" \
	"fdt_addr_r_align=00200000\0" \
	"fdt_addr_r_offset=00000000\0" \
	"fdt_addr_r_size=00200000\0" \
	"scriptaddr_align=00200000\0" \
	"scriptaddr_offset=00000000\0" \
	"scriptaddr_size=00200000\0" \
	"pxefile_addr_r_align=00200000\0" \
	"pxefile_addr_r_offset=00000000\0" \
	"pxefile_addr_r_size=00200000\0" \
	"ramdisk_addr_r_align=00200000\0" \
	"ramdisk_addr_r_offset=00000000\0" \
	"ramdisk_addr_r_size=02000000\0" \
/* Above were copied from p2771-0000 */  \
	"fpgadata_align=00100000\0" \
	"fpgadata_offset=00000000\0" \
	"fpgadata_size=00100000\0" \
	"led_en=i2c mw 0x60 0x14.1 0xff 4\0" \
	"led_on=i2c mw 0x60 0.1 0x21\0" \
	"led_pwm=i2c mw 0x60 0x2.1 0x20 0x10\0" \
	"led_white=i2c dev 2;run led_on;run led_en;run led_pwm\0" \
	"fpga_file=/boot/fpga/top.rbf\0" \
	"fpga_file_load=load mmc 0:1 $fpgadata $fpga_file\0" \
	"fpga_load=fpga load 0 $fpgadata $filesize\0" \

#include "tegra-common-post.h"

/* Crystal is 38.4MHz. clk_m runs at half that rate */
#define COUNTER_FREQUENCY	19200000

#undef CONFIG_NR_DRAM_BANKS
#define CONFIG_NR_DRAM_BANKS	(1024 + 2)

/* FPGA support (Cyclone IV) */
#define CONFIG_FPGA
#define CONFIG_FPGA_ALTERA
#define CONFIG_FPGA_STRATIX_V
#endif
