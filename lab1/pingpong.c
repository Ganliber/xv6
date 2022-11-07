#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

/*
* The parent should send a byte to the child; the child should print 
* "<pid>: received ping", where <pid> is its process ID, write the byte 
* on the pipe to the parent, and exit; the parent should read the byte
* from the child, print "<pid>: received pong", and exit.
*/

int main(int argc, char const *argv[])
{
/* Method 1 */
    // int p1[2]; // create a pipe
    // int p2[2];
    // pipe(p1); // create a pipe and return its fds in p, p[0]=>read; p[1]=>write
    // pipe(p2);

    // if (fork()==0) {
    //     // child process
    //     close(0); // close stdin and bind it to pipe
    //     dup(p1[0]); // 子进程将管道的读端口拷贝在描述符0上,关闭 p1中的描述符
    //     close(p1[0]);
    //     close(p1[1]);
    //     char buffer_ch[2];
    //     read(0, buffer_ch, 1);// read(fd, buf, n) fd -> buf (n bytes upmost)
    //     printf("%d: received ping\n", getpid());

    //     write(p2[1],buffer_ch, 1);// send back
    //     close(p2[0]);
    //     close(p2[1]);
    //     exit(0); // Indispensable, u can't miss this line!
    // } else {
    //     // father process

    //     write(p1[1],"ccc",1); // write(fd, buf) buf -> fd (n bytes upmost)
    //     close(p1[0]);
    //     close(p1[1]);

    //     close(0);
    //     dup(p2[0]);
    //     close(p2[0]);
    //     close(p2[0]);
    //     char buffer_fa[2];
    //     read(0, buffer_fa, 1);
    //     printf("%d: received pong\n",getpid());
    //     exit(0); // u can't miss this line!
    // }

/* Method 2 */
    int p1[2]; // create a pipe
    int p2[2];
    pipe(p1); // create a pipe and return its fds in p, p[0]=>read pipe; p[1]=>write pipe
    pipe(p2);

    if (fork()==0) {

        char buffer_ch[2];
        read(p1[0], buffer_ch, 1);// read(fd, buf, n) fd -> buf (n bytes upmost)
        printf("%d: received ping\n", getpid());

        write(p2[1],buffer_ch, 1);// send back
        exit(0); // Indispensable, u can't miss this line!
    } else {
        // father process

        write(p1[1],"ccc",1); // write(fd, buf) buf -> fd (n bytes upmost)
            // 向管道写 c
        char buffer_fa[2];
        read(p2[0], buffer_fa, 1);
        printf("%d: received pong\n",getpid());
        exit(0); // u can't miss this line!
    }

/* Method 3 */
    // int p1[2]; // create a pipe
    // int p2[2];
    // pipe(p1); // create a pipe and return its fds in p, p[0]=>read pipe; p[1]=>write pipe
    // pipe(p2);
    // if (fork()==0) {
    //     // child process
    //     close(2); // close stdin and bind it to pipe
    //     dup(p1[0]); // 子进程将管道的读端口拷贝在描述符0上,关闭 p1中的描述符
    //     close(p1[0]);
    //     close(p1[1]);
    //     char buffer_ch[2];
    //     read(2, buffer_ch, 1);// read(fd, buf, n) fd -> buf (n bytes upmost)
    //     printf("%d: received ping\n", getpid());

    //     write(p2[1],buffer_ch, 1);// send back
    //     close(p2[0]);
    //     close(p2[1]);
    //     exit(0); // Indispensable, u can't miss this line!
    // } else {
    //     // father process

    //     write(p1[1],"ccc",1); // write(fd, buf) buf -> fd (n bytes upmost)
    //     close(p1[0]);
    //     close(p1[1]);

    //     close(2);
    //     dup(p2[0]);
    //     close(p2[0]);
    //     close(p2[0]);
    //     char buffer_fa[2];
    //     read(2, buffer_fa, 1);
    //     printf("%d: received pong\n",getpid());
    //     exit(0); // u can't miss this line!
    // }
    // return 0;
}
