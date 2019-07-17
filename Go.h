//
// Created by mfbut on 1/20/2018.
//

#ifndef HOARDINGCPPVERSION_GO_H
#define HOARDINGCPPVERSION_GO_H

#include <string>
#include "CSVReader.h"
#include "Space.h"


namespace Monopoly {
    class GameState;
  class Go : public Space{
   public:
    explicit Go(CSVReader& boardFile, int spaceNumber, GameState& gameState);
    int getSalary() const;
    const std::string& getName() const override;
    void display() const override;
   private:
    int salary;
    int landing_multiplier;
   public:
      void activate(Monopoly::Player &activatingPlayer) override;
   private:
    std::string name;
    GameState &gameStateCopy;

  };
}

#endif //HOARDINGCPPVERSION_GO_H
