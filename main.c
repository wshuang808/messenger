# include <stdio.h>
# include <stdbool.h>
# include <string.h>

# define NUM_MAX_CITY 10
# define NO_ROUTE -1

bool getDataFromPlayer(unsigned int*, int[NUM_MAX_CITY][NUM_MAX_CITY]);
void normalizeData(int[NUM_MAX_CITY][NUM_MAX_CITY]); 
void traceInput(unsigned int, int[NUM_MAX_CITY][NUM_MAX_CITY]);

int main() {
    unsigned int numCity = 0;
    int ajMatrix[NUM_MAX_CITY][NUM_MAX_CITY];
    memset(ajMatrix , NO_ROUTE, sizeof(ajMatrix[0][0]) * NUM_MAX_CITY * NUM_MAX_CITY);

    bool dataReceived = getDataFromPlayer(&numCity, ajMatrix);
    if (dataReceived) {
        normalizeData(ajMatrix);
        traceInput(numCity, ajMatrix);
    }
}


bool getDataFromPlayer(unsigned int* numCity, int matrix[NUM_MAX_CITY][NUM_MAX_CITY]) {
    bool result = false;
    printf("Input number of city (1-100):");
    scanf("%d", numCity);
    if (1 <= *numCity && *numCity <= NUM_MAX_CITY) {
        for (int i = 1; i < *numCity; ++i) {
            for (int j = 0; j < i; ++j) {
                scanf("%d", &matrix[i][j]);
            }
        }
        result = true;
    }
    else {
        printf("Invalid city number %d.\n", *numCity);
    }
    return result;
}


void normalizeData(int matrix[NUM_MAX_CITY][NUM_MAX_CITY]) {
    for (int i = 0; i < NUM_MAX_CITY; ++i) {
        for (int j = 0; j < NUM_MAX_CITY; ++j) {
            if (i == j) {
                matrix[i][j] = 0;
            }          
            else if (i < j) {
                matrix[i][j] = matrix[j][i];
            }
        }
    }
}


void traceInput(unsigned int numCity, int ajMatrix[NUM_MAX_CITY][NUM_MAX_CITY]) {
    printf("%d cities in total.\n", numCity);
    for (int i = 0; i < NUM_MAX_CITY; ++i) {
        for (int j = 0; j < NUM_MAX_CITY; ++j) {
            printf("%d ", ajMatrix[i][j]);
        }
        printf("\n");
    }
}
