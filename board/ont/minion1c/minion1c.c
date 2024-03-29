/*
 * Copyright (c) 2019, Oxford Nanopore Technolgies Ltd.
 *
 * SPDX-License-Identifier: GPL-2.0
 */

#include <common.h>
#include <i2c.h>
#include "minion1c.h"
#include "max77620_init.h"

int tegra_board_init(void)
{
	struct udevice *dev;
        uchar val;
        int ret;

        /* Turn on MAX77620 LDO3 to 3.3V for SD card power */
        debug("%s: Set LDO3 for VDDIO_SDMMC_AP power to 3.3V\n", __func__);
        ret = i2c_get_chip_for_busnum(0, MAX77620_I2C_ADDR_7BIT, 1, &dev);
        if (ret) {
                printf("%s: Cannot find MAX77620 I2C chip\n", __func__);
                return ret;
	}
        /* 0xF2 for 3.3v, enabled: bit7:6 = 11 = enable, bit5:0 = voltage */
        val = 0xF2;
        ret = dm_i2c_write(dev, MAX77620_CNFG1_L3_REG, &val, 1);
        if (ret) {
                printf("i2c_write 0 0x3c 0x27 failed: %d\n", ret);
                return ret;
        }

	board_fpga_add();
	return 0;
}
