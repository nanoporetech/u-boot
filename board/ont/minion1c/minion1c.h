/*
 * Copyright (C) 2019 Oxford Nanopore Technologies Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _CONFIG_MINION1C_H
#define _CONFIG_MINION1C_H

/* FPGA programming support */
#define CONFIG_FPGA
#define CONFIG_FPGA_ALTERA
#define CONFIG_FPGA_STRATIX_V

void board_fpga_add(void);
#endif /* _CONFIG_MINION1C_H*/
