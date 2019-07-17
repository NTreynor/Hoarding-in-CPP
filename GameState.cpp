//
// Created by mfbut on 1/20/2018.
//

#include "GameState.h"
#include "MonopolyUtility.h"
#include <iostream>
#include "Go.h"
#include "Move.h"
#include "PropertyManager.h"

Monopoly::GameState::GameState(const std::string& RulesFileName,
                               const std::string& BoardFileName,
                               const std::string& RandomFileName,
                               const std::vector<std::string>& playerNames)
    : rules(RulesFileName),
      board(BoardFileName, *this),
      diceRoller(RandomFileName),
      turn_count(0),
      player_turn(0),
      players() {

  PropertyManager emptyPropertyManager(board);

  for (unsigned int i = 0; i < playerNames.size(); ++i) {
    players.push_back(std::make_unique<Player>(i,
                                               playerNames[i],
                                               rules.getStarting_cash(),
                                               &(*board.getSpace(0)),
                                               emptyPropertyManager));

  }

  /*
   * Placing the player on the space needed to be done after
   * all the players were created because the players'  locations
   * in memory keep moving as they get inserted into the players
   * vector
   */
  for (auto& player : players) {
    player->setOn((*board.getGoSpace()), false);
  }

  Player::length_of_longest_player_name =
      Utility::max(Utility::get_max_string_length(playerNames), std::string("Owner").size());

}

void Monopoly::GameState::display() const {

  board.display();

}

std::vector<std::string> Monopoly::GameState::get_player_names() {
  int num_players;
  std::vector<std::string> names;

  std::cout << "Enter how many players will be playing: ";
  std::cin >> num_players;
  names.resize(num_players);

  for (int i = 0; i < num_players; ++i) {
    std::cout << "Enter the name of player " << i + 1 << ": ";
    std::cin >> names[i];
  }
  return names;
}


bool Monopoly::GameState::attemptEscapeJailViaDice(){

  int roll = this->diceRoller.getDiceRoll();
  std::cout << getCurrentPlayer().getName() << ", you rolled a " << roll << std::endl;
  bool leaveJail = this->diceRoller.multiplesRolled();
  return leaveJail;

}

void Monopoly::GameState::playGame() {
  Move move;
  while (!isGameOver()) {
    diceRoller.setNumConsecutiveMultiplesRolled(0);
    do {
      display();
      if (getCurrentPlayer().getRemainingSentence() == -1) {
        move = getCurrentPlayer().getMove();
        make_move(getCurrentPlayer(), move);
      } else {
        move = getCurrentPlayer().getMoveInJail(rules, *this);
        if (getCurrentPlayer().getRemainingSentence() != -1) {
          make_move_in_jail(getCurrentPlayer(), move);
        }
        else {
          make_move(getCurrentPlayer(), move);
        }

      }
    } while (!move.endsTurn(diceRoller, rules));
    changeTurn();
  }
  display();
  declareWinner();
}

/**
 * Check if the game is over or not
 * @return
 */
bool Monopoly::GameState::isGameOver() const {
  return players.size() <= static_cast<unsigned long>(rules.getNum_players_left_to_end_game())
      || (rules.isTurnLimit() && turn_count >= rules.getTurn_limit());
}

Monopoly::Player& Monopoly::GameState::getCurrentPlayer() {
  return *players[player_turn];
}

void Monopoly::GameState::make_move(Monopoly::Player& player, Monopoly::Move& move) {
  if (move.getAction() == MoveAction::rollDice) {
    rollDice(player);

  } else if (move.getAction() == MoveAction::buyUpgrade) {
    buyUpgrade();
  } else if (move.getAction() == MoveAction::sellUpgrade) {
    sellUpgrade();
  } else if (move.getAction() == MoveAction::leaveGame) {
    removeFromGame(player);
  }
}

void Monopoly::GameState::make_move_in_jail(Monopoly::Player& player, Monopoly::Move& move) {
  bool escaped = false;
  if (move.getAction() == MoveAction::rollDice) {
    escaped = attemptEscapeJailViaDice();
    if (escaped){
      std::cout << "You are free!" << std::endl; // Don't think this actually gets tested.
      player.setSentence(-1);
      make_move(player, move); // reroll dice and move
    } else{
      std::cout << "You are still stuck in jail" << std::endl;
      return;
    }

  } else if (move.getAction() == MoveAction::buyUpgrade) {
    buyUpgrade();
  } else if (move.getAction() == MoveAction::sellUpgrade) {
    sellUpgrade();
  } else if (move.getAction() == MoveAction::leaveGame) {
    removeFromGame(player);
  }
}

void Monopoly::GameState::rollDice(Monopoly::Player& player) {
  int spacesToMove = this->diceRoller.getDiceRoll();
  int newLocation = spacesToMove + player.getSpaceOn().getSpaceNumber();
  int timesPastGo = newLocation / this->board.getNumSpaces();

  std::cout << player.getName() << ", you rolled a " << spacesToMove << std::endl;

  // give the player their salary for passing go


      auto& space = board.getSpaces().at(0);
      Go& childRef = dynamic_cast<Go&>(*space);
      player.giveCash(childRef.getSalary() * timesPastGo);





  newLocation %= this->board.getNumSpaces();
  player.moveTo(*this->board.getSpace(newLocation));

  if (player.getCash() < 0) {
    this->removeFromGame(player);
  }
}

void Monopoly::GameState::buyUpgrade() {
  unsigned long purchase_num = 99;
  std::vector<Monopoly::Property*> upgradeableProperties = getCurrentPlayer().getUpgradeableProperties(rules);
  if (upgradeableProperties.size() == 0) {
    std::cout << "You don't have any properties that you can upgrade" << std::endl;
  } else {
    std::cout << "Which property do you want to upgrade?" << std::endl;
    for (unsigned long i = 0; i < upgradeableProperties.size(); ++i) {
      std::cout << i << ". " << upgradeableProperties.at(i)->getName() << " [$"
                << upgradeableProperties.at(i)->getUpgradeCost() << "]" << std::endl;
    }
    std::cout << "Your choice: ";
    std::cin >> purchase_num;
    upgradeableProperties.at(purchase_num)->upgrade();
  }
}

void Monopoly::GameState::sellUpgrade() {
  unsigned long sell_num;
  std::vector<Property*> downgradeableProperties = getCurrentPlayer().getDowngradeableProperties(rules);

  if (downgradeableProperties.size() == 0) {
    std::cout << "You don't have any upgrades that you can sell" << std::endl;
  } else {
    std::cout << "Which property do you want to downgrade?" << std::endl;
    for (unsigned long i = 0; i < downgradeableProperties.size(); ++i) {
      std::cout << i << ". " << downgradeableProperties.at(i)->getName() << " [$"
                << downgradeableProperties.at(i)->getUpgradeCost()  / 2 << "]" << std::endl;
    }
    std::cout << "Your choice: ";
    std::cin >> sell_num;
    downgradeableProperties.at(sell_num)->downgrade();
  }

}

void Monopoly::GameState::declareWinner() {
  int maxWorth = 0;

  //find the maximum net worth
  for (const auto& player : players) {
    int cur_player_worth = player->getNetWorth();
    if (cur_player_worth > maxWorth) {
      maxWorth = cur_player_worth;
    }
  }

  std::cout << "The winners are" << std::endl;
  for (const auto& player : players) {
    if (player->getNetWorth() == maxWorth) {
      std::cout << player->getName() << std::endl;
    }
  }

}

void Monopoly::GameState::changeTurn() {
  turn_count++;
  player_turn = (player_turn + 1) % players.size();
}

void Monopoly::GameState::removeFromGame(Monopoly::Player& player) {
  // remove the player from the vector
  for (auto playerIt = players.cbegin(); playerIt != players.cend(); ++playerIt) {
    if ((*playerIt)->getId() == player.getId()) {
      player.getSpaceOn().removePlayer(player);
      players.erase(playerIt);
      break;
    }
  }
  /*
  * update all pointers to players now that we've moved their locations in memory
  * Currently these are just the property owner pointers and the pointers on the spaces that point to who is on them
  * Has to be in a separate loop from the other because we need to get the players' new location in memory first
  */
  board.removeAllPlayers(); // remove all references from the spaces on the board to the players
  for (auto& rem_player :players) {
    rem_player->refreshReferencesTo();
  }

  //because we removed a player and everyone shifted over we need to
  player_turn--;

}

void Monopoly::GameState::decrementPlayerTurn(){
  player_turn--;
}

void Monopoly::GameState::auction(Monopoly::Property& property) {
  int highestBid = 0;
  int curBid = -99;
  std::vector<Player*> bidders;
  Player* highestBidder = nullptr;
  //create the bidders
  for (auto& player: players) {
    bidders.push_back(player.get());
  }

  std::cout << "Starting the auction for " << property.getName() << std::endl;
  while (bidders.size() > 1) { // while players are bidding
    for (auto bidder = bidders.begin(); bidder != bidders.end(); ++bidder) {
      if ((*bidder)->getCash() < highestBid + 1) { // player can't afford the bid
        bidder = bidders.erase(bidder); // remove them from the auction
        --bidder; // erasing an element from a vector actually advances you to the one after it
        //so we need to move back one so the ++bidder at the top doesn't skip anyone
        continue;
      } else if (highestBid > 0 && bidders.size() == 1) {
        break;
      } else if (highestBid == 0) {
        std::cout << "No one has bid on " << property.getName() << " [$ " << property.getCost() << "] yet" << std::endl;
      } else {
        std::cout << "The current bid for " << property.getName() << " [$ " << property.getCost() << "] is $"
                  << highestBid << " by " << highestBidder->getName() << std::endl;
      }
      std::cout << (*bidder)->getName() << ", enter a bid of at least $" << highestBid + 1
                << " to bid on the property or a value less than that to leave the auction" << std::endl;
      std::cout << "Your bid: ";
      std::cin >> curBid;

      if (curBid > highestBid) { //new highest bidder
        highestBid = curBid;
        highestBidder = *bidder;
      } else { //player is out of the auction
        bidder = bidders.erase(bidder);
        --bidder; // erasing an element from a vector actually advances you to the one after it
        //so we need to move back one so the ++bidder at the top doesn't skip anyone

      }
    }
  }

  if (highestBidder) {
    std::cout << highestBidder->getName() << " won " << property.getName() << " for $" << highestBid << std::endl;
    highestBidder->purchase(property, highestBid);
  } else {
    std::cout << "No one decided to purchase " << property.getName() << std::endl;
  }

}



