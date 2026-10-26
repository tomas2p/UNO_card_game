/**
 * @author Tomás Pino Pérez
 * @brief Implementation of the UNO game engine
 * @date 09-02-2023
 * @file game_class.cc
 */

#include "game_class.h"

#include <algorithm>
#include <array>
#include <utility>

namespace {
constexpr std::size_t kHandSize{7};
constexpr std::size_t kMaxPlayers{8};
}  // namespace

// creates and starts a game (deal + first card)
Game::Game(std::vector<Player> players) : players_{std::move(players)} {
  Start();
}

// deal cards and reveal the first number card
void Game::Start() {
  if (players_.empty() || players_.size() > kMaxPlayers) {
    over_ = true;
    return;
  }
  deck_ = CardPack::FullDeck();
  deck_.ShufflePack();
  uno_shouted_.assign(players_.size(), false);

  for (std::size_t round{0}; round < kHandSize; ++round) {
    for (std::size_t i{0}; i < players_.size(); ++i) {
      DrawCards(i, 1);
    }
  }

  // the game starts with a number card so no special rule applies on turn 0
  std::size_t first{kNoCard};
  for (std::size_t i{0}; i < deck_.size(); ++i) {
    if (deck_[i].type() <= Type::Nine) {
      first = i;
      break;
    }
  }
  if (first == kNoCard) {
    over_ = true;
    return;
  }
  const Card first_card{deck_[first]};
  deck_.erase(first);
  discard_.push_back(first_card);
  current_color_ = first_card.color();
}

// check if the current player can play the card at index
bool Game::CanPlay(std::size_t index) const {
  if (over_ || players_.empty() || discard_.empty()) {
    return false;
  }
  const CardPack& hand{players_[current_].hand()};
  if (index >= hand.size()) {
    return false;
  }
  const Card& card{hand[index]};
  if (card.color() == Color::Wild) {
    return true;
  }
  if (card.type() == discard_.top().type()) {
    return true;
  }
  return card.color() == current_color_;
}

// play the card at index and apply its effect
bool Game::Play(std::size_t index, Color chosen_color) {
  if (!CanPlay(index)) {
    return false;
  }
  const Card card{players_[current_].hand()[index]};
  players_[current_].PlayTo(discard_, index);

  if (card.color() == Color::Wild) {
    current_color_ = (chosen_color == Color::Wild) ? Color::Red : chosen_color;
  } else {
    current_color_ = card.color();
  }

  if (players_[current_].hand().empty()) {
    over_ = true;
    winner_ = players_[current_].name();
    return true;
  }

  // playing down to one card without shouting UNO costs two cards
  const bool penalty{
      NeedsUnoPenalty(players_[current_].hand().size(),
                      current_ < uno_shouted_.size() && uno_shouted_[current_])};
  if (current_ < uno_shouted_.size()) {
    uno_shouted_[current_] = false;
  }
  if (penalty) {
    DrawCards(current_, 2);
    notice_ = players_[current_].name() + " no gritó UNO y roba 2 cartas.";
  }

  switch (card.type()) {
    case Type::Reverse:
      direction_ = -direction_;
      // with two players, reverse works as a skip
      AdvanceTurn(players_.size() == 2 ? 2 : 1);
      break;
    case Type::Skip:
      AdvanceTurn(2);
      break;
    case Type::DrawTwo: {
      const std::size_t next{NextIndex(1)};
      DrawCards(next, 2);
      AdvanceTurn(2);
      break;
    }
    case Type::WildDrawFour: {
      const std::size_t next{NextIndex(1)};
      DrawCards(next, 4);
      AdvanceTurn(2);
      break;
    }
    default:
      AdvanceTurn();
      break;
  }
  return true;
}

// draw one card and pass the turn
void Game::Draw() {
  if (over_) {
    return;
  }
  DrawCards(current_, 1);
  // a fresh card means the UNO situation is gone
  if (current_ < uno_shouted_.size()) {
    uno_shouted_[current_] = false;
  }
  AdvanceTurn();
}

// basic AI: play a non-wild card if possible, otherwise draw
void Game::AiTurn() {
  if (over_) {
    return;
  }
  // the AI always shouts when it is about to play its second-to-last card
  if (players_[current_].hand().size() == 2) {
    ShoutUno();
  }
  const std::size_t index{AiChooseCard()};
  if (index == kNoCard) {
    Draw();
    return;
  }
  Play(index, AiChooseColor());
}

// shout UNO; only valid with one or two cards left
bool Game::ShoutUno() {
  if (over_ || players_.empty()) {
    return false;
  }
  const std::size_t hand_size{players_[current_].hand().size()};
  if (hand_size < 1 || hand_size > 2) {
    return false;
  }
  if (current_ < uno_shouted_.size()) {
    uno_shouted_[current_] = true;
  }
  if (human_turn()) {
    notice_ = players_[current_].name() + " grita ¡UNO++!";
  }
  return true;
}

// take the message of the last action (empty when there is nothing to say)
std::string Game::TakeNotice() {
  std::string notice;
  notice.swap(notice_);
  return notice;
}

// first playable non-wild card, then the first playable wild card
std::size_t Game::AiChooseCard() const {
  const CardPack& hand{players_[current_].hand()};
  std::size_t wild{kNoCard};
  for (std::size_t i{0}; i < hand.size(); ++i) {
    if (!CanPlay(i)) {
      continue;
    }
    if (hand[i].color() != Color::Wild) {
      return i;
    }
    if (wild == kNoCard) {
      wild = i;
    }
  }
  return wild;
}

// pick the color the AI has the most of
Color Game::AiChooseColor() const {
  std::array<std::size_t, 4> counts{};
  for (const Card& card : players_[current_].hand().pack()) {
    if (card.color() != Color::Wild) {
      ++counts[static_cast<std::size_t>(card.color())];
    }
  }
  const auto best{std::max_element(counts.begin(), counts.end())};
  return static_cast<Color>(std::distance(counts.begin(), best));
}

// advance the turn in the current direction
void Game::AdvanceTurn(std::size_t steps) {
  const int size{static_cast<int>(players_.size())};
  if (size == 0) {
    return;
  }
  for (std::size_t i{0}; i < steps; ++i) {
    const int next{(static_cast<int>(current_) + direction_ + size) % size};
    current_ = static_cast<std::size_t>(next);
  }
}

// player index `steps` turns ahead (without changing the turn)
std::size_t Game::NextIndex(std::size_t steps) const {
  const int size{static_cast<int>(players_.size())};
  if (size == 0) {
    return 0;
  }
  int index{(static_cast<int>(current_) +
             direction_ * static_cast<int>(steps)) %
            size};
  if (index < 0) {
    index += size;
  }
  return static_cast<std::size_t>(index);
}

// draw count cards for a player, recycling the discard pile if needed
void Game::DrawCards(std::size_t player_index, std::size_t count) {
  for (std::size_t i{0}; i < count; ++i) {
    if (deck_.empty()) {
      RecycleDiscard();
    }
    if (deck_.empty()) {
      return;
    }
    players_[player_index].DrawFrom(deck_);
  }
}

// move all but the top discard card back to the deck and shuffle it
void Game::RecycleDiscard() {
  if (discard_.size() <= 1) {
    return;
  }
  const Card top{discard_.top()};
  std::vector<Card> rest{discard_.pack()};
  rest.pop_back();
  discard_.clear();
  discard_.push_back(top);
  for (const Card& card : rest) {
    deck_.push_back(card);
  }
  deck_.ShufflePack();
}
