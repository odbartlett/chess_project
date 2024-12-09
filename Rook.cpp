#include "Rook.h"

namespace Chess
{
  bool Rook::legal_move_shape(const Position& start, const Position& end) const {
    if (end.first == start.first || end.second == start.second) { 
        return true;     
    }
    return false;
  }
}