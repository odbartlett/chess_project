#include "Pawn.h"
#include "Game.h"
#include "Piece.h"

namespace Chess
{
  bool Pawn::legal_move_shape(const Position& start, const Position& end) const {
        if (this->is_white() && start.second == '2') {
            if (end.second == start.second + 1 || end.second == start.second + 2) {
                if (end.first == start.first) {
                    return true; 
                }
            }
        } else if (!this->is_white() && start.second == '7'){
            if (end.second == start.second - 1 || end.second == start.second - 2) {
                if (end.first == start.first) {
                    return true; 
                }
            }
        } else if (this->is_white()) {
            if (end.second == start.second + 1) {
                if (end.first == start.first) {
                    return true; 
                }
            }
        } else if (!this->is_white()) {
            if (end.second == start.second - 1) {
                if (end.first == start.first) {
                    return true; 
                }
            }
        }
    return false;
  }

  bool Pawn::legal_capture_shape(const Position& start, const Position& end) const {
    if (this->is_white()) {
            if (end.second == start.second + 1) {
                if (end.first == start.first + 1 || end.first == start.first - 1) {
                    return true; 
                }
            }
    } else {
        if (end.second == start.second - 1) {
            if (end.first == start.first + 1 || end.first == start.first - 1) {
                return true; 
            }
        }
    }
    return false; 
  }
}
