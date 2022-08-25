/*
 * SPDX-License-Identifier:     GPL-2.0+
 *
 * Copyright (c) 2020 Rockchip Electronics Co., Ltd
 */

#ifndef __CONFIGS_RK3568_EVB_H
#define __CONFIGS_RK3568_EVB_H

#include <configs/rk3568_common.h>

#ifndef CONFIG_SPL_BUILD

#undef ROCKCHIP_DEVICE_SETTINGS
#define ROCKCHIP_DEVICE_SETTINGS \
		"stdin=serial,usbkbd\0" \
		"stdout=serial,vidconsole\0" \
		"stderr=serial,vidconsole\0"

#define CONFIG_SYS_MMC_ENV_DEV		0
#undef CONFIG_ENV_OFFSET
#undef CONFIG_ENV_SIZE
#define CONFIG_ENV_OFFSET                    0x003f8000
#define CONFIG_ENV_SIZE                         0x40000

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND RKIMG_BOOTCOMMAND

#ifdef CONFIG_USB_FUNCTION_DFU
#define CONFIG_SET_DFU_ALT_INFO
#endif

#define DFU_ALT_BOOT_EMMC \
	"gpt raw 0x0 0x20000;" \
	"loader raw 0x20000 0xE0000;"\
	"uboot part 0 1;" \
	"boot part 0 2;" \
	"rootfs part 0 3;" \
	"userdata part 0 4\0"

#ifdef CONFIG_ANDROID_AB
#define DFU_ALT_BOOT_MTD_A \
	"gpt raw 0x0 0x20000;" \
	"loader raw 0x20000 0xE0000;"\
	"vnvm part vnvm;" \
	"uboot part uboot;" \
	"boot raw 0x700000 0x600000\0"

#define DFU_ALT_BOOT_MTD_B \
	"gpt raw 0x0 0x20000;" \
	"loader raw 0x20000 0xE0000;"\
	"vnvm part vnvm;" \
	"uboot part uboot;" \
	"boot raw 0xd00000 0x600000\0"
#else
#define DFU_ALT_BOOT_MTD \
	"gpt raw 0x0 0x20000;" \
	"loader raw 0x20000 0xE0000;"\
	"vnvm part vnvm;" \
	"uboot part uboot;" \
	"boot part boot;" \
	"rootfs partubi rootfs;" \
	"userdata partubi userdata\0"


#define NCX_SLOT_SETTINGS \
        "bankz_sel=" \
        "if test ${bank} = a; then " \
				"setenv bootargs \"storagemedia=emmc androidboot.storagemedia=emmc androidboot.mode=normal root=/dev/mmcblk0p3 rw rootwait ; \" " \
                "echo Boot from Bank A;" \
       "elif test ${bank} = b; then " \
                "echo Boot from Bank B;" \
				"setenv bootargs \"storagemedia=emmc androidboot.storagemedia=emmc androidboot.mode=normal root=/dev/mmcblk0p6 rw rootwait ; \" " \
        "fi; \0"
		
#endif /* CONFIG_ANDROID_AB */
#endif /* CONFIG_SPL_BUILD */
#endif /* __CONFIGS_RK3568_EVB_H */
