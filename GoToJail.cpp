//
// Created by Nicholas Treynor on 3/5/2018.
//

#include "GoToJail.h"
#include "Player.h"
#include "GameState.h"
#include <iostream>

Monopoly::GoToJail::GoToJail(CSVReader& boardFile, int spaceNumber, GameState& gameState) : Space(spaceNumber, SpaceType::freeParking), gameStateCopy(gameState) {
    name = boardFile.get_next_field();
    spaceIndex = boardFile.get_next_field_as_int();

    //this->spaceNumber = spaceNumber;
    //spaceType = SpaceType::goSpace;
    // goPtr = new Go(boardFile);
    boardFile.skip_line();

}



void Monopoly::GoToJail::activate(Monopoly::Player& activatingPlayer) {

    auto& jailSpace = dynamic_cast<Jail&> (*gameStateCopy.board.getSpace(spaceIndex));
    int turns = jailSpace.getSentence(); // getSentence gives us the amount of turns they will be in jail for.


    std::cout << activatingPlayer.getName() << ", you were sent to " << jailSpace.getName() << " for " << turns << " turns" << std::endl;

    // now set the player to be in jail.

    activatingPlayer.moveTo(jailSpace, false);
    activatingPlayer.setSentence(turns);
}

void Monopoly::GoToJail::display() const {

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

const std::string& Monopoly::GoToJail::getName() const {
    return name;
}