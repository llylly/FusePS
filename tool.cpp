#include "tool.h"

PSStatus *getPSStatus() {
    fuse_context *context = fuse_get_context();
    GlobalInfo *global = (GlobalInfo*)context->private_data;
    PSStatus *ans = global->psStatus;
    return ans;
}

struct stat *genPSFolderStat() {
    struct stat *res = (struct stat*)malloc(sizeof(struct stat));
    memset(res, 0, sizeof(struct stat));
    res->st_mode = S_IRWXU | S_IRWXG | S_IRWXO;
    res->st_mode |= S_IFDIR;
    res->st_nlink = 2;
    return res;
}

struct stat *genPSConfigFileStat() {
    struct stat *res = (struct stat*)malloc(sizeof(struct stat));
    memset(res, 0, sizeof(struct stat));
    PSStatus *status = getPSStatus();
    struct stat bufStat;
    lstat(status->bufferFolder, &bufStat);
    res->st_dev = bufStat.st_dev;
    res->st_ino = 0;
    res->st_mode = S_IFREG | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    res->st_nlink = 1;
    res->st_uid = bufStat.st_uid;
    res->st_gid = bufStat.st_gid;
    res->st_rdev = bufStat.st_rdev;
    return res;
}
