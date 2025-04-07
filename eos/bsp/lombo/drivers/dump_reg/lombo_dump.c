/*
 * lombo_dump.c - dump module realization
 *
 * Copyright (C) 2016-2018, LomboTech Co.Ltd.
 * Author: lomboswer <lomboswer@lombotech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "lombo_dump.h"

/*
 * addr_valid_judge - judge the validty of the input address
 * @param: none
 *
 * return: RT_EOK, valid; RT_ERROR, unvalid
 */
int addr_valid_judge(u32 addr)
{
	int ret = RT_ERROR;

	if ((addr % 0x04) == 0)
		ret = RT_EOK;
	return ret;
}

void printf_help(void)
{
	rt_kprintf("---------user guide--------------\n");
	rt_kprintf("1.read a register:\n");
	rt_kprintf("   lombo_dump dump address\n");
	rt_kprintf("   e.g.lombo_dump dump 0x400b000\n");
	rt_kprintf("2.read a group of registers:\n");
	rt_kprintf("   lombo_dump start_address end_address\n");
	rt_kprintf("   e.g.lombo_dump dump 0x400b000 0x400b020\n");
	rt_kprintf("3.write value to a register:\n");
	rt_kprintf("   lombo_dump write address value\n");
	rt_kprintf("   e.g.lombo_dump write 0x400b020 0x0f02\n");
	rt_kprintf("NOTE: 1.Address must be aligned in 4 bytes and hexadecimal.\n");
	rt_kprintf("      2.The value of input should be hexadecimal.\n");
}

/*
 * lombo_dump - dump or write the input address
 * @param: none
 *
 * return: RT_EOK, failed; RT_ERROR, succeed
 */
int lombo_dump(int argc, char **argv)
{
	int ret = RT_ERROR;
	u32 addr = 0, start_addr = 0, end_addr = 0;
	u32 dump_val = 0, bef_val = 0, chg_val = 0;
	u32 dump_addr = 0, cnt = 0;

	if (argc < 3) {
		if (argv[1] == RT_NULL)
			rt_kprintf("the arguments of input was NULL!\n");
		printf_help();
		goto err_exit;
	}

	if (strcmp(argv[1], "dump") == 0) {
		sscanf(argv[2], "%x", &addr);
#ifdef ARCH_LOMBO_N7V0
		addr |= VA_OFF_N7V0;
#elif defined ARCH_LOMBO_N7V1
		addr |= VA_OFF_N7V1;
#endif
		ret = addr_valid_judge(addr);
		if (RT_EOK != ret) {
			rt_kprintf("error address:0x%08x\n", addr);
			printf_help();
			goto err_exit;
		}
		/* dump value of address */
		if ((argc > 3)) {
			start_addr = addr;
			if (strcmp(argv[3], ",") == 0)
				sscanf(argv[4], "%x", &end_addr);
			else
				sscanf(argv[3], "%x", &end_addr);
#ifdef ARCH_LOMBO_N7V0
			end_addr |= VA_OFF_N7V0;
#elif defined ARCH_LOMBO_N7V1
			end_addr |= VA_OFF_N7V1;
#endif
			ret = addr_valid_judge(end_addr);
			if (RT_EOK != ret)
				rt_kprintf("error address:0x%08x\n", end_addr);

			dump_addr = start_addr;
			do {
				if (cnt % 4 == 0) {
					rt_kprintf("\n");
					rt_kprintf("0x%08x:", dump_addr);
				}
				rt_kprintf("   0x%08x", READREG32(dump_addr));
				dump_addr += 0x04;
				cnt++;
			} while (dump_addr <= end_addr);
			rt_kprintf("\n");
		}

		if (argc <= 3) {
			dump_val = READREG32(addr);
			rt_kprintf("\n");
			rt_kprintf("%-16s\t%-16s\n", "address", "dump_val");
			rt_kprintf("0x%08x      \t0x%08x\n", addr, dump_val);
		}
		ret = RT_EOK;

	/* write value to address */
	} else if (strcmp(argv[1], "write") == 0) {
		sscanf(argv[2], "%x", &addr);
		sscanf(argv[3], "%x", &chg_val);
#ifdef ARCH_LOMBO_N7V0
		addr |= VA_OFF_N7V0;
#elif defined ARCH_LOMBO_N7V1
		addr |= VA_OFF_N7V1;
#endif

		ret = addr_valid_judge(addr);
		if (RT_EOK != ret) {
			rt_kprintf("error address:0x%08x\n", addr);
			printf_help();
			goto err_exit;
		}

		bef_val = READREG32(addr);
		WRITEREG32(addr, chg_val);
		mdelay(200);
		dump_val = READREG32(addr);
		rt_kprintf("\n");
		rt_kprintf("%-16s\t%-16s\t%-16s\n",
					"address", "before_write", "after_write");
		rt_kprintf("0x%08x      \t0x%08x      \t0x%08x\n",
					addr, bef_val, dump_val);
		ret = RT_EOK;
	} else {
		rt_kprintf("the options was error!\n");
		printf_help();
		goto err_exit;
	}

err_exit:
	return ret;
}
MSH_CMD_EXPORT(lombo_dump, dump or write reg of lombo)
