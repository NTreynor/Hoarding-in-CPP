//
// Created by mfbut on 1/20/2018.
//
#include <iostream>
#include "Space.h"
#include "MonopolyUtility.h"
#include "Player.h"
#include "GameState.h"

unsigned long Monopoly::Space::length_of_longest_space_name;

Monopoly::Space::Space(const int spaceNumber, SpaceType spaceType)
    : spaceType(spaceType){

  this->spaceNumber = spaceNumber;
  /*
  std::string space_type_string = boardFile.get_next_field();

  if (space_type_string == "Go") {
    spaceType = SpaceType::goSpace;
    goPtr = std::make_unique<Go>(boardFile);
    // goPtr = new Go(boardFile);
    boardFile.skip_line(); // skip the remainder of the line in the file
  } else if (space_type_string == "Property") {
    spaceType = SpaceType::propertySpace;
    propertyPtr = std::make_unique<Property>(boardFile, gameState.rules);
  } else if (space_type_string == "jail"){
    std::cout << "Unrecognized Space type found in " << boardFile.getFileName() << std::endl;
  } else {
    std::cout << "Unrecognized Space type found in " << boardFile.getFileName() << std::endl;
  }
   */
}


void Monopoly::Space::display() const{
  std::cout << "This is a virtual function. If this is printed, you are not overriding appropriately" << std::endl;
}
  /*
   * {
   *
  const auto frmt_flags = std::cout.flags();

  //display space name
  std::cout.width(length_of_longest_space_name);
  std::cout << getName() << " | ";

  //display owner
  std::cout.width(Monopoly::Player::length_of_longest_player_name);
  if (spaceType == SpaceType::goSpace) {
    const std::string temp = "None";
    std::cout << temp << " | ";
    std::cout << "         | ";
  } else if (spaceType == SpaceType::propertySpace) {
    std::cout << propertyPtr->getOwnerName() << " | ";
    std::cout << propertyPtr->getUpgradesString() << " | ";
  } else {
    std::cout << " This space has an unknown type" << std::endl;
  }
  std::cout.flags(frmt_flags);

  auto player = playersOnSpace.cbegin();
  for (; player != playersOnSpace.cend(); ++player) {
    if (player + 1 == playersOnSpace.cend()) {
      break;
    }

    (*player)->display();
    std::cout << ", ";

  }

  if (player != playersOnSpace.cend()) {
    (*player)->display();
  }
}
   */
  //virtual int Monopoly::Space::getSalary() {
    // No new jobs for yas
    //std::cout << "This is a virtual function. If this is printed, you are not overriding appropriately" << std::endl;
    //return -99;
  //}

//virtual const std::string& Monopoly::Space::getName() const{
  //  std::cout << "This is a virtual function. If this is printed, you are not overriding appropriately" << std::endl;
  //}
/* {
  if (spaceType == SpaceType::goSpace) {
    return goPtr->getName();
  } else {
    return propertyPtr->getName();
  }
}
 */

void Monopoly::Space::addPlayer(Monopoly::Player& player) {
  auto playerIt = playersOnSpace.cbegin();
  for (; playerIt != playersOnSpace.cend(); ++playerIt) {
    if (player.getId() < (*playerIt)->getId()) {
      break;
    }
  }
  playersOnSpace.insert(playerIt, &player);
}

void Monopoly::Space::removePlayer(const Monopoly::Player& player) {
  for (auto playerIt = playersOnSpace.cbegin(); playerIt != playersOnSpace.cend(); ++playerIt) {
    if ((*playerIt)->getId() == player.getId()) {
      playersOnSpace.erase(playerIt);
      return;
    }
  }
}

void Monopoly::Space::removeAllPlayers() {
  playersOnSpace.clear();
}

Monopoly::SpaceType Monopoly::Space::getSpaceType() const {
  return spaceType; // Note, should only be using this function no more than 5 times
}

int Monopoly::Space::getSpaceNumber() const {
  return spaceNumber;
}

//int Monopoly::Property::getSetId() const {
  //return setId; // should only apply for properties.
//}

//virtual void Monopoly::Space::activate(Monopoly::Player& activatingPlayer) {
  //std::cout << "This is a virtual function. If this is printed, you are not overriding appropriately" << std::endl;
//}
   /* {
  if (spaceType == SpaceType::goSpace) {
    // -1 below because they've already been given some of the salary for
    // "passing" go
    activatingPlayer.giveCash(goPtr->getSalary() * (gameState.rules.getSalary_multiplier_for_landing_on_go() - 1));
  } else if (spaceType == SpaceType::propertySpace) {
    Player* owner = propertyPtr->getOwner();
    if (owner == nullptr) {
      if (activatingPlayer.getCash() >= propertyPtr->getCost()) { //can afford the property
        bool toBuy = activatingPlayer.getBuyDecision(*this);
        if (toBuy) {
          std::cout << activatingPlayer.getName() << " bought " << getName() << " for $" << propertyPtr->getCost()
                    << std::endl;
          activatingPlayer.purchase(*propertyPtr, propertyPtr->getCost());
        } else if(gameState.rules.isAuction_properties()){
          gameState.auction(*propertyPtr);
        }
      } else { // not enough money to buy property
        std::cout << activatingPlayer.getName() << ", you don't have enough money to afford " << getName() << ". "
                  << getName() << " costs $" << getCost() << " but you only have $" << activatingPlayer.getCash()
                  << std::endl;
        if(gameState.rules.isAuction_properties()){
          gameState.auction(*propertyPtr);
        }

      }
    } else if (owner->getId() == activatingPlayer.getId()) {// player landed on their own property
      std::cout << activatingPlayer.getName() << ", you landed on " << getName() << " which you already own" <<
                                                                                                             std::endl;
    } else { //landed on a space another player owes
      activatingPlayer.payPlayer(*owner, *propertyPtr, gameState.rules);
    }
  }
   }
   */


/*
virtual int Monopoly::Space::getSalary() const {

  if (spaceType == SpaceType::goSpace) {
    return goPtr->getSalary();
  } else {
    std::cout << "Get Salary called on non Go Space" << std::endl;
    return -99;
  }

}


int Monopoly::Space::getCost() const {
  if (spaceType == SpaceType::propertySpace) {
    return propertyPtr->getCost();
  } else {
    std::cout << "Get Cost called on non Property Space" << std::endl;
    return -99;
  }
}

int Monopoly::Space::getSetId() const {
  if (spaceType == SpaceType::propertySpace) {
    return propertyPtr->getSetId();
  } else {
    return -99;
  }
}

int Monopoly::Space::getIntraSetId() const {
  if (spaceType == SpaceType::propertySpace) {
    return propertyPtr->getIntraSetId();
  } else {
    return -99;
  }
}

int Monopoly::Space::getBasicRent() const {
  if (spaceType == SpaceType::propertySpace) {
    return propertyPtr->getRent();
  } else {
    return -99;
  }
}

*/




