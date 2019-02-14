/*
 * Copyright (c) 2019, Oxford Nanopore Technolgies Ltd.
 *
 * SPDX-License-Identifier: GPL-2.0
 */

#include <common.h>
#include <i2c.h>
#include "minion1c.h"

int tegra_board_init(void)
{
	board_fpga_add();
	return 0;
}
