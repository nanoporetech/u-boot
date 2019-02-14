/*
 * (C) Copyright 2014 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

struct tegra_spi_platdata {
#ifdef CONFIG_TEGRA186
	struct reset_ctl	reset_ctl;
	struct clk		clk;
#else
	enum periph_id periph_id;
#endif
	int frequency;		/* Default clock frequency, -1 for none */
	ulong base;
	uint deactivate_delay_us;	/* Delay to wait after deactivate */
	bool en_le_bit;
	bool en_le_byte;
};
