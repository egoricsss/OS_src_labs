#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define BUFFER_SIZE 1024

void server(int fdr, int fdw)
{
    char buffer[BUFFER_SIZE];

    const char *file_name = "../input.txt";
    if (write(fdw, file_name, strlen(file_name) + 1) == -1)
    {
        perror("Error writing to chan1");
        exit(1);
    }

    ssize_t bytes_read = read(fdr, buffer, sizeof(buffer));
    if (bytes_read == -1)
    {
        perror("Error reading from chan2");
        exit(1);
    }

    printf("Server received: %s\n", buffer);
}

int main()
{
    const char *chan1 = "/tmp/chan1";  
    const char *chan2 = "/tmp/chan2"; 

    if (mknod(chan1, S_IFIFO | 0666, 0) == -1)
    {
        perror("Error creating chan1");
        exit(1);
    }

    if (mknod(chan2, S_IFIFO | 0666, 0) == -1)
    {
        perror("Error creating chan2");
        exit(1);
    }

    // Открытие каналов
    int fdw = open(chan1, O_WRONLY);
    if (fdw == -1)
    {
        perror("Error opening chan1 for writing");
        exit(1);
    }

    int fdr = open(chan2, O_RDONLY);
    if (fdr == -1)
    {
        perror("Error opening chan2 for reading");
        exit(1);
    }

    server(fdr, fdw);

    close(fdr);
    close(fdw);

    unlink(chan1);
    unlink(chan2);

    return 0;
}
