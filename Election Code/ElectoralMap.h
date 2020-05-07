#ifndef _ELECTORALMAP_H_
#define _ELECTORALMAP_H_
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>


#define MIN_AREA 5
#define MAX_AREA 29
#define NUM_PARTIES 3

enum class Party{GALACTIC_REPUBLIC, SEPERATISTS, NONE};

class District {
public:
    District();
    District(int id);

    int get_id(){ return id_; };
    int get_area(){ return area_; };
    int GetNumParties();
    int GetPartySize(Party p);
    int GetTotalSize();
    void MoveConstituent(Party from, Party to);
    Party MajorityParty();
    Party MajorityParty(std::vector<Party> include);
    Party MajorityPartyExclude(std::vector<Party> exclude);

    static std::vector<Party> GetPartyVector(){
        std::vector<Party> ret;
        ret.push_back(Party::GALACTIC_REPUBLIC);
        ret.push_back(Party::SEPERATISTS);
        ret.push_back(Party::NONE);
        return ret;
    }

    static std::string PartyToString(Party p){
        if(p == Party::GALACTIC_REPUBLIC){
            return "Galactic Republic";
        }else if(p == Party::SEPERATISTS){
            return "Seperatist";
        }else{
            return "None";
        }
    }

    friend std::ostream& operator<<(std::ostream &os, const District &d);

private:
    int id_;
    int area_;
    std::map<Party, int> parties_; 
}; // class District


class ElectoralMap {
public:
	
	static ElectoralMap& GetInstance();
    District* GetDistrict(int id);
    int GetTotalSize();

    friend std::ostream& operator<<(std::ostream &os, const ElectoralMap &em);

    static const int num_districts_ = 4;

	// Delete the methods we don't want
	ElectoralMap(ElectoralMap const&) = delete; // copy constructor
	void operator=(ElectoralMap const&) = delete; // assignment operator

 private:
 	ElectoralMap();  // private constructor
    std::map<int, District> districts_;
  
};  // class ElectoralMap

#endif  // _ELECTORALMAP_H_