#ifndef _ELECTION_H_
#define _ELECTION_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <math.h>
#include "ElectoralMap.h"

class Candidate {
public:
    Candidate();
    Candidate(int id, Party party, std::string name);

    int get_id(){ return id_; };
    Party get_party(){ return party_; };
    std::string get_name(){ return name_; };

    friend std::ostream& operator<<(std::ostream &os, const Candidate &c);

private:
    int id_;
    Party party_;
    std::string name_;

}; // class Candidate

class Election {
public:

    std::map<int, Candidate> get_candidates(){ return candidates_; };
    int NumCandidates();
    void CampaignCandidateInDistrict(Candidate *p_candidate, District *p_district);
    Candidate* GetCandidate(int id);
    void RegisterCandidate(Candidate c);
    virtual std::map<int, int> CalculateVote();
    void CalculateVoteDistrict(District d, std::map<int,int> &vt);
    void CalculateVoteParty(District d, Party p, std::map<int,int> &vt);
    std::vector<Candidate> GetCandidatesFromParties(std::vector<Party> include);
    std::vector<Candidate> GetCandidatesNotFromParties(std::vector<Party> exclude);
    Candidate PickRandomCandidate(std::vector<Candidate> choices);
    std::vector<Party> PartiesWithCandidates();
    Candidate CalculateWinner(std::map<int,int> votes);

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
    friend std::ostream& operator<<(std::ostream &os, const Election &e);

private:
    std::map<int, Candidate> candidates_; 
};

class RepresentativeElection : public Election {
public:
    std::map<int, int> CalculateVote();
private:

};


#endif  // _ELECTION_H_