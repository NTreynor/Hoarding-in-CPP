//
// Created by Nicholas Treynor on 3/4/2018.
//

#include "Pay.h"

Monopoly::Pay::Pay(CSVReader& boardFile, int spaceNumber, GameState& gameState) : Space(spaceNumber, SpaceType::pay), gameStateCopy(gameState) {
    name = boardFile.get_next_field();
    salary = boardFile.get_next_field_as_int();

    //this->spaceNumber = spaceNumber;

    //spaceType = SpaceType::goSpace;
    // goPtr = new Go(boardFile);
    boardFile.skip_line();

}



void Monopoly::Pay::activate(Monopoly::Player& activatingPlayer) {
    // -1 below because they've already been given some of the salary for
    // "passing" go


    activatingPlayer.payBank(this->getPayAmount(), gameStateCopy.rules, *this, gameStateCopy);
}


int Monopoly::Pay::getPayAmount() const {
    return salary;
}

const std::string& Monopoly::Pay::getName() const {
    return name;
}

void Monopoly::Pay::display() const {


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
