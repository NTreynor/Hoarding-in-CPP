//
// Created by Nicholas Treynor on 3/4/2018.
//

#ifndef INSTRSOL_PAY_H
#define INSTRSOL_PAY_H


#include <string>
#include "CSVReader.h"
#include "Space.h"
#include "Player.h"
#include "GameState.h"
#include <iostream>

namespace Monopoly {
    class GameState;
    class Pay : public Space{
    public:
        explicit Pay(CSVReader& boardFile, int spaceNumber, GameState& gameState);
        int getPayAmount() const;
        const std::string& getName() const override;
        void display() const override;
    private:
        int salary;
    public:
        void activate(Monopoly::Player &activatingPlayer) override;
    private:
        std::string name;
        GameState &gameStateCopy;

    };
}


#endif //INSTRSOL_PAY_H
