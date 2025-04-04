#include <stdio.h>
#include <unistd.h>

int main() {
    int pid = getpid();
    int ppid = getppid();
    printf("\nCHILD PARAM: pid=%d ppid=%d\n", pid, ppid);
    sleep(15);
    return 0;
}