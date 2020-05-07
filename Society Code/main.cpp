#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "Society.h"

// Name(s): Colin Craighead

// The program stores user created cities in a map and then adjusts their populations based on factors

#define STARTING_CITY_SIZE_UB 140000 // The max size a city can start out as
#define STARTING_CITY_SIZE_LB 10000  // The min size a city can start out as

int main() {
    Society s;
    srand (time(NULL));
    // Prompt the user for the cities they would like to add
    // until they are done adding cities
    int id_count = 0;
    std::string input = "";
    while(input != "DONE"){
        std::cout << "You have " << id_count << " cities." << std::endl;
        std::cout << "Type a city name to add a city or DONE to stop" << std::endl;
        getline(std::cin,input);
        if(input != "DONE"){
            s.AddCity(id_count, rand() % STARTING_CITY_SIZE_UB + STARTING_CITY_SIZE_LB, input);
            id_count++;
        }
    }
    std::cout << s << std::endl;
    // Asks the user how many times they would like to grow the cities

    std::string input_2 = "";
    std::cout << "Type the number of times you want to grow the cities." << std::endl;
    getline(std::cin,input_2);
    int times = std::stoi(input_2);
    for(int i=0;i<times;i++){
        s.GrowCities();
        std::cout << s << std::endl;
    }
}
