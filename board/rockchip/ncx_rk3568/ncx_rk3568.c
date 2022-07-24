/*
 * SPDX-License-Identifier:     GPL-2.0+
 *
 * (C) Copyright 2020 Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <dwc3-uboot.h>
#include <usb.h>
#include <dm.h>
#include <i2c_eeprom.h>
#include <netdev.h>

#define RK3568_MAC_EEPROM_DEBUG    0
#define AT24_MAC_OFFSET                      0x9A

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_USB_DWC3
static struct dwc3_device dwc3_device_data = {
	.maximum_speed = USB_SPEED_HIGH,
	.base = 0xfcc00000,
	.dr_mode = USB_DR_MODE_PERIPHERAL,
	.index = 0,
	.dis_u2_susphy_quirk = 1,
	.usb2_phyif_utmi_width = 16,
};

int usb_gadget_handle_interrupts(void)
{
	dwc3_uboot_handle_interrupt(0);
	return 0;
}

int board_usb_init(int index, enum usb_init_type init)
{
	return dwc3_uboot_init(&dwc3_device_data);
}
#endif

void ncx_print_eeprom_mac_addr(const u8 *addr)
{
		int i=0;
		printf("ncx mac address read from eeprom: ");
		for (i=0; i<6;i++)
			printf ("%x: ", ethaddr[i]);
		printf("\n");
}

static int get_ethaddr_from_eeprom(u8 *addr)
{
	int ret;
	int read_val;
	struct udevice *dev;

	ret = uclass_first_device_err(UCLASS_I2C_EEPROM, &dev);
	if (ret)
	{
#if RK3568_MAC_EEPROM_DEBUG
		printf("ncx mac eeprom : driver binding is failed\n");
#endif
		return ret;
	}
	return i2c_eeprom_read(dev, AT24_MAC_OFFSET, addr, 6);
	
}

int ncx_rk3568_board_late_init(void)
{
	u8 ethaddr[6];

	if (get_ethaddr_from_eeprom(ethaddr))
	{
		return 0;
	}
	if (is_valid_ethaddr(ethaddr))
	{
#if RK3568_MAC_EEPROM_DEBUG
		ncx_print_eeprom_mac_addr(ethaddr);
#endif
		eth_env_set_enetaddr("eth1addr", ethaddr);
	}

	return 0;
}

int rk_board_late_init(void)
{
     return ncx_rk3568_board_late_init();
}
