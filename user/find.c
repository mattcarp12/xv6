#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(const char*, const char*);

int main(int argc, char *argv[]) {
    if (argc <= 1 || argc > 3)
        printf("Usage: find <directory> filename\n");
    else if (argc == 2) 
        find(".", argv[1]);
    else 
        find(argv[1], argv[2]);
    exit();
}

void find(const char *path, const char *filename) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (st.type != T_DIR) {
        printf("Usage: find <directory> filename\n");
        return;
    }

    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (!strcmp(de.name, ".") 
            || !strcmp(de.name, "..") 
            || de.inum == 0)
            continue;
        
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        
        if(!strcmp(de.name, filename))
            printf("%s\n", buf);

        if(stat(buf, &st) < 0) {
            printf("find: cannot stat %s\n", buf);
            continue;
        } else if(st.type == T_DIR) {
            find(buf, filename);
        }

    }
}