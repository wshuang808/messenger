# include <stdio.h>
# include <stdbool.h>
# include <limits.h>


# define NUM_MAX_CITY 10
# define TIME_INF INT_MAX
# define TIME_IN_SAME_CITY 0
# define NULL_CITY -1
# define VISITED 1
# define UNVISITED 0


bool getDataFromPlayer();
void normalizeData(); 
void traceInput();
void caculation();
void initIntArrayByValue(int* pArray, unsigned int length, int value);
void traverseCity(unsigned int cityId); 
void updateCityData(unsigned int cityId, unsigned int preCityId, int minTime);
void evaluatePath(unsigned int startCity, unsigned int endCity);


unsigned int numCity;
int matrix[NUM_MAX_CITY][NUM_MAX_CITY];
int minTimes[NUM_MAX_CITY];
int preCities[NUM_MAX_CITY];  // previous city on the shortest path for each city


int main(){
    bool dataReceived = getDataFromPlayer();
    if (dataReceived){
        normalizeData();
        traceInput();
    }
}


bool getDataFromPlayer(){
    bool result = false;
    printf("Input number of city (1-100):");
    scanf("%d", &numCity);
    if (1 <= numCity && numCity <= NUM_MAX_CITY){
        for (unsigned int i = 1; i < numCity; ++i){
            for (unsigned int j = 0; j < i; ++j){
                scanf("%d", &matrix[i][j]);
            }
        }
        result = true;
    }
    else {
        printf("Invalid city number %d.\n", numCity);
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
    initIntArrayByValue(minTimes, numCity, TIME_INF);
    minTimes[0] = TIME_IN_SAME_CITY; // set time for capitol
    initIntArrayByValue(preCities, numCity, NULL_CITY);

    traverseCity(0);
} 


void traverseCity(unsigned int rootCity){
    unsigned int startCity, endCity;
    startCity = rootCity;
    endCity = startCity + 1;

    int pathTime, minTime;
    for (; endCity < numCity; ++endCity){
        if (matrix[endCity][startCity] == UNVISITED){
            matrix[endCity][startCity] = VISITED; // set path visited
            evaluatePath(startCity, endCity);
            traverseCity(endCity);
        }
    }
}


void evaluatePath(unsigned int startCity, unsigned int endCity){
    int pathTime = matrix[startCity][endCity];
    int minTime;
    if (pathTime != TIME_INF){
        if (minTimes[endCity] == TIME_INF){
            minTime = minTimes[startCity] + pathTime;
            updateCityData(endCity, startCity, minTime);
        }
        else{
            minTime = minTimes[startCity] + pathTime;
            if (minTimes[endCity] > minTime){
                updateCityData(endCity, startCity, minTime);
            }
            else{
                minTime= minTimes[endCity] + pathTime;
               if (minTimes[startCity] > minTime){
                   updateCityData(startCity, endCity, minTime);
               } 
            }
        }
    }
}


void updateCityData(unsigned int cityId, unsigned int preCityId, int minTime){
    unsigned int oldPreCity = preCities[cityId];
    minTimes[cityId] = minTime;
    preCities[cityId] = preCityId;

    if (oldPreCity != NULL_CITY){
        evaluatePath(cityId, preCityId);
    }
}


void initIntArrayByValue(int* pArray, unsigned int length, int value){
    for (unsigned int i = 0; i < length; ++i){
        pArray[i] = value;
    }
}

