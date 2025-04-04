#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pid, ppid, status;
    pid = getpid();
    ppid = getppid();
    printf("\nPARENT PARAM: pid=%d ppid=%d\n", pid, ppid);
    
    if (fork() == 0) {
        execl("./build/child", "child", NULL);
    }
    
    system("ps xf");
    wait(&status);
    return 0;
}