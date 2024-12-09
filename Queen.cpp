#include "Queen.h"

namespace Chess
{
  bool Queen::legal_move_shape(const Position& start, const Position& end) const {
    if (end.first == start.first || end.second == start.second) { 
        return true;     
    }else if (abs(start.first - end.first) == abs(start.second - end.second)) {
      return true;
    } else {
        return false; 
    }
    
  }
}