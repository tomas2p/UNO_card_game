/**
 * @author Tomás Pino Pérez
 * @brief Lightweight tests for cards, deck, player and game rules
 * @date 09-02-2023
 * @file uno_test.cc
 */

#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "card_class.h"
#include "card_pack_class.h"
#include "game_class.h"
#include "player_class.h"
#include "version.h"

namespace {

int failures{0};

void Check(bool condition, const std::string& message) {
  if (!condition) {
    std::cerr << "FAIL: " << message << '\n';
    ++failures;
  }
}

std::size_t CountType(const CardPack& pack, Type type) {
  std::size_t count{0};
  for (const Card& card : pack.pack()) {
    if (card.type() == type) {
      ++count;
    }
  }
  return count;
}

std::size_t CountColor(const CardPack& pack, Color color) {
  std::size_t count{0};
  for (const Card& card : pack.pack()) {
    if (card.color() == color) {
      ++count;
    }
  }
  return count;
}

std::size_t TotalCards(const Game& game) {
  std::size_t total{game.deck_size() + game.discard().size()};
  for (const Player& player : game.players()) {
    total += player.hand().size();
  }
  return total;
}

void TestCards() {
  const Card red_five{Color::Red, Type::Five};
  const Card other_red_five{Color::Red, Type::Five};
  const Card blue_five{Color::Blue, Type::Five};

  Check(red_five == other_red_five, "cards with same color and type are equal");
  Check(!(red_five == blue_five), "cards with different color are not equal");
  Check(red_five.color() == Color::Red, "card keeps its color");
  Check(red_five.type() == Type::Five, "card keeps its type");
  Check(!red_five.color_display().empty(), "card has a color display");
  Check(!red_five.type_display().empty(), "card has a type display");
  Check(ColorName(Color::Wild) == "comodín", "wild color name");
}

void TestFullDeck() {
  const CardPack deck{CardPack::FullDeck()};

  Check(deck.size() == 112, "deck has 112 cards");
  Check(CountType(deck, Type::Zero) == 4, "one zero per color");
  Check(CountType(deck, Type::DrawTwo) == 8, "two +2 per color");
  Check(CountType(deck, Type::Wild) == 8, "eight wild cards");
  Check(CountType(deck, Type::WildDrawFour) == 4, "four +4 cards");
  Check(CountColor(deck, Color::Wild) == 12, "twelve wild cards");
  for (int type{static_cast<int>(Type::One)};
       type <= static_cast<int>(Type::Skip); ++type) {
    Check(CountType(deck, static_cast<Type>(type)) == 8,
          "two action cards per color");
  }
}

void TestCanPlay() {
  const Card red_five{Color::Red, Type::Five};
  const Card red_nine{Color::Red, Type::Nine};
  const Card blue_five{Color::Blue, Type::Five};
  const Card blue_nine{Color::Blue, Type::Nine};
  const Card wild{Color::Wild, Type::Wild};
  const Card wild_four{Color::Wild, Type::WildDrawFour};

  Check(IsPlayable(red_nine, Type::Five, Color::Red, false),
        "same color is playable");
  Check(IsPlayable(blue_five, Type::Five, Color::Red, false),
        "same type is playable");
  Check(IsPlayable(wild, Type::Five, Color::Red, true),
        "wild card is always playable");
  Check(!IsPlayable(blue_nine, Type::Five, Color::Red, false),
        "different color and type is not playable");
  Check(IsPlayable(red_five, Type::Wild, Color::Red, true),
        "color still matches after a wild card");

  Check(IsPlayable(wild_four, Type::Five, Color::Red, false),
        "+4 is playable without cards of the current color");
  Check(!IsPlayable(wild_four, Type::Five, Color::Red, true),
        "+4 is not playable while holding the current color");
}

void TestPlayer() {
  CardPack deck{CardPack::FullDeck()};
  CardPack discard;
  Player player{"Test"};

  player.DrawFrom(deck, 7);
  Check(player.hand().size() == 7, "player draws seven cards");
  Check(deck.size() == 105, "draw removes cards from the deck");

  const Card played{player.hand()[0]};
  player.PlayTo(discard, 0);
  Check(player.hand().size() == 6, "play removes the card from the hand");
  Check(discard.size() == 1, "play moves the card to the discard pile");
  Check(discard.top() == played, "played card is on top of the discard pile");
}

void TestGameSetup() {
  std::vector<Player> players;
  players.emplace_back("Humano");
  players.emplace_back("IA_1");
  Game game{std::move(players)};

  Check(!game.over(), "new game is running");
  Check(game.players().size() == 2, "game keeps its players");
  for (const Player& player : game.players()) {
    Check(player.hand().size() == 7, "every player starts with seven cards");
  }
  Check(game.discard().size() == 1, "one card starts on the discard pile");
  Check(game.deck_size() == 112 - 14 - 1, "deck keeps the remaining cards");
  Check(game.discard().top().type() <= Type::Nine,
        "game starts with a number card");
  Check(game.current_index() == 0, "human starts");
  Check(game.current_color() == game.discard().top().color(),
        "current color matches the first card");
}

void TestUnoCall() {
  Check(!NeedsUnoPenalty(0, false), "an empty hand needs no penalty");
  Check(NeedsUnoPenalty(1, false), "one card without a shout needs the penalty");
  Check(!NeedsUnoPenalty(1, true), "one card after shouting is fine");
  Check(!NeedsUnoPenalty(2, false), "two cards do not need a shout yet");
  Check(!NeedsUnoPenalty(2, true), "two cards after shouting are fine");

  std::vector<Player> players;
  players.emplace_back("Humano");
  players.emplace_back("IA_1");
  Game game{std::move(players)};

  Check(!game.ShoutUno(), "cannot shout UNO with seven cards");
  Check(game.players()[0].hand().size() == 7,
        "shouting does not change the hand");
}

std::size_t FirstPlayable(const Game& game) {
  for (std::size_t i{0}; i < game.current_player().hand().size(); ++i) {
    if (game.CanPlay(i)) {
      return i;
    }
  }
  return Game::kNoCard;
}

void PlayOrDraw(Game& game) {
  const std::size_t index{FirstPlayable(game)};
  if (index == Game::kNoCard) {
    game.Draw();
  } else {
    game.Play(index);
  }
}

void TestUnoPenalty() {
  bool shouted_ok{false};
  bool silent_ok{false};

  for (int trial{0}; trial < 300 && !(shouted_ok && silent_ok); ++trial) {
    std::vector<Player> players;
    players.emplace_back("Humano");
    players.emplace_back("IA_1");
    Game game{std::move(players)};
    const bool shout{trial % 2 == 0};

    while (!game.over()) {
      if (game.current_index() != 0) {
        game.AiTurn();
        continue;
      }
      if (game.players()[0].hand().size() != 2) {
        PlayOrDraw(game);
        continue;
      }

      // the human is about to play its second-to-last card
      if (shout) {
        Check(game.ShoutUno(), "shouting UNO with two cards works");
      }
      const std::size_t index{FirstPlayable(game)};
      if (index == Game::kNoCard) {
        game.Draw();
        continue;
      }
      game.Play(index);
      const std::size_t after{game.players()[0].hand().size()};
      if (shout) {
        Check(after == 1, "shouting UNO avoids the penalty");
        shouted_ok = true;
      } else {
        Check(after == 3, "not shouting UNO costs two cards");
        silent_ok = true;
      }
      break;
    }
  }

  Check(shouted_ok, "a game reached the UNO situation with a shout");
  Check(silent_ok, "a game reached the UNO situation without a shout");
}

void TestGamesFinish() {
  for (std::size_t num_players{2}; num_players <= 8; ++num_players) {
    for (int trial{0}; trial < 20; ++trial) {
      std::vector<Player> players;
      players.reserve(num_players);
      for (std::size_t i{0}; i < num_players; ++i) {
        players.emplace_back(i == 0 ? "Humano"
                                    : "IA_" + std::to_string(i));
      }
      Game game{std::move(players)};

      std::size_t steps{0};
      while (!game.over() && steps < 20000) {
        Check(TotalCards(game) == 112, "no card is lost during a game");
        game.AiTurn();
        ++steps;
      }
      Check(game.over(), "game always ends");
      Check(!game.winner().empty(), "game always has a winner");
      Check(TotalCards(game) == 112, "no card is lost at the end");
    }
  }
}

void TestPrint() {
  CardPack pack;
  pack.push_back(Card(Color::Red, Type::Zero));
  pack.push_back(Card(Color::Blue, Type::One));
  const std::string out{pack.Print(2)};

  Check(!out.empty(), "printing a pack produces output");
  Check(out.find("00") != std::string::npos, "printed pack shows card types");
  Check(pack.Print(0).empty(), "zero width prints nothing");
}

void TestVersion() {
  Check(!uno::Version().empty(), "version is not empty");
  Check(uno::VersionLine().rfind("UNO++ ", 0) == 0,
        "version line starts with the program name");
  Check(uno::VersionLine().find(uno::Version()) != std::string::npos,
        "version line contains the version");
}

}  // namespace

int main() {
  TestCards();
  TestFullDeck();
  TestCanPlay();
  TestPlayer();
  TestGameSetup();
  TestUnoCall();
  TestUnoPenalty();
  TestGamesFinish();
  TestPrint();
  TestVersion();

  if (failures == 0) {
    std::cout << "All tests passed.\n";
    return 0;
  }
  std::cerr << failures << " test(s) failed.\n";
  return 1;
}
