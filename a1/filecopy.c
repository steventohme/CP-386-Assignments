#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

     int file_src, file_des, line;
     char buf[BUFSIZ];

     if (argc != 3) {
          printf("Insufficient parameters passed.\n");
          exit(1);
     }

     if ((file_src = open(argv[1], O_RDONLY)) == -1) {
          perror(argv[1]);
          exit(2);
     }

     if ((file_des = creat(argv[2], 0644)) == -1) {
          perror(argv[2]);
          exit(3);
     }

     while ((line = read(file_src, buf, BUFSIZ)) > 0) {
          if (write(file_des, buf, line) != line) {
               perror(argv[2]);
               exit(4);
          }

          else { 
               printf("The contents of file %s have been successfully copied into the %s file.\n", argv[1], argv[2]);
          }
     }

     close(file_src);
     close(file_des);

     return 0;
}