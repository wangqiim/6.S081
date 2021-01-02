#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

int Open(const char* path, int x) {
    int fd;
    if ((fd = open(path, x)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        exit(1);
    }
    return fd;
}

void Fstat(int fd, struct stat* st) {
    if (fstat(fd, st) < 0) {
        fprintf(2, "find: cannot stat fd\n", fd);
        close(fd);
        exit(1);
    }
}

void find(char *path, char *pattern) {
    char buf[512];
    char *p;
    int fd;
    struct dirent de;
    struct stat st;
    fd = Open(path, 0);
    Fstat(fd, &st);
    if (st.type != T_DIR)
        fprintf(2, "find: %s is not a directory\n", path);
    // printf("wq...debug\n");
    strcpy(buf, path);
    p = (buf + strlen(buf));
    *(p++) = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
        // printf("de.inum:%d, de.name:%s\n", de.inum, de.name);
        // printf("wq...debug_begin\n");
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        int tfd = Open(buf, 0);
        Fstat(tfd, &st);
        // printf("buf:%s, de.name:%s, st.type:%d\n", buf, de.name, st.type);
        if (st.type == T_DIR) {
            find(buf, pattern);
        } else if (strcmp(pattern, de.name) == 0) {
            printf("%s\n", buf);
        }
        // printf("wq...debug_end\n");
        close(tfd);
    }
    close(fd);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(2, "find: find dir files...\n");
        exit(1);
    }
    char* path = argv[1];
    char* pattern = argv[2];
    find(path, pattern);
    exit(0);
    //printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
}