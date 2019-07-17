//
// Created by Nicholas Treynor on 3/4/2018.
//

#ifndef INSTRSOL_FREEPARKING_H
#define INSTRSOL_FREEPARKING_H


#include <string>
#include "CSVReader.h"
#include "Space.h"
#include <iostream>

namespace Monopoly {
    class GameState;
    class FreeParking : public Space{
    public:
        explicit FreeParking(CSVReader& boardFile, int spaceNumber, GameState& gameState);
        const std::string& getName() const override;
        void display() const override;
    private:
    public:
        void activate(Monopoly::Player &activatingPlayer) override;
    private:
        std::string name;
        GameState &gameStateCopy;

    };
}


#endif //INSTRSOL_FREEPARKING_H
