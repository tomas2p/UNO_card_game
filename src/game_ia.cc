/**
 * @author Tomás Pino Pérez
 * @brief IA game UI (the rules live in Game)
 * @date 09-02-2023
 * @file game_ia.cc
 */

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "game_class.h"
#include "player_class.h"
#include "uno_tools.h"

namespace {

constexpr auto kAiPause{std::chrono::milliseconds(350)};
constexpr std::size_t kMaxIas{7};
constexpr std::size_t kHandWidth{7};

// configuration for the game (number of players, name_human)
void GameConfigurations(int& num_players, std::string& name_human) {
  std::cout << "\n Vas a jugar contra la IA, ajustes de la partida:\n\n";
  const int ias{
      AskInt("  - Cantidad de oponentes IA (1-7): ", 1,
             static_cast<int>(kMaxIas))};
  num_players = ias + 1;  // add human

  std::cout << "  - Nombre del Jugador: " << std::flush;
  std::cin >> name_human;
  if (name_human.empty()) {
    name_human = "Jugador";
  }
  Clear();
}

// get the position of the card in the hand
std::size_t CardIndex(std::size_t hand_size) {
  const int max{static_cast<int>(hand_size)};
  return static_cast<std::size_t>(
      AskInt("  - Posición de la carta (desde 1): ", 1, max) - 1);
}

// ask the color for a wild card
Color AskColor() {
  std::cout << "  Elige color:\n"
            << "   [1] rojo\n   [2] azul\n   [3] verde\n   [4] amarillo\n";
  return static_cast<Color>(AskInt("  Color >>> ", 1, 4) - 1);
}

// draw the whole game state
void RenderGame(Game& game) {
  Clear();
  std::cout << "\n Jugando al UNO++ contra " << game.players().size() - 1
            << " IAs\n";

  std::cout << "\n Orden: ";
  for (std::size_t i{0}; i < game.players().size(); ++i) {
    const Player& player{game.players()[i]};
    std::cout << player.name();
    if (i == game.current_index()) {
      std::cout << " (turno)";
    }
    if (i + 1 < game.players().size()) {
      std::cout << " > ";
    }
  }

  std::cout << "\n\n" << game.discard().PrintGameTable();
  std::cout << "\n  Color actual: " << ColorName(game.current_color())
            << "   |   Cartas en el mazo: " << game.deck_size() << "\n\n";

  const std::string notice{game.TakeNotice()};
  if (!notice.empty()) {
    std::cout << "  " << notice << "\n\n";
  }

  const Player& human{game.players()[0]};
  std::cout << " Tu baraja (" << human.hand().size() << " cartas):\n"
            << human.Print(kHandWidth);

  if (!game.human_turn()) {
    std::cout << " Turno de " << game.current_player().name() << "...\n";
  } else if (human.hand().size() <= 2 && !game.current_shouted_uno()) {
    std::cout << " ¡Te quedan " << human.hand().size()
              << " cartas! Grita UNO++ o robarás 2.\n";
  }
}

// show the game result
void ShowResult(const Game& game) {
  Clear();
  if (game.over()) {
    std::cout << "\n ¡" << game.winner() << " gana la partida!\n";
  } else {
    std::cout << "\n Partida abandonada.\n";
  }
  PressKey();
}

// debug view: every hand, the discard pile and the deck
void RenderDebug(const Game& game) {
  Clear();
  for (const Player& player : game.players()) {
    std::cout << " Baraja " << player.name() << "\n" << player.Print(kHandWidth);
  }
  std::cout << " Descarte\n" << game.discard().Print(14);
  std::cout << " Mazo\n" << game.deck().Print(14);
  PressKey();
}

// true when the current player has at least one legal card
bool HasPlayable(const Game& game) {
  for (std::size_t i{0}; i < game.current_player().hand().size(); ++i) {
    if (game.CanPlay(i)) {
      return true;
    }
  }
  return false;
}

// one human action; returns false when the player abandons
bool HumanTurn(Game& game) {
  std::cout << " [0] - Abandonar partida.\n";
  std::cout << " [1] - Poner carta.\n";
  std::cout << " [2] - Coger carta.\n";
  std::cout << " [3] - Gritar UNO++.\n";
  std::cout << " [4] - Debug options.\n";
  const int option{AskInt("\n >>> ", 0, 4)};

  switch (option) {
    case 0:
      return false;

    case 1: {
      const std::size_t hand_size{game.current_player().hand().size()};
      if (hand_size == 0) {
        std::cout << "\n No tienes cartas para poner.\n";
        PressKey();
        break;
      }
      std::size_t index{0};
      if (!HasPlayable(game)) {
        std::cout << "\n No tienes cartas jugables: coges una del mazo.\n";
        game.Draw();
        PressKey();
        break;
      }
      while (true) {
        index = CardIndex(hand_size);
        if (game.CanPlay(index)) {
          break;
        }
        std::cout << "\n No es una carta válida, prueba otra.\n";
        if (game.current_player().hand()[index].type() ==
            Type::WildDrawFour) {
          std::cout << " El +4 solo se puede jugar si no tienes cartas del"
                       " color actual.\n";
        }
        std::cout << '\n';
      }
      Color color{Color::Wild};
      if (game.current_player().hand()[index].color() == Color::Wild) {
        color = AskColor();
      }
      game.Play(index, color);
      break;
    }

    case 2:
      std::cout << "\n Coges una carta del mazo.\n";
      game.Draw();
      break;

    case 3:
      if (!game.ShoutUno()) {
        std::cout << "\n Solo puedes gritar UNO con 1 o 2 cartas.\n";
        PressKey();
      }
      break;

    case 4:
      RenderDebug(game);
      break;

    default:
      break;
  }
  return true;
}

}  // namespace

// event IA game sequence
void EventIAGame() {
  Clear();

  int num_players{0};
  std::string name_human;
  GameConfigurations(num_players, name_human);

  std::vector<Player> players;
  players.reserve(static_cast<std::size_t>(num_players));
  players.emplace_back(name_human);
  for (int i{1}; i < num_players; ++i) {
    players.emplace_back("IA_" + std::to_string(i));
  }

  Game game{std::move(players)};

  while (!game.over()) {
    RenderGame(game);
    if (game.human_turn()) {
      if (!HumanTurn(game)) {
        break;
      }
    } else {
      std::this_thread::sleep_for(kAiPause);
      game.AiTurn();
    }
  }

  ShowResult(game);
}
