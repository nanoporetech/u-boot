/*
 * Based on theadorable board 
 * MinION 1C uses Cyclone IV GX, but all timings are compatible 
 * with Stratix V. No cookie this time as only one FPGA is used.
 * Copyright (C) 2016 Stefan Roese <sr@denx.de>
 * Copyright (c) 2019 Oxford Nanopore Technolgies Ltd.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <altera.h>
#include <errno.h>
#include <dm.h>
#include <spi.h>
#include <asm/gpio.h>
#include <asm/io.h>
//#include <asm/arch/cpu.h>
//#include <asm/arch/soc.h>
#include <dt-bindings/gpio/tegra186-gpio.h>
#include "minion1c.h"

#define GPIO_NCONFIG TEGRA_MAIN_GPIO(I, 5)
#define GPIO_CONF_DONE TEGRA_MAIN_GPIO(I, 4)
#define GPIO_NSTATUS TEGRA_AON_GPIO(AA, 2)
#define SPI_BUS_FPGA_CONF 1
#define SPI_DEV_FPGA_CONF 0
#define SPI_FREQ_FPGA_CONF 5000000000

/*
 * FPGA programming support
 */
static int fpga_pre_fn(int cookie)
{
	int ret;

	/* Configure nCONFIG pin */
	/* Set to output */
	ret = gpio_request(GPIO_NCONFIG, "nCONFIG");
	if (ret < 0)
		return ret;
	gpio_direction_output(GPIO_NCONFIG, 1);

	/* Configure done pin */
	/* Set to input */
	ret = gpio_request(GPIO_CONF_DONE, "CONF_DONE");
	if (ret < 0)
		return ret;

	gpio_direction_input(GPIO_CONF_DONE);

	/* Configure nSTATUS pin */
	/* Set to input */
	ret = gpio_request(GPIO_NSTATUS, "nSTATUS");
	if (ret < 0)
		return ret;

	gpio_direction_input(GPIO_NSTATUS);

	return 0;
}

static int fpga_config_fn(int assert, int flush, int cookie)
{

	/*Check nSTATUS is high*/
	int nstatus_val;

	nstatus_val = gpio_get_value(GPIO_NSTATUS);
	if (IS_ERR_VALUE(nstatus_val))
		return nstatus_val;
	if (nstatus_val)
		return -ENODEV;


	if (assert)
		gpio_set_value(GPIO_NCONFIG, 1);
	else
		gpio_set_value(GPIO_NCONFIG, 0);

	return 0;
}

static int fpga_write_fn(const void *buf, size_t len, int flush, int cookie)
{

	/*
	 * Use SPI API to write data.
	 * This results in the fastest and easiest way to program the
	 * bitstream into the FPGA.
	 */

	int ret = 0;
	struct spi_slave *slave;
	char name[30], *str;
	struct udevice *dev;

	snprintf(name, sizeof(name), "generic_%d:%d", SPI_BUS_FPGA_CONF,\ 
	SPI_DEV_FPGA_CONF);
        str = strdup(name);
        if (!str)
                return -ENOMEM;

	ret = spi_get_bus_and_cs(SPI_BUS_FPGA_CONF, SPI_DEV_FPGA_CONF, \
	SPI_FREQ_FPGA_CONF, SPI_MODE_0, "spi_generic_drv", str, &dev, &slave);

	if (ret)
		return ret;

	ret = spi_claim_bus(slave);
	if (ret)
		goto done;
	
	/* Ignore data in as the MISO pin isn't connected*/
	ret = spi_xfer(slave, (len * 8), (uchar *)buf, NULL, \
	SPI_XFER_BEGIN | SPI_XFER_END);
	
	if (ret)
		printf("Error %d during SPI transaction\n", ret);

done:
	spi_release_bus(slave);

	return ret;
}

/* Returns the state of CONF_DONE Pin */
static int fpga_done_fn(int cookie)
{
	unsigned long ts;

	ts = get_timer(0);
	do {
		if (gpio_get_value(GPIO_CONF_DONE))
			return 0;
	} while (get_timer(ts) < 1000);

	/* timeout so return error */
	return -ENODEV;
}

static altera_board_specific_func stratixv_fns = {
	.pre = fpga_pre_fn,
	.config = fpga_config_fn,
	.write = fpga_write_fn,
	.done = fpga_done_fn,
};

static Altera_desc altera_fpga = {
	
	/* Family */
	Altera_StratixV,
	/* Interface type */
	passive_serial,
	/* No limitation as additional data will be ignored */
	-1,
	/* Device function table */
	(void *)&stratixv_fns,
	/* Base interface address specified in driver */
	NULL,
	/* No Cookie implementation */
	NULL
	
};

/* Add device descriptor to FPGA device table */
void board_fpga_add(void)
{
	fpga_init();
	fpga_add(fpga_altera, &altera_fpga);
}
