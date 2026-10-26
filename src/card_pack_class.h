/**
 * @author Tomás Pino Pérez
 * @brief Declarations of class for card_pack
 * @date 09-02-2023
 * @file card_pack_class.h
 */

#ifndef CARD_PACK_CLASS_H
#define CARD_PACK_CLASS_H

#include <cstddef>
#include <string>
#include <vector>

#include "card_class.h"

// Class for manage the card_pack
class CardPack {
 public:
  // empty pack; use FullDeck() for a complete UNO deck
  CardPack() = default;

  // complete UNO deck (112 cards)
  static CardPack FullDeck();

  // shuffle the CardPack
  void ShufflePack();

  // clear the card_pack
  void clear() { pack_.clear(); }

  // erase given index
  void erase(std::size_t index);

  // push_back
  void push_back(const Card& card);

  // print cards until width
  std::string Print(std::size_t width) const;

  // print the game table (backcard and last card in pack)
  std::string PrintGameTable() const;

  // check if card to put is valid
  bool CanPlay(const Card& card) const;

  // getters
  std::size_t size() const { return pack_.size(); }
  bool empty() const { return pack_.empty(); }
  const Card& top() const { return pack_.back(); }
  const std::vector<Card>& pack() const { return pack_; }
  const Card& operator[](std::size_t index) const { return pack_[index]; }
  Card& operator[](std::size_t index) { return pack_[index]; }

 private:
  static constexpr std::size_t kSize_{4 * (1 + 12 * 2) + 8 + 4};
  std::vector<Card> pack_;
};

#endif
