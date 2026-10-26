/**
 * @author Tomás Pino Pérez
 * @brief Implementation of class for player
 * @date 09-02-2023
 * @file player_class.cc
 */

#include "player_class.h"

#include <limits>

// constructor for online game
Player::Player(const std::string& name, const std::string& ip,
               const int& port) {
  name_ = name;
  ip_ = ip;
  port_ = port;
}
// constructor for ia game
Player::Player(const std::string& name) {
  name_ = name;
}

// put card from global_card_pack to player_card_pack
void Player::CardGlobalToPlayerByIndex(CardPack& global_pack,
                                       const int& index) {
  // the line of the card with type and color
  card_pack_.reserve(7);
  card_pack_.push_back(global_pack.pack()[index]);

  // the line of the card with color
  color_pack_.reserve(7);
  color_pack_.push_back(global_pack.pack_color()[index]);

  // erase the card from the global_pack
  global_pack.erase(index);
}

// select the color for the special card
int SpecialCardColorPicker() {
  int option{0};
  while (option < 1 || option > 4) {
    std::cout << "  - Color disponibles para cambiar:\n";
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
  return (option - 1);
}

// put card from player_card_pack to global_card_pack
void Player::CardPlayerToGlobalByIndex(CardPack& global_pack,
                                       const int& index) {
  if (card_pack_[index].color() == 4) {
    int new_color{SpecialCardColorPicker()};
    card_pack_[index].color(new_color);
    color_pack_[index] = card_pack_[index].color_display() + "    " + reset;
  }
  // append card to global card_pack
  global_pack.push_back(card_pack_[index], color_pack_[index]);

  // erase the value_card from player and put on global
  card_pack_.erase(card_pack_.begin() + index);

  // erase the color_card from player and put on global
  color_pack_.erase(color_pack_.begin() + index);
}

// print the card_pack
std::string Player::Print(const int& width) {
  std::string out;
  for (int i{0}; i < card_pack_.size() && i < color_pack_.size(); i += width) {
    for (int j{i}; (j < i + width) && (j < color_pack_.size()); ++j) {
      out.append(color_pack_[j] + " ");
    }
    out.append("\n");
    for (int j{i}; (j < i + width) && (j < card_pack_.size()); ++j) {
      out.append(card_pack_[j].Print());
      out.append(" ");
    }
    out.append("\n");
    for (int j{i}; (j < i + width) && (j < color_pack_.size()); ++j) {
      out.append(color_pack_[j] + " ");
    }
    out.append("\n\n");
  }
  return out;
}