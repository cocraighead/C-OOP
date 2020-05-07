#include "TextUI.h"


/**
    Prompts the User if they want to choose a Representative or regular election or stop.

    @param 
    @return The users choice as an int
**/
int TextUI::ElectionTypeMenu(){
    std::string input = "";
    while(input != "-1"){
        std::cout << "What kind of election should we have (direct or representative) (-1 to stop)" << std::endl;
        std::getline(std::cin,input);
        if(input == "direct"){
            return 0;
        }else if(input == "representative"){
            return 1;
        }else if(input == "-1"){
            return -1;
        }
    }
    return -1;
}


/**
    Prompts the User if they want to create a candidate for each party (more than one is possible).
    Prompts the user for a cadidate name if they want to create one.

    @param Election pointer to the created election that the cadidates are a part of.
    @return 
**/
void TextUI::RegistrationMenu(Election *elect){
    std::vector<Party> party_vector = Election::GetPartyVector();

    int current_party_id = 0;// Keeps track of which party is being prompted for
    int candidate_id = 0;// The new candidates id - is forced to be 0,1,2,...,n
    while(current_party_id < party_vector.size()-1){
        std::cout << "Do you want to register a candidate for party " << Election::PartyToString(party_vector[current_party_id]) << " (y or n)?" << std::endl;
        std::string input_reg = "";
        std::getline(std::cin, input_reg);
        if(input_reg == "y"){
            std::cout << "What is their name?" << std::endl;
            std::string input_name = "";
            std::getline(std::cin, input_name);
            Candidate temp(candidate_id, party_vector[current_party_id],input_name);
            elect->RegisterCandidate(temp);
            candidate_id++;
        }else if(input_reg == "n"){
            current_party_id++;
        }
    }
}


/**
    Prompts the User for which candidate and district they want to canmpaign in.

    @param Election pointer to the created election that the cadidates are a part of.
    @return
**/
void TextUI::CampaignMenu(Election *elect){
    ElectoralMap &em = ElectoralMap::GetInstance();
    std::string input_cand_id = "";
    while(input_cand_id != "-1"){
        std::cout << "Which candidate is campaigning (id) (-1 to stop) ?" << std::endl;
        std::getline(std::cin, input_cand_id);
        if(ValidateId(input_cand_id, elect->NumCandidates())){
            Candidate *selected_candidate = elect->GetCandidate(std::stoi(input_cand_id));
            std::string input_dist_id = "";
            while(input_dist_id != "-1"){
                std::cout << "Where is this candidate campaigning (id) (-1 to stop) ?" << std::endl;
                std::getline(std::cin, input_dist_id);
                if(ValidateId(input_dist_id, ElectoralMap::num_districts_)){
                    District *selected_district = em.GetDistrict(std::stoi(input_dist_id));
                    elect->CampaignCandidateInDistrict(selected_candidate, selected_district);
                    std::cout << *(selected_district) << std::endl;
                }
            }
        }
    }
    std::cout << em << std::endl;
}


/**
    Displays the voteing results for the election.

    @param Election pointer to the created election that the cadidates are a part of.
    @return
**/
void TextUI::VoteResults(Election *elect){
    ElectoralMap &em = ElectoralMap::GetInstance();
    std::cout << em << std::endl;
    std::map <int,int> votes = elect->CalculateVote(); // the candidate id and the num votes they have as key values
    auto it = votes.begin();
    while(it != votes.end()){
        std::cout << *(elect->GetCandidate(it->first)) << " has " << it->second << " votes" << std::endl;
        it++;
    }
    std::cout << std::endl;
    Candidate winner = elect->CalculateWinner(votes);
    std::cout << winner << " is the winner" << std::endl;
}


/**
    Makes sure the user input string is a valid int and in the range of id [0,id_max]

    @param s is the user input, size is the rannge [0,size) that the ids can be in
    @return true if the input works, false if not
**/
bool TextUI::ValidateId(std::string s, int size){
    int id;
    try {
        id = std::stoi(s);
    } catch (const std::invalid_argument& ia) {
        return false;
    }
    return (0 <= id && id < size);
}