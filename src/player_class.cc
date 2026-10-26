/**
 * @author Tomás Pino Pérez
 * @brief Implementation of class for player
 * @date 09-02-2023
 * @file player_class.cc
 */

#include "player_class.h"

#include <iostream>
#include <limits>
#include <utility>

namespace {

// select the color for the special card
Color SpecialCardColorPicker() {
  int option{0};
  while (option < 1 || option > 4) {
    std::cout << "  - Colores disponibles para cambiar:\n";
    std::cout << "\t [1] - red\n\t [2] - blue\n\t [3] - green\n\t [4] - yellow\n";
    std::cout << "\t >>> " << std::flush;
    if (!(std::cin >> option)) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    if (option < 1 || option > 4) {
      std::cout << " Color inválido.\n";
    }
  }
  return static_cast<Color>(option - 1);
}

}  // namespace

// constructor for online game
Player::Player(std::string name, std::string ip, int port)
    : name_{std::move(name)}, ip_{std::move(ip)}, port_{port} {}

// constructor for ia game
Player::Player(std::string name) : name_{std::move(name)} {}

// put card from global_card_pack to player_card_pack
void Player::CardGlobalToPlayerByIndex(CardPack& global_pack,
                                       std::size_t index) {
  hand_.push_back(global_pack[index]);
  global_pack.erase(index);
}

// put card from player_card_pack to global_card_pack
void Player::CardPlayerToGlobalByIndex(CardPack& global_pack,
                                       std::size_t index) {
  if (hand_[index].color() == Color::Wild) {
    hand_[index].color(SpecialCardColorPicker());
  }
  global_pack.push_back(hand_[index]);
  hand_.erase(index);
}

// print the card_pack
std::string Player::Print(std::size_t width) const {
  return FormatCards(hand_.pack(), width);
}
