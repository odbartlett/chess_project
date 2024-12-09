#include "Knight.h"
#include "Piece.h"

namespace Chess
{
  bool Knight::legal_move_shape(const Position& start, const Position& end) const {
    
    if (end.second == start.second + 1 && (end.first == start.first + 2 
        || end.first == start.first - 2)) {
        return true; 
    }
    if (end.second == start.second - 1 && (end.first == start.first + 2 
        || end.first == start.first - 2)) {
        return true; 
    }

    if (end.second == start.second + 2 && (end.first == start.first + 1 
        || end.second == start.second - 1)) {
        return true; 
    }
    if (end.second == start.second - 2 && (end.first == start.first + 1 
        || end.first == start.first - 1)) {
        return true; 
    }
    
    return false;
  }

}
