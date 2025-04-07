/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2016-09-28     armink       first version.
 */

#ifndef _SPI_FLASH_SFUD_H_
#define _SPI_FLASH_SFUD_H_

#include <rtthread.h>
#include <rtdevice.h>
#include "./sfud/inc/sfud.h"
#include "spi_flash.h"

#ifdef ARCH_LOMBO
/**
 * lombo_sfud_flash_probe - probe nor flash device by SFUD driver library.
 * @flash_name: pointer to the nor device's name.
 *
 * Return probed nor flash device, return RT_NULL if failed.
 */
rt_spi_flash_device_t lombo_sfud_flash_probe(const char *flash_name);

/**
 * lombo_sfud_get_device_priv - get sfud nor device's private data.
 * @flash_name: pointer to the nor device's name.
 *
 * Return pointer of sfud nor device's private data.
 */
sfud_flash *lombo_sfud_get_device_priv(rt_spi_flash_device_t rtt_dev);
#endif

/**
 * Probe SPI flash by SFUD(Serial Flash Universal Driver) driver library and though SPI device.
 *
 * @param spi_flash_dev_name the name which will create SPI flash device
 * @param spi_dev_name using SPI device name
 *
 * @return probed SPI flash device, probe failed will return RT_NULL
 */
rt_spi_flash_device_t rt_sfud_flash_probe(const char *spi_flash_dev_name, const char *spi_dev_name);

/**
 * Delete SPI flash device
 *
 * @param spi_flash_dev SPI flash device
 *
 * @return the operation status, RT_EOK on successful
 */
rt_err_t rt_sfud_flash_delete(rt_spi_flash_device_t spi_flash_dev);

/**
 * Find sfud flash device by SPI device name
 *
 * @param spi_dev_name using SPI device name
 *
 * @return sfud flash device if success, otherwise return RT_NULL
 */
sfud_flash_t rt_sfud_flash_find(const char *spi_dev_name);

/**
 * Find sfud flash device by flash device name
 *
 * @param flash_dev_name using flash device name
 *
 * @return sfud flash device if success, otherwise return RT_NULL
 */
sfud_flash_t rt_sfud_flash_find_by_dev_name(const char *flash_dev_name);

#endif /* _SPI_FLASH_SFUD_H_ */
