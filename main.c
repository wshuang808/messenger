# include <stdio.h>
# include <stdbool.h>
# include <limits.h>
# include <stdlib.h>
# include <string.h>

# define NUM_MAX_CITY 100
# define TIME_INF INT_MAX
# define TIME_IN_SAME_CITY 0
# define NULL_CITY -1
# define VISITED 1
# define UNVISITED 0
# define MAX_INPUT_WIDTH 4
# define MAX_INPUT_VALUE 9999


bool getDataFromPlayer();
void normalizeData(); 
void traceInput();
void caculation();
void initIntArrayByValue(int* pArray, unsigned int length, int value);
void traversePathFromCity(unsigned int cityId); 
void updateCityData(unsigned int cityId, unsigned int preCityId);
void evaluatePath(unsigned int startCity, unsigned int endCity);
int getSpreadTime(unsigned int city);
int getTimeOnPath(unsigned int startCity, unsigned int endCity);
int compareInt(const void* a, const void* b);


unsigned int numCity;
int matrix[NUM_MAX_CITY][NUM_MAX_CITY];
int minTimes[NUM_MAX_CITY];
int preCities[NUM_MAX_CITY];  // previous city on shortest path for each city


int main(){
    bool dataReceived = getDataFromPlayer();
    if (dataReceived){
        normalizeData();
        //traceInput();
        caculation();
        qsort(minTimes, numCity, sizeof(int), compareInt);
        if (minTimes[0] != TIME_INF){
            printf("Result is %d.\n",minTimes[0]);
        }
        else{
            printf("Invalid input, at least one city can't be reached from capitol.\n");
        }
    }
}


bool getDataFromPlayer(){
    bool result;
    printf("Input number of city (1-100):");
    scanf("%d", &numCity);
    char tempStr[MAX_INPUT_WIDTH + 1];
    if (1 <= numCity && numCity <= NUM_MAX_CITY){
        printf("Input adjacency matrix (1-%d or x for each element):\n", MAX_INPUT_VALUE);
        for (unsigned int i = 1; i < numCity; ++i){
            for (unsigned int j = 0; j < i; ++j){
                scanf("%4s", tempStr);
                if (strcmp(tempStr, "x") == 0){
                    matrix[i][j] = TIME_INF;
                }
                else{
                    int time = atoi(tempStr);
                    if (time <= 0){
                        printf("Invalid input '%s'", tempStr);
                        return false;
                    }
                    else{
                        matrix[i][j] = time;
                    }
                }
            }
        }
        result = true;
    }
    else {
        printf("Invalid city number '%d'.\n", numCity);
        result = false;
    }
    return result;
}


void normalizeData(){
    for (unsigned int i = 0; i < numCity; ++i){
        for (unsigned int j = 0; j < i + 1; ++j){
            if (i == j){
                matrix[i][j] = TIME_IN_SAME_CITY;
            }          
            else if (i > j){
                matrix[j][i] = matrix[i][j];
                matrix[i][j] = UNVISITED;
            }
        }
    }
}


void traceInput(){
    printf("%d cities in total.\n", numCity);
    for (unsigned int i = 0; i < numCity; ++i){
        for (unsigned int j = 0; j < numCity; ++j){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}


void caculation(){
    initIntArrayByValue(preCities, numCity, NULL_CITY);

    traversePathFromCity(0);

    initIntArrayByValue(minTimes, numCity, TIME_INF);
    minTimes[0] = 0;
    for (int i = 1; i < numCity; ++i){
        minTimes[i] = getSpreadTime(i);
    }
} 


void traversePathFromCity(unsigned int rootCity){
    unsigned int startCity, endCity;
    startCity = rootCity;
    endCity = startCity + 1;

    int pathTime, minTime;
    for (; endCity < numCity; ++endCity){
        if (matrix[endCity][startCity] == UNVISITED){
            matrix[endCity][startCity] = VISITED; // set path visited
            evaluatePath(startCity, endCity);
            traversePathFromCity(endCity);
        }
    }
}


void evaluatePath(unsigned int startCity, unsigned int endCity){
    int pathTime = getTimeOnPath(startCity, endCity);
    int minTime, startCitySpreadTime, endCitySpreadTime;
    if (pathTime != TIME_INF){
        startCitySpreadTime = getSpreadTime(startCity);
        endCitySpreadTime = getSpreadTime(endCity);
        if (endCitySpreadTime == TIME_INF){
            updateCityData(endCity, startCity);
        }
        else{
            minTime = startCitySpreadTime + pathTime;
            if (endCitySpreadTime > minTime){
                updateCityData(endCity, startCity);
            }
            else{
                minTime= endCitySpreadTime + pathTime;
               if (startCitySpreadTime > minTime){
                   updateCityData(startCity, endCity);
               } 
            }
        }
    }
}


// update path info for target city and previouse city on the path
void updateCityData(unsigned int cityId, unsigned int preCityId){
    unsigned int oldPreCity = preCities[cityId];
    preCities[cityId] = preCityId;

    if (oldPreCity != NULL_CITY){
        evaluatePath(cityId, oldPreCity);
    }
}


void initIntArrayByValue(int* pArray, unsigned int length, int value){
    for (unsigned int i = 0; i < length; ++i){
        pArray[i] = value;
    }
}


int getSpreadTime(unsigned int city){
    int spreadTime = TIME_INF;
    if (city == 0){
        spreadTime = 0;
    }
    else{
        unsigned int preCityOnPath = preCities[city];
        // go through the path and caculate spread time
        while (preCityOnPath != NULL_CITY){
            if (spreadTime == TIME_INF){
                spreadTime = 0;
            } 
            
            spreadTime += getTimeOnPath(preCityOnPath, city);
            city = preCityOnPath;
            preCityOnPath = preCities[preCityOnPath];
        }
    }
    return spreadTime;
}


int getTimeOnPath(unsigned int startCity, unsigned int endCity){
    int time;
    if (startCity > endCity){
        time = matrix[endCity][startCity];
    }
    else{
        time = matrix[startCity][endCity];
    }
    return time;
}


int compareInt(const void* a, const void* b){
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;

    if (arg1 < arg2) return 1;
    if (arg1 > arg2) return -1;
    return 0;
}
