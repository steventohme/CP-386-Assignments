#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// this variable hold the sudoku board
int sudoku[9][9];
// this is the vaiable that will hold the validity of each component of the sudoko board
// the first 2 elements hold the validity of the rows and columns and the other 9 are the validity of the columns
int result[11];
// index of where we are on the valid board after we finish checking the rows and columns
int current3x3 = 2;

typedef struct {
    int row;
    int column;
} parameters;

pthread_t column_thread, row_thread, square_thread_1, square_thread_2, square_thread_3, square_thread_4, square_thread_5, square_thread_6, square_thread_7, square_thread_8, square_thread_9;


// thread function to check the validity of all of the columsn
void *check_columns(void *params){
    parameters * data = (parameters *) params;
    int starting_col = data->column;
    int starting_row = data->row;

    //checking for duplicates in each row
    for (int i = starting_col; i < 9; i++){
        int col[9] = {0};
        for(int j = starting_row; j < 9; j++){
            int value = sudoku[i][j];
            if (col[value - 1] == 0 && value > 0){
                col[value - 1] = value;
            }
            else {
                // value is duplicate, sudoku is not solved
                result[0] = 0;
                printf("Column %d is not valid\n", i);
                pthread_exit(0);
            }
        }
    }
    // no duplicates found, columns are valid
    result[0] = 1;
    pthread_exit(0);
}

//thread function to check the validity of all of the rows, same as column check
void *check_rows(void *params){
    parameters *data = (parameters *) params;
    int starting_col = data->column;
    int starting_row = data->row;

    for (int i = starting_col; i < 9; i++){
        int row[9] = {0};
        for(int j = starting_row; j < 9; j++){
            int value = sudoku[i][j];
            if(row[value - 1] == 0 && value > 0){
                row[value - 1] = value;
            }
            else{
                result[1] = 0;
                printf("Invalid row: %d\n", j);
                pthread_exit(0);
            }
        }
    }
    result[1] = 1;
    pthread_exit(0);
}

//thread function to check the validity of a 3x3 square
void *square_check(void *params){
    parameters *data = (parameters *) params;
    int starting_col = data->column;
    int starting_row = data->row;

    int square[9] = {0};
    for (int i = starting_row; i < starting_row + 3; i++){
        for (int j = starting_col; j < starting_col + 3; j++){
            int value = sudoku[i][j];
            if (square[value - 1] == 0 && value > 0){
                square[value - 1] = value;
            }
            else{
                // the current 3x3 square is not valid
                result[current3x3] = 0;
                printf("Invalid 3x3 square: %d\n",current3x3);
                current3x3++;
                pthread_exit(0);
            }
        }
    }
    // the current 3x3 square is valid
    result[current3x3] = 1;
    current3x3++;
    pthread_exit(0);
}

int main(int argc, char *argv[]){
    // file was not provided
    if (argc == 1){
        printf("Insufficient parameters passed.\n");
        return -1;
    }
    // open the file and put sudoku board into our sudoku array
    FILE *sudoku_board = fopen(argv[1], "r");
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            fscanf(sudoku_board, "%d", &sudoku[i][j]);
        }
    }

    //create the threads to perform the checks
    parameters *rowcheck = (parameters *) malloc(sizeof(parameters));
    rowcheck->row = 0;
    rowcheck->column = 0;
    pthread_create(&row_thread, NULL, check_rows, (void *) rowcheck);
    
    parameters *columncheck = (parameters *) malloc(sizeof(parameters));
    rowcheck->row = 0;
    rowcheck->column = 0;
    pthread_create(&column_thread, NULL, check_columns, (void *) columncheck);

    parameters *square1 = (parameters *) malloc(sizeof(parameters));
    rowcheck->row = 0;
    rowcheck->column = 0;
    pthread_create(&square_thread_1, NULL, square_check, (void*) square1);

    parameters *square2 = (parameters *) malloc(sizeof(parameters));
    rowcheck->row = 0;
    rowcheck->column = 3;
    pthread_create(&square_thread_2, NULL, square_check, (void*) square2);

    parameters *square3 = (parameters *) malloc(sizeof(parameters));
    rowcheck->row = 0;
    rowcheck->column = 6;
    pthread_create(&square_thread_3, NULL, square_check, (void*) square3);

    parameters *square4 = (parameters *) malloc(sizeof(parameters));
    rowcheck->row = 3;
    rowcheck->column = 0;
    pthread_create(&square_thread_4, NULL, square_check, (void*) square4);

    parameters *square5 = (parameters *) malloc(sizeof(parameters));
    rowcheck->row = 3;
    rowcheck->column = 3;
    pthread_create(&square_thread_5, NULL, square_check, (void*) square5);

    parameters *square6 = (parameters *) malloc(sizeof(parameters));
    rowcheck->row = 3;
    rowcheck->column = 6;
    pthread_create(&square_thread_6, NULL, square_check, (void*) square6);

    parameters *square7 = (parameters *) malloc(sizeof(parameters));
    rowcheck->row = 6;
    rowcheck->column = 0;
    pthread_create(&square_thread_7, NULL, square_check, (void*) square7);

    parameters *square8 = (parameters *) malloc(sizeof(parameters));
    rowcheck->row = 6;
    rowcheck->column = 3;
    pthread_create(&square_thread_8, NULL, square_check, (void*) square8);
    
    parameters *square9 = (parameters *) malloc(sizeof(parameters));
    rowcheck->row = 6;
    rowcheck->column = 6;
    pthread_create(&square_thread_9, NULL, square_check, (void*) square9);


    // use pthread_join to wait for all threads to finish
    pthread_join(column_thread, NULL);
    pthread_join(row_thread, NULL);
    pthread_join(square_thread_1, NULL);
    pthread_join(square_thread_2, NULL);
    pthread_join(square_thread_3, NULL);
    pthread_join(square_thread_4, NULL);
    pthread_join(square_thread_5, NULL);
    pthread_join(square_thread_6, NULL);
    pthread_join(square_thread_7, NULL);
    pthread_join(square_thread_8, NULL);
    pthread_join(square_thread_9, NULL);


    // iterate through our results and check if any of them are 0, if so return NOT solved
    int k = 0;
    while (k < 11){
        if (result[k] == 1){
            k++;
        }
        else{
            printf("The sudoku puzzle is NOT solved \n");
            exit(0);
        }
    }
    printf("Sudoku Puzzle Soultion is: \n");
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }
    printf("The sodoku is solved \n");
    return 0;
}
    
    
    
    
    
    
    
    
    
    
    
