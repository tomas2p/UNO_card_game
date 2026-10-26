/**
 * @author Tomás Pino Pérez
 * @brief Implementation of tools
 * @date 09-02-2023
 * @file uno_tool.cc
 */

#include "uno_tools.h"

#include <cstdlib>
#include <limits>

#include "ansi_colors.h"
#include "card_pack_class.h"
#include "player_class.h"

// read art from file resources.art (inicio line to fin line)
void ReadArt(const std::string& file_name, const int inicio, const int fin) {
  std::ifstream File(file_name);
  std::string file_text;
  for (int i{0}; i < inicio; ++i) {
    getline(File, file_text);
  }
  std::cout << '\n';
  for (int i{inicio}; i < fin - 1; ++i) {
    getline(File, file_text);
    std::cout << file_text << '\n';
  }
  std::cout << '\n';
  File.close();
}

// read an integer in [min, max]; returns min on EOF
int AskInt(const std::string& prompt, int min, int max) {
  int value;
  while (true) {
    std::cout << prompt << std::flush;
    if (std::cin >> value) {
      if (value >= min && value <= max) {
        return value;
      }
      std::cout << " Valor fuera de rango.\n";
    } else {
      if (std::cin.eof()) {
        std::exit(EXIT_SUCCESS);
      }
      std::cout << " Entrada inválida.\n";
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}

// wait until ENTER or SPACE keypressed
void PressKey() {
  std::cout << " Presione ENTER para continuar... " << std::flush;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  int key{std::cin.get()};
  while (key != '\n' && key != ' ' && key != EOF) {
    key = std::cin.get();
  }
  if (key == EOF) {
    std::cin.clear();
  }
}

// manage menu of the game
void Menu(int& option) {
  Clear();
  // UNO logo for the game
  UnoLogo();
  // display menu options
  DisplayOptions();
  option = AskInt("\n >>> ", 0, 6);
}

// events
void EventFinish() { Clear(); }

void EventInfo() {
  Clear();
  DisplayInfo();
  PressKey();
}

void EventOptions() {
  Clear();
  DisplayOptions();
  PressKey();
}

void EventRules() {
  Clear();
  DisplayRules();
  PressKey();
}

void EventCredits() {
  Clear();
  DisplayCredits();
  PressKey();
}

void EventOnlineGame() {
  Clear();
  DisplayOnlineGame();
  PressKey();
}

// display SeeCardPack title
void DisplaySeeCardPack() {
  std::cout << ansi::bg_cyan << ansi::bold_black << " || " << ansi::reset << "\t\t\t\t\t\t\t\t " << ansi::bg_cyan
            << ansi::bold_black << " || " << ansi::reset << "\n";
  std::cout << ansi::bg_cyan << ansi::bold_black << "-++-" << ansi::reset
            << "\t  Mira la baraja de cartas con las que vas a jugar\t "
            << ansi::bg_cyan << ansi::bold_black << "-++-" << ansi::reset << "\n";
  std::cout << ansi::bg_cyan << ansi::bold_black << " || " << ansi::reset << "\t\t\t\t\t\t\t\t " << ansi::bg_cyan
            << ansi::bold_black << " || " << ansi::reset << "\n\n";
}
void EventSeeCardPack() {
  // initialize variables
  int option{0};
  CardPack card_pack{CardPack::FullDeck()};
  CardPack card_pack_shuffle{CardPack::FullDeck()};

  // menu logic
  do {
    Clear();

    // print the back of card and event title
    DisplaySeeCardPack();

    // menu options
    std::cout << " [0] - Volver atrás.\n";
    std::cout << " [1] - Baraja original.\n";
    std::cout << " [2] - Barajar (la baraja del juego es independiente).\n";
    option = AskInt("\n >>> ", 0, 2);
    switch (option) {
      case 0:  // finish event
        Clear();
        break;

      case 1:  // see card pack
        Clear();
        // print the back of card and event title
        DisplaySeeCardPack();
        std::cout << card_pack.Print(14);
        PressKey();
        break;

      case 2:  // shuffle card pack
        Clear();
        card_pack_shuffle.ShufflePack();
        // print the back of card and event title
        DisplaySeeCardPack();
        std::cout << card_pack_shuffle.Print(14);
        PressKey();
        break;

      default:
        break;
    }
  } while (option != 0);
}