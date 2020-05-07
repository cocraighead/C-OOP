#include "Election.h"


/**
    Constructor - sets the candidate withe the default values for all fields

    @param 
    @return 
**/
Candidate::Candidate(){};


/**
    Constructor - sets the candidate withe the passed values for all fields

    @param id to set to id_, party to set to party_, name to set to name_
    @return 
**/
Candidate::Candidate(int id, Party party, std::string name){
    id_ = id;
    party_ = party;
    name_ = name;
}


/**
    Overloads the << operator for Candidate 

    @param os is the output stream to fill, c is the Candidate to fill the output stream with
    @return output stream of the name and Party of c
**/
std::ostream& operator<<(std::ostream &os, const Candidate &c){
    os << c.name_ << " [Party: " << Election::PartyToString(c.party_) << "]";
    return os;
}


/**
    Returns the number of registered Candidates

    @param 
    @return total Candidates
**/
int Election::NumCandidates(){
    return candidates_.size();
}


/**
    Gets a pointer to the Candidate with the passed id

    @param id to look for
    @return pointer to Candidate found
**/
Candidate* Election::GetCandidate(int id){
    return &candidates_.at(id);
}


/**
    Gets a vector of Candidates that are from the Parties in the include vector

    @param include is a vector of Parties to kepp
    @return a vector of Cadidates
**/
std::vector<Candidate> Election::GetCandidatesFromParties(std::vector<Party> include){
    std::vector<Candidate> ret;
    auto it = candidates_.begin();
    while(it != candidates_.end()){
        for(int i=0;i<include.size();i++){
            if(it->second.get_party() == include[i]){
                ret.push_back(it->second);
            }
        }
        it++;
    }
    return ret;
}


/**
    Gets a vector of Candidates that are not from the Parties in the exclude vector

    @param exclude is a vector of Parties to igrnore
    @return a vector of Cadidates
**/
std::vector<Candidate> Election::GetCandidatesNotFromParties(std::vector<Party> exclude){
    std::vector<Candidate> ret;
    auto it = candidates_.begin();
    while(it != candidates_.end()){
        bool found_flag = false; // flag if Candidate matches any of excludes Parties
        for(int i=0;i<exclude.size();i++){
            if(it->second.get_party() == exclude[i]){
                found_flag = true;
            }
        }
        if(!found_flag){
            ret.push_back(it->second);
        }
        it++;
    }
    return ret;
}

/**
    Gets a Random Candidate from the passed vector with equal chances

    @param choices is the vector of Candidates to choose from
    @return Choosen Candidate
**/
Candidate Election::PickRandomCandidate(std::vector<Candidate> choices){
    int rand_int = rand() % choices.size();
    return choices[rand_int];
}


/**
    Gets a vector of all the Parties who have a Candidate

    @param 
    @return vector of all the Parties
**/
std::vector<Party> Election::PartiesWithCandidates(){
    std::vector<Party> ret;
    auto it = candidates_.begin();
    while(it != candidates_.end()){
        bool flag_not_in = true; // Checks if the party is already been added to the return vector
        for(int i=0;i<ret.size();i++){
            if(it->second.get_party() == ret[i]){
                flag_not_in = false;
            }
        }
        if(flag_not_in){
            ret.push_back(it->second.get_party());
        }
        it++;
    }
    return ret;
}


/**
    Adds the passed candidate to the Election's Candidate Map

    @param c is the Candidate to add
    @return 
**/
void Election::RegisterCandidate(Candidate c){
    candidates_.insert(std::pair<int, Candidate>(c.get_id(),c));
}


/**
    Calculates if the candidate converts andy voters to their party when campaigning in the district

    @param p_candidate is a pointer to the candidate who is campaigning, p_district is a pointer to the district to campaign in
    @return 
**/
void Election::CampaignCandidateInDistrict(Candidate *p_candidate, District *p_district){
    std::cout << "Candidate " << *p_candidate << " is campaigning in district " << p_district->get_id() << std::endl;

    float random_cutoff = (rand() % 100)/100.0; // a random decimal between 0 and .99
    int constituents_cands_party = p_district->GetPartySize(p_candidate->get_party()); // the number of voters in the Candidates party
    int constituents_other_exclude_none = p_district->GetTotalSize() - p_district->GetPartySize(Party::NONE) - p_district->GetPartySize(p_candidate->get_party()); // Voters from other parties that are not the Candidates or none
    float p_success = 0;
    if(constituents_other_exclude_none >= 1){ // avoids div by 0
        p_success = ( (float)((constituents_cands_party+1)*2) / (constituents_other_exclude_none) ) * ( (float)((constituents_cands_party+1)*2) / (p_district->get_area()) ); // Prob of a none voter moving to the Candidates party
    }
    float p_extra_success = 0.1*p_success; // Prob of a majority party voter moving to the candidates party
    std::cout << "Chances to convert: " << p_success*100 << std::endl;
    std::cout << "Chances to convert from another party: " << p_extra_success*100 << std::endl;
    std::cout << std::endl;
    int size_party_none = p_district->GetPartySize(Party::NONE);
    std::vector<Party> exclude_parties = {p_candidate->get_party(), Party::NONE};
    Party majority_party = p_district->MajorityPartyExclude(exclude_parties); // Majority party thats not the candidates or none to take a voter from (if prob lines up)
    int size_majority_party = p_district->GetPartySize(majority_party); // Size of the party found one line above

    if(random_cutoff < p_extra_success){ // if a Majority party voter should be taken
        if(size_majority_party > 0){ // if there is a voter to take
            p_district->MoveConstituent(majority_party, p_candidate->get_party());
        }
    }
    if(random_cutoff < p_success){ // if a none party voter should be taken
        if(size_party_none > 0){ // if there is a voter to take
            p_district->MoveConstituent(Party::NONE, p_candidate->get_party());
        }
    }
}


/**
    Calculates the total number of voters for each Candidate

    @param 
    @return a map of the Candidates id to the number of votes they have
**/
std::map<int, int> Election::CalculateVote(){
    ElectoralMap &em = ElectoralMap::GetInstance();
    std::map<int, int> vote_tracker; // to be passed by ref to all the helper functions
    auto it = candidates_.begin();
    while(it != candidates_.end()){
        vote_tracker[it->first] = 0; // zeroing out votes
        it++;
    }

    for(int i=0;i<ElectoralMap::num_districts_;i++){
        CalculateVoteDistrict(*(em.GetDistrict(i)), vote_tracker);
    }

    return vote_tracker;
}


/**
    Calculates the total number of voters for each Candidate in this District

    @param  d the District to collect votes in, the refernce map to accumulate votes in
    @return 
**/
void Election::CalculateVoteDistrict(District d, std::map<int,int> &vt){
    std::vector<Party> party_vector = Election::GetPartyVector(); 
    for(int i=0;i<party_vector.size();i++){
        CalculateVoteParty(d, party_vector[i], vt);
    }
}


/**
    Calculates the number of voters for each Candidate for a certain party

    @param d the District to collect votes in, Party p to collect votes from, the refernce map to accumulate votes in
    @return
**/
void Election::CalculateVoteParty(District d, Party p, std::map<int,int> &vt){
    for(int i=0;i<d.GetPartySize(p);i++){ // represents each voter in p
        Candidate choosen;
        float rand_prob = (rand() % 100)/100.0; // a random decimal between 0 and .99
        std::vector<Party> my_party_vector = {p};
        std::vector<Candidate> my_party_candidates = GetCandidatesFromParties(my_party_vector); // Vector of Candidates from p's Party
        if(my_party_candidates.size() != 0){ // If their party has a Candidate
            if(rand_prob < .9){ // If they vote for their Party
                choosen = PickRandomCandidate(my_party_candidates);
            }else{ // They vote for another Party
                std::vector<Candidate> not_my_party_candidates = GetCandidatesNotFromParties(my_party_vector); // Candidates from other Parties
                if(not_my_party_candidates.size() != 0 ){ // If there are Candidates from other Parties
                    choosen = PickRandomCandidate(not_my_party_candidates);
                }else{ // If not choose a Candidate
                    choosen = PickRandomCandidate(GetCandidatesFromParties(PartiesWithCandidates()));
                }
            }
        }else{ // If their party doesn't have a Candidate, This also applies to all of Party none voters
            Party majority_party_with_candidate = d.MajorityParty(PartiesWithCandidates());
            std::vector<Party> majority_party_with_candidate_vector = {majority_party_with_candidate}; // Majority Party with a Candidate
            if(rand_prob < .7){ // If they vote for the majority party
                std::vector<Candidate> majority_candidates = GetCandidatesFromParties(majority_party_with_candidate_vector);
                choosen = PickRandomCandidate(majority_candidates);
            }else{ // if they don't vote for the majority party
                std::vector<Candidate> not_majority_candidates = GetCandidatesNotFromParties(majority_party_with_candidate_vector);
                if(not_majority_candidates.size() != 0){ // If there is another party besides the majority with a Candidate
                    choosen = PickRandomCandidate(not_majority_candidates);
                }else{ // If there isn't then vote for a Candidate
                    choosen = PickRandomCandidate(GetCandidatesFromParties(PartiesWithCandidates()));
                }
            }
        }
        vt[choosen.get_id()]++; // Record the voters choice in the ref map
    }
}


/**
    Calculates the the winner of the vote based on the map that holds the tally

    @param  votes is a map that holds the Candidates id and the total votes they have
    @return The Candidate with the most votes
**/
Candidate Election::CalculateWinner(std::map<int,int> votes){
    auto it = votes.begin();
    int max_votes_id = it->first;
    int max_votes = it->second;
    while(it != votes.end()){
        if(it->second > max_votes){
            max_votes_id = it->first;
            max_votes = it->second;
        }
        it++;
    }
    return candidates_.at(max_votes_id);
}


/**
    Overloads the << operator for Election 

    @param os is the output stream to fill, e is the Election to fill the output stream with
    @return output stream of all Candidates in e
**/
std::ostream& operator<<(std::ostream &os, const Election &e){
    auto it = e.candidates_.begin();
    while(it != e.candidates_.end()){
        os << "id:" << it->first << " " << it->second << "\n";
        it++;
    }
    return os;
}


/**
    Calculates the total number of district votes for each Candidate

    @param 
    @return a map of the Candidates id to the number of district votes they have
**/
std::map<int, int> RepresentativeElection::CalculateVote(){
    ElectoralMap &em = ElectoralMap::GetInstance();
    int total_districts_votes = 5*ElectoralMap::num_districts_; // the total number of votes for the election
    std::map<int, int> vote_tracker; // Tracks the district votes for each candidate
    auto it_cand = get_candidates().begin();
    while(it_cand != get_candidates().end()){
        vote_tracker[it_cand->first] = 0; 
        it_cand++;
    }

    std::map<int, int> empty_vote_tracker; // An empty map to use for each district
    auto it_emp = get_candidates().begin();
    while(it_emp != get_candidates().end()){
        empty_vote_tracker[it_emp->first] = 0; 
        it_emp++;
    }
    
    for(int i=0;i<ElectoralMap::num_districts_;i++){
        int district_votes = floor(((float)em.GetDistrict(i)->GetTotalSize()/em.GetTotalSize())*total_districts_votes); // How many votes the current district has
        std::map<int, int> vote_tracker_district = empty_vote_tracker; 
        CalculateVoteDistrict(*(em.GetDistrict(i)), vote_tracker_district); // Tallies the vote of the district per candidate base on voters
        vote_tracker[CalculateWinner(vote_tracker_district).get_id()] += district_votes; // Calculates who won the district and gives the district votes to that Candidate
    }
    return vote_tracker;
}