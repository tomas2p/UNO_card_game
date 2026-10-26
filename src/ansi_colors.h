/**
 * @author Tomás Pino Pérez
 * @brief ANSI color constants used by the game
 * @date 09-02-2023
 * @file ansi_colors.h
 */

#ifndef ANSI_COLORS_H
#define ANSI_COLORS_H

#include <string_view>

// ANSI escape sequences are kept in a namespace to avoid macro pollution
namespace ansi {

inline constexpr std::string_view reset{"\x1b[0m"};
inline constexpr std::string_view bold_black{"\x1b[1;30m"};

inline constexpr std::string_view bg_red{"\x1b[1;41m"};
inline constexpr std::string_view bg_blue{"\x1b[1;44m"};
inline constexpr std::string_view bg_green{"\x1b[1;42m"};
inline constexpr std::string_view bg_yellow{"\x1b[1;43m"};
inline constexpr std::string_view bg_white{"\x1b[1;47m"};
inline constexpr std::string_view bg_cyan{"\x1b[1;46m"};

}  // namespace ansi

#endif
