/**
 * @author Tomás Pino Pérez
 * @brief Declarations of class for player
 * @date 09-02-2023
 * @file player_class.h
 */

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "ANSI-color-codes.h"
#include "card_pack_class.h"

// Class for manage the card_pack
#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H

class Player {
 public:
  // constructor for online game
  Player(const std::string& name, const std::string& ip, const int& port);
  // constructor for ia game
  Player(const std::string& name);

  // put card from global_card_pack to player_card_pack
  void CardGlobalToPlayerByIndex(CardPack& global_pack, const int& index);
  // put card from player_card_pack to global_card_pack
  void CardPlayerToGlobalByIndex(CardPack& global_pack, const int& index);

  // print cards until width
  std::string Print(const int& width);

  // getters
  std::string name() const { return name_; }
  std::string ip() const { return ip_; }
  int port() const { return port_; }
  std::vector<Card> card_pack() const { return card_pack_; }
  std::vector<std::string> color_pack() const { return color_pack_; }
  int turn() const { return turn_; }

  // setters
  void turn(int turn) { turn_ = turn; }

 private:
  std::string name_;
  // to manage the online mode
  std::string ip_;
  int port_;
  // to manage the card_pack of the player
  std::vector<Card> card_pack_;
  std::vector<std::string> color_pack_;
  // turn of the player in game
  int turn_{0};
};

#endif