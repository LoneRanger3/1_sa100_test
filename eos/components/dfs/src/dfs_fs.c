/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2005-02-22     Bernard      The first version.
 * 2010-06-30     Bernard      Optimize for RT-Thread RTOS
 * 2011-03-12     Bernard      fix the filesystem lookup issue.
 * 2017-11-30     Bernard      fix the filesystem_operation_table issue.
 * 2017-12-05     Bernard      fix the fs type search issue in mkfs.
 */

#include <dfs_fs.h>
#include <dfs_file.h>
#include "dfs_private.h"
#ifdef ARCH_LOMBO
#ifdef RT_USING_ISP
#include "lombo_isp.h"
#endif
#endif
/**
 * @addtogroup FsApi
 */
/*@{*/

/**
 * this function will register a file system instance to device file system.
 *
 * @param ops the file system instance to be registered.
 *
 * @return 0 on successful, -1 on failed.
 */
int dfs_register(const struct dfs_filesystem_ops *ops)
{
    int ret = RT_EOK;
    const struct dfs_filesystem_ops **empty = NULL;
    const struct dfs_filesystem_ops **iter;

    /* lock filesystem */
    dfs_lock();
    /* check if this filesystem was already registered */
    for (iter = &filesystem_operation_table[0];
            iter < &filesystem_operation_table[DFS_FILESYSTEM_TYPES_MAX]; iter ++)
    {
        /* find out an empty filesystem type entry */
        if (*iter == NULL)
            (empty == NULL) ? (empty = iter) : 0;
        else if (strcmp((*iter)->name, ops->name) == 0)
        {
            rt_set_errno(-EEXIST);
            ret = -1;
            break;
        }
    }

    /* save the filesystem's operations */
    if (empty == NULL)
    {
        rt_set_errno(-ENOSPC);
        LOG_E("There is no space to register this file system (%s).", ops->name);
        ret = -1;
    }
    else if (ret == RT_EOK)
    {
        *empty = ops;
    }

    dfs_unlock();
    return ret;
}

/**
 * this function will return the file system mounted on specified path.
 *
 * @param path the specified path string.
 *
 * @return the found file system or NULL if no file system mounted on
 * specified path
 */
struct dfs_filesystem *dfs_filesystem_lookup(const char *path)
{
    struct dfs_filesystem *iter;
    struct dfs_filesystem *fs = NULL;
    uint32_t fspath, prefixlen;

    prefixlen = 0;

    RT_ASSERT(path);

    /* lock filesystem */
    dfs_lock();

    /* lookup it in the filesystem table */
    for (iter = &filesystem_table[0];
            iter < &filesystem_table[DFS_FILESYSTEMS_MAX]; iter++)
    {
        if ((iter->path == NULL) || (iter->ops == NULL))
            continue;

        fspath = strlen(iter->path);
        if ((fspath < prefixlen)
            || (strncmp(iter->path, path, fspath) != 0))
            continue;

        /* check next path separator */
        if (fspath > 1 && (strlen(path) > fspath) && (path[fspath] != '/'))
            continue;

        fs = iter;
        prefixlen = fspath;
    }

    dfs_unlock();

    return fs;
}

/**
 * this function will return the mounted path for specified device.
 *
 * @param device the device object which is mounted.
 *
 * @return the mounted path or NULL if none device mounted.
 */
const char *dfs_filesystem_get_mounted_path(struct rt_device *device)
{
    const char *path = NULL;
    struct dfs_filesystem *iter;

    dfs_lock();
    for (iter = &filesystem_table[0];
            iter < &filesystem_table[DFS_FILESYSTEMS_MAX]; iter++)
    {
        /* fint the mounted device */
        if (iter->ops == NULL) continue;
        else if (iter->dev_id == device)
        {
            path = iter->path;
            break;
        }
    }

    /* release filesystem_table lock */
    dfs_unlock();

    return path;
}

/**
 * this function will fetch the partition table on specified buffer.
 *
 * @param part the returned partition structure.
 * @param buf the buffer contains partition table.
 * @param pindex the index of partition table to fetch.
 *
 * @return RT_EOK on successful or -RT_ERROR on failed.
 */
int dfs_filesystem_get_partition(struct dfs_partition *part,
                                 uint8_t         *buf,
                                 uint32_t        pindex)
{
#define DPT_ADDRESS     0x1be       /* device partition offset in Boot Sector */
#define DPT_ITEM_SIZE   16          /* partition item size */

    uint8_t *dpt;
    uint8_t type;

    RT_ASSERT(part != NULL);
    RT_ASSERT(buf != NULL);

    dpt = buf + DPT_ADDRESS + pindex * DPT_ITEM_SIZE;

    /* check if it is a valid partition table */
    if ((*dpt != 0x80) && (*dpt != 0x00))
        return -EIO;

    /* get partition type */
    type = *(dpt + 4);

#ifndef ARCH_LOMBO_N7V0
	if ((type == 0) || (type == 0xee))
		return -EIO;
#endif

    /* set partition information
     *    size is the number of 512-Byte */
    part->type = type;
    part->offset = *(dpt + 8) | *(dpt + 9) << 8 | *(dpt + 10) << 16 | *(dpt + 11) << 24;
    part->size = *(dpt + 12) | *(dpt + 13) << 8 | *(dpt + 14) << 16 | *(dpt + 15) << 24;

    rt_kprintf("found part[%d], begin: %d, size: ",
               pindex, part->offset * 512);
    if ((part->size >> 11) == 0)
        rt_kprintf("%d%s", part->size >> 1, "KB\n"); /* KB */
    else
    {
        unsigned int part_size;
        part_size = part->size >> 11;                /* MB */
        if ((part_size >> 10) == 0)
            rt_kprintf("%d.%d%s", part_size, (part->size >> 1) & 0x3FF, "MB\n");
        else
            rt_kprintf("%d.%d%s", part_size >> 10, part_size & 0x3FF, "GB\n");
    }

    return RT_EOK;
}

/**
 * this function will mount a file system on a specified path.
 *
 * @param device_name the name of device which includes a file system.
 * @param path the path to mount a file system
 * @param filesystemtype the file system type
 * @param rwflag the read/write etc. flag.
 * @param data the private data(parameter) for this file system.
 *
 * @return 0 on successful or -1 on failed.
 */
int dfs_mount(const char   *device_name,
              const char   *path,
              const char   *filesystemtype,
              unsigned long rwflag,
              const void   *data)
{
    const struct dfs_filesystem_ops **ops;
    struct dfs_filesystem *iter;
    struct dfs_filesystem *fs = NULL;
    char *fullpath = NULL;
    rt_device_t dev_id;

    /* open specific device */
    if (device_name == NULL)
    {
        /* which is a non-device filesystem mount */
        dev_id = NULL;
    }
    else if ((dev_id = rt_device_find(device_name)) == NULL)
    {
        /* no this device */
        rt_set_errno(-ENODEV);
        return -1;
    }

    /* find out the specific filesystem */
    dfs_lock();

    for (ops = &filesystem_operation_table[0];
            ops < &filesystem_operation_table[DFS_FILESYSTEM_TYPES_MAX]; ops++)
        if ((*ops != NULL) && (strcmp((*ops)->name, filesystemtype) == 0))
            break;

    dfs_unlock();

    if (ops == &filesystem_operation_table[DFS_FILESYSTEM_TYPES_MAX])
    {
        /* can't find filesystem */
        rt_set_errno(-ENODEV);
        return -1;
    }

    /* check if there is mount implementation */
    if ((*ops == NULL) || ((*ops)->mount == NULL))
    {
        rt_set_errno(-ENOSYS);
        return -1;
    }

    /* make full path for special file */
    fullpath = dfs_normalize_path(NULL, path);
    if (fullpath == NULL) /* not an abstract path */
    {
        rt_set_errno(-ENOTDIR);
        return -1;
    }

    /* Check if the path exists or not, raw APIs call, fixme */
    if ((strcmp(fullpath, "/") != 0) && (strcmp(fullpath, "/dev") != 0))
    {
        struct dfs_fd fd;

        if (dfs_file_open(&fd, fullpath, O_RDONLY | O_DIRECTORY) < 0)
        {
            rt_free(fullpath);
            rt_set_errno(-ENOTDIR);

            return -1;
        }
        dfs_file_close(&fd);
    }

    /* check whether the file system mounted or not  in the filesystem table
     * if it is unmounted yet, find out an empty entry */
    dfs_lock();

    for (iter = &filesystem_table[0];
            iter < &filesystem_table[DFS_FILESYSTEMS_MAX]; iter++)
    {
        /* check if it is an empty filesystem table entry? if it is, save fs */
        if (iter->ops == NULL)
            (fs == NULL) ? (fs = iter) : 0;
        /* check if the PATH is mounted */
        else if (strcmp(iter->path, path) == 0)
        {
            rt_set_errno(-EINVAL);
            goto err1;
        }
    }

    if ((fs == NULL) && (iter == &filesystem_table[DFS_FILESYSTEMS_MAX]))
    {
        rt_set_errno(-ENOSPC);
        LOG_E("There is no space to mount this file system (%s).", filesystemtype);
        goto err1;
    }

    /* register file system */
    fs->path   = fullpath;
    fs->ops    = *ops;
    fs->dev_id = dev_id;
    /* release filesystem_table lock */
    dfs_unlock();

    /* open device, but do not check the status of device */
    if (dev_id != NULL)
    {
        if (rt_device_open(fs->dev_id,
                           RT_DEVICE_OFLAG_RDWR) != RT_EOK)
        {
            /* The underlaying device has error, clear the entry. */
            dfs_lock();
            memset(fs, 0, sizeof(struct dfs_filesystem));

            goto err1;
        }
    }

    /* call mount of this filesystem */
    if ((*ops)->mount(fs, rwflag, data) < 0)
    {
        /* close device */
        if (dev_id != NULL)
            rt_device_close(fs->dev_id);

        /* mount failed */
        dfs_lock();
        /* clear filesystem table entry */
        memset(fs, 0, sizeof(struct dfs_filesystem));

        goto err1;
    }

    return 0;

err1:
    dfs_unlock();
    rt_free(fullpath);

    return -1;
}

/**
 * this function will unmount a file system on specified path.
 *
 * @param specialfile the specified path which mounted a file system.
 *
 * @return 0 on successful or -1 on failed.
 */
int dfs_unmount(const char *specialfile)
{
    char *fullpath;
    struct dfs_filesystem *iter;
    struct dfs_filesystem *fs = NULL;

    fullpath = dfs_normalize_path(NULL, specialfile);
    if (fullpath == NULL)
    {
        rt_set_errno(-ENOTDIR);

        return -1;
    }

    /* lock filesystem */
    dfs_lock();

    for (iter = &filesystem_table[0];
            iter < &filesystem_table[DFS_FILESYSTEMS_MAX]; iter++)
    {
        /* check if the PATH is mounted */
        if ((iter->path != NULL) && (strcmp(iter->path, fullpath) == 0))
        {
            fs = iter;
            break;
        }
    }

    if (fs == NULL ||
        fs->ops->unmount == NULL ||
        fs->ops->unmount(fs) < 0)
    {
        goto err1;
    }

    /* close device, but do not check the status of device */
    if (fs->dev_id != NULL)
        rt_device_close(fs->dev_id);

    if (fs->path != NULL)
        rt_free(fs->path);

    /* clear this filesystem table entry */
    memset(fs, 0, sizeof(struct dfs_filesystem));

    dfs_unlock();
    rt_free(fullpath);

    return 0;

err1:
    dfs_unlock();
    rt_free(fullpath);

    return -1;
}

/**
 * make a file system on the special device
 *
 * @param fs_name the file system name
 * @param device_name the special device name
 *
 * @return 0 on successful, otherwise failed.
 */
int dfs_mkfs(const char *fs_name, const char *device_name)
{
    int index;
    rt_device_t dev_id = NULL;

    /* check device name, and it should not be NULL */
    if (device_name != NULL)
        dev_id = rt_device_find(device_name);

    if (dev_id == NULL)
    {
        rt_set_errno(-ENODEV);
        LOG_E("Device (%s) was not found", device_name);
        return -1;
    }

    /* lock file system */
    dfs_lock();
    /* find the file system operations */
    for (index = 0; index < DFS_FILESYSTEM_TYPES_MAX; index ++)
    {
        if (filesystem_operation_table[index] != NULL &&
            strcmp(filesystem_operation_table[index]->name, fs_name) == 0)
            break;
    }
    dfs_unlock();

    if (index < DFS_FILESYSTEM_TYPES_MAX)
    {
        /* find file system operation */
        const struct dfs_filesystem_ops *ops = filesystem_operation_table[index];
        if (ops->mkfs == NULL)
        {
            LOG_E("The file system (%s) mkfs function was not implement", fs_name);
            rt_set_errno(-ENOSYS);
            return -1;
        }

        return ops->mkfs(dev_id);
    }

    LOG_E("File system (%s) was not found.", fs_name);

    return -1;
}
#ifdef ARCH_LOMBO
RTM_EXPORT(dfs_mkfs);
#endif

/**
 * this function will return the information about a mounted file system.
 *
 * @param path the path which mounted file system.
 * @param buffer the buffer to save the returned information.
 *
 * @return 0 on successful, others on failed.
 */
int dfs_statfs(const char *path, struct statfs *buffer)
{
    struct dfs_filesystem *fs;

    fs = dfs_filesystem_lookup(path);
    if (fs != NULL)
    {
        if (fs->ops->statfs != NULL)
            return fs->ops->statfs(fs, buffer);
    }

    return -1;
}

#ifdef RT_USING_DFS_MNTTABLE
int dfs_mount_table(void)
{
    int index = 0;

    while (1)
    {
        if (mount_table[index].path == NULL) break;

        if (dfs_mount(mount_table[index].device_name,
                      mount_table[index].path,
                      mount_table[index].filesystemtype,
                      mount_table[index].rwflag,
                      mount_table[index].data) != 0)
        {
            rt_kprintf("mount fs[%s] on %s failed.\n", mount_table[index].filesystemtype,
                       mount_table[index].path);
            return -RT_ERROR;
        }

        index ++;
    }
    return 0;
}
INIT_ENV_EXPORT(dfs_mount_table);
#endif

#ifdef ARCH_LOMBO
#ifdef ARCH_LOMBO_N7V0
	#include <cfg/config_api.h>
#else
	#include <boot_param.h>
	#define MAX_FSTAB_SIZE		4096
	#define FSTAB_ITEM		6
	#define FSTAB_PATH		"/etc/fstab"
#endif
#define EXT_SDCARD_PATH		"/mnt/sdcard"
#define EXT_UDISK_PATH		"/mnt/udisk"
#define MAX_MOUNT_CNT	6

static struct rt_mutex auto_mount_lock;
static char medium[8] = { 0 };

char *get_medium()
{
	return medium;
}

int init_auto_mount_lock(void)
{
#ifdef ARCH_LOMBO_N7V0
	const char *media;
	if (config_get_string("medium", "type", &media)) {
		DFS_ERR("config_get_string medium fail");
		return -EINVAL;
	}
#else
	char *media;
	media = boot_get_boot_type();
#endif
	if (rt_strlen(media) >= sizeof(medium)) {
		DFS_ERR("medium name too long(%d >= %d)",
			rt_strlen(media), sizeof(medium));
		return -EINVAL;
	}
	rt_memset(medium, 0, sizeof(medium));
	rt_strncpy(medium, media, rt_strlen(media));
	DFS_DBG("medium = %s", medium);

	if (rt_mutex_init(&auto_mount_lock, "auto_mount_lock",
		RT_IPC_FLAG_FIFO) != RT_EOK) {
		DFS_ERR("init auto_mount_lock failed");
		return -EINVAL;
	}

	/* dev and fs is not ready when init_prev */
	rt_mutex_take(&auto_mount_lock, RT_WAITING_FOREVER);

	return 0;
}
INIT_PREV_EXPORT(init_auto_mount_lock);

#ifndef ARCH_LOMBO_N7V0
static char *fstab[MAX_MOUNT_CNT][FSTAB_ITEM] = { 0 };

static unsigned int get_line_size(char *buf, int rest_size)
{
	unsigned int n = 0;

	RT_ASSERT(buf);
	RT_ASSERT(rest_size >= 0);

	while (rest_size != 0 && buf[n] != '\n') {
		n++;
		rest_size--;
	}

	return rest_size ? (n + 1) : n;
}

static int line_gets(char *buf, char **s, int line_size)
{
	int i;
	unsigned int n = 0;

	RT_ASSERT(buf);
	*s = NULL;

	for (i = 0; i < line_size; i++) {
		if (buf[i] == '#' || buf[i] == ';'
		|| buf[i] == '\r' || buf[i] == '\n')
			break;		/* line end */

		if (buf[i] != '\t' && buf[i] != ' ') {
			if (n++ == 0)
				*s = &(buf[i]);
		} else {
			if (n > 0)
				break;
		}
	}

	return n;
}

static void put_fstab(void)
{
	int i, j;

	for (i = 0; i < MAX_MOUNT_CNT; i++) {
		for (j = 0; j < FSTAB_ITEM; j++) {
			if (fstab[i][j]) {
				rt_free(fstab[i][j]);
				fstab[i][j] = NULL;
			}
		}
	}
}

static int get_fstab(void)
{
	char fstab_path[DFS_PATH_MAX];
	struct dfs_fd fd;
	int rest_size;
	char *readbuf;
	char *buf;
	int read_len;
	int res = 0;

	rt_sprintf(fstab_path, "%s_%s", FSTAB_PATH, get_medium());
	if (dfs_file_open(&fd, fstab_path, O_RDONLY) < 0) {
		DFS_ERR("can't find out %s", fstab_path);
		return -1;
	}

	if ((fd.size > MAX_FSTAB_SIZE) || (fd.size <= 0)) {
		DFS_ERR("fstab size(%d) invalid", fd.size);
		dfs_file_close(&fd);
		return -1;
	}
	readbuf = rt_malloc(fd.size);
	if (!readbuf) {
		DFS_ERR("rt_malloc %d byte fail", fd.size);
		dfs_file_close(&fd);
		return -1;
	}

	read_len = dfs_file_read(&fd, readbuf, fd.size);
	if (read_len != (int)(fd.size)) {
		DFS_ERR("read_size(%d) != fsize(%d)", read_len, fd.size);
		res = -1;
		goto out;
	}

	rt_memset(&(fstab[0][0]), 0, sizeof(fstab));

	rest_size = fd.size;
	buf = readbuf;
	while (rest_size > 0) {
		int i;
		char *line_buf;
		int line_size;
		int strlen;
		char *str = NULL;

		line_size = get_line_size(buf, rest_size);
		line_buf = buf;
		buf += line_size;
		rest_size -= line_size;

		strlen = line_gets(line_buf, &str, line_size);
		if ((strlen <= 0) || !str)
			continue;

		if ((res + 1) >= MAX_MOUNT_CNT) {
			DFS_ERR("fstab over MAX_MOUNT_CNT(%d)", MAX_MOUNT_CNT);
			goto out;
		}

		for (i = 0; (i < FSTAB_ITEM) && (strlen > 0) && str; i++) {
			int walklen = str - line_buf + strlen;

			line_buf += walklen;
			line_size -= walklen;

			fstab[res][i] = rt_malloc(strlen + 1);
			if (!fstab[res][i]) {
				put_fstab();
				res = -1;
				DFS_ERR("rt_malloc %d byte fail", strlen + 1);
				goto out;
			}
			rt_strncpy(fstab[res][i], str, strlen);
			fstab[res][i][strlen] = '\0';

			strlen = line_gets(line_buf, &str, line_size);
		}

		res++;
	}

out:
	rt_free(readbuf);
	dfs_file_close(&fd);

	return res;
}

static int mount_rootfs(void)
{
	int res;
	struct boot_dfs_mount_tbl *rootfs = _boot_para_get_rootfs();

	if ((rootfs->path[0] != '/') || (rootfs->path[1] != '\0')) {
		DFS_ERR("mount rootfs path invalid(%s)", rootfs->path);
		return -EINVAL;
	}

	res = dfs_mount(rootfs->dev_name, rootfs->path, rootfs->fs_type,
				rootfs->rwflag, rootfs->data);

	if (res) {
#ifdef ARCH_LOMBO
#ifdef RT_USING_ISP
		lb_isp_mq_send(LB_ISPMSG_ROOTFS_MOUNT_FAIL, NULL, 0, 0);
#endif
#endif
		DFS_ERR("mount rootfs fail(%d)", res);
		return res;
	}

#ifdef ARCH_LOMBO
#ifdef RT_USING_ISP
	lb_isp_mq_send(LB_ISPMSG_ROOTFS_MOUNT_OK, NULL, 0, 0);
#endif
#endif
	return 0;
}
#endif

static int auto_mount(const char *mnt_path, int umount)
{
	u32 i;
	u32 mount_cnt;
	u32 rwflag[MAX_MOUNT_CNT];
	u32 type[MAX_MOUNT_CNT];
	int res = 0;

	/* wait dev and fs to ready */
	rt_mutex_take(&auto_mount_lock, RT_WAITING_FOREVER);

#ifdef ARCH_LOMBO_N7V0
	const char *dev[MAX_MOUNT_CNT];
	const char *path[MAX_MOUNT_CNT];
	const char *fs[MAX_MOUNT_CNT];
	const char *data[MAX_MOUNT_CNT];

	/* get mount_cnt */
	if (config_get_u32("fstab", "cnt", &mount_cnt)) {
		DFS_ERR("config_get_u32 mount cnt fail");
		return -EINVAL;
	}
	if (mount_cnt > MAX_MOUNT_CNT) {
		DFS_ERR("invalid mount cnt(%d)", mount_cnt);
		return -EINVAL;
	}

	/* get dev */
	res = config_count_elems("fstab", "dev");
	if (res != mount_cnt) {
		DFS_ERR("dev_cnt(%d) != mount_cnt(%d)", res, mount_cnt);
		return -EINVAL;
	}
	config_get_string_array("fstab", "dev", dev, mount_cnt);

	/* get path */
	res = config_count_elems("fstab", "path");
	if (res != mount_cnt) {
		DFS_ERR("path_cnt(%d) != mount_cnt(%d)", res, mount_cnt);
		return -EINVAL;
	}
	config_get_string_array("fstab", "path", path, mount_cnt);

	/* get fs */
	res = config_count_elems("fstab", "fs");
	if (res != mount_cnt) {
		DFS_ERR("fs_cnt(%d) != mount_cnt(%d)", res, mount_cnt);
		return -EINVAL;
	}
	config_get_string_array("fstab", "fs", fs, mount_cnt);

	/* get rwflag */
	res = config_count_elems("fstab", "rwflag");
	if (res != mount_cnt) {
		DFS_ERR("rwflag_cnt(%d) != mount_cnt(%d)", res, mount_cnt);
		return -EINVAL;
	}
	config_get_u32_array("fstab", "rwflag", rwflag, mount_cnt);

	/* get data */
	res = config_count_elems("fstab", "data");
	if (res != mount_cnt) {
		DFS_ERR("data_cnt(%d) != mount_cnt(%d)", res, mount_cnt);
		return -EINVAL;
	}
	config_get_string_array("fstab", "data", data, mount_cnt);

	/* get type */
	res = config_count_elems("fstab", "type");
	if (res != mount_cnt) {
		DFS_ERR("type_cnt(%d) != mount_cnt(%d)", res, mount_cnt);
		return -EINVAL;
	}
	config_get_u32_array("fstab", "type", type, mount_cnt);

	res = 0;
#else
	char *dev[MAX_MOUNT_CNT];
	char *path[MAX_MOUNT_CNT];
	char *fs[MAX_MOUNT_CNT];
	char *data[MAX_MOUNT_CNT];

	res = get_fstab();
	if ((res <= 0) || (res > MAX_MOUNT_CNT)) {
		DFS_ERR("get_fstab fail(%d)", res);
		return -EINVAL;
	}

	mount_cnt = res;
	res = 0;
	for (i = 0; i < mount_cnt; i++) {
		int j;
		for (j = 0; j < FSTAB_ITEM; j++) {
			if (!fstab[i][j]) {
				DFS_ERR("invalid fstab (%d : %d)", i, j);
				res = -EINVAL;
				goto out;
			}
		}
		dev[i] = fstab[i][0];
		path[i] = fstab[i][1];
		fs[i] = fstab[i][2];
		data[i] = fstab[i][3];
		rwflag[i] = strtoul(fstab[i][4], NULL, 0);
		type[i] = strtoul(fstab[i][5], NULL, 0);

		if (!rt_strncmp(data[i], "null", rt_strlen("null")))
			data[i] = NULL;
	}
#endif

	for (i = 0; i < mount_cnt; i++) {
		/* external part mount */
		if (mnt_path) {
			if (!type[i])		/* internal partition */
				continue;
			if (rt_strncmp(mnt_path, path[i], rt_strlen(path[i])))
				continue;

			if (umount) {
				DFS_DBG("umount [%s] ", path[i]);
				res = dfs_unmount(path[i]);
			} else {
				DFS_DBG("mount [%s] as [%s] on [%s] ",
					dev[i], fs[i], path[i]);
				res = dfs_mount(dev[i], path[i], fs[i],
						rwflag[i], data[i]);
			}

			if (res) {
				DFS_ERR("fail");
				res = -RT_ERROR;
				break;
			} else {
				DFS_DBG("ok");
				break;
			}
		}

		/* internal part mount */
		if (type[i])			/* external partition */
			continue;

		if (dfs_mount(dev[i], path[i], fs[i], rwflag[i], data[i])) {
			DFS_ERR("mount [%s] as [%s] on [%s] fail",
				dev[i], fs[i], path[i]);

			DFS_DBG("format [%s] as [%s] ...",
				dev[i], fs[i]);
			if (dfs_mkfs(fs[i], dev[i])) {
				DFS_ERR("format [%s] as [%s] fail",
					dev[i], fs[i]);
				res = -RT_ERROR;
				break;
			} else
				DFS_DBG("format [%s] as [%s] ok",
					dev[i], fs[i]);

			if (dfs_mount(dev[i], path[i], fs[i],
				rwflag[i], data[i])) {
				DFS_ERR("mount [%s] as [%s] on [%s] fail",
					dev[i], fs[i], path[i]);
				res = -RT_ERROR;
				break;
			} else {
				DFS_DBG("mount [%s] as [%s] on [%s] ok",
					dev[i], fs[i], path[i]);
			}
		} else
			DFS_DBG("mount [%s] as [%s] on [%s] ok",
				dev[i], fs[i], path[i]);
	}
	rt_mutex_release(&auto_mount_lock);

#ifndef ARCH_LOMBO_N7V0
out:
	put_fstab();
#endif

	return res;
}

int internal_part_auto_mount(void)
{
#ifndef ARCH_LOMBO_N7V0
	if (rt_strncmp(medium, "sdcard", rt_strlen("sdcard"))) {
		int res = mount_rootfs();
		if (res) {
			rt_mutex_release(&auto_mount_lock);
			return res;
		}
	}
#endif

	rt_mutex_release(&auto_mount_lock);

	if (rt_strncmp(medium, "sdcard", rt_strlen("sdcard")))
		return auto_mount(0, 0);
	else
		return RT_EOK;
}
INIT_ENV_EXPORT(internal_part_auto_mount);

/*
 * It must be called in a separate thread, or may cause a deadlock.
 * @return 0 on successful or on failed.
 */
int sdcard_mount(void)
{
	if (!rt_strncmp(medium, "sdcard", rt_strlen("sdcard"))) {
#ifndef ARCH_LOMBO_N7V0
		int res;
		/* wait dev and fs to ready */
		rt_mutex_take(&auto_mount_lock, RT_WAITING_FOREVER);
		res = mount_rootfs();
		rt_mutex_release(&auto_mount_lock);
		if (res)
			return res;
		else
#endif
			return auto_mount(0, 0);
	} else
		return auto_mount(EXT_SDCARD_PATH, 0);
}
RTM_EXPORT(sdcard_mount);

int sdcard_umount(void)
{
	if (!rt_strncmp(medium, "sdcard", rt_strlen("sdcard")))
		return RT_EOK;
	else
		return auto_mount(EXT_SDCARD_PATH, 1);
}
RTM_EXPORT(sdcard_umount);

/*
 * It must be called in a separate thread, or may cause a deadlock.
 * @return 0 on successful or on failed.
 */
int udisk_mount(void)
{
	return auto_mount(EXT_UDISK_PATH, 0);
}

int udisk_umount(void)
{
	return auto_mount(EXT_UDISK_PATH, 1);
}
#endif

#ifdef RT_USING_FINSH
#include <finsh.h>
void mkfs(const char *fs_name, const char *device_name)
{
    dfs_mkfs(fs_name, device_name);
}
FINSH_FUNCTION_EXPORT(mkfs, make a file system);

int df(const char *path)
{
    int result;
    int minor = 0;
    long long cap;
    struct statfs buffer;

    int unit_index = 0;
    char *unit_str[] = {"KB", "MB", "GB"};

    result = dfs_statfs(path ? path : NULL, &buffer);
    if (result != 0)
    {
        rt_kprintf("dfs_statfs failed.\n");
        return -1;
    }

    cap = ((long long)buffer.f_bsize) * ((long long)buffer.f_bfree) / 1024LL;
    for (unit_index = 0; unit_index < 2; unit_index ++)
    {
        if (cap < 1024) break;

        minor = (cap % 1024) * 10 / 1024; /* only one decimal point */
        cap = cap / 1024;
    }

    rt_kprintf("disk free: %d.%d %s [ %d block, %d bytes per block ]\n",
               (unsigned long)cap, minor, unit_str[unit_index], buffer.f_bfree, buffer.f_bsize);
    return 0;
}
FINSH_FUNCTION_EXPORT(df, get disk free);

#ifdef ARCH_LOMBO
#ifdef FINSH_USING_MSH
#include "msh.h"
int lsfs(void)
{
	const struct dfs_filesystem_ops **ops;
	struct dfs_filesystem *iter;

	/* lock filesystem */
	dfs_lock();

	rt_kprintf("\nregistered filesystems(max_reg = %d):\n",
		DFS_FILESYSTEM_TYPES_MAX);

	/* filesystem operation table for registered fs */
	for (ops = &filesystem_operation_table[0];
	ops < &filesystem_operation_table[DFS_FILESYSTEM_TYPES_MAX]; ops++) {
		if (*ops == NULL)
			continue;

		rt_kprintf("%s\n", (*ops)->name);
	}

	rt_kprintf("\nmounted filesystems(max_mnt = %d):\n",
		DFS_FILESYSTEMS_MAX);
	rt_kprintf("%-20s%-40s%-60s\n",
		"dev_name", "mount_point", "filesystems");

	/* filesystem table for mounted fs */
	for (iter = &filesystem_table[0];
	iter < &filesystem_table[DFS_FILESYSTEMS_MAX]; iter++) {
		if ((iter->path == NULL) || (iter->ops == NULL))
			continue;

		rt_kprintf("%-20s%-40s%-60s\n",
		iter->dev_id->parent.name,
		iter->path,
		iter->ops->name);
	}

	dfs_unlock();

	return 0;
}
MSH_CMD_EXPORT(lsfs, list file system);

int cmd_mount(int argc, char **argv)
{
	int result = 0;

	if (argc == 5) {
		if (strcmp(argv[1], "-t") != 0) {
			rt_kprintf("Usage: mount -t type device path\n");
			return 0;
		}

		result = dfs_mount(argv[3], argv[4], argv[2], 0, NULL);
	} else {
		rt_kprintf("Usage: mount -t type device path\n");
		return 0;
	}

	if (result != RT_EOK)
		rt_kprintf("mount failed, result=%d\n", result);

	return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(cmd_mount, __cmd_mount, mount fs);

int cmd_umount(int argc, char **argv)
{
	int result = 0;

	if (argc == 2)
		result = dfs_unmount(argv[1]);
	else {
		rt_kprintf("Usage: umount path\n");
		return 0;
	}

	if (result != RT_EOK)
		rt_kprintf("umount failed, result=%d\n", result);

	return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(cmd_umount, __cmd_umount, umount fs);
#endif
#endif /* ARCH_LOMBO */

#endif

/* @} */
