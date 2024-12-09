#include "King.h"

namespace Chess
{
  bool King::legal_move_shape(const Position& start, const Position& end) const {
    if (end.first == start.first + 1 || end.first == start.first - 1 || end.first == start.first) {
        if (end.second == start.second + 1 || end.second == start.second - 1) {
            return true; 
        }
    }
    return false;
  }
}