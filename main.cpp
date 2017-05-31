
#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <dirent.h>
#include <sys/types.h>
#include <sys/statvfs.h>

#include <iostream>

using namespace std;

#include "GlobalInfo.h"
#include "tool.h"

int ps_getattr(const char *path, struct stat *stbuf) {
    PSStatus *status = getPSStatus();

    if ((strcmp(path, "/") == 0) || (strcmp(path, "/PS") == 0)) {
        lstat(status->bufferFolder, stbuf);
        return 0;
    }

    for (int i=0; i<status->foldersN; ++i)
        if (strcmp(path, status->foldersPath[i]) == 0) {
            lstat(status->bufferFolder, stbuf);
            return 0;
        }

    for (int i=0; i<status->configsN; ++i)
        if (strcmp(path, status->configsPath[i]) == 0) {
            struct stat *res = genPSConfigFileStat();
            res->st_size = status->configs[i]->getSize();
            memcpy((void*)stbuf, (void*)res, sizeof(struct stat));
            return 0;
        }

    /* To be filled: if equals to a img file */

    return -ENOENT;
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

int ps_chmod(const char *path , mode_t mode) {
    PSStatus *status = getPSStatus();

    bool reserved = false;

    if ((strcmp(path, "/") == 0) || (strcmp(path, "/PS") == 0))
        reserved = true;
    for (int i=0; i<status->foldersN; ++i)
        if (strcmp(path, status->foldersPath[i]) == 0)
            reserved = true;
    for (int i=0; i<status->configsN; ++i)
        if (strcmp(path, status->configsPath[i]) == 0)
            reserved = true;

    int ret = 0;
    if (!reserved) {
        ret = chmod(path, mode);
    }
    return ret;
}

int ps_chown(const char *path, uid_t uid, gid_t gid) {
    PSStatus *status = getPSStatus();

    bool reserved = false;

    if ((strcmp(path, "/") == 0) || (strcmp(path, "/PS") == 0))
        reserved = true;
    for (int i=0; i<status->foldersN; ++i)
        if (strcmp(path, status->foldersPath[i]) == 0)
            reserved = true;
    for (int i=0; i<status->configsN; ++i)
        if (strcmp(path, status->configsPath[i]) == 0)
            reserved = true;

    int ret = 0;
    if (!reserved) {
        ret = chown(path, uid, gid);
    }
    return ret;
}

int ps_truncate(const char *path, off_t offset) {
    PSStatus *status = getPSStatus();

    bool reserved = false;

    if ((strcmp(path, "/") == 0) || (strcmp(path, "/PS") == 0))
        reserved = true;
    for (int i=0; i<status->foldersN; ++i)
        if (strcmp(path, status->foldersPath[i]) == 0)
            reserved = true;
    for (int i=0; i<status->configsN; ++i)
        if (strcmp(path, status->configsPath[i]) == 0)
            reserved = true;

    int ret = 0;
    if (!reserved) {
        ret = truncate(path, offset);
    }
    return ret;
}

int ps_open(const char *path, struct fuse_file_info *fi) {
    PSStatus *status = getPSStatus();

    for (int i=0; i<status->configsN; ++i)
        if (strcmp(path, status->configsPath[i]) == 0) {
            return 0;
        }

    return -ENOENT;
}

int ps_read(const char *path, char *buf, size_t size, off_t offset,
            struct fuse_file_info *fi) {
    PSStatus *status = getPSStatus();

    for (int i=0; i<status->configsN; ++i)
        if (strcmp(path, status->configsPath[i]) == 0) {
            return status->configs[i]->read(buf, size, offset);
        }

    return -ENOENT;
}

int ps_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    PSStatus *status = getPSStatus();

    for (int i=0; i<status->configsN; ++i)
        if (strcmp(path, status->configsPath[i]) == 0) {
            return status->configs[i]->write(buf, size, offset);
        }

    return -ENOENT;
}

int ps_statfs(const char *path, struct statvfs *vfs) {
    int MAX_SIZE = 1048576;
    int NAME_MAXLEN = 63;

    PSStatus *status = getPSStatus();
    struct stat buf;
    stat(status->bufferFolder, &buf);
    vfs->f_bsize = buf.st_blksize;
    vfs->f_flag = 16;
    vfs->f_blocks = buf.st_blocks;
    vfs->f_bfree = vfs->f_bavail = MAX_SIZE;
    vfs->f_namemax = NAME_MAXLEN;
    vfs->f_frsize = MAX_SIZE;

    return 0;
}

int ps_release(const char *path, struct fuse_file_info *fi) {
    PSStatus *status = getPSStatus();
    bool reserved = false;

    if ((strcmp(path, "/") == 0) || (strcmp(path, "/PS") == 0))
        reserved = true;
    for (int i=0; i<status->foldersN; ++i)
        if (strcmp(path, status->foldersPath[i]) == 0)
            reserved = true;
    for (int i=0; i<status->configsN; ++i)
        if (strcmp(path, status->configsPath[i]) == 0)
            reserved = true;

    int ret = 0;
    if (!reserved) {
        ret = close(fi->fh);
    }

    return ret;
}

int ps_opendir(const char *path, struct fuse_file_info *fi) {
    PSStatus *status = getPSStatus();

    bool legalDir = false;
    if ((strcmp(path, "/") == 0) || (strcmp(path, "/PS") == 0))
        legalDir = true;
    for (int i=0; i<status->foldersN; ++i)
        if (strcmp(path, status->foldersPath[i]) == 0)
            legalDir = true;

    if (legalDir) return 0;

    return -ENOENT;
}

int ps_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
               off_t offset, struct fuse_file_info *fi) {
    PSStatus *status = getPSStatus();

    struct stat bufStat;
    lstat(status->bufferFolder, &bufStat);

    bool legalDir = false;

    if (strcmp(path, "/") == 0) {
        filler(buf, ".", &bufStat, 0);
        filler(buf, "..", &bufStat, 0);
        filler(buf, "PS", &bufStat, 0);
        legalDir = true;
    }

    if (strcmp(path, "/PS") == 0) {
        filler(buf, ".", &bufStat, 0);
        filler(buf, "..", &bufStat, 0);
        for (int i=0; i<status->foldersN; ++i) {
            filler(buf, status->foldersName[i], NULL, 0);
        }
        legalDir = true;
    }

    for (int i=0; i<status->foldersN; ++i)
        if (strcmp(path, status->foldersPath[i]) == 0) {
            filler(buf, ".", &bufStat, 0);
            filler(buf, "..", &bufStat, 0);
            if (status->foldersConfig[i] != -1) {
                struct stat nowStat = *genPSConfigFileStat();
                nowStat.st_size = status->configs[status->foldersConfig[i]]->getSize();
                filler(buf, "config.txt", &nowStat, 0);
            }
            legalDir = true;
        }
    if (legalDir) return 0;

    return -ENOENT;
}

int ps_releasedir(const char *path, struct fuse_file_info *fi) {
    PSStatus *status = getPSStatus();

    bool legalDir = false;

    if ((strcmp(path, "/") == 0) || (strcmp(path, "/PS") == 0))
        legalDir = true;
    for (int i=0; i<status->foldersN; ++i)
        if (strcmp(path, status->foldersPath[i]) == 0)
            legalDir = true;

    if (legalDir) return 0;

    return -ENOENT;
}

void ps_destroy(void *) {

}

int ps_create(const char *path , mode_t mode, struct fuse_file_info *fi) {
    PSStatus *status = getPSStatus();

    for (int i=0; i<status->foldersN; ++i)
        if (strstr(path, status->foldersPath[i]) == path) {
            const char *fileName = path + strlen(status->foldersPath[i]);
            char *realPlace = new char[100];
            sprintf(realPlace, "%s%s", status->foldersPath, fileName);
            if (access(realPlace, F_OK)) {
                int ret = open(realPlace, (int)mode | O_CREAT);
                if (ret >= 0) {
                    fi->fh = ret;
                    return 0;
                } else
                    return ret;
            } else {
                return -EEXIST;
            }
        }

    return -EPERM;
}

int ps_utimens(const char *path, const struct timespec tv[2]) {
    PSStatus *status = getPSStatus();

    bool reserved = false;

    if ((strcmp(path, "/") == 0) || (strcmp(path, "/PS") == 0))
        reserved = true;
    for (int i=0; i<status->foldersN; ++i)
        if (strcmp(path, status->foldersPath[i]) == 0)
            reserved = true;
    for (int i=0; i<status->configsN; ++i)
        if (strcmp(path, status->configsPath[i]) == 0)
            reserved = true;

    int ret = 0;
    if (!reserved) {

    }

    return ret;
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
    //.unlink = ps_unlink,mk
    //.rmdir = ps_rmdir,
    //.symlink = ps_symlink,
    //.rename = ps_rename,
    //.link = ps_link,
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
    //.setxattr
    //.getxattr
    //.listxattr
    //.removexattr
    .opendir = ps_opendir,
    .readdir = ps_readdir,
    .releasedir = ps_releasedir,
    //.fsyncdir
    //.init
    //.destroy = ps_destroy,
    //.access
    .create = ps_create,
    //.lock
    .utimens = ps_utimens,
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
