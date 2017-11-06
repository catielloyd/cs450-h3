#include <stdio.h>
#include <stdlib.h>


#define true 1
#define false 0
#define bool int
#define MAX_LINE_SIZE 20

struct Data {
    int board[9][9];
    bool valid;
    int row;
};

int _atoi(char c){
    int i = c - '0';
    if(i < 0 || i > '9')
        return -1;
    return i;
}

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

bool validateRow(struct Data* data, int row){
    int rowCount[9] = {0,0,0,0,0,0,0,0,0};
    for(int i = 0; i < 9; i++){
        if(data->board[row][i] < 1 || data->board[row][i] > 9){
           return false; 
        }

        rowCount[data->board[row][i] - 1]++;
        if(rowCount[data->board[row][i] - 1] > 1){
            return false;
        }
    }
    return true;
}

bool validateColumn(struct Data *data, int column){
    int colCount[9] = {0,0,0,0,0,0,0,0,0};
    for(int i = 0; i < 9; i++){
        if(data->board[i][column] < 1 || data->board[i][column] > 9){
           return false; 
        }

        colCount[data->board[i][column] - 1]++;
        if(colCount[data->board[i][column] - 1] > 1){
            return false;
        }
    }
    return true;
}

bool validateSquare(struct Data *data, int square){
    int sqrCount[9] = {0,0,0,0,0,0,0,0,0};
    int columnStart = (square % 3) * 3;
    int columnEnd = columnStart + 3;
    int rowStart = (square / 3) * 3;
    int rowEnd = rowStart + 3;

    for(int i = rowStart; i < rowEnd; i++) {
        for (int j = columnStart; j < columnEnd; j++) {
           if(data->board[i][j] < 1 || data->board[i][j] > 9){
               return false;
           }

           sqrCount[data->board[i][j] - 1]++;
           if(sqrCount[data->board[i][j] - 1] > 1){
               return false;
           }
        }
    }
    return true;
}

void validateBoard(struct Data* data){
    data->valid = true;
    printf("Testing board...\n");
    for(int i = 0; i < 9; i++){
        if(!validateRow(data, i)) {
            printf("Row %d doesn't have the required values.\n", i + 1);
            data->valid = false;
        }
        if(!validateColumn(data, i)) {
            printf("Column %d doesn't have the required values.\n", i + 1);
            data->valid = false;
        }
        if(!validateSquare(data, i)) {
            printf("The %s subgrid doesn't have the required values.\n", nameOfSubgrid(i));
            data->valid = false;
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
    while(fgets(line, MAX_LINE_SIZE, stdin) && data.row < 9) {
        parseRow(&data, line);
    }

    validateBoard(&data);

    return 0;
}
