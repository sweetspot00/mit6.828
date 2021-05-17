#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int matchhere(char*, char*);
int matchstar(int, char*, char*);

int match(char *re, char *text) {//match(name, filename)
    if (re[0] == '^') {
        return matchhere(re+1,text);
    }
    do{  // must look at empty string
        if(matchhere(re, text))
        return 1;
    }while(*text++ != '\0');
    return 0;
}

//matchhere:search for re at the beginning of text
//'\0' not empty
int matchhere(char *re, char *text) {//name,filename
    if (re[0] == '\0') 
        return 1;
    if (re[1] == '*') 
        return matchstar(re[0], re+2, text);
    if (re[0] == '$' && re[1]=='\0')
        return *text == '\0';
    if (*text !='\0' && (re[0]=='.' || re[0]== *text))
        return matchhere(re+1, text+1);//该char match了，继续往后搜索
    return 0;
}
//matchstar: search for c*re at the beginning of text
int matchstar(int c, char *re, char *text) {
    do{
        if (matchhere(re,text))
            return 1;
    } while (*text != '\0' && (*text++==c || c=='.'));
    return 0;
}

#define PATH_SIZE 50

char*
getfilename(char *path) {
    static char buf[DIRSIZ];
    char *p;
    //find first character after last slash
    for(p=path+strlen(path); p>=path && *p!='/';p--);
    p++;
    int len = strlen(p);
    memmove(buf, p, len);
    buf[len] = '\0';
    return buf;
}
//C 库函数 void *memmove(void *str1, const void *str2, size_t n) 
// 从 str2 复制 n 个字符到 str1，但是在重叠内存块这方面，
// memmove() 是比 memcpy() 更安全的方法。
// 如果目标区域和源区域有重叠的话，
// memmove() 能够保证源串在被覆盖之前将重叠区域的字节拷贝到目标区域中，
// 复制后源区域的内容会被更改。如果目标区域与源区域没有重叠，则和 memcpy() 函数功能相同。
char* concat(char* s1, char* s2) {
    char* buf = (char*) malloc(PATH_SIZE);
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    memmove(buf, s1, len1);
    buf[len1] = '/';
    memmove(buf+len1+1, s2, len2);
    buf[len1+len2+1] = '\0';
    return buf;
}

int canin(char* name) {
    int len = strlen(name);
    if (len == 1 && name[0] == '.') 
        return 0;
    if (len == 2 && name[0] == '.' && name[1] == '.') 
        return 0;
    return 1;
}
//https://blog.csdn.net/zhuyi2654715/article/details/7605051
void find(char* path, char* name) {
    struct stat st;
    struct dirent de;
    int fd;

    if ((fd = open(path, 0)) < 0) {
        //printf("find: could not open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        // printf("find: could not stat %s\n", path);
        close(fd);
        return;
    }
    switch (st.type) {
        case T_DIR://keep reading and find recusively
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0 || !canin(de.name))//not '.' nor '..'
                    continue;
                char* nxt = concat(path, de.name);
                find(nxt, name);
                free(nxt);
            }
            break;
        case T_FILE:
            {
                char* filename = getfilename(path);
                if (match(name, filename)) {//找到了
                    printf("%s\n", path);
                }
            }
            break;
    }
    close(fd);
    return;
}

void main(int argc, char** argv) {
    if (argc != 3) {
        exit(0);
    }
    char* path = argv[1];
    char* filename = argv[2];
    find(path, filename);
    exit(0);
}