#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define BUFFER_SIZE 1024

void client(int fdr, int fdw)
{
    char buffer[BUFFER_SIZE];

    // Чтение имени файла из канала для записи (chan1)
    ssize_t bytes_read = read(fdr, buffer, sizeof(buffer));
    if (bytes_read == -1)
    {
        perror("Error reading from chan1");
        exit(1);
    }
    printf("Client received file name: %s\n", buffer);

    // Открытие файла для чтения
    int input_file = open(buffer, O_RDONLY);
    if (input_file == -1)
    {
        perror("Error opening input file");
        exit(1);
    }

    // Чтение данных из файла и запись в канал для записи (chan2)
    while ((bytes_read = read(input_file, buffer, sizeof(buffer))) > 0)
    {
        if (write(fdw, buffer, bytes_read) == -1)
        {
            perror("Error writing to chan2");
            exit(1);
        }
    }

    close(input_file);  // Закрытие файла
}

int main()
{
    const char *chan1 = "/tmp/chan1";  // Имя канала для чтения
    const char *chan2 = "/tmp/chan2";  // Имя канала для записи

    // Открытие каналов
    int fdr = open(chan1, O_RDONLY);
    if (fdr == -1)
    {
        perror("Error opening chan1 for reading");
        exit(1);
    }

    int fdw = open(chan2, O_WRONLY);
    if (fdw == -1)
    {
        perror("Error opening chan2 for writing");
        exit(1);
    }

    // Запуск клиентской части программы
    client(fdr, fdw);

    // Закрытие файловых дескрипторов
    close(fdr);
    close(fdw);

    return 0;
}