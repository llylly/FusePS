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

#include "PS.h"

using namespace std;

#define PS "/PS"
#define FTP "/ftp"
#define MAIL "/mail"
#define LEARN "/learn"

int getattr(const char *path, struct stat *stbuf) {
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0x755;
        stbuf->st_nlink = 6;
    }
    if (strstr(path, PS) == path) {
        return ps_getattr(path, stbuf);
    }
    if (strstr(path, FTP) == path) {
        //
    }
    if (strstr(path, MAIL) == path) {
        //
    }
    if (strstr(path, LEARN) == path) {
        //
    }
    return -EPERM;
}

int mkdir(const char *path , mode_t mode) {
    if (strcmp(path, "/") == 0) {
        return -EPERM;
    }
    if (strstr(path, PS) == path) {
        return ps_mkdir(path, mode);
    }
    if (strstr(path, FTP) == path) {
        //
    }
    if (strstr(path, MAIL) == path) {
        //
    }
    if (strstr(path, LEARN) == path) {
        //
    }
    return -EPERM;
}

int unlink(const char *path) {
    if (strcmp(path, "/") == 0) {
        return -EPERM;
    }
    if (strstr(path, PS) == path) {
        return ps_unlink(path);
    }
    if (strstr(path, FTP) == path) {
        //
    }
    if (strstr(path, MAIL) == path) {
        //
    }
    if (strstr(path, LEARN) == path) {
        //
    }
    return -EPERM;
}

static struct fuse_operations global_oper = {
    .getattr = getattr,
    //.readlink
    //.mknod
    .mkdir = ps_mkdir,
    .unlink = ps_unlink,
    .rmdir = ps_rmdir,
    //.symlink
    .rename = ps_rename,
    .link = ps_link,
    .chmod = ps_chmod,
    .chown = ps_chown,
    .truncate = ps_truncate,
    .open = ps_open,
    .read = ps_read,
    .write = ps_write,
    .statfs = ps_statfs,
    //.flush
    .release = ps_release,
    //.fsync
    .setxattr = ps_setxattr,
    .getxattr = ps_getxattr,
    .listxattr = ps_listxattr,
    //.removexattr
    .opendir = ps_opendir,
    .readdir = ps_readdir,
    .releasedir = ps_releasedir,
    //.fsyncdir
    //.init
    .destroy = ps_destroy,
    //.access
    .create = ps_create,
    //.lock
    .utimens = ps_utimens,
    //.bmap
    //.ioctl
    //.poll
    //.write_buf
    //.read_buf
    //.flock
    .fallocate = ps_fallocate
};

int main(int argc, char *argv[]) {
    GlobalInfo *globalInfo = new GlobalInfo();
    if (globalInfo->psStatus->initBuffer(&argc, &argv)) {
        cerr << "Buffer init error." << endl;
        return 0;
    }

    return fuse_main(argc, argv, &global_oper, (void*)globalInfo);
}
