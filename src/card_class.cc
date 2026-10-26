/**
 * @author Tomás Pino Pérez
 * @brief Implementation of class for card
 * @date 09-02-2023
 * @file card_class.cc
 */

#include "card_class.h"

const std::array<std::string_view, 5> Card::kColors{BREDB, BBLUB, BGRNB,
                                                     BYELB, BWHTB};

const std::array<std::string_view, 15> Card::kTypes{
    BBLK "00", BBLK "01", BBLK "02", BBLK "03", BBLK "04",
    BBLK "05", BBLK "06", BBLK "07", BBLK "08", BBLK "09",
    BBLK "+2", BBLK "><", BBLK "XX", BBLK "<>", BBLK "+4"};

// constructor
Card::Card(Color color, Type type) : color_{color}, type_{type} {}

// print card
std::string Card::Print() const {
  return color_display() + ' ' + type_display() + ' ' + reset;
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
  return std::string(kTypes[static_cast<std::size_t>(type_)]);
}
