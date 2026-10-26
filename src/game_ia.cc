/**
 * @author Tomás Pino Pérez
 * @brief Implementation of tools
 * @date 09-02-2023
 * @file uno_tool.cc
 */

#include "card_pack_class.h"
#include "player_class.h"
#include "uno_tools.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// configuration for the game (number of players, name_human)
void GameConfigurations(int& num_players, std::string& name_human) {
  // get number of IA
  std::cout << "\n Vas a jugar contra la IA, ajustes de la partida:\n\n";
  num_players = AskInt("  - Cantidad de oponentes IA (1-7): ", 1, 7) + 1;

  // get human data
  std::cout << "  - Nombre del Jugador: " << std::flush;
  std::cin >> name_human;
  Clear();
}

// get the position of the card in the card_pack
std::size_t CardIndex(std::size_t hand_size) {
  const int max{static_cast<int>(hand_size)};
  return static_cast<std::size_t>(
      AskInt("  - Posición de la carta (desde 1): ", 1, max) - 1);
}

// event IA game secuence
void EventIAGame() {
  Clear();
  int option{0};

  // game configurations
  int num_players;
  std::string name_human;
  GameConfigurations(num_players, name_human);

  // card_pack to shuffle and give to players
  CardPack card_pack{CardPack::FullDeck()};
  card_pack.ShufflePack();  // shuffle it

  // game_card_pack
  CardPack game_card_pack;

  // create players human[0] and IAs[1+]
  std::vector<Player> players;
  players.reserve(num_players);
  players.emplace_back(name_human);  // human
  for (int i{1}; i < num_players; ++i) {
    players.emplace_back("IA_" + std::to_string(i));  // IAs
  }

  // give players cards from shuffled card_pack
  for (int round{0}; round < 7; ++round) {
    for (int i{0}; i < num_players; ++i) {
      players[i].CardGlobalToPlayerByIndex(card_pack, 0);
    }
  }

  // give the game_card_pack the first number card to play with
  std::size_t first_card{0};
  while (first_card < card_pack.size() &&
         card_pack[first_card].type() > Type::Nine) {
    ++first_card;
  }
  game_card_pack.push_back(card_pack[first_card]);
  card_pack.erase(first_card);

  // vector with the order turns by player name
  std::vector<std::string> players_name;
  players_name.reserve(num_players);
  for (int i{0}; i < num_players; ++i) {
    players_name.push_back(players[i].name());
    players[i].turn(i);
  }

  // IAGameMenu();
  do {
    Clear();

    // print title
    std::cout << "\n Jugando al UNO++ contra " << num_players - 1 << " IAs\n";

    // print the players name and number of card
    std::cout << "\n Orden: ";
    for (int i{0}; i < num_players; ++i) {
      std::cout << players_name.at(i);
      if (i != num_players - 1) {
        std::cout << " > ";
      }
    }

    std::cout << "\n\n\n";

    // print backcard and last card in game_card_pack
    std::cout << game_card_pack.PrintGameTable() << "\n\n";

    // print human player card_pack
    std::cout << players[0].Print(7);

    // print option for human to do in the game
    std::cout << " [0] - Abandonar partida.\n";
    std::cout << " [1] - Poner carta.\n";
    std::cout << " [2] - Coger carta.\n";
    std::cout << " [3] - Gritar UNO++.\n";
    std::cout << " [4] - Debug options.\n";
    option = AskInt("\n >>> ", 0, 4);

    // card_index initialization
    std::size_t card_index{0};
    // case 1 loop checker
    bool is_card_valid{false};

    switch (option) {
      case 0:  // finish game
        Clear();
        break;

      case 1:  // put index card - 1 from player to game_card_pack
        std::cout << "\n Vas a poner una carta en el mazo de juego\n";
        if (players[0].hand().empty()) {
          std::cout << "\n No tienes cartas para poner.\n";
          PressKey();
          break;
        }
        while (!is_card_valid) {
          card_index = CardIndex(players[0].hand().size());
          Card card{players[0].hand()[card_index]};
          if (game_card_pack.CanPlay(card)) {
            const bool is_reverse{card.type() == Type::Reverse};
            players[0].CardPlayerToGlobalByIndex(game_card_pack, card_index);
            if (is_reverse) {
              std::reverse(players_name.begin(), players_name.end());
            }
            is_card_valid = true;
          } else {
            std::cout << "\n No es una carta válida, prueba otra.\n\n";
            is_card_valid = false;
          }
        }
        break;

      case 2:  // take first card from card_pack to player
        std::cout << "\n Vas a coger una carta del mazo\n";
        if (card_pack.empty()) {
          std::cout << "\n No quedan cartas en el mazo.\n";
          PressKey();
          break;
        }
        players[0].CardGlobalToPlayerByIndex(card_pack, 0);
        break;

      case 3:  // shout UNO++
        Clear();
        break;

      case 4:  // debug options
        // print all the card_pack to debug
        for (int i{0}; i < num_players; ++i) {
          std::cout << " Baraja " << players[i].name() << "\n"
                    << players[i].Print(7);
        }
        std::cout << " Baraja game_card_pack\n" << game_card_pack.Print(14);
        std::cout << " Baraja card_pack\n" << card_pack.Print(14);
        PressKey();
        break;

      default:
        break;
    }
  } while (option != 0);
}