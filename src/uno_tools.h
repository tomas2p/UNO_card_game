/**
 * @author Tomás Pino Pérez
 * @brief Declarations of tools
 * @date 09-02-2023
 * @file uno_tools.h
 */

#ifndef UNO_TOOLS_H
#define UNO_TOOLS_H

#include <iostream>
#include <string>

// tools for Uno

// print the content of a section of file_name (marker: "// section")
void ReadArt(const std::string& file_name, const std::string& section);

// manage menu of the game
void Menu(int& option);

// clear the terminal
inline void Clear() { std::cout << "\x1b[2J\x1b[H" << std::flush; }

// read an integer in [min, max]; exits cleanly on EOF
int AskInt(const std::string& prompt, int min, int max);

// wait until ENTER or SPACE keypressed
void PressKey();

// read uno logo
inline void UnoLogo() { ReadArt("resources/art.txt", "Uno Logo"); }

// display credits
inline void DisplayCredits() { ReadArt("resources/texts.txt", "Display credits"); }
// display information
inline void DisplayInfo() { ReadArt("resources/texts.txt", "Display info"); }
// display options
inline void DisplayOptions() { ReadArt("resources/texts.txt", "Display options"); }
// display finish game
inline void DisplayFinish() { ReadArt("resources/texts.txt", "Display finish"); }
// display online game
inline void DisplayOnlineGame() {
  ReadArt("resources/texts.txt", "Display online game");
}
// display rules
inline void DisplayRules() { ReadArt("resources/texts.txt", "Display rules"); }

// events
void EventFinish();
void EventInfo();
void EventRules();
void EventCredits();
void EventIAGame();
void EventOnlineGame();
void EventSeeCardPack();

#endif
