#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// calculate the average of each column of grades
float calc_avg(int column, int grades[10][6]){
    float sum = 0;
    float average = 0;
    for (int i = 0; i < 10; i++){
        int grade = grades[i][column];
        sum = sum + grade;
    }
    average = sum/10;
    return average;
}

// prints the input grades
void print_grades(int grades[10][6]){
    printf("\nGrades: \n");
    for (int i = 0; i < 10; i++){
        for (int j = 0; j<6; j++){
            printf("%d ", grades[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Creates a child process to calculate and print the average of each column
void GRAD_FORK(int grad_num, int grades[10][6]){

    int column;
    float average;

    if (fork() == 0){
        column = (grad_num * 2) - 2;
        average = calc_avg(column,grades);
        printf("Assignment %d - Average = %f\n", column + 1, average);

        exit(0);
    }
    if (fork() == 0){
        column = (grad_num * 2) - 1;
        average = calc_avg(column,grades);
        printf("Assignment %d - Average = %f\n", column + 1, average);
        
        exit(0);
    }
    wait(NULL);
}

int main(int argc, char *argv[]){
    int grades[10][6];
    if (argc != 2){
        printf("Insufficient parameters passed.\n");
        exit(1);
    }

    FILE *file = fopen(argv[1], "r");

    int i = 0;
    while(!feof(file)){
        fscanf(file, "%d %d %d %d %d %d",&grades[i][0],&grades[i][1],&grades[i][2],&grades[i][3],&grades[i][4],&grades[i][5]);
        i++;
    }
    print_grades(grades);

    int grad_num;

    if (fork() == 0){
        grad_num = 1;
        GRAD_FORK(grad_num, grades);
        exit(0);
    }
    else{
        wait(NULL);
    }
    if (fork() == 0){
        grad_num = 2;
        GRAD_FORK(grad_num, grades);
        exit(0);
    }
    else{
        wait(NULL);
    }

    if (fork() == 0){
        grad_num = 3;
        GRAD_FORK(grad_num, grades);
        exit(0);
    }
    else{
        wait(NULL);
    }

    fclose(file);

    return 0;
    
}