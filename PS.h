#ifndef PS_H
#define PS_H

#define FUSE_USE_VERSION 26

#ifndef __APPLE__
#define _GNU_SOURCE
#define _XOPEN_SOURCE 700
#endif

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/xattr.h>

#include <iostream>

using namespace std;

#include "GlobalInfo.h"
#include "tool.h"

int ps_getattr(const char *path, struct stat *stbuf);
int ps_mkdir(const char *, mode_t);
int ps_unlink(const char *path);
int ps_rmdir(const char *);
int ps_rename(const char *path, const char *newPath);
int ps_link(const char *, const char *);
int ps_chmod(const char *path , mode_t mode);
int ps_chown(const char *path, uid_t uid, gid_t gid);
int ps_truncate(const char *path, off_t offset);
int ps_open(const char *path, struct fuse_file_info *fi);
int ps_read(const char *path, char *buf, size_t size, off_t offset,
            struct fuse_file_info *fi);
int ps_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
int ps_statfs(const char *path, struct statvfs *vfs);
int ps_release(const char *path, struct fuse_file_info *fi);
int ps_setxattr(const char *path, const char *name, const char *value, size_t size, int flags, uint32_t position);
int ps_getxattr(const char *path, const char *name, char *value, size_t size, uint32_t position);
int ps_listxattr(const char *path, char *list, size_t size);
int ps_removexattr(const char *path, const char *name);
int ps_opendir(const char *path, struct fuse_file_info *fi);
int ps_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
               off_t offset, struct fuse_file_info *fi);
int ps_releasedir(const char *path, struct fuse_file_info *fi);
void ps_destroy(void *);
int ps_create(const char *path , mode_t mode, struct fuse_file_info *fi);
int ps_utimens(const char *path, const struct timespec tv[2]);
int ps_fallocate(const char *path, int mode, off_t offset, off_t len, struct fuse_file_info *fi);

#endif // PS_H
