//
// Created by mfbut on 1/20/2018.
//

#include "Board.h"
#include <iostream>
#include "MonopolyUtility.h"
#include "Player.h"
#include "GameState.h"



Monopoly::Board::Board(const std::string& boardFileName, GameState& gameState) {
  CSVReader boardFile(boardFileName);
  constexpr int num_lines_to_skip = 4;

  if (boardFile.good()) {
    boardFile.skip_field();
    const int numSpaces = boardFile.get_next_field_as_int();

    for (int i = 0; i < num_lines_to_skip; ++i) {
      boardFile.skip_line();
    }

    for (int i = 0; i < numSpaces; ++i) {

      // cdo fancy stuff here

      const int spaceNumber = i;

        std::string space_type_string = boardFile.get_next_field();
        if (space_type_string == "Go") {


          std::unique_ptr<Space> goPtr = std::make_unique<Go>(boardFile, spaceNumber, gameState);
          spaces.push_back(std::move(goPtr)); // need to use move.

        } else if (space_type_string == "Property") {

          std::unique_ptr<Space> propertyPtr = std::make_unique<Property>(boardFile, gameState.rules, spaceNumber, gameState);
          spaces.push_back(std::move(propertyPtr));

        } else if (space_type_string == "Jail"){

          std::unique_ptr<Space> propertyPtr = std::make_unique<Jail>(boardFile, spaceNumber, gameState);
          spaces.push_back(std::move(propertyPtr));

        } else if (space_type_string == "GoToJail"){

          std::unique_ptr<Space> propertyPtr = std::make_unique<GoToJail>(boardFile, spaceNumber, gameState);
          spaces.push_back(std::move(propertyPtr));

        } else if (space_type_string == "Pay"){

          std::unique_ptr<Space> payPtr = std::make_unique<Pay>(boardFile, spaceNumber, gameState);
          spaces.push_back(std::move(payPtr)); // need to use move.

        } else if (space_type_string == "FreeParking"){


          std::unique_ptr<Space> freeParkingPtr = std::make_unique<FreeParking>(boardFile, spaceNumber, gameState);
          spaces.push_back(std::move(freeParkingPtr)); // need to use move.

        } else {

            std::cout << "Unrecognized Space type found in " << boardFile.getFileName() << std::endl;

        }



      //spaces.emplace_back(boardFile, i, gameState);
    }

    Monopoly::Space::length_of_longest_space_name = calc_length_of_longest_space_name();
    Monopoly::Space::length_of_longest_space_name =
        Utility::max(Monopoly::Space::length_of_longest_space_name, std::string("Space Name").size());

  } else {
    std::cout << "Failed to open board file: " << boardFileName << std::endl;
  }
}

void Monopoly::Board::display() const {

  //const std::string header("Space Number | Space Name | Owner | Upgrades | Players");

  //std::cout << header << std::endl;
  const std::string spaceNumber("Space Number");
  const std::string spaceName("Space Name");
  const std::string owner("Owner");
  const std::string Upgrades("Upgrades");
  const std::string playersString("Players");
  const auto frmt_flags = std::cout.flags();
  std::cout.setf(std::ios::left);

  std::cout << spaceNumber; std::cout << " | ";
  std::cout.width(Space::length_of_longest_space_name);
  std::cout << spaceName; std::cout << " | ";

  std::cout.width(Player::length_of_longest_player_name);
  std::cout << owner; std::cout << " | ";

  std::cout.width(Upgrades.size());
  std::cout << Upgrades << " | ";

  std::cout.width(Player::length_of_longest_player_name);
  std::cout << playersString; std::cout << std::endl;

  int i = 0;
  for (const auto& space : spaces) {
    std::cout.width(spaceNumber.size());
    std::cout << i << " | ";
    (*space.get()).display();
    std::cout << std::endl;
    ++i;
  }
  std::cout.flags(frmt_flags);
}

unsigned long Monopoly::Board::calc_length_of_longest_space_name() const {
  std::vector<std::string> space_names;
  for (const auto& space : spaces) {
    space_names.emplace_back((*space.get()).getName());
  }
  return Utility::get_max_string_length(space_names);
}

Monopoly::Space* Monopoly::Board::getSpace(int spaceNum) {
  return (spaces[spaceNum].get());
}

Monopoly::Space* Monopoly::Board::getGoSpace() {
    return (spaces[0].get()); // should only ever grab the first space.
}

int Monopoly::Board::getNumSpaces() const {
  return static_cast<int>(spaces.size());
}

const std::vector<std::unique_ptr<Monopoly::Space>>& Monopoly::Board::getSpaces() const {
  return spaces;
}

void Monopoly::Board::removeAllPlayers() {
  for (auto& space : spaces) {
    (*space.get()).removeAllPlayers();
  }
}

