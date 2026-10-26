/**
 * @author Tomás Pino Pérez
 * @brief Declarations of class for player
 * @date 09-02-2023
 * @file player_class.h
 */

#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H

#include <cstddef>
#include <string>

#include "card_pack_class.h"

// Class for manage the player
class Player {
 public:
  // constructor for online game
  Player(std::string name, std::string ip, int port);
  // constructor for ia game
  explicit Player(std::string name);

  // draw count cards from the top of deck
  void DrawFrom(CardPack& deck, std::size_t count = 1);
  // move the card at index from the hand to the discard pile
  void PlayTo(CardPack& discard, std::size_t index);

  // print cards until width
  std::string Print(std::size_t width) const;

  // getters
  const std::string& name() const { return name_; }
  const std::string& ip() const { return ip_; }
  int port() const { return port_; }
  const CardPack& hand() const { return hand_; }

 private:
  std::string name_;
  // to manage the online mode
  std::string ip_;
  int port_{0};
  // cards in the player's hand
  CardPack hand_;
};

#endif
