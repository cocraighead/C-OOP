#include <iostream>
#include <string>
#include <map>
#include <iterator>
#include "Society.h"

/*
*   Function returns if the City is larger than the constant number for a big city
*
*   Params: 
*
*   return: bool of true if the city is biger than big and false otherwise
*/
bool City::IsBig() const {
    return population_ > BIG_CITY_SIZE;
}

/*
*   Function increses the city by the passed amount and records the percent change
*
*   Params:  num_people is the amount to increase the population by.
*
*   return: void
*/
void City::Grow(int const num_people) {
    if (num_people >= 0) {
        int prev = population_;
        population_ += num_people;
        percent_change_ = ((float)num_people) / prev;
    }
}

/*
*   Function decreses the city by the passed amount and records the percent change
*
*   Params:  num_people is the amount to dencrease the population by.
*
*   return: void
*/
void City::Shrink(int const num_people) {
    if (num_people >= 0) {
        population_ -= num_people;
    }
}

/*
*   Function returns a pointer to the biggest city by population in the society
*
*   Params: 
*
*   return: pointer to the largest city
*/
City* Society::GetBiggestCity(){
    std::map<int, City*>::iterator it = cities_.begin();
    City *biggest = it->second;
    while (it != cities_.end()){
		City *value = it->second;
		if(value->get_population() > biggest->get_population()){
            biggest = value;
        }
		it++;
	}
    return biggest;
}

/*
*   Function calculates the amount that each city will grow and shrink and updates them
*
*   Params:  
*
*   return: void
*/
void Society::GrowCities() {
    if(!cities_.empty()){
        City * biggest = GetBiggestCity();
        std::map<int, City*>::iterator it = cities_.begin();
        while (it != cities_.end()){
            City *c = it->second;
		    if(!c->IsBig()){
                int prev = c->get_population();
                double ratio = ((float)c->get_population()) / biggest->get_population();
                c->Grow((int) ratio * GROWTH_FACTOR * c->get_population());
                c->Shrink((int) c->get_population() * SHRINK_FACTOR);
                double percent_change = (((float)c->get_population()) - prev) / prev;
                c->set_percent_change(percent_change);
            }else{
                int prev_small = c->get_population();
                double ratio = ((float)c->get_population()) / biggest->get_population();
                c->Grow((int) ratio * c->get_population());
                c->Shrink((int) c->get_population() * SHRINK_FACTOR);
                double percent_change_small = (((float)c->get_population()) - prev_small) / prev_small;
                c->set_percent_change(percent_change_small);
            }
		    it++;
	    }
    }
}

/*
*   Overloading the << operator so that a city can be printed
*
*   Params: os: output stream, c reference to a city
*
*   return: the output stream filled
*/
std::ostream& operator<<(std::ostream &os, const City &c) {
    os << c.name_ << ": " << c.population_  << " (" << c.percent_change_ << "% change)";
    return os;
}

/*
*   Overloading the << operator so that a Society can be printed
*
*   Params: os: output stream, s reference to a Society
*
*   return: the output stream filled
*/
std::ostream& operator<<(std::ostream &os, const Society &s) {
    std::map<int, City*>::const_iterator it = s.cities_.begin();
    while (it != s.cities_.end()){
		os << *(it->second) << std::endl;
		it++;
	}
    return os;
}