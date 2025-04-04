#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 1024

void handle_error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(void)
{
    int filedes[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    if (pipe(filedes) < 0)
    {
        handle_error("Father: can`t create pipe\n");
    }
    printf("Father: pipe is created\n");

    pid = fork();

    if (pid < 0)
    {
        handle_error("Fork failed\n");
    }

    if (pid == 0)
    {
        int input_file = open("input.txt", O_RDONLY);
        if (input_file < 0)
        {
            handle_error("Can`t open input file\n");
        }

        while ((bytes_read = read(input_file, buffer, BUFFER_SIZE)) > 0)
        {
            bytes_written = write(filedes[1], buffer, bytes_read);
            if (bytes_written != bytes_read)
            {
                handle_error("Write to pipe failed\n");
            }
        }
        close(input_file);
        close(filedes[1]);
        exit(0);
    }
    else
    {
        close(filedes[1]);
        int output_file = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_file < 0)
        {
            handle_error("Can`t open output file");
        }
        while ((bytes_read = read(filedes[0], buffer, BUFFER_SIZE)) > 0)
        {
            bytes_written = write(output_file, buffer, bytes_read);
            if (bytes_written != bytes_read)
            {
                handle_error("Write to output file failed \n");
            }
        }
        close(output_file);
        close(filedes[0]);
        wait(NULL);
    }
    return 0;
}