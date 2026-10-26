/**
 * @author Tomás Pino Pérez
 * @brief Declarations of class for card_pack
 * @date 09-02-2023
 * @file card_pack_class.h
 */

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "ANSI-color-codes.h"
#include "card_class.h"

// Class for manage the card_pack
#ifndef CARD_PACK_CLASS_H
#define CARD_PACK_CLASS_H

class CardPack {
 public:
  // constructor for default kSize cards
  CardPack();
  // constructor of player card_pack
  CardPack(const int& size);

  // generate pack_
  void GeneratePack();

  // generate pack_color_
  void GeneratePackColor();

  // shuffle the CardPack
  void ShufflePack();

  // clear the card_pack
  void clear();

  // erase given index
  void erase(const int& index);

  // push_back
  void push_back(const Card& card_type, const std::string& card_color);

  // print cards until width
  std::string Print(const std::size_t width);
  
  // print the game table (backcard and las card in pack)
  std::string PrintGameTable();

  // check if card to put is valid
  bool Check(const Card& card) const;

  // getters
  std::size_t size() const { return pack_.size(); }
  bool empty() const { return pack_.empty(); }
  std::vector<Card> pack() const { return pack_; }
  std::vector<std::string> pack_color() const { return pack_color_; }

 private:
  static constexpr std::size_t kSize_{4 * (1 + 12 * 2) + 8 + 4};
  std::vector<Card> pack_;
  std::vector<std::string> pack_color_;
};

#endif