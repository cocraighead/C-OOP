#ifndef _TEXT_UI_H_
#define _TEXT_UI_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip> // setprecision
#include <sstream> // stringstream

#include "Election.h"


class TextUI {
public:

  int ElectionTypeMenu();
  void RegistrationMenu(Election *elect);
  void CampaignMenu(Election *elect);
  void VoteResults(Election *elect);
  bool ValidateId(std::string s, int size);


private:
  
}; // TextUI Class


#endif  // _TEXT_UI_H_