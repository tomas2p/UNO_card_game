/**
 * @author Tomás Pino Pérez
 * @brief Declarations of tools
 * @date 09-02-2023
 * @file uno_tool.h
 */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// tools for Uno
#ifndef UNO_TOOLS_H
#define UNO_TOOLS_H

// read art from file_name (inicio line to fin line)
void ReadArt(const std::string& file_name, const int inicio, const int fin);

// manage menu of the game
void Menu(int& option);

// clear the terminal
inline void Clear() { system("clear"); }

// read an integer in [min, max]; exits cleanly on EOF
int AskInt(const std::string& prompt, int min, int max);

// wait until ENTER or SPACE keypressed
void PressKey();

// read uno logo
inline void UnoLogo() { return ReadArt("resources/art.txt", 2, 12); }

// display credits
inline void DisplayCredits() { return ReadArt("resources/texts.txt", 1, 7); }
// display information
inline void DisplayInfo() { return ReadArt("resources/texts.txt", 7, 12); }
// display options
inline void DisplayOptions() { return ReadArt("resources/texts.txt", 12, 20); }
// display finish game
inline void DisplayFinish() { return ReadArt("resources/texts.txt", 20, 22); }
// display IA game
inline void DisplayIAGame() { return ReadArt("resources/texts.txt", 22, 24); }
// display online game
inline void DisplayOnlineGame() {
  return ReadArt("resources/texts.txt", 24, 26);
}
// display rules
inline void DisplayRules() { return ReadArt("resources/texts.txt", 26, 28); }

// events
void EventFinish();
void EventInfo();
void EventOptions();
void EventRules();
void EventCredits();
void EventIAGame();
void EventOnlineGame();
void EventSeeCardPack();

#endif