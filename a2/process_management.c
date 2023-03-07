#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

const int BUF_SIZE = 3500;

//copies the commands in the input file to the shared memory
void file_to_shared(char *shm_ptr, char *filename){
    char *p = shm_ptr;

    if (fork() == 0){
        FILE *f = fopen(filename, "r");
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        // while not at end of file
        while ((read = getline(&line, &len, f)) != -1){
            // add line to shared memory
            p += sprintf(p,"%s",line);
        }
        exit(0);
    }
    else if (fork() > 0){
        int status;
        wait(&status);
        if (WEXITSTATUS(status) == -1){
            perror("\nProblem occured while waiting for child to finish the file_to_shared function \n");
            exit(1);
        }
    }
    else {
        printf("\nFork failed in file_to_shared function\n");
        exit(1);
    }
}
// copies the outputs of the performed commands to the pipe
void command_outputs_to_pipe(char *buffer_p, int pipe_id){
    char buffer[BUF_SIZE];
    while (buffer_p){
        FILE *f = popen(buffer_p, "r");
        char line[1100];
        if (f){
            char first_line[50];
            sprintf(first_line, "\nThe output of: %s is\n>>>>>>>>>>>>>>>\n", buffer_p);
            strcat(buffer, first_line);
            while(fgets(line,sizeof(line),f) != NULL){
                strcat(line,"\n");
                strcat(buffer,line);
            }
            strcat(buffer,"<<<<<<<<<<<<<<<");
        }
        else {
            printf("\n Command: '%s' failed to execute \n", buffer_p);
            exit(1);
        }
        fclose(f);
        buffer_p = strtok(NULL, "\r\n");
    }
    // write outputs to pipe with id pipe_id
    write(pipe_id,buffer,BUF_SIZE);
}

// copies contents of pipe to the output file
void pipe_to_file(int pipe_id){
    char buffer[3500];
    read(pipe_id,buffer, BUF_SIZE);
    FILE *f = fopen("output.txt","w");
    char *p = strtok(buffer, "\r\n");
    while (p){
        fprintf(f, "%s\n",p);
        p = strtok(NULL, "\r\n");
    }
    fclose(f);
    printf("\n All tasks were successful, check output file\n\n");
}

// runs the commands in the shared memory
void run_shared(char *shm_ptr){
    char *p = shm_ptr;
    char *pipe = "/tmp/pipe";
    mkfifo(pipe, 0666);

    if (fork() == 0){
        int pipe_id = open(pipe, O_WRONLY);
        char buffer[3500];
        char next;
        for (int i = 0; i<66; i ++){
            next = (char)p[i];
            strncat(buffer, &next, 1);
        }
        // write command ouputs to buffer and call command_outputs_to_pipe to write command output to pipe
        char *buffer_p = strtok(buffer, "\n");
        command_outputs_to_pipe(buffer_p, pipe_id);
        close(pipe_id);
    }
    else if (fork() > 0){
        int pipe_id = open(pipe, O_RDONLY);
        int status;
        wait(&status);
        if (WEXITSTATUS(status) == -1){
            perror("\nProblem occured while waiting for child to finish the run_shared function\n");
            exit(1);
        }
        pipe_to_file(pipe_id);
        close(pipe_id);
    }
    else {
        printf("\nFork failed in run_shared function\n");
        exit(1);
    }
}

int main(int argc, char *argv[]){
    if (argc > 1){
        int shared_id = shm_open("/shm", O_CREAT | O_RDWR, 0666);
        if (shared_id == -1){
            printf("\n Failed to create shared memory area: %s\n", strerror(errno));
            exit(1);
        }
        ftruncate(shared_id, BUF_SIZE);
        char *shm_ptr = mmap(0, BUF_SIZE, PROT_READ| PROT_WRITE, MAP_SHARED, shared_id, 0);
        if (shm_ptr == MAP_FAILED){
            printf("\n Failed to map shared memory area to process %s\n", strerror(errno));
            exit(1);
        }
        char *file = argv[1];
        file_to_shared(shm_ptr, file);
        run_shared(shm_ptr);
    }
    else {
        printf("\nInsufficient parameters given \n");
        exit(1);
    }
}