
#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <cstdlib>

#include <iostream>

using namespace std;

#include "GlobalInfo.h"
#include "tool.h"

int ps_getattr(const char *path, struct stat *stbuf) {

    int res = 0;
    bool find = false;

    PSStatus *status = getPSStatus();


    if ((strcmp(path, "/") == 0) || (strcmp(path, "/PS") == 0)) {
        find = true;
        struct stat *source = genPSFolderStat();
        memcpy((void*)stbuf, (void*)source, sizeof(struct stat));
        delete source;
    }
    for (int i=0; i<status->foldersN; ++i) {
        if (strcmp(path, status->foldersPath[i]) == 0) {
            find = true;
            struct stat *source = genPSFolderStat();
            memcpy((void*)stbuf, (void*)source, sizeof(struct stat));
            delete source;
        }
    }
    for (int i=0; i<status->configsN; ++i) {
        if (strcmp(path, status->configsPath[i]) == 0) {
            find = true;
            struct stat *source = genPSConfigFileStat();
            memcpy((void*)stbuf, (void*)source, sizeof(struct stat));
            delete source;
        }
    }

    /* To be filled: if equals to a img file */

    if (!find)
        res = -ENOENT;

    return res;
}

int ps_readlink(const char *, char *, size_t) {

}

int ps_mkdir(const char *, mode_t) {

}

int ps_unlink(const char *) {

}

int ps_rmdir(const char *) {

}

int ps_symlink(const char *, const char *) {

}

int ps_rename(const char *, const char *) {

}

int ps_link(const char *, const char *) {

}

int ps_open(const char *path, struct fuse_file_info *fi) {
    bool legal = false;

    if (strcmp(path, "/") == 0)
        legal = true;

    if (strcmp(path, "/PS") == 0)
        legal = true;

    PSStatus *status = getPSStatus();
    for (int i=0; i<status->foldersN; ++i) {
        if (strcmp(path, status->foldersPath[i]) == 0)
            legal = true;
    }
    for (int i=0; i<status->configsN; ++i) {
        if (strcmp(path, status->configsPath[i]) == 0)
            legal = true;
    }

    if (!legal) {
        /* To be filled: real pics open */
    }

    return 0;
}

int ps_read(const char *path, char *buf, size_t size, off_t offset,
            struct fuse_file_info *fi) {
    PSStatus *status = getPSStatus();

    for (int i=0; i<status->configsN; ++i) {
        if (strcmp(path, status->configsPath[i]) == 0) {
            /* To be filled */
            memset(buf, 'A', size);
        }
    }

    /* To be filled: real pics read */

    return size;
}

int ps_write(const char *, const char *, size_t, off_t, struct fuse_file_info *) {

}

int ps_statfs(const char *, struct statvfs *) {

}

int ps_release(const char *, struct fuse_file_info *) {

}

int ps_opendir(const char *, struct fuse_file_info *) {

}

int ps_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
               off_t offset, struct fuse_file_info *fi) {
    PSStatus *status = getPSStatus();

    bool find = false;

    if (strcmp(path, "/") == 0) {
        find = true;
        filler(buf, ".", NULL, 0);
        filler(buf, "..", NULL, 0);
        filler(buf, "PS", genPSFolderStat(), 0);
    }
    if (strcmp(path, "/PS") == 0) {
        find = true;
        filler(buf, ".", NULL, 0);
        filler(buf, "..", NULL, 0);
        for (int i=0; i<status->foldersN; ++i)
            filler(buf, status->foldersName[i], genPSFolderStat(), 0);
    }
    for (int i=0; i<status->foldersN; ++i)
        if (strcmp(path, status->foldersPath[i]) == 0) {
            find = true;
            filler(buf, ".", NULL, 0);
            filler(buf, "..", NULL, 0);
            if (strstr(path, "Graying") == NULL) {
                filler(buf, "config.txt", genPSConfigFileStat(), 0);
            }
        }

    if (!find)
        return -ENOENT;

    return 0;
}

int ps_releasedir(const char *, struct fuse_file_info *) {

}

void ps_destroy(void *) {

}

int ps_create(const char *, mode_t, struct fuse_file_info *) {

}

int ps_write_buf(const char *, struct fuse_bufvec *buf, off_t off, struct fuse_file_info *) {

}

int ps_read_buf(const char *, struct fuse_bufvec **bufp, size_t size, off_t off, struct fuse_file_info *) {

}

int ps_fallocate(const char *, int, off_t, off_t, struct fuse_file_info *) {

}

static struct fuse_operations global_oper = {
    .getattr = ps_getattr,
    //.readlink = ps_readlink,
    //.mknode
    //.mkdir = ps_mkdir,
    //.unlink = ps_unlink,
    //.rmdir = ps_rmdir,
    //.symlink = ps_symlink,
    //.rename = ps_rename,
    //.link = ps_link,
    //.chmod
    //.chown
    //.truncate
    .open = ps_open,
    .read = ps_read,
    //.write = ps_write,
    //.statfs = ps_statfs,
    //.flush
    //.release = ps_release,
    //.fsync
    //.setxattr
    //.getxattr
    //.listxattr
    //.removexattr
    //.opendir = ps_opendir,
    .readdir = ps_readdir,
    //.releasedir = ps_releasedir,
    //.fsyncdir
    //.init
    //.destroy = ps_destroy,
    //.access
    //.create = ps_create,
    //.lock
    //.utimens
    //.bmap
    //.ioctl
    //.poll
    //.write_buf = ps_write_buf,
    //.read_buf = ps_read_buf,
    //.flock
    //.fallocate = ps_fallocate
};

int main(int argc, char *argv[]) {
    GlobalInfo *globalInfo = new GlobalInfo();
    globalInfo->psStatus->initBuffer(&argc, &argv);

    return fuse_main(argc, argv, &global_oper, (void*)globalInfo);
}
