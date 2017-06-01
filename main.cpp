
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

    for (int i=0; i<status->foldersN; ++i) {
        if (strstr(path, status->foldersPath[i]) == path) {
            const char *fileName = path + strlen(status->foldersPath[i]) + 1;
            if ((strlen(fileName) > 0) && (status->fileMap[i].count(fileName))) {
                string newPath = status->bufferFolder;
                newPath += "/";
                newPath += status->fileMap[i][fileName];
                lstat(newPath.c_str(), stbuf);
                return 0;
            }
        }
    }

    return -ENOENT;
}

int ps_mkdir(const char *, mode_t) {
    return -EPERM;
}

int ps_unlink(const char *path) {
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

    int ret = -EPERM;
    if (!reserved) {
        ret = -ENOENT;
        for (int i=0; i<status->foldersN; ++i)
            if (strstr(path, status->foldersPath[i]) == path) {
                const char *fileName = path + strlen(status->foldersPath[i]) + 1;
                if ((strlen(fileName) > 0) && (status->fileMap[i].count(fileName))) {
                    string newPath = status->bufferFolder;
                    newPath += "/";
                    newPath += status->fileMap[i][fileName];
                    int ret = unlink(newPath.c_str());
                    if (ret < 0) return ret;
                    status->fileMap[i].erase(status->fileMap[i].find(fileName));
                    status->baked[i].erase(status->baked[i].find(fileName));
                    return 0;
                }
            }
    }
    return ret;
}

int ps_rmdir(const char *) {
    return -EPERM;
}

int ps_rename(const char *path, const char *newPath) {
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

    int ret = -EPERM;
    if (!reserved) {
        ret = -ENOENT;
        for (int i=0; i<status->foldersN; ++i)
            if (strstr(path, status->foldersPath[i]) == path) {
                const char *fileName = path + strlen(status->foldersPath[i]) + 1;
                if ((strlen(fileName) > 0) && (status->fileMap[i].count(fileName))) {
                    if (strstr(newPath, status->foldersPath[i]) == newPath) {
                        const char *newFileName = newPath + strlen(status->foldersPath[i]) + 1;
                        bool ok = legalImgName(newFileName);
                        if (!ok) return -EPERM;
                        if (status->fileMap[i].count(newFileName)) return -EEXIST;
                        status->fileMap[i][newFileName] = status->fileMap[i][fileName];
                        status->baked[i][newFileName] = status->baked[i][fileName];
                        status->fileMap[i].erase(status->fileMap[i].find(fileName));
                        status->baked[i].erase(status->baked[i].find(fileName));
                        return 0;
                    }
                }
            }
    }
    return ret;
}

int ps_link(const char *, const char *) {
    return -EPERM;
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
        ret = -ENOENT;
        for (int i=0; i<status->foldersN; ++i)
            if (strstr(path, status->foldersPath[i]) == path) {
                const char *fileName = path + strlen(status->foldersPath[i]) + 1;
                if ((strlen(fileName) > 0) && (status->fileMap[i].count(fileName))) {
                    string newPath = status->bufferFolder;
                    newPath += "/";
                    newPath += status->fileMap[i][fileName];
                    ret = chmod(newPath.c_str(), mode);
                    return ret;
                }
            }
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
        ret = -ENOENT;
        for (int i=0; i<status->foldersN; ++i)
            if (strstr(path, status->foldersPath[i]) == path) {
                const char *fileName = path + strlen(status->foldersPath[i]) + 1;
                if ((strlen(fileName) > 0) && (status->fileMap[i].count(fileName))) {
                    string newPath = status->bufferFolder;
                    newPath += "/";
                    newPath += status->fileMap[i][fileName];
                    ret = chown(newPath.c_str(), uid, gid);
                    return ret;
                }
            }
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
        ret = -ENOENT;
        for (int i=0; i<status->foldersN; ++i)
            if (strstr(path, status->foldersPath[i]) == path) {
                const char *fileName = path + strlen(status->foldersPath[i]) + 1;
                if ((strlen(fileName) > 0) && (status->fileMap[i].count(fileName))) {
                    string newPath = status->bufferFolder;
                    newPath += "/";
                    newPath += status->fileMap[i][fileName];
                    ret = truncate(newPath.c_str(), offset);
                    return ret;
                }
            }
    }
    return ret;
}

int ps_open(const char *path, struct fuse_file_info *fi) {
    PSStatus *status = getPSStatus();

    for (int i=0; i<status->configsN; ++i)
        if (strcmp(path, status->configsPath[i]) == 0) {
            return 0;
        }

    int ret = -ENOENT;
    for (int i=0; i<status->foldersN; ++i)
        if (strstr(path, status->foldersPath[i]) == path) {
            const char *fileName = path + strlen(status->foldersPath[i]) + 1;
            if ((strlen(fileName) > 0) && (status->fileMap[i].count(fileName))) {
                string newPath = status->bufferFolder;
                newPath += "/";
                newPath += status->fileMap[i][fileName];
                ret = open(newPath.c_str(), O_RDWR);
                if (ret >= 0) //fi->fh = ret, ret = 0;
                    close(ret), ret = 0;
                return ret;
            }
        }
    return ret;
}

int ps_read(const char *path, char *buf, size_t size, off_t offset,
            struct fuse_file_info *fi) {
    PSStatus *status = getPSStatus();

    for (int i=0; i<status->configsN; ++i)
        if (strcmp(path, status->configsPath[i]) == 0) {
            return status->configs[i]->read(buf, size, offset);
        }

    int ret = -ENOENT;
    for (int i=0; i<status->foldersN; ++i)
        if (strstr(path, status->foldersPath[i]) == path) {
            const char *fileName = path + strlen(status->foldersPath[i]) + 1;
            if ((strlen(fileName) > 0) && (status->fileMap[i].count(fileName))) {
                string newPath = status->bufferFolder;
                newPath += "/";
                newPath += status->fileMap[i][fileName];

                int fd = open(newPath.c_str(), O_RDONLY);
                lseek(fd, offset, SEEK_SET);
                ret = read(fd, (void*)buf, size);
                close(fd);
                /*
                lseek(fi->fh, offset, SEEK_SET);
                ret = read(fi->fh, (void*)buf, size);
                */
                return ret;
            }
        }
    return ret;
}

int ps_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    PSStatus *status = getPSStatus();

    for (int i=0; i<status->configsN; ++i)
        if (strcmp(path, status->configsPath[i]) == 0) {
            return status->configs[i]->write(buf, size, offset);
        }

    int ret = -ENOENT;
    for (int i=0; i<status->foldersN; ++i)
        if (strstr(path, status->foldersPath[i]) == path) {
            const char *fileName = path + strlen(status->foldersPath[i]) + 1;
            if ((strlen(fileName) > 0) && (status->fileMap[i].count(fileName))) {
                string newPath = status->bufferFolder;
                newPath += "/";
                newPath += status->fileMap[i][fileName];

                int fd = open(newPath.c_str(), O_WRONLY);
                lseek(fd, offset, SEEK_SET);
                ret = write(fd, (void*)buf, size);
                close(fd);
                /*
                lseek(fi->fh, offset, SEEK_SET);
                ret = write(fi->fh, (void*)buf, size);
                */
                return ret;
            }
        }
    return ret;
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
        //ret = close(fi->fh);
        //if (ret >= 0) fi->fh = 0;

        for (int i=0; i<status->foldersN; ++i)
            if (strstr(path, status->foldersPath[i]) == path) {
                const char *fileName = path + strlen(status->foldersPath[i]) + 1;
                if ((strlen(fileName) > 0) && (status->fileMap[i].count(fileName))
                        && (!status->baked[i][fileName])) {
                    string newPath = status->bufferFolder;
                    newPath += "/";
                    newPath += status->fileMap[i][fileName];
                    if (status->work(newPath, i) == 0)
                        status->baked[i][fileName] = true;
                    return ret;
                }
            }
    }

    return ret;
}

int ps_setxattr(const char *path, const char *name, const char *value, size_t size, int flags, uint32_t position) {
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
        for (int i=0; i<status->foldersN; ++i)
            if (strstr(path, status->foldersPath[i]) == path) {
                const char *fileName = path + strlen(status->foldersPath[i]) + 1;
                if ((strlen(fileName) > 0) && (status->fileMap[i].count(fileName))) {
                    string newPath = status->bufferFolder;
                    newPath += "/";
                    newPath += status->fileMap[i][fileName];
#ifdef __APPLE__
                    return setxattr(newPath.c_str(), name, value, size, position, flags);
#else
                    return setxattr(newPath.c_str(), name, value, flags);
#endif
                }
            }
    }
    return 0;
}

int ps_getxattr(const char *path, const char *name, char *value, size_t size, uint32_t position) {
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
        for (int i=0; i<status->foldersN; ++i)
            if (strstr(path, status->foldersPath[i]) == path) {
                const char *fileName = path + strlen(status->foldersPath[i]) + 1;
                if ((strlen(fileName) > 0) && (status->fileMap[i].count(fileName))) {
                    string newPath = status->bufferFolder;
                    newPath += "/";
                    newPath += status->fileMap[i][fileName];
#ifdef __APPLE__
                    return getxattr(newPath.c_str(), name, value, size, position, 0);
#else
                    return getxattr(newPath.c_str(), name, value, 0);
#endif
                }
            }
    }
    return 0;
}

int ps_listxattr(const char *path, char *list, size_t size) {
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
        for (int i=0; i<status->foldersN; ++i)
            if (strstr(path, status->foldersPath[i]) == path) {
                const char *fileName = path + strlen(status->foldersPath[i]) + 1;
                if ((strlen(fileName) > 0) && (status->fileMap[i].count(fileName))) {
                    string newPath = status->bufferFolder;
                    newPath += "/";
                    newPath += status->fileMap[i][fileName];

#ifdef __APPLE__
                    return listxattr(newPath.c_str(), list, size, 0);
#else
                    return getxattr(newPath.c_str(), list, size);
#endif
                }
            }
    }
    return 0;
}

int ps_removexattr(const char *path, const char *name) {
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
        for (int i=0; i<status->foldersN; ++i)
            if (strstr(path, status->foldersPath[i]) == path) {
                const char *fileName = path + strlen(status->foldersPath[i]) + 1;
                if ((strlen(fileName) > 0) && (status->fileMap[i].count(fileName))) {
                    string newPath = status->bufferFolder;
                    newPath += "/";
                    newPath += status->fileMap[i][fileName];

#ifdef __APPLE__
                    return removexattr(newPath.c_str(), name, 0);
#else
                    return getxattr(newPath.c_str(), name);
#endif
                }
            }
    }
    return 0;
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

            map<string, string> *nowMap = &(status->fileMap[i]);
            for (map<string, string>::iterator ite = nowMap->begin(); ite != nowMap->end(); ++ite) {
                struct stat nowStat;
                string newPath = status->bufferFolder;
                newPath += "/";
                newPath += ite->second.c_str();
                lstat(newPath.c_str(), &nowStat);
                filler(buf, ite->first.c_str(), &nowStat, 0);
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
    PSStatus *status = getPSStatus();

    DIR *dir = opendir(status->bufferFolder);
    if (dir == NULL) return;

    struct dirent* p;
    char tmp[100];
    while ((p = readdir(dir)) != NULL) {
        sprintf(tmp, "%s/%s", status->bufferFolder, p->d_name);
        unlink(tmp);
    }

    closedir(dir);
}

int ps_create(const char *path , mode_t mode, struct fuse_file_info *fi) {
    PSStatus *status = getPSStatus();

    for (int i=0; i<status->foldersN; ++i)
        if (strstr(path, status->foldersPath[i]) == path) {
            const char *fileName = path + strlen(status->foldersPath[i]) + 1;
            int len = strlen(fileName);
            if (len <= 0) {
                return -EISDIR;
            }
            if (status->fileMap[i].count(fileName) > 0)
                return -EEXIST;
            bool ok = legalImgName(fileName);
            if (!ok) return -EPERM;

            int dotPos = -1;
            for (int i=0; i<len; ++i)
                if (fileName[i] == '.') dotPos = i;
            const char *formatName = fileName + dotPos + 1;

            char *realPlace = new char[100];
            sprintf(realPlace, "%s/%d.%s", status->bufferFolder, status->stamp, formatName);
            char *realFileName = new char[100];
            sprintf(realFileName, "%d.%s", status->stamp, formatName);
            ++status->stamp;

            if (access(realPlace, F_OK)) {
                int ret = open(realPlace, (int)mode | O_CREAT);
                if (ret >= 0) {
                    close(ret);
                    //fi->fh = ret;
                    status->fileMap[i][fileName] = realFileName;
                    status->baked[i][fileName] = false;
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
        for (int i=0; i<status->foldersN; ++i)
                if (strstr(path, status->foldersPath[i]) == path) {
                    const char *fileName = path + strlen(status->foldersPath[i]) + 1;
                    if ((strlen(fileName) > 0) && (status->fileMap[i].count(fileName))) {
                        string newPath = status->bufferFolder;
                        newPath += "/";
                        newPath += status->fileMap[i][fileName];
                        int fd = open(newPath.c_str(), O_WRONLY);
                        if (fd >= 0) {
                            #ifdef __APPLE__
                                struct timeval times[2];
                                times[0].tv_sec = tv[0].tv_sec;
                                times[0].tv_usec = tv[0].tv_nsec * 1000;
                                times[1].tv_sec = tv[1].tv_sec;
                                times[1].tv_usec = tv[1].tv_nsec * 1000;
                                futimes(fd, times);
                            #else
                                futimens(fd, tv);
                            #endif
                            close(fd);
                        }
                        return ret;
                    }
                }
    }

    return ret;
}

int ps_fallocate(const char *path, int mode, off_t offset, off_t len, struct fuse_file_info *fi) {
    // No need to pre-allocate
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

    if (reserved) return -EPERM;

    int ret = -ENOENT;
    for (int i=0; i<status->foldersN; ++i)
        if (strstr(path, status->foldersPath[i]) == path) {
            const char *fileName = path + strlen(status->foldersPath[i]) + 1;
            if ((strlen(fileName) > 0) && (status->fileMap[i].count(fileName))) {
                string newPath = status->bufferFolder;
                newPath += "/";
                newPath += status->fileMap[i][fileName];

                #ifndef __APPLE__
                int fd = open(newPath.c_str(), O_WRONLY);
                if (fd >= 0) {
                    fallocate(fd, mode, offset, len);
                    close(fd);
                }
                #endif
                return 0;
            }
        }
    return ret;
}

static struct fuse_operations global_oper = {
    .getattr = ps_getattr,
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
