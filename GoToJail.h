//
// Created by Nicholas Treynor on 3/5/2018.
//

#ifndef INSTRSOL_GOTOJAIL_H
#define INSTRSOL_GOTOJAIL_H



#include <string>
#include "CSVReader.h"
#include "Space.h"
#include <iostream>

namespace Monopoly {
    class GameState;
    class GoToJail : public Space{
    public:
        explicit GoToJail(CSVReader& boardFile, int spaceNumber, GameState& gameState);
        const std::string& getName() const override;
        void display() const override;
    private:
    public:
        void activate(Monopoly::Player &activatingPlayer) override;
    private:
        int spaceIndex;
        std::string name;
        GameState &gameStateCopy;

    };
}




#endif //INSTRSOL_GOTOJAIL_H
