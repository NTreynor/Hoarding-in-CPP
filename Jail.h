//
// Created by Nicholas Treynor on 3/5/2018.
//

#ifndef INSTRSOL_JAIL_H
#define INSTRSOL_JAIL_H


#include <string>
#include "CSVReader.h"
#include "Space.h"
#include <iostream>

namespace Monopoly {
    class GameState;
    class Jail : public Space{
    public:
        explicit Jail(CSVReader& boardFile, int spaceNumber, GameState& gameState);
        const std::string& getName() const override;
        const int getFee() const;
        const int payFee() const;
        const int getSentence() const;
        void display() const override;
    private:
    public:
        void activate(Monopoly::Player &activatingPlayer) override;
    private:
        int fee;
        int turnsLimit;
        std::string name;
        GameState &gameStateCopy;

    };
}




#endif //INSTRSOL_JAIL_H
