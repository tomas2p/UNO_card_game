/**
 * @author Tomás Pino Pérez
 * @brief Implementation of class for player
 * @date 09-02-2023
 * @file player_class.cc
 */

#include "player_class.h"

#include <utility>

// constructor for online game
Player::Player(std::string name, std::string ip, int port)
    : name_{std::move(name)}, ip_{std::move(ip)}, port_{port} {}

// constructor for ia game
Player::Player(std::string name) : name_{std::move(name)} {}

// draw count cards from the top of deck
void Player::DrawFrom(CardPack& deck, std::size_t count) {
  for (std::size_t i{0}; i < count && !deck.empty(); ++i) {
    hand_.push_back(deck[0]);
    deck.erase(0);
  }
}

// move the card at index from the hand to the discard pile
void Player::PlayTo(CardPack& discard, std::size_t index) {
  discard.push_back(hand_[index]);
  hand_.erase(index);
}

// print the card_pack
std::string Player::Print(std::size_t width) const {
  return FormatCards(hand_.pack(), width);
}
