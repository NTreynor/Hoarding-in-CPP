//
// Created by mfbut on 1/20/2018.
//

#ifndef HOARDINGCPPVERSION_SPACE_H
#define HOARDINGCPPVERSION_SPACE_H

#include <memory>
#include <vector>

#include "CSVReader.h"
#include "Rules.h"



namespace Monopoly {
  class Player;
  class GameState;
  enum class SpaceType { goSpace, propertySpace, freeParking, pay };

  class Space {

      friend class Property;
      friend class Go;
      // friend class Jail;
      // friend class GoToJail
      friend class Pay;
      friend class FreeParking;

   public:
      SpaceType spaceType;
    static unsigned long length_of_longest_space_name;
    Space(int spaceNumber, SpaceType spaceType);
      //Space();
    int getSpaceNumber() const;
    virtual void display() const = 0;
    virtual const std::string& getName() const = 0;
    void addPlayer(Player& player);
    void removePlayer(const Player& player);
    void removeAllPlayers();



    SpaceType getSpaceType() const;
      int spaceNumber;
      virtual void activate(Player& activatingPlayer) = 0;
/*
    int getSalary() const;
    int getCost() const;
    int getSetId() const;
    int getIntraSetId() const;
    int getBasicRent() const;

*/
      std::vector<Player*> playersOnSpace;




       // GameState& gameState;
  };
}

#endif //HOARDINGCPPVERSION_SPACE_H
