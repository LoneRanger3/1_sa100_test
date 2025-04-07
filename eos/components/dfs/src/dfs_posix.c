/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-05-27     Yi.qiu       The first version
 * 2018-02-07     Bernard      Change the 3rd parameter of open/fcntl/ioctl to '...'
 */

#include <dfs.h>
#include <dfs_posix.h>
#include "dfs_private.h"

#ifdef DFS_SHOW_ERROR_INFO

static void pr_fd(struct dfs_fd *d)
{
	if (d) {
		DFS_ERR("fname = %s", d->path);
		DFS_ERR("fsize = %d", d->size);
		DFS_ERR("fpos = %d", d->pos);
		DFS_ERR("fflags = 0x%08x", d->flags);
		DFS_ERR("ftype = 0x%04x", d->type);
		DFS_ERR("f_ref_count = %d", d->ref_count);
	}
}

#if 0
#define _DFS_ERR(format, ...)	\
	DFS_ERR("[%s][%d]: "format, __func__, __LINE__, ##__VA_ARGS__)

static inline void pr_dfs_err(int result, struct dfs_fd *d)
{
	_DFS_ERR("errno = %d", result);
	pr_fd(d);
	list_fd();
	lsfs();
}

static inline void pr_dfs_res(int result)
{
	_DFS_ERR("errno = %d", result);
	list_fd();
	lsfs();
}
#else
#define pr_dfs_err(result, d)	do {		\
	DFS_ERR("errno = %d", result);		\
	pr_fd(d);				\
	list_fd();				\
	lsfs();					\
} while (0)

#define pr_dfs_res(result)	do {		\
	DFS_ERR("errno = %d", result);		\
	list_fd();				\
	lsfs();					\
} while (0)
#endif

#define DFS_ERR_INFO()			pr_dfs_err(result, d)
#define DFS_ERR_RES()			pr_dfs_res(result)
#define DFS_ERR_PR(format, ...)		DFS_ERR(format, ##__VA_ARGS__)

#else

#define DFS_ERR_INFO()
#define DFS_ERR_RES()
#define DFS_ERR_PR(format, ...)

#endif

/**
 * @addtogroup FsPosixApi
 */

/*@{*/

/**
 * this function is a POSIX compliant version, which will open a file and
 * return a file descriptor according specified flags.
 *
 * @param file the path name of file.
 * @param flags the file open flags.
 *
 * @return the non-negative integer on successful open, others for failed.
 */
int open(const char *file, int flags, ...)
{
    int fd, result;
    struct dfs_fd *d;

    /* allocate a fd */
    fd = fd_new();
    if (fd < 0)
    {
        rt_set_errno(-ENOMEM);
#ifdef DFS_SHOW_ERROR_INFO
	DFS_ERR_PR("[%s][%s]: fd overflow !!!", rt_thread_self()->name, file);
	list_fd();
	lsfs();
#endif

        return -1;
    }
    d = fd_get(fd);

    result = dfs_file_open(d, file, flags);
    if (result < 0)
    {
#ifdef DFS_SHOW_ERROR_INFO
	if ((-ENOENT != result) && (-EEXIST != result))
		DFS_ERR_INFO();
#endif
        /* release the ref-count of fd */
        fd_put(d);
        fd_put(d);

        rt_set_errno(result);

        return -1;
    }

    /* release the ref-count of fd */
    fd_put(d);

    return fd;
}
RTM_EXPORT(open);

/**
 * this function is a POSIX compliant version, which will close the open
 * file descriptor.
 *
 * @param fd the file descriptor.
 *
 * @return 0 on successful, -1 on failed.
 */
int close(int fd)
{
    int result;
    struct dfs_fd *d;

    d = fd_get(fd);
    if (d == NULL)
    {
        rt_set_errno(-EBADF);
	DFS_ERR_PR("errno = %d", -EBADF);

        return -1;
    }

    result = dfs_file_close(d);
#ifdef DFS_SHOW_ERROR_INFO
	if (result)
		DFS_ERR_INFO();
#endif

    fd_put(d);

#ifndef LOMBO_FORCE_CLOSE
    if (result < 0)
    {
        rt_set_errno(result);

        return -1;
    }
#endif

    fd_put(d);

    return 0;
}
RTM_EXPORT(close);

/**
 * this function is a POSIX compliant version, which will read specified data
 * buffer length for an open file descriptor.
 *
 * @param fd the file descriptor.
 * @param buf the buffer to save the read data.
 * @param len the maximal length of data buffer
 *
 * @return the actual read data buffer length. If the returned value is 0, it
 * may be reach the end of file, please check errno.
 */
#if defined(RT_USING_NEWLIB) && defined(_EXFUN)
_READ_WRITE_RETURN_TYPE _EXFUN(read, (int fd, void *buf, size_t len))
#else
int read(int fd, void *buf, size_t len)
#endif
{
    int result;
    struct dfs_fd *d;

    /* get the fd */
    d = fd_get(fd);
    if (d == NULL)
    {
        rt_set_errno(-EBADF);
	DFS_ERR_PR("errno = %d", -EBADF);

        return -1;
    }

    result = dfs_file_read(d, buf, len);
    if (result < 0)
    {
	DFS_ERR_INFO();
        fd_put(d);
        rt_set_errno(result);

        return -1;
    }

    /* release the ref-count of fd */
    fd_put(d);

    return result;
}
RTM_EXPORT(read);

/**
 * this function is a POSIX compliant version, which will write specified data
 * buffer length for an open file descriptor.
 *
 * @param fd the file descriptor
 * @param buf the data buffer to be written.
 * @param len the data buffer length.
 *
 * @return the actual written data buffer length.
 */
#if defined(RT_USING_NEWLIB) && defined(_EXFUN)
_READ_WRITE_RETURN_TYPE _EXFUN(write, (int fd, const void *buf, size_t len))
#else
int write(int fd, const void *buf, size_t len)
#endif
{
    int result;
    struct dfs_fd *d;

    /* get the fd */
    d = fd_get(fd);
    if (d == NULL)
    {
        rt_set_errno(-EBADF);
	DFS_ERR_PR("errno = %d", -EBADF);

        return -1;
    }

    result = dfs_file_write(d, buf, len);
    if (result < 0)
    {
	DFS_ERR_INFO();
        fd_put(d);
        rt_set_errno(result);

        return -1;
    }

    /* release the ref-count of fd */
    fd_put(d);

    return result;
}
RTM_EXPORT(write);

/**
 * this function is a POSIX compliant version, which will seek the offset for
 * an open file descriptor.
 *
 * @param fd the file descriptor.
 * @param offset the offset to be seeked.
 * @param whence the directory of seek.
 *
 * @return the current read/write position in the file, or -1 on failed.
 */
off_t lseek(int fd, off_t offset, int whence)
{
    int result;
    struct dfs_fd *d;

    d = fd_get(fd);
    if (d == NULL)
    {
        rt_set_errno(-EBADF);
	DFS_ERR_PR("errno = %d", -EBADF);

        return -1;
    }

    switch (whence)
    {
    case SEEK_SET:
        break;

    case SEEK_CUR:
        offset += d->pos;
        break;

    case SEEK_END:
        offset += d->size;
        break;

    default:
        fd_put(d);
        rt_set_errno(-EINVAL);
	DFS_ERR_PR("errno = %d", -EINVAL);

        return -1;
    }

    if (offset < 0)
    {
        fd_put(d);
        rt_set_errno(-EINVAL);
	DFS_ERR_PR("errno = %d", -EINVAL);

        return -1;
    }
    result = dfs_file_lseek(d, offset);
    if (result < 0)
    {
	DFS_ERR_INFO();
        fd_put(d);
        rt_set_errno(result);

        return -1;
    }

    /* release the ref-count of fd */
    fd_put(d);

    return offset;
}
RTM_EXPORT(lseek);

/**
 * this function is a POSIX compliant version, which will rename old file name
 * to new file name.
 *
 * @param old the old file name.
 * @param new the new file name.
 *
 * @return 0 on successful, -1 on failed.
 *
 * note: the old and new file name must be belong to a same file system.
 */
int rename(const char *old, const char *new)
{
    int result;

    result = dfs_file_rename(old, new);
    if (result < 0)
    {
	DFS_ERR_PR("rename %s to %s fail", old, new);
	DFS_ERR_RES();
        rt_set_errno(result);

        return -1;
    }

    return 0;
}
RTM_EXPORT(rename);

/**
 * this function is a POSIX compliant version, which will unlink (remove) a
 * specified path file from file system.
 *
 * @param pathname the specified path name to be unlinked.
 *
 * @return 0 on successful, -1 on failed.
 */
int unlink(const char *pathname)
{
    int result;

    result = dfs_file_unlink(pathname);
    if (result < 0)
    {
	DFS_ERR_PR("unlink %s fail", pathname);
	DFS_ERR_RES();
        rt_set_errno(result);

        return -1;
    }

    return 0;
}
RTM_EXPORT(unlink);

#ifndef _WIN32 /* we can not implement these functions */
/**
 * this function is a POSIX compliant version, which will get file information.
 *
 * @param file the file name
 * @param buf the data buffer to save stat description.
 *
 * @return 0 on successful, -1 on failed.
 */
int stat(const char *file, struct stat *buf)
{
    int result;

    result = dfs_file_stat(file, buf);
    if (result < 0)
    {
#ifdef DFS_SHOW_ERROR_INFO
	if (-ENOENT != result) {
		DFS_ERR_PR("stat %s fail", file);
		DFS_ERR_RES();
	}
#endif
        rt_set_errno(result);

        return -1;
    }

    return result;
}
RTM_EXPORT(stat);

/**
 * this function is a POSIX compliant version, which will get file status.
 *
 * @param fildes the file description
 * @param buf the data buffer to save stat description.
 *
 * @return 0 on successful, -1 on failed.
 */
int fstat(int fildes, struct stat *buf)
{
    struct dfs_fd *d;

    /* get the fd */
    d = fd_get(fildes);
    if (d == NULL)
    {
        rt_set_errno(-EBADF);
	DFS_ERR_PR("errno = %d", -EBADF);

        return -1;
    }

    /* it's the root directory */
    buf->st_dev = 0;

    buf->st_mode = S_IFREG | S_IRUSR | S_IRGRP | S_IROTH |
                   S_IWUSR | S_IWGRP | S_IWOTH;
    if (d->type == FT_DIRECTORY)
    {
        buf->st_mode &= ~S_IFREG;
        buf->st_mode |= S_IFDIR | S_IXUSR | S_IXGRP | S_IXOTH;
    }

    buf->st_size    = d->size;
    buf->st_mtime   = 0;

    fd_put(d);

    return RT_EOK;
}
RTM_EXPORT(fstat);
#endif

/**
 * this function is a POSIX compliant version, which shall request that all data
 * for the open file descriptor named by fildes is to be transferred to the storage
 * device associated with the file described by fildes.
 *
 * @param fildes the file description
 *
 * @return 0 on successful completion. Otherwise, -1 shall be returned and errno
 * set to indicate the error.
 */
int fsync(int fildes)
{
    int ret;
    struct dfs_fd *d;

    /* get the fd */
    d = fd_get(fildes);
    if (d == NULL)
    {
        rt_set_errno(-EBADF);
	DFS_ERR_PR("errno = %d", -EBADF);
        return -1;
    }

    ret = dfs_file_flush(d);
#ifdef DFS_SHOW_ERROR_INFO
	if (ret) {
		int result = ret;
		DFS_ERR_INFO();
	}
#endif

    fd_put(d);
    return ret;
}
RTM_EXPORT(fsync);

/**
 * this function is a POSIX compliant version, which shall perform a variety of
 * control functions on devices.
 *
 * @param fildes the file description
 * @param cmd the specified command
 * @param data represents the additional information that is needed by this
 * specific device to perform the requested function.
 *
 * @return 0 on successful completion. Otherwise, -1 shall be returned and errno
 * set to indicate the error.
 */
int fcntl(int fildes, int cmd, ...)
{
    int ret = -1;
    struct dfs_fd *d;

    /* get the fd */
    d = fd_get(fildes);
    if (d)
    {
        void *arg;
        va_list ap;

        va_start(ap, cmd);
        arg = va_arg(ap, void *);
        va_end(ap);

        ret = dfs_file_ioctl(d, cmd, arg);
#ifdef DFS_SHOW_ERROR_INFO
	if (ret) {
		int result = ret;
		DFS_ERR_INFO();
	}
#endif
        fd_put(d);
    }
    else ret = -EBADF;

    if (ret < 0)
    {
        rt_set_errno(ret);
	DFS_ERR_PR("errno = %d", ret);
        ret = -1;
    }

    return ret;
}
RTM_EXPORT(fcntl);

/**
 * this function is a POSIX compliant version, which shall perform a variety of
 * control functions on devices.
 *
 * @param fildes the file description
 * @param cmd the specified command
 * @param data represents the additional information that is needed by this
 * specific device to perform the requested function.
 *
 * @return 0 on successful completion. Otherwise, -1 shall be returned and errno
 * set to indicate the error.
 */
int ioctl(int fildes, int cmd, ...)
{
    void *arg;
    va_list ap;

    va_start(ap, cmd);
    arg = va_arg(ap, void *);
    va_end(ap);

    /* we use fcntl for this API. */
    return fcntl(fildes, cmd, arg);
}
RTM_EXPORT(ioctl);

/**
 * this function is a POSIX compliant version, which will return the
 * information about a mounted file system.
 *
 * @param path the path which mounted file system.
 * @param buf the buffer to save the returned information.
 *
 * @return 0 on successful, others on failed.
 */
int statfs(const char *path, struct statfs *buf)
{
    int result;

    result = dfs_statfs(path, buf);
    if (result < 0)
    {
	DFS_ERR_PR("statfs %s fail", path);
	DFS_ERR_RES();
        rt_set_errno(result);

        return -1;
    }

    return result;
}
RTM_EXPORT(statfs);

/**
 * this function is a POSIX compliant version, which will make a directory
 *
 * @param path the directory path to be made.
 * @param mode
 *
 * @return 0 on successful, others on failed.
 */
int mkdir(const char *path, mode_t mode)
{
    int fd;
    struct dfs_fd *d;
    int result;

    fd = fd_new();
    if (fd == -1)
    {
        rt_set_errno(-ENOMEM);
#ifdef DFS_SHOW_ERROR_INFO
	DFS_ERR_PR("[%s][%s]: fd overflow !!!", rt_thread_self()->name, path);
	list_fd();
	lsfs();
#endif

        return -1;
    }

    d = fd_get(fd);

    result = dfs_file_open(d, path, O_DIRECTORY | O_CREAT);

    if (result < 0)
    {
#ifdef DFS_SHOW_ERROR_INFO
	if ((-ENOENT != result) && (-EEXIST != result))
		DFS_ERR_INFO();
#endif
        fd_put(d);
        fd_put(d);
        rt_set_errno(result);

        return -1;
    }

    dfs_file_close(d);
    fd_put(d);
    fd_put(d);

    return 0;
}
RTM_EXPORT(mkdir);

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(mkdir, create a directory);
#endif

/**
 * this function is a POSIX compliant version, which will remove a directory.
 *
 * @param pathname the path name to be removed.
 *
 * @return 0 on successful, others on failed.
 */
int rmdir(const char *pathname)
{
    int result;

    result = dfs_file_unlink(pathname);
    if (result < 0)
    {
	DFS_ERR_PR("rmdir %s fail", pathname);
	DFS_ERR_RES();
        rt_set_errno(result);

        return -1;
    }

    return 0;
}
RTM_EXPORT(rmdir);

/**
 * this function is a POSIX compliant version, which will open a directory.
 *
 * @param name the path name to be open.
 *
 * @return the DIR pointer of directory, NULL on open directory failed.
 */
DIR *opendir(const char *name)
{
    struct dfs_fd *d;
    int fd, result;
    DIR *t;

    t = NULL;

    /* allocate a fd */
    fd = fd_new();
    if (fd == -1)
    {
        rt_set_errno(-ENOMEM);
#ifdef DFS_SHOW_ERROR_INFO
	DFS_ERR_PR("[%s][%s]: fd overflow !!!", rt_thread_self()->name, name);
	list_fd();
	lsfs();
#endif

        return NULL;
    }
    d = fd_get(fd);

    result = dfs_file_open(d, name, O_RDONLY | O_DIRECTORY);
    if (result >= 0)
    {
        /* open successfully */
        t = (DIR *) rt_malloc(sizeof(DIR));
        if (t == NULL)
        {
            dfs_file_close(d);
            fd_put(d);
        }
        else
        {
            memset(t, 0, sizeof(DIR));

            t->fd = fd;
        }
        fd_put(d);

        return t;
    }

    /* open failed */
#ifdef DFS_SHOW_ERROR_INFO
	if ((-ENOENT != result) && (-EEXIST != result))
		DFS_ERR_INFO();
#endif
    fd_put(d);
    fd_put(d);
    rt_set_errno(result);

    return NULL;
}
RTM_EXPORT(opendir);

/**
 * this function is a POSIX compliant version, which will return a pointer
 * to a dirent structure representing the next directory entry in the
 * directory stream.
 *
 * @param d the directory stream pointer.
 *
 * @return the next directory entry, NULL on the end of directory or failed.
 */
struct dirent *readdir(DIR *d)
{
    int result;
    struct dfs_fd *fd;

    fd = fd_get(d->fd);
    if (fd == NULL)
    {
        rt_set_errno(-EBADF);
	DFS_ERR_PR("errno = %d", -EBADF);
        return NULL;
    }

    if (d->num)
    {
        struct dirent *dirent_ptr;
        dirent_ptr = (struct dirent *)&d->buf[d->cur];
        d->cur += dirent_ptr->d_reclen;
    }

    if (!d->num || d->cur >= d->num)
    {
        /* get a new entry */
        result = dfs_file_getdents(fd,
                                   (struct dirent *)d->buf,
                                   sizeof(d->buf) - 1);
        if (result <= 0)
        {
#ifdef DFS_SHOW_ERROR_INFO
		if (result < 0) {
			struct dfs_fd *d = fd;
			DFS_ERR_INFO();
		}
#endif
            fd_put(fd);
            rt_set_errno(result);

            return NULL;
        }

        d->num = result;
        d->cur = 0; /* current entry index */
    }

    fd_put(fd);

    return (struct dirent *)(d->buf + d->cur);
}
RTM_EXPORT(readdir);

/**
 * this function is a POSIX compliant version, which will return current
 * location in directory stream.
 *
 * @param d the directory stream pointer.
 *
 * @return the current location in directory stream.
 */
long telldir(DIR *d)
{
    struct dfs_fd *fd;
    long result;

    fd = fd_get(d->fd);
    if (fd == NULL)
    {
        rt_set_errno(-EBADF);
	DFS_ERR_PR("errno = %d", -EBADF);

        return 0;
    }

    result = fd->pos - d->num + d->cur;
    fd_put(fd);

    return result;
}
RTM_EXPORT(telldir);

/**
 * this function is a POSIX compliant version, which will set position of
 * next directory structure in the directory stream.
 *
 * @param d the directory stream.
 * @param offset the offset in directory stream.
 */
void seekdir(DIR *d, off_t offset)
{
    struct dfs_fd *fd;

    fd = fd_get(d->fd);
    if (fd == NULL)
    {
        rt_set_errno(-EBADF);
	DFS_ERR_PR("errno = %d", -EBADF);

        return ;
    }

    /* seek to the offset position of directory */
    if (dfs_file_lseek(fd, offset) >= 0)
        d->num = d->cur = 0;
    fd_put(fd);
}
RTM_EXPORT(seekdir);

/**
 * this function is a POSIX compliant version, which will reset directory
 * stream.
 *
 * @param d the directory stream.
 */
void rewinddir(DIR *d)
{
    struct dfs_fd *fd;

    fd = fd_get(d->fd);
    if (fd == NULL)
    {
        rt_set_errno(-EBADF);
	DFS_ERR_PR("errno = %d", -EBADF);

        return ;
    }

    /* seek to the beginning of directory */
    if (dfs_file_lseek(fd, 0) >= 0)
        d->num = d->cur = 0;
    fd_put(fd);
}
RTM_EXPORT(rewinddir);

/**
 * this function is a POSIX compliant version, which will close a directory
 * stream.
 *
 * @param d the directory stream.
 *
 * @return 0 on successful, -1 on failed.
 */
int closedir(DIR *d)
{
    int result;
    struct dfs_fd *fd;

    fd = fd_get(d->fd);
    if (fd == NULL)
    {
        rt_set_errno(-EBADF);
	DFS_ERR_PR("errno = %d", -EBADF);

        return -1;
    }

    result = dfs_file_close(fd);
#ifdef DFS_SHOW_ERROR_INFO
	if (result) {
		struct dfs_fd *d = fd;
		DFS_ERR_INFO();
	}
#endif
    fd_put(fd);

    fd_put(fd);
    rt_free(d);

    if (result < 0)
    {
        rt_set_errno(result);

        return -1;
    }
    else
        return 0;
}
RTM_EXPORT(closedir);

#ifdef DFS_USING_WORKDIR
/**
 * this function is a POSIX compliant version, which will change working
 * directory.
 *
 * @param path the path name to be changed to.
 *
 * @return 0 on successful, -1 on failed.
 */
int chdir(const char *path)
{
    char *fullpath;
    DIR *d;

    if (path == NULL)
    {
        dfs_lock();
        rt_kprintf("%s\n", working_directory);
        dfs_unlock();

        return 0;
    }

    if (strlen(path) > DFS_PATH_MAX)
    {
        rt_set_errno(-ENOTDIR);
	DFS_ERR_PR("errno = %d", -ENOTDIR);

        return -1;
    }

    fullpath = dfs_normalize_path(NULL, path);
    if (fullpath == NULL)
    {
        rt_set_errno(-ENOTDIR);
	DFS_ERR_PR("errno = %d", -ENOTDIR);

        return -1; /* build path failed */
    }

    dfs_lock();
    d = opendir(fullpath);
    if (d == NULL)
    {
        rt_free(fullpath);
        /* this is a not exist directory */
        dfs_unlock();

        return -1;
    }

    /* close directory stream */
    closedir(d);

    /* copy full path to working directory */
    strncpy(working_directory, fullpath, DFS_PATH_MAX);
    /* release normalize directory path name */
    rt_free(fullpath);

    dfs_unlock();

    return 0;
}
RTM_EXPORT(chdir);

#ifdef RT_USING_FINSH
FINSH_FUNCTION_EXPORT_ALIAS(chdir, cd, change current working directory);
#endif
#endif

/**
 * this function is a POSIX compliant version, which shall check the file named
 * by the pathname pointed to by the path argument for accessibility according
 * to the bit pattern contained in amode.
 *
 * @param path the specified file/dir path.
 * @param amode the value is either the bitwise-inclusive OR of the access
 * permissions to be checked (R_OK, W_OK, X_OK) or the existence test (F_OK).
 */
int access(const char *path, int amode)
{
    struct stat sb;
    if (stat(path, &sb) < 0)
        return -1; /* already sets errno */

    /* ignore R_OK,W_OK,X_OK condition */
    return 0;
}

/**
 * this function is a POSIX compliant version, which will return current
 * working directory.
 *
 * @param buf the returned current directory.
 * @param size the buffer size.
 *
 * @return the returned current directory.
 */
char *getcwd(char *buf, size_t size)
{
#ifdef DFS_USING_WORKDIR
    dfs_lock();
    strncpy(buf, working_directory, size);
    dfs_unlock();
#else
    rt_kprintf(NO_WORKING_DIR);
#endif

    return buf;
}
RTM_EXPORT(getcwd);

#ifdef ARCH_LOMBO
#if defined(RT_USING_FINSH) && defined(FINSH_USING_MSH)
#include <finsh.h>

#define IO_FALLOCATE	0xfa
static int fallocate(int argc, char **argv)
{
	if (argc == 3) {
		int res;
		int fsize = strtol(argv[2], NULL, 0);
		int fd;
		fd = open(argv[1], O_RDWR | O_CREAT);
		if (fd < 0) {
			DFS_ERR("open %s fail", argv[1]);
			return 0;
		}

		DFS_DBG("fallocate 0x%x byte to %s start ...",
			fsize, argv[1]);
		res = ioctl(fd, IO_FALLOCATE, &fsize);

#if 0
		{
			int _res;
			struct stat stat;
			rt_memset(&stat, 0, sizeof(stat));
			_res = fstat(fd, &stat);
			if (!_res)
				DFS_DBG("fsize = %u", stat.st_size);
			else
				DFS_DBG("fstat fail(%d)", _res);

			_res = lseek(fd, 0, SEEK_CUR);
			if (_res >= 0)
				DFS_DBG("fpos = %d", _res);
			else
				DFS_DBG("fpos fail(%d)", _res);
		}
#endif

		close(fd);

		if (!res) {
			DFS_DBG("fallocate successful");
		} else {
			DFS_DBG("fallocate fail(%d)", res);

			if (-EEXIST != res)
				unlink(argv[1]);
		}

		return 0;
	} else {
		rt_kprintf("Usage: fallocate file_path file_size\n");
		return 0;
	}
}
MSH_CMD_EXPORT(fallocate, file preallocate.);

#define IO_FTRUNCATE	0xfb
static int resize(int argc, char **argv)
{
	if (argc == 3) {
		int res;
		int fsize = strtol(argv[2], NULL, 0);
		int fd;
		fd = open(argv[1], O_RDWR | O_CREAT);
		if (fd < 0) {
			DFS_ERR("open %s fail", argv[1]);
			return 0;
		}

		DFS_DBG("resize %s to 0x%x byte start ...",
			argv[1], fsize);
		res = ioctl(fd, IO_FTRUNCATE, &fsize);

#if 0
		{
			int _res;
			struct stat stat;
			rt_memset(&stat, 0, sizeof(stat));
			_res = fstat(fd, &stat);
			if (!_res)
				DFS_DBG("fsize = %u", stat.st_size);
			else
				DFS_DBG("fstat fail(%d)", _res);

			_res = lseek(fd, 0, SEEK_CUR);
			if (_res >= 0)
				DFS_DBG("fpos = %d", _res);
			else
				DFS_DBG("fpos fail(%d)", _res);
		}
#endif

		close(fd);

		if (!res)
			DFS_DBG("resize successful");
		else
			DFS_DBG("resize fail(%d)", res);

		return 0;
	} else {
		rt_kprintf("Usage: resize file_path file_size\n");
		return 0;
	}
}
MSH_CMD_EXPORT(resize, file resize.);

static int ftruncate(int argc, char **argv)
{
	if (argc == 2) {
		int fd = open(argv[1], O_RDWR | O_TRUNC);
		if (fd < 0) {
			DFS_ERR("open %s fail", argv[1]);
			return 0;
		}

		close(fd);
		return 0;
	} else {
		rt_kprintf("Usage: ftruncate file_path\n");
		return 0;
	}
}
MSH_CMD_EXPORT(ftruncate, file truncate.);

static int fsize(int argc, char **argv)
{
	if (argc == 2) {
		int res;
		struct stat stat;
		int fd = open(argv[1], O_RDONLY);
		if (fd < 0) {
			DFS_ERR("open %s fail", argv[1]);
			return 0;
		}

		rt_memset(&stat, 0, sizeof(stat));
		res = fstat(fd, &stat);
		if (!res)
			DFS_DBG("fsize = %u", stat.st_size);
		else
			DFS_DBG("fstat fail(%d)", res);

		close(fd);
		return 0;
	} else {
		rt_kprintf("Usage: fsize file_path\n");
		return 0;
	}
}
MSH_CMD_EXPORT(fsize, file size.);

static int rename_cmd(int argc, char **argv)
{
	if (argc == 3) {
		if (rename(argv[1], argv[2]))
			DFS_ERR("rename fail(%d)", rt_get_errno());

		return 0;
	} else {
		rt_kprintf("Usage: rename_cmd oldname newname\n");
		return 0;
	}

}
MSH_CMD_EXPORT(rename_cmd, file rename.);

#endif	/* defined(RT_USING_FINSH) && defined(FINSH_USING_MSH) */
#endif	/* ARCH_LOMBO */

/* @} */
