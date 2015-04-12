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
void traversePathFromCity(unsigned int cityId); 
void updateCityData(unsigned int cityId, unsigned int preCityId);
void evaluatePath(unsigned int startCity, unsigned int endCity);
int getSpreadTime(unsigned int city);
int getTimeOnPath(unsigned int startCity, unsigned int endCity);


unsigned int numCity;
int matrix[NUM_MAX_CITY][NUM_MAX_CITY];
int minTimes[NUM_MAX_CITY];
int preCities[NUM_MAX_CITY];  // previous city on the shortest path for each city


int main(){
    bool dataReceived = getDataFromPlayer();
    if (dataReceived){
        normalizeData();
        traceInput();
        caculation();
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
    initIntArrayByValue(preCities, numCity, NULL_CITY);

    traversePathFromCity(0);

    initIntArrayByValue(minTimes, numCity, TIME_INF);
    minTimes[0] = 0;
    for (int i = 1; i < numCity; ++i){
        minTimes[i] = getSpreadTime(i);
    }

    for (int i = 0; i < numCity; ++i){
        printf("%d ", minTimes[i]);
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
        evaluatePath(cityId, preCityId);
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
