#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *, char *);
char* fmtname(char *path);

int
main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(2, "arguments not proper!\n");
        return 0;
    }
    find(argv[1], argv[2]);
    // argv[1] : path
    // argv[2] : filename
    exit(0);
}

void find(char *path, char *fname) {
    struct dirent de;
    struct stat st;
    int fd;
    char buf[512], *new_path; // new path pointer
    if ((fd = open(path, 0))<0) {
        fprintf(2, "find: path '%s' doesn't exist.\n", path);
        return;
    }
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    
    char *pname;
    char *pp;
    switch (st.type)
    {
    case T_FILE:
        pname = fmtname(path);
        pp = pname;
        while(*pp != 32 && *pp != 0) {
            pp++;
        }
        *pp = 0;
        //printf("pname %s? fname %s?\n", path, fname);
        if (strcmp(pname, fname) == 0) {
            printf("%s\n", path);
        }
        break;
    case T_DIR:
        // 递归遍历文件夹
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        new_path = buf + strlen(buf);
        *new_path++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0) { // 文件夹下的没有文件
                continue;
            }
            memmove(new_path, de.name, DIRSIZ);
            new_path[DIRSIZ] = 0;
            if (stat(buf, &st) < 0) {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            char * fn = fmtname(buf);
            //printf("?%s?\n", fn);

            //清除DIRSIZ的空格
            char * p = fn;
            while(*p != ' ' && *p != '\0') {
                p++;
            }
            *p = '\0';
            if (strcmp(fn, ".") == 0 || strcmp(fn, "..") == 0) {
                //printf("not include %s?\n", buf);
                continue;
            } else {
                //printf("subfolder path: %s?\n", buf);
                find(buf, fname);
            }
        }
        break;
    }

    close(fd);
    return;
}


char* fmtname(char *path) {
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}