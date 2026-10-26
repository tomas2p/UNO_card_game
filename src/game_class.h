/**
 * @author Tomás Pino Pérez
 * @brief Declarations of the UNO game engine
 * @date 09-02-2023
 * @file game_class.h
 */

#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include <cstddef>
#include <string>
#include <vector>

#include "card_pack_class.h"
#include "player_class.h"

// Game state and UNO rules; UI lives outside this class
class Game {
 public:
  static constexpr std::size_t kNoCard{static_cast<std::size_t>(-1)};

  // creates and starts a game (deal + first card)
  explicit Game(std::vector<Player> players);

  // getters
  const std::vector<Player>& players() const { return players_; }
  const Player& current_player() const { return players_[current_]; }
  std::size_t current_index() const { return current_; }
  const CardPack& discard() const { return discard_; }
  const CardPack& deck() const { return deck_; }
  std::size_t deck_size() const { return deck_.size(); }
  Color current_color() const { return current_color_; }
  int direction() const { return direction_; }
  bool over() const { return over_; }
  const std::string& winner() const { return winner_; }
  bool human_turn() const { return current_ == 0; }

  // actions of the current player
  bool CanPlay(std::size_t index) const;
  bool Play(std::size_t index, Color chosen_color = Color::Wild);
  void Draw();
  void AiTurn();

 private:
  void Start();
  void AdvanceTurn(std::size_t steps = 1);
  std::size_t NextIndex(std::size_t steps) const;
  void DrawCards(std::size_t player_index, std::size_t count);
  void RecycleDiscard();
  std::size_t AiChooseCard() const;
  Color AiChooseColor() const;

  CardPack deck_;
  CardPack discard_;
  std::vector<Player> players_;
  std::size_t current_{0};
  int direction_{1};
  Color current_color_{Color::Red};
  bool over_{false};
  std::string winner_;
};

#endif
