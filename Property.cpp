//
// Created by mfbut on 1/20/2018.
//

#include <iostream>
#include <cmath>
#include "Property.h"
#include "Player.h"
#include "GameState.h"

Monopoly::Property::Property(CSVReader &boardFile, const Rules &rules, int spaceNumber, GameState& gameState)
        : Space(spaceNumber, SpaceType::propertySpace), owner(nullptr), numHousesOn(0), numHotelsOn(0), rules(rules), gameStateCopy(gameState) {


    setId = boardFile.get_next_field_as_int();
    intraSetId = boardFile.get_next_field_as_int();
    name = boardFile.get_next_field();
    cost = boardFile.get_next_field_as_int();
    house_cost = boardFile.get_next_field_as_int();
    hotel_cost = boardFile.get_next_field_as_int();
    rent = boardFile.get_next_field_as_int();
    rent_with_house = boardFile.get_next_field_as_int();
    rent_with_hotel = boardFile.get_next_field_as_int();
}

//  Monopoly::SpaceType Monopoly::Space::getSpaceType() const {
  //  return spaceType; // Note, should only be using this function no more than 5 times
// }


void Monopoly::Property::activate(Monopoly::Player &activatingPlayer) {
    Player *owner = getOwner();
    if (owner == nullptr) {
        if (activatingPlayer.getCash() >= getCost()) { //can afford the property
            bool toBuy = activatingPlayer.getBuyDecision(*this);
            if (toBuy) {
                std::cout << activatingPlayer.getName() << " bought " << getName() << " for $" << getCost()
                          << std::endl;
                activatingPlayer.purchase(*this, getCost());
            } else if (gameStateCopy.rules.isAuction_properties()) {
                gameStateCopy.auction(*this);
            }
        } else { // not enough money to buy property
            std::cout << activatingPlayer.getName() << ", you don't have enough money to afford " << getName() << ". "
                      << getName() << " costs $" << getCost() << " but you only have $" << activatingPlayer.getCash()
                      << std::endl;
            if (gameStateCopy.rules.isAuction_properties()) {
                gameStateCopy.auction(*this);
            }

        }
    } else if (owner->getId() == activatingPlayer.getId()) {// player landed on their own property
        std::cout << activatingPlayer.getName() << ", you landed on " << getName() << " which you already own" <<
                  std::endl;
    } else { //landed on a space another player owes
        activatingPlayer.payPlayer(*owner, *this, gameStateCopy.rules);
    }



}

int Monopoly::Property::getSetId() const {
    return setId;
}

int Monopoly::Property::getIntraSetId() const {
    return intraSetId;
}

const std::string &Monopoly::Property::getName() const {
    return name;
}

int Monopoly::Property::getCost() const {
    return cost;
}



int Monopoly::Property::getHouse_cost() const {
    return house_cost;
}

int Monopoly::Property::getHotel_cost() const {
    return hotel_cost;
}

int Monopoly::Property::getRent() const {
    return rent;
}

int Monopoly::Property::getRent_with_house() const {
    return rent_with_house;
}

int Monopoly::Property::getRent_with_hotel() const {
    return rent_with_hotel;
}

Monopoly::Player *Monopoly::Property::getOwner() const {
    return owner;
}

int Monopoly::Property::getLanding_multiplier() const {
    return landing_multiplier;
}

void Monopoly::Property::setLanding_multiplier(int landing_multiplier) {
    this->landing_multiplier = landing_multiplier;
}

void Monopoly::Property::display() const {

    const auto frmt_flags = std::cout.flags();

    //display space name
    std::cout.width(length_of_longest_space_name);
    std::cout << getName() << " | ";

    //display owner
    std::cout.width(Monopoly::Player::length_of_longest_player_name);

    std::cout << getOwnerName() << " | ";
    std::cout << getUpgradesString() << " | ";

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

std::string Monopoly::Property::getOwnerName() const {
    if (owner != nullptr) {
        return owner->getName();
    } else {
        return "None";
    }
}

void Monopoly::Property::setOwner(Monopoly::Player *owner) {
    Property::owner = owner;
}

int Monopoly::Property::calculateRent(const Rules &rules) const {
    if (getNumHotelsOn() > 0) {
        return getRent_with_hotel() * getNumHotelsOn();
    } else if (getNumHousesOn() > 0) {
        return getRent_with_house() * pow(2, getNumHousesOn() - 1);
    } else if (owner->ownsAllPropertiesInSet(setId)) {
        return rent * rules.getProperty_set_multiplier();
    } else {
        return rent;
    }

}

int Monopoly::Property::getNumHousesOn() const {
    return numHousesOn;
}

int Monopoly::Property::getNumHotelsOn() const {
    return numHotelsOn;
}

bool Monopoly::Property::isFullyUpgraded() const {
    return getNumHotelsOn() == rules.getMaxHotels();
}

bool Monopoly::Property::nextUpgradeIsHouse() const {
    return getNumHousesOn() < rules.getNum_houses_before_hotel();
}

bool Monopoly::Property::nextUpgradeIsHotel() const {
    return getNumHousesOn() == rules.getNum_houses_before_hotel();
}

void Monopoly::Property::upgrade() {
    if (nextUpgradeIsHouse()) {
        numHousesOn++;
        owner->giveCash(-1 * getHouse_cost());
    } else if (nextUpgradeIsHotel()) {
        numHotelsOn++;
        numHousesOn = 0;
        owner->giveCash(-1 * getHotel_cost());
    } else {
        std::cout << "Something impossible happened when trying to upgrade " << getName() << std::endl;
    }
}

void Monopoly::Property::downgrade() {
    if (getNumHotelsOn() > 0) {
        numHotelsOn--;
        numHousesOn += rules.getNum_houses_before_hotel();
        owner->giveCash(getHotel_cost() / 2);
    } else if (getNumHousesOn() > 0) {
        numHousesOn--;
        owner->giveCash(getHotel_cost() / 2);
    } else {
        std::cout << "Something impossible happened when trying to downgrade " << getName() << std::endl;
    }
}

std::string Monopoly::Property::getUpgradesString() const {
    const std::string upgrade("Upgrades");
    std::string upgradeString;
    for (int i = 0; i < getNumHousesOn(); ++i) {
        upgradeString.push_back('h');
    }
    for (int i = 0; i < getNumHotelsOn(); ++i) {
        upgradeString.push_back('H');
    }

    unsigned long upgradeLength = upgradeString.size();
    for (unsigned long i = 0; i < upgrade.size() - upgradeLength; ++i) {
        upgradeString.push_back(' ');
    }
    return upgradeString;
}

int Monopoly::Property::getUpgradeCost() const {
    if (nextUpgradeIsHouse()) {
        return getHouse_cost();
    } else if (nextUpgradeIsHotel()) {
        return getHouse_cost();
    } else {
        std::cout << "Problem getting upgrade cost for " << getName() << std::endl;
        return -99;
    }
}

void Monopoly::Property::setNumHousesOn(int numHousesOn) {
    Property::numHousesOn = numHousesOn;
}

void Monopoly::Property::setNumHotelsOn(int numHotelsOn) {
    Property::numHotelsOn = numHotelsOn;
}

int Monopoly::Property::getNumUpgrades() const {
    if (getNumHotelsOn() > 0) {
        return getNumHotelsOn() + rules.getNum_houses_before_hotel();
    } else {
        return getNumHousesOn();
    }
}

int Monopoly::Property::getValue() const {
    return cost + getNumHotelsOn() * getHotel_cost() + getNumHousesOn() * getHotel_cost();
}



