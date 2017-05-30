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
    res->st_mode = S_IRWXU | S_IRWXG | S_IRWXO;
    res->st_mode |= S_IFREG;
    res->st_nlink = 1;
    res->st_size = MAX_CONFIG_LEN;
    return res;
}
