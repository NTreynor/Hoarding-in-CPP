//
// Created by Nicholas Treynor on 3/5/2018.
//

#include "Jail.h"
#include "Player.h"
#include "GameState.h"
#include <iostream>

Monopoly::Jail::Jail(CSVReader& boardFile, int spaceNumber, GameState& gameState) : Space(spaceNumber, SpaceType::freeParking), gameStateCopy(gameState) {
    name = boardFile.get_next_field();
    turnsLimit = boardFile.get_next_field_as_int();
    fee = boardFile.get_next_field_as_int();

    //this->spaceNumber = spaceNumber;
    //spaceType = SpaceType::goSpace;
    // goPtr = new Go(boardFile);
    boardFile.skip_line();

}



void Monopoly::Jail::activate(Monopoly::Player& activatingPlayer) {
    // we do nothing.
}

void Monopoly::Jail::display() const {

    const auto frmt_flags = std::cout.flags();

    //display space name
    std::cout.width(length_of_longest_space_name);
    std::cout << getName() << " | ";

    //display owner
    std::cout.width(Monopoly::Player::length_of_longest_player_name);


    const std::string temp = "None";
    std::cout << temp << " | ";
    std::cout << "         | ";


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

const std::string& Monopoly::Jail::getName() const {
    return name;
}
const int Monopoly::Jail::getFee() const {
    return fee;
}
const int Monopoly::Jail::payFee() const {

    if (gameStateCopy.rules.isPut_money_in_free_parking()){
        gameStateCopy.rules.addToFreeParking(fee);
    }

    return fee;
}
const int Monopoly::Jail::getSentence() const {

    return turnsLimit;
}