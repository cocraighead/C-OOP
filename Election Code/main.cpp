

/**
    Colin Craighead
    Homework 3
    This program alows users to customize an election for semi-randomly created districts.
    To run just use the Makefile and ./election with no command line arguments

**/


#include <time.h>
#include "ElectoralMap.h"
#include "Election.h"
#include "TextUI.h"


int main(){
    srand (time(NULL)); //single seting of the random

    ElectoralMap &electoral_map = ElectoralMap::GetInstance();
    TextUI text_menu;
    bool election_flag = true;// If the user still wants so create an election
    while(election_flag){
        int choice = text_menu.ElectionTypeMenu();
        if(choice >= 0){
            Election *e_p;
            if(choice == 0){
                e_p = new Election();
            }else if(choice == 1){
                e_p = new RepresentativeElection();
            }
            text_menu.RegistrationMenu(e_p);
            std::cout << std::endl;
            std::cout << electoral_map;
            std::cout << "------------------------" << std::endl;
            std::cout << std::endl;
            std::cout << *(e_p) << std::endl;
            text_menu.CampaignMenu(e_p);
            std::cout << "Collecting Votes" << std::endl;
            text_menu.VoteResults(e_p);
        }else{
            election_flag = false;
        }
    } 
    
    return 0;
}