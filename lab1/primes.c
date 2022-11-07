
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
void child_process_handler(int);

/*
* The first process feeds the numbers 2 through 35 into the pipeline. 
* For each prime number, you will arrange to create one process that reads 
* from its left neighbor over a pipe and writes to its right neighbor 
* over another pipe. Since xv6 has limited number of file descriptors 
* and processes, the first process can stop at 35.
*/

// pseudocode //
    // p = get a number from left neighbor
    // print p
    // loop:
    //     n = get a number from left neighbor
    //     if (p does not divide n)
    //         send n to right neighbor

int p[35][2];
int index = 0; // children fd number
int main(int argc, char const *argv[])
{
    pipe(p[index]); // create pipe for child
    printf("prime 2\n");
    int i;
    for (i=3; i<=35; i++) {
        if(i%2 != 0){
            write(p[index][1], &i, 4);
        }
    }

    i = -1;
    write(p[index][1], &i, 4); // 终止信号
    close(p[index][1]);
    
    if (fork()==0) {
        child_process_handler(index);
        exit(0);
    } else {
        wait(0);//等待子进程结束
        exit(0);
    }
    return 0;
}

void child_process_handler(int index) {
    int number;
    int flw_num; // following numbers
    read(p[index][0], &number, 4);
                                // read returns zero when the write-side of a pipe is closed.
                                // 4 bytes -> integer
    if (number == -1) {
        //已经全部结束
        return;
    }
    printf("prime %d\n", number);

    pipe(p[index+1]);//不要放到判断 number==-1上面
    while(read(p[index][0], &flw_num, 4)!=-1) {
        // not closed by that pipe, but it will block.
        if (flw_num == -1) {
            break;
        }
        if (flw_num%number != 0) {
            write(p[index+1][1], &flw_num, 4);
        }
    }
    flw_num = -1;
    write(p[index+1][1], &flw_num, 4);// 终止信号

    close(p[index][0]);//关闭上一个读的
    close(p[index+1][1]);//关闭下一个写的

    if (fork()==0) {
        child_process_handler(index+1);
        exit(0);
    } else {
        wait(0);//等待子进程结束
        exit(0);
    }
    return;
}
