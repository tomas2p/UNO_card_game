/**
 * @author Tomás Pino Pérez
 * @brief Declarations of class for card
 * @date 09-02-2023
 * @file card_class.h
 */

#ifndef CARD_CLASS_H
#define CARD_CLASS_H

#include <array>
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>

#include "ANSI-color-codes.h"

// Card colors; Wild is only used by <>, +4
enum class Color { Red = 0, Blue, Green, Yellow, Wild };

// Card types, in display order
enum class Type {
  Zero = 0,
  One,
  Two,
  Three,
  Four,
  Five,
  Six,
  Seven,
  Eight,
  Nine,
  DrawTwo,
  Reverse,
  Skip,
  Wild,
  WildDrawFour
};

// Class for manage the card
class Card {
 public:
  // constructor
  Card(Color color, Type type);

  // operator<< overload
  friend std::ostream& operator<<(std::ostream&, const Card&);
  // operator== overload
  bool operator==(const Card& card) const;

  // print card
  std::string Print() const;

  // getters
  Color color() const { return color_; }
  Type type() const { return type_; }
  std::string color_display() const;
  std::string type_display() const;

  // setters
  void color(Color color) { color_ = color; }

 private:
  // background colors: red, blue, green, yellow, white
  static const std::array<std::string_view, 5> kColors;
  // numbers 0-9; plus +2, +4; change direccion ><; change color <>; block XX
  static const std::array<std::string_view, 15> kTypes;

  // easy to implement by index of the const string with the similar name
  Color color_;
  Type type_;
};

#endif
