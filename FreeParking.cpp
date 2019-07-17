//
// Created by Nicholas Treynor on 3/4/2018.
//

#include "FreeParking.h"
#include "GameState.h"

Monopoly::FreeParking::FreeParking(CSVReader& boardFile, int spaceNumber, GameState& gameState) : Space(spaceNumber, SpaceType::freeParking), gameStateCopy(gameState) {
    name = boardFile.get_next_field();

    //this->spaceNumber = spaceNumber;
    //spaceType = SpaceType::goSpace;
    // goPtr = new Go(boardFile);
    boardFile.skip_line();

}



void Monopoly::FreeParking::activate(Monopoly::Player& activatingPlayer) {
    // -1 below because they've already been given some of the salary for
    // "passing" go
    if (gameStateCopy.rules.isPut_money_in_free_parking()) {
        int payment = gameStateCopy.rules.emptyFreeParkingMoney();\
        if (payment > 0) {
            activatingPlayer.giveCash(payment);
            std::cout << activatingPlayer.getName() << " got $" << payment << " for landing on "
                      << getName() << std::endl;
        }
    }
}

void Monopoly::FreeParking::display() const {

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

const std::string& Monopoly::FreeParking::getName() const {
    return name;
}