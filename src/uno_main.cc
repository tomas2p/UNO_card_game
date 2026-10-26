/**
 * @author Tomás Pino Pérez
 * @brief Uno card game
 * @date 09-02-2023
 * @test g++ -o uno *.cc
 * @file uno_main.cc
 * @todo Game: UI(player_card_pack, game_card_pack, back_card,
 * game_options), mechanics(1 card advice, change direction, etc)
 * @todo Online system: client/server connections
 */

#include <iostream>
#include <string>
#include <vector>

#include "uno_tools.h"

int main() {
  // menu option
  int option{0};

  // menu logic
  do {
    // menu texts and intro secuence
    Menu(option);

    // menu manage
    switch (option) {
      case 0:  // finish game
        EventFinish();
        break;
      case 1:  // rules screen
        EventRules();
        break;
      case 2:  // credits screen
        EventCredits();
        break;
      case 3:  // information screen
        EventInfo();
        break;
      case 4:  // IA game
        EventIAGame();
        break;
      case 5:  // online game
        EventOnlineGame();
        break;
      case 6:  // see card pack
        EventSeeCardPack();
        break;
      default:
        break;
    }
  } while (option != 0);
  return 0;
}
