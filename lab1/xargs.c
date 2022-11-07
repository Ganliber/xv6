#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h" // where max number of args is defined.

/*
* Write a simple version of the UNIX xargs program: 
* read lines from the standard input and run a command for each line,
* supplying the line as arguments to the command.
*/
#define MAXLENGTH 512
char *child_argv[MAXARG];// MAX 

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "miss arguments!\n");
        exit(0);
    } else {
        //printf("\ninit arguments :\n");

        child_argv[0] = argv[1];

        for( int j = 1; j < argc; j++) {
            //printf("> %s\n", argv[j]);
            child_argv[j-1] = argv[j]; 
                // copy initial arguments
                // 这些不是在堆区申请的因此后来不能 free
        }
        char input[512]; 
            // following argv for xargs cmd
            // find . b | xargs grep hello, every line output by "find . b" will be
            // feeded to "grep hellp ./b" for example 
        char *p = (char *)malloc(1);
        int i = 0;
        int child_argc = argc-1;
        // find . b | ... 通过管道把标准输出的内容变为 stdin 给 xargs
        while(1) {
            int ret = read(0, p, 1);
            //printf("> read return value is %d\n", ret);
            if(ret<=0){
                exit(0);
            }
            
            if (*p == ' ') {
                
                input[i] = '\0';
                if (child_argc >= MAXARG) {
                    fprintf(2, "too much arguments!\n");
                    exit(0);
                }
                child_argv[child_argc] = (char*)malloc(MAXLENGTH);
                strcpy(child_argv[child_argc], input);
                
                child_argc++;
                //准备接收下一个参数
                i = 0;
                memset(input, '\0', sizeof(input));
            } else if (*p == '\n') {
                input[i] = '\0';
                
                if (child_argc >= MAXARG) {
                    fprintf(2, "too much arguments!\n");
                    exit(0);
                }
                child_argv[child_argc] = (char*)malloc(MAXLENGTH);
                strcpy(child_argv[child_argc], input);
                child_argc ++;
                
                child_argv[child_argc] = 0;
                //准备执行
                if (fork()==0) {
                    // child process
                    exec(argv[1], child_argv);
                    exit(0);
                } else {
                    // parent process
                    wait((int *)0); //等待子进程结束
                    //释放堆区
                    //printf("argc-1: %d\tchild argc: %d\n", argc-1, child_argc);
                    for (int x = argc-1; x < child_argc; x++) {
                        //printf("To be freed: %s\n", child_argv[x]);
                        free(child_argv[x]);
                    }
                    //准备接收第二行
                    child_argc = argc - 1;
                    input[i] = '\0';
                    i = 0;
                    continue;
                }                            
            } else {
                input[i] = *p;
                i ++;
            }
        }
        
        free(p);
    }
    return 0;
}