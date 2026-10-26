/**
 * @author Tomás Pino Pérez
 * @brief Implementation of class for card_pack
 * @date 09-02-2023
 * @file card_pack_class.cc
 */

#include "card_pack_class.h"

#include <algorithm>
#include <random>

#include "ansi_colors.h"

// complete UNO deck (112 cards)
CardPack CardPack::FullDeck() {
  CardPack deck;
  // generate the 0-9, +2, > <, XX cards
  for (int j{0}; j < 4; ++j) {
    const Color color{static_cast<Color>(j)};
    deck.push_back(Card(color, Type::Zero));
    for (int k{1}; k <= 12; ++k) {
      for (int i{0}; i < 2; ++i) {
        deck.push_back(Card(color, static_cast<Type>(k)));
      }
    }
  }
  // generate the <> card
  for (int i{0}; i < 8; ++i) {
    deck.push_back(Card(Color::Wild, Type::Wild));
  }
  // generate the +4 card
  for (int i{0}; i < 4; ++i) {
    deck.push_back(Card(Color::Wild, Type::WildDrawFour));
  }
  return deck;
}

// erase given index
void CardPack::erase(std::size_t index) {
  pack_.erase(pack_.begin() + static_cast<std::ptrdiff_t>(index));
}

// push_back
void CardPack::push_back(const Card& card) { pack_.push_back(card); }

// print cards until width
std::string CardPack::Print(std::size_t width) const {
  return FormatCards(pack_, width);
}

// print the game table (backcard and last card in pack)
std::string CardPack::PrintGameTable() const {
  if (pack_.empty()) {
    return "  (No hay cartas en la mesa)\n";
  }
  const Card& last{pack_.back()};
  std::string out;
  out.append("\t  ");
  out.append(ansi::bg_cyan);
  out.append(ansi::bold_black);
  out.append(" || ");
  out.append(ansi::reset);
  out.append("\t    " + last.ColorLine() + "\n");
  out.append("\t  ");
  out.append(ansi::bg_cyan);
  out.append(ansi::bold_black);
  out.append("-++-");
  out.append(ansi::reset);
  out.append("\t    " + last.Print() + "\n");
  out.append("\t  ");
  out.append(ansi::bg_cyan);
  out.append(ansi::bold_black);
  out.append(" || ");
  out.append(ansi::reset);
  out.append("\t    " + last.ColorLine() + "\n");
  return out;
}

// mix the CardPack
void CardPack::ShufflePack() {
  static std::mt19937 rgn{std::random_device{}()};
  std::shuffle(pack_.begin(), pack_.end(), rgn);
}
