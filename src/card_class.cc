/**
 * @author Tomás Pino Pérez
 * @brief Implementation of class for card
 * @date 09-02-2023
 * @file card_class.cc
 */

#include "card_class.h"

#include <algorithm>

#include "ansi_colors.h"

const std::array<std::string_view, 5> Card::kColors{
    ansi::bg_red, ansi::bg_blue, ansi::bg_green, ansi::bg_yellow, ansi::bg_white};

const std::array<std::string_view, 15> Card::kTypes{
    "00", "01", "02", "03", "04", "05", "06", "07", "08",
    "09", "+2", "><", "XX", "<>", "+4"};

// constructor
Card::Card(Color color, Type type) : color_{color}, type_{type} {}

// print card
std::string Card::Print() const {
  std::string out{color_display()};
  out.append(" ");
  out.append(type_display());
  out.append(" ");
  out.append(ansi::reset);
  return out;
}

// colored band used as the top/bottom line of a card
std::string Card::ColorLine() const {
  std::string out{color_display()};
  out.append("    ");
  out.append(ansi::reset);
  return out;
}

// operator<< overload
std::ostream& operator<<(std::ostream& out, const Card& card) {
  out << card.Print();
  return out;
}

// operator== overload
bool Card::operator==(const Card& card) const {
  return (type_ == card.type_ && color_ == card.color_);
}

std::string Card::color_display() const {
  return std::string(kColors[static_cast<std::size_t>(color_)]);
}

std::string Card::type_display() const {
  std::string out{ansi::bold_black};
  out.append(kTypes[static_cast<std::size_t>(type_)]);
  return out;
}

// human-readable color name
std::string ColorName(Color color) {
  static const std::array<std::string_view, 5> kNames{
      "rojo", "azul", "verde", "amarillo", "comodín"};
  return std::string(kNames[static_cast<std::size_t>(color)]);
}

// format a group of cards, width cards per row
std::string FormatCards(const std::vector<Card>& cards, std::size_t width) {
  if (width == 0) {
    return "";
  }
  std::string out;
  for (std::size_t i{0}; i < cards.size(); i += width) {
    const std::size_t end{std::min(i + width, cards.size())};
    for (std::size_t j{i}; j < end; ++j) {
      out.append(cards[j].ColorLine() + " ");
    }
    out.append("\n");
    for (std::size_t j{i}; j < end; ++j) {
      out.append(cards[j].Print() + " ");
    }
    out.append("\n");
    for (std::size_t j{i}; j < end; ++j) {
      out.append(cards[j].ColorLine() + " ");
    }
    out.append("\n\n");
  }
  return out;
}
