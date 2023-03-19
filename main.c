#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


const int buf_size = 25000;
const int output_size = 10;

void reader_from_file(char filename[], int output_pipe) {
    int input_fd = open(filename, O_RDONLY, 0666);

    char   from_file_readed[buf_size + 1];
    ssize_t readed_size = read(input_fd, from_file_readed, buf_size);
    if (readed_size > 5000) {
        printf("only files smaller than 5000");
        exit(-1);
    }
    close(input_fd);
    from_file_readed[readed_size] = 0;

    ssize_t writen_size = write(output_pipe, from_file_readed, readed_size + 1);
    if(writen_size != readed_size + 1){
        printf("Can\'t write all string to pipe\n");
        exit(-1);
    }
}

void process_data(int input_pipe, int output_pipe) {
    char str_buf[buf_size];
    ssize_t size = read(input_pipe, str_buf, buf_size);
    if (size < 0) {
        printf("Can\'t read string from pipe\n");
        exit(-1);
    }

    /// logic
    int numeric[10];
    for (int i = 0; i < 10; ++i) {
        numeric[i] = 0;
    }

    for (size_t i = 0; i < size; ++i) {
        if ('0' <= str_buf[i] && str_buf[i] <= '9') {
            numeric[str_buf[i] - '0'] = 1;
        }
    }

    int res = 0;
    for (int i = 0; i < 10; ++i) {
        res += numeric[i];
    }

    char resulted_str[output_size];
    memset(resulted_str, 0, output_size);
    sprintf(resulted_str, "%d", res);
    size = write(output_pipe, resulted_str, strlen(resulted_str));
    if (size != strlen(resulted_str)) {
        printf("Can\'t write string to pipe\n");
        exit(-1);
    }
}

void write_to_file(int input_pipe, char output_file[]) {
    char output[output_size];

    ssize_t size = read(input_pipe, output, output_size);
    if (size < 0) {
        printf("Can\'t read string from pipe\n");
        exit(-1);
    }

    int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    size = write(output_fd, output, size);
    if (size < 0) {
        printf("Can\'t read string from pipe\n");
        exit(-1);
    }

    close(output_fd);
}


int main(int argc, char* argv[]) {
    int    from_file[2], into_file[2], result, result_new;

    if (argc < 3) {
        printf("Not provided input and output filename");
        exit(-1);
    }

    if(pipe(from_file) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    if(pipe(into_file) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }


    result = fork();
    if(result < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (result > 0) { /* READER process */

        if(close(from_file[0]) < 0){
            printf("Reader: Can\'t close reading side of from_file pipe\n"); exit(-1);
        }

        reader_from_file(argv[1], from_file[1]);

        if(close(from_file[1]) < 0) {
            printf("Reader: Can\'t close writing side of from_file pipe\n");
            exit(-1);
        }

    } else { /* Child process */

        result_new = fork();
        if (result_new < 0){printf("Can\'t fork child\n");
            exit(-1);
        } else if (result_new > 0) {   /* LOGIC process */
            if(close(from_file[1]) < 0){
                printf("Logic: Can\'t close writing side of from_file pipe\n"); exit(-1);
            }
            if(close(into_file[0]) < 0){
                printf("Logic: Can\'t close reading side of into_file pipe\n"); exit(-1);
            }

            process_data(from_file[0], into_file[1]);

            if(close(from_file[0]) < 0){
                printf("Logic: Can\'t close reading side of from_file pipe\n"); exit(-1);
            }
            if(close(into_file[1]) < 0) {
                printf("Logic: Can\'t close writing side of into_file pipe\n"); exit(-1);
            }
        } else { /* WRITER process */

            if(close(into_file[1]) < 0){
                printf("Writer: Can\'t close writing side of from_file pipe\n"); exit(-1);
            }
            write_to_file(into_file[0], argv[2]);

            if (close(into_file[0] < 0)) {
                printf("child: Can\'t close reading side of into_file pipe\n"); exit(-1);
            }
        }

    }

    return 0;
}
