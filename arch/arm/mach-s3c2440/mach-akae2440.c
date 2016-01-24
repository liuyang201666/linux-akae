/* linux/arch/arm/mach-s3c2440/mach-akae2440.c
 *
 * Copyright (c) 2004,2005 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 * Copyright (c) 2009 akaedu
 *	weiqin <qinwei1998@hotmail.com> 
 *
 * Thanks to Dimity Andric and TomTom for the loan of an SMDK2440.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <mach/hardware.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/mach-types.h>

#include <asm/plat-s3c/regs-serial.h>
#include <mach/regs-gpio.h>
#include <mach/regs-lcd.h>

#include <mach/idle.h>
#include <mach/fb.h>

#include <asm/plat-s3c24xx/s3c2410.h>
#include <asm/plat-s3c24xx/s3c2440.h>
#include <asm/plat-s3c24xx/clock.h>
#include <asm/plat-s3c24xx/devs.h>
#include <asm/plat-s3c24xx/cpu.h>

#include <asm/plat-s3c24xx/common-smdk.h>
#include <mach/regs-mem.h>

#include <mach/regs-gpio.h>
#include <mach/leds-gpio.h>
#include <asm/plat-s3c/regs-serial.h>
#include <mach/fb.h>
#include <asm/plat-s3c/nand.h>
#include <asm/plat-s3c24xx/udc.h>
#include <mach/spi.h>
#include <mach/spi-gpio.h>

static struct map_desc akae2440_iodesc[] __initdata = {
	/* cs8900 IO Space map */
	{   .virtual    = 0xe0000000,
		.pfn        = __phys_to_pfn(S3C2410_CS3+0x01000000), 
		.length     = SZ_1M,
		.type       = MT_DEVICE 
	}
};

#define UCON S3C2410_UCON_DEFAULT | S3C2410_UCON_UCLK
#define ULCON S3C2410_LCON_CS8 | S3C2410_LCON_PNONE | S3C2410_LCON_STOPB
#define UFCON S3C2410_UFCON_RXTRIG8 | S3C2410_UFCON_FIFOMODE

static struct s3c2410_uartcfg akae2440_uartcfgs[] __initdata = {
	[0] = {
		.hwport	     = 0,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,
		.ufcon	     = 0x51,
	},
	[1] = {
		.hwport	     = 1,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,
		.ufcon	     = 0x51,
	},
	/* IR port */
	[2] = {
		.hwport	     = 2,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x43,
		.ufcon	     = 0x51,
	}
};

/* LCD driver info */
static struct s3c2410fb_display akae2440_lcd_cfg __initdata = {

	.lcdcon5	= S3C2410_LCDCON5_FRM565 |
			  S3C2410_LCDCON5_INVVLINE |
			  S3C2410_LCDCON5_INVVFRAME |
			  S3C2410_LCDCON5_PWREN |
			  S3C2410_LCDCON5_HWSWP,

	.type		= S3C2410_LCDCON1_TFT,

	.width		= 240,
	.height		= 320,

	.pixclock	= 166667, /* HCLK 60 MHz, divisor 10 */
	.xres		= 240,
	.yres		= 320,
	.bpp		= 16,
	.left_margin	= 20,
	.right_margin	= 8,
	.hsync_len	= 4,
	.upper_margin	= 8,
	.lower_margin	= 7,
	.vsync_len	= 4,
};

static struct s3c2410fb_mach_info akae2440_fb_info __initdata = {
	.displays	= &akae2440_lcd_cfg,
	.num_displays	= 1,
	.default_display = 0,
	.lpcsel	= ((0xCE6) & ~7) | 1<<4,
};

/* CS8900 */
static struct resource akae24xx_cs89x0_resources[] = {
	[0] = {
		.start	= 0x19000000,
		.end	= 0x19000000 + 16,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_EINT9,
		.end	= IRQ_EINT9,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device akae24xx_cs89x0 = {
	.name		= "cs89x0",
	.num_resources	= ARRAY_SIZE(akae24xx_cs89x0_resources),
	.resource	= akae24xx_cs89x0_resources,
};


static struct platform_device *akae2440_devices[] __initdata = {
	&s3c_device_usb,
	&s3c_device_lcd,
	&s3c_device_wdt,
	&s3c_device_i2c,
	&s3c_device_iis,
	&s3c_device_nand,
	&s3c_device_rtc,
	&akae24xx_cs89x0,
};


static struct s3c2410_nand_set akae2440_nand_sets[] = {
	[0] = {
		.name		= "akae2440-nand",
		.nr_chips	= 1,
		.nr_partitions	= 0,
		.partitions	= NULL,
	},
};

static struct s3c2410_platform_nand akae2440_nand_info = {
	.tacls		= 25,
	.twrph0		= 55,
	.twrph1		= 40,
	.nr_sets	= ARRAY_SIZE(akae2440_nand_sets),
	.sets		= akae2440_nand_sets,
};


static void __init akae2440_map_io(void)
{
	s3c24xx_init_io(akae2440_iodesc, ARRAY_SIZE(akae2440_iodesc));
	s3c24xx_init_clocks(12000000);
	s3c24xx_init_uarts(akae2440_uartcfgs, ARRAY_SIZE(akae2440_uartcfgs));
}

static void __init akae2440_machine_init(void)
{
	unsigned long bus_conf;
	unsigned long flags;
	unsigned long temp;

	/* Configure the LEDs (even if we have no LED support)*/
	s3c2410_gpio_cfgpin(S3C2410_GPF4, S3C2410_GPF4_OUTP);
	s3c2410_gpio_cfgpin(S3C2410_GPF5, S3C2410_GPF5_OUTP);
	s3c2410_gpio_cfgpin(S3C2410_GPF6, S3C2410_GPF6_OUTP);
	s3c2410_gpio_cfgpin(S3C2410_GPF7, S3C2410_GPF7_OUTP);

	s3c2410_gpio_setpin(S3C2410_GPF4, 1);
	s3c2410_gpio_setpin(S3C2410_GPF5, 1);
	s3c2410_gpio_setpin(S3C2410_GPF6, 1);
	s3c2410_gpio_setpin(S3C2410_GPF7, 1);

	s3c24xx_fb_set_platdata(&akae2440_fb_info);
	s3c_device_nand.dev.platform_data = &akae2440_nand_info;
	
	platform_add_devices(akae2440_devices, ARRAY_SIZE(akae2440_devices));
	
	local_irq_save(flags);
	bus_conf = __raw_readl(S3C2410_BWSCON);
	__raw_writel(  (bus_conf & ~0x0000F000) | 0x0000D000, S3C2410_BWSCON );
	__raw_writel(0x1f7c,S3C2410_BANKCON3);

	s3c2410_gpio_cfgpin(S3C2410_GPG1, S3C2410_GPG1_EINT9);
	temp = __raw_readl(S3C2410_EXTINT1);
	__raw_writel(  (temp & ~0x00000070) | 0x00000060, S3C2410_EXTINT1 );

	local_irq_restore(flags);
}

MACHINE_START(AKAE2440, "AKAE2440")
	.phys_io	= S3C2410_PA_UART,
	.io_pg_offst	= (((u32)S3C24XX_VA_UART) >> 18) & 0xfffc,
	.boot_params	= S3C2410_SDRAM_PA + 0x100,
	.init_irq	= s3c24xx_init_irq,
	.map_io		= akae2440_map_io,
	.init_machine	= akae2440_machine_init,
	.timer		= &s3c24xx_timer,
MACHINE_END
