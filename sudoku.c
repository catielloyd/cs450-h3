/* GROUP MEMBERS:
 * Catie Lloyd
 * Amanda Tomasetti
 */

#include <stdio.h>
#include <stdlib.h>
#include<pthread.h>


#define true 1
#define false 0
#define bool int
#define MAX_LINE_SIZE 20

struct Data {
    int board[9][9];
    bool valid;
    bool validRows[9];
    bool validColumns[9];
    bool validSquares[9];
    int row;
    int id;
    pthread_mutex_t m; // to ensure id is passed successfully
    pthread_cond_t cv; // to signal id has been passed successfully
};

int _atoi(char c){
    int i = c - '0';
    if(i < 0 || i > '9')
        return -1;
    return i;
}

// for debugging
void printData(struct Data* data){
    for(int i = 0; i < 9; i++){
        printf("%d %d %d    %d %d %d    %d %d %d\n", data->board[i][0], data->board[i][1], data->board[i][2],
               data->board[i][3], data->board[i][4], data->board[i][5], data->board[i][6], data->board[i][7],
               data->board[i][8]);
    }
    printf("\n");
}

void parseRow(struct Data* data, char* row){
    int column = 0;
    while(*row != NULL){
        int i = _atoi(row[0]);
        if(i >= 0) {
            data->board[data->row][column] = i;
            column++;
        }
        row++;
    }
    data->row++;
}

char* nameOfSubgrid(int i){
    switch (i){
        case 0:
            return "top left";
        case 1:
            return "top middle";
        case 2:
            return "top right";
        case 3:
            return "middle left";
        case 4:
            return "middle";
        case 5:
            return "middle right";
        case 6:
            return "bottom left";
        case 7:
            return "bottom middle";
        case 8:
            return "bottom right";
        default:
            return "???";
    }
}

void validateRow(struct Data* data){
    int row = data->id;
    pthread_cond_signal(&data->cv);
    int rowCount[9] = {0,0,0,0,0,0,0,0,0};
    for(int i = 0; i < 9; i++){
        rowCount[data->board[row][i] - 1]++;
        if(data->board[row][i] < 1 
            || data->board[row][i] > 9
            || rowCount[data->board[row][i] - 1] > 1){
           data->valid = false;
           data->validRows[row] = false;
           return;
        }
    }
    data->validRows[row] = true;
}

void validateColumn(struct Data *data){
    int column = data->id;
    pthread_cond_signal(&data->cv);
    int colCount[9] = {0,0,0,0,0,0,0,0,0};
    for(int i = 0; i < 9; i++){
        colCount[data->board[i][column] - 1]++;
        if(data->board[i][column] < 1 
            || data->board[i][column] > 9
            || colCount[data->board[i][column] - 1] > 1){
            data->valid = false;
            data->validColumns[column] = false;
            return;
        }
    }
    data->validColumns[column] = true;
}

void validateSquare(struct Data *data){
    int square = data->id;
    pthread_cond_signal(&data->cv);
    int sqrCount[9] = {0,0,0,0,0,0,0,0,0};
    int columnStart = (square % 3) * 3;
    int columnEnd = columnStart + 3;
    int rowStart = (square / 3) * 3;
    int rowEnd = rowStart + 3;

    for(int i = rowStart; i < rowEnd; i++) {
        for (int j = columnStart; j < columnEnd; j++) {
           sqrCount[data->board[i][j] - 1]++;
           if(data->board[i][j] < 1 
               || data->board[i][j] > 9
               || sqrCount[data->board[i][j] - 1] > 1){
               data->valid = false;
               data->validSquares[square] = false;
               return;
           }

        }
    }
    data->validSquares[square] = true;
}

void validateBoard(struct Data* data){
    data->valid = true;

    int total_threads = 27; // 9 rows + 9 columns + 9 squares
    int thread_i = 0;
    pthread_t* thread_handles =  malloc(total_threads * sizeof(pthread_t));
    pthread_mutex_init(&data->m, NULL); // initialize id lock
    pthread_cond_init(&data->cv, NULL); //initialize id signal

    for(int i = 0; i < 9; i++){
        data->id = i;
        
        // check rows
        pthread_create(&thread_handles[thread_i], NULL, validateRow, data); // create thread
        thread_i++; // increment thread counter
        while(pthread_cond_wait(&data->cv, &data->m));


        // check columns
        pthread_create(&thread_handles[thread_i], NULL, validateColumn, data); // create thread
        thread_i++; // increment thread counter
        while(pthread_cond_wait(&data->cv, &data->m));
        
        // check squares
        pthread_create(&thread_handles[thread_i], NULL, validateSquare, data); // create thread
        thread_i++; // increment thread counter
        while(pthread_cond_wait(&data->cv, &data->m));
    }


    // join threads
    for(int i = 0; i < thread_i; i++){
        pthread_join(thread_handles[i], NULL);
    }

    // cleanup
    free(thread_handles);

    // print results
    for(int i = 0; i < 9; i++){
        if(!data->validRows[i]){
           printf("Row %d doesn't have the required values.\n", i + 1);
        }

        if(!data->validColumns[i]){
           printf("Column %d doesn't have the required values.\n", i + 1);
        }

        if(!data->validSquares[i]){
           printf("Square %d doesn't have the required values.\n", i + 1);
        }
    }

    if(data->valid){
        printf("This input is a valid Sudoku.\n");
    } else {
        printf("This input is not a valid Sudoku.\n");
    }
}

int main() {
    struct Data data;
    char* line[MAX_LINE_SIZE];
    data.row = 0;
    while(fgets(line, MAX_LINE_SIZE, stdin) && data.row < 9) {
        parseRow(&data, line);
    }

    validateBoard(&data);

    return 0;
}
