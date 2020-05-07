#include "ElectoralMap.h"


/**
    Constructor - Creates the static number of Districts and sets their id to be the index

    @param 
    @return
**/
ElectoralMap::ElectoralMap(){
    for(int i=0;i<ElectoralMap::num_districts_;i++){
        District d(i);
        districts_[i] = d;
    }
}


/**
    Gets the singleton Instace of the ElectoralMap class

    @param 
    @return Reference to single instance
**/
ElectoralMap& ElectoralMap::GetInstance(){
	// gets instantiated the first time
	static ElectoralMap instance; // guaranteed to be destroyed
	return instance;
}


/**
    Gets a pointer to the District with the passed id

    @param id int id of the District to return
    @return pointer to the found District
**/
District* ElectoralMap::GetDistrict(int id){
    return &districts_.at(id);
}


/**
    Gets the total number of constituents from all districts

    @param 
    @return int total number of constituents
**/
int ElectoralMap::GetTotalSize(){
    int sum = 0;
    for(int i=0;i<districts_.size();i++){
        sum += districts_.at(i).GetTotalSize();
    }
    return sum;
}

/**
    Overloads the << operator for ElectoralMap 

    @param os is the output stream to fill, em is the electoral map to fill the output stream with
    @return output stream of all the Districts in em
**/
std::ostream& operator<<(std::ostream &os, const ElectoralMap &em){
    auto it = em.districts_.begin();
    while(it != em.districts_.end()){
        os << it->second << "\n";
        it++;
    }
    return os;
}


/**
    Constructor - Deafult District has the id of -1

    @param 
    @return 
**/
District::District(){
    id_ = -1;
    area_ = rand() % (MAX_AREA - MIN_AREA) + MIN_AREA; // random area between max and min
    for(int i = 0;i<NUM_PARTIES;i++){
        parties_[static_cast<Party>(i)] = rand() % 10; // random number of constituents between 0 and 9
    }
}


/**
    Constructor - District has the id of the passed id

    @param int id is the id_ of the district
    @return 
**/
District::District(int id){
    id_ = id;
    area_ = rand() % (MAX_AREA - MIN_AREA) + MIN_AREA; // random area between max and min
    for(int i = 0;i<NUM_PARTIES;i++){
        parties_[static_cast<Party>(i)] = rand() % 10; // random number of constituents between 0 and 9
    }
}


/**
    Gets the number of parties in the District

    @param 
    @return number of parties
**/
int District::GetNumParties(){
    return parties_.size();
}


/**
    Gets the number of constituents in Party p in the district

    @param Party p is the party to look at
    @return Number of constituents
**/
int District::GetPartySize(Party p){
    return parties_.at(p);
}


/**
    Gets the total number of constituents inall Parties in the District

    @param 
    @return Total Number of constituents
**/
int District::GetTotalSize(){
    int sum = 0;
    auto it = parties_.begin();
    while(it != parties_.end()){
        sum += it->second;
        it++;
    }
    return sum;
}


/**
    Adds a constituent to Party to and removes one from Party From

    @param Party from is the Party losing a constituent, Party to is the Party gaining a constituent
    @return
**/
void District::MoveConstituent(Party from, Party to){
    auto it_from = parties_.find(from);
    auto it_to = parties_.find(to);
    it_from->second--;
    it_to->second++;
}


/**
    Gets the Party with the most constituents

    @param 
    @return That Party
**/
Party District::MajorityParty(){
    auto it = parties_.begin();
    int max = it->second; // Current max number of constituents
    Party max_party = it->first; // The Party enum tag that holds the current max
    while(it != parties_.end()){
        if(it->second > max){
            max = it->second;
            max_party = it->first;
        }
        it++;
    }
    return max_party;
}


/**
    Gets the Party with the most constituents from the Parties in include

    @param Parties to include while looking for the majority Party
    @return That Party
**/
Party District::MajorityParty(std::vector<Party> include){
    std::map<Party, int> temp; // Temp map that holds the Parties we wanted
    for(int i=0;i<include.size();i++){
        temp[include[i]] = parties_.at(include[i]);
    }
    auto it = temp.begin();
    int max = it->second;
    Party max_party = it->first;
    while(it != temp.end()){
        if(it->second > max){
            max = it->second;
            max_party = it->first;
        }
        it++;
    }
    return max_party;
}

/**
    Gets the Party with the most constituents from the Parties not in  exclude

    @param Parties to ignore while looking for the majority Party
    @return That Party
**/
Party District::MajorityPartyExclude(std::vector<Party> exclude){
    std::map<Party, int> temp = parties_; // Temp is parties_ but with the values of exclude removed
    for(int i=0;i<exclude.size();i++){
        temp.erase(exclude[i]);
    }
    auto it = temp.begin();
    int max = it->second;
    Party max_party = it->first;
    while(it != temp.end()){
        if(it->second > max){
            max = it->second;
            max_party = it->first;
        }
        it++;
    }
    return max_party;
}


/**
    Overloads the << operator for District 

    @param os is the output stream to fill, d is the District to fill the output stream with
    @return output stream of all the Parties in d
**/
std::ostream& operator<<(std::ostream &os, const District &d){
    os << "District: " << d.id_ << "\n";
    os << "square miles: " << d.area_ << "\n";
    auto it = d.parties_.begin();
    while(it != d.parties_.end()){
        os << "[Party: " << District::PartyToString(it->first) << "] : " << it->second << "\t";
        it++;
    }
    os << "\n";
    return os;
}
