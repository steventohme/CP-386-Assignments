#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>


// task 1
void create_directory(){
    int check;
    char dirname[50];
    printf("Enter the directory name you want to create: ");
    scanf("%s",dirname);
    check = mkdir(dirname, 0777);
    if (!check){
        printf("Directory created succesfully\n");
    }
    else {
        printf("Directory not created succesfully, may already exist\n");

    }
}

// task 2

void delete_directory(){
    int check;
    char dirname[50];
    printf("Enter the directory name you want to delete: ");
    scanf("%s",dirname);
    check = rmdir(dirname);
    if (!check){
        printf("Directory deleted succesfully\n");
    }
    else{
        printf("Directory not deleted succesfully, may not exist \n");
    }
}

//task 3
void print_working(){
    char dirname[50];
    char buf[BUFSIZ];
    strcpy(dirname,getcwd(buf,BUFSIZ));
    printf("Current Working Directory is: %s ",dirname);
}

//task 4
void change_directory(){
    int check;
    char dirname[50];
    char buf[BUFSIZ];
    check = chdir("..");
    strcpy(dirname,getcwd(buf,BUFSIZ));
    printf("Current Working Directory is: %s ",dirname);
}

//task 5
void read_contents(){
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d){
        while ((dir = readdir(d)) != NULL){
            printf("%s\n",dir->d_name);
        }
    }
    return;
}

//task 6
void close_directory(){
    DIR *dir;
    dir = opendir(".");
    closedir(dir);
}

int main(){
    char task;
    while (task != 'q'){
    printf("\nSelect the option(s) appropriately by entering the number:\n");
    printf("  Enter 1 for creating a directory \n  Enter 2 for removing directory \n  Enter 3 for printing working directory\n  Enter 4 for changing directory one level up \n  Enter 5 for reading the contents of directory\n  Enter 6 for closing the directory \n  Enter q to exit the program\n");
    scanf("%s",&task);
    if (task == 'q'){
        return 0;
    }
    else if (task == '1'){
        create_directory();
    }
    else if (task == '2'){
        delete_directory();
    }
    else if (task == '3'){
        print_working();
    }
    else if (task == '4'){
        change_directory();
    }
    else if (task == '5'){
        read_contents();
    }
    else if (task == '6'){
        close_directory();
    }
    else {
        printf("Not a valid input, try again!\n");
    }
    
    }
    return 0;
}

