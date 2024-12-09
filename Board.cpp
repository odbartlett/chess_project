#include "Piece.h"
#include <iostream>
#include <utility>
#include <map>
#ifndef _WIN32
#include "Terminal.h"
#endif // !_WIN32
#include "Board.h"
#include "CreatePiece.h"
#include "Exceptions.h"

namespace Chess
{
  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  Board::Board(){}

  const Piece* Board::operator()(const Position& position) const {
    auto it = occ.find(position);
    if (it != occ.end()) {
        return it->second;
    } else {
        return nullptr;
    }
  }
  
  void Board::add_piece(const Position& position, const char& piece_designator) {
    occ[position] = create_piece(piece_designator);
  }

  void Board::remove_piece(const Position& position) {
    delete occ[position];
    occ.erase(position);
  }

  void Board::move_piece(const Position& start, const Position& end) {
    remove_piece(end);
    occ[end] = occ[start];
    occ.erase(start);
  }


  void Board::display() const {
    std::cout << std::endl;
    //display letters
    std::cout << " A B C D E F G H ";
    std::cout << std::endl;
    for(char row = '8'; row >= '1'; row--) {
      std::cout << row;
      for (char col = 'A'; col <= 'H'; col++) {
        Position pos(col, row);
        const Piece* piece = operator()(pos);

        //display checker pattern 
        if ((col + row) % 2 == 0) {
          Terminal::color_bg(Terminal::MAGENTA);
          Terminal::color_fg(true, Terminal::CYAN);
        } else {
          Terminal::color_bg(Terminal::CYAN);
          Terminal::color_fg(true, Terminal::MAGENTA);
        }

        //'-' if no piece and piece->to_ascii if there is a piece there
        if (piece != nullptr) {
          std::cout << piece->to_ascii() << ' ';        
        } else {
          std::cout << "- ";
        }
      }
      Terminal::set_default();
      std::cout <<std::endl;
    }
  }

  bool Board::has_valid_kings() const {
    int white_king_count = 0;
    int black_king_count = 0;
    for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.begin();
	 it != occ.end();
	 it++) {
      if (it->second) {
	switch (it->second->to_ascii()) {
	case 'K':
	  white_king_count++;
	  break;
	case 'k':
	  black_king_count++;
	  break;
	}
      }
    }
    return (white_king_count == 1) && (black_king_count == 1);
  }

  //function to find the location of 
  Position Board::find_king_pos(const bool& white) const {
  //iterate through board for positions and pieces
  for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.begin();
	 it != occ.end();
	 it++) {
      //compare each piece to prospective king.
      if (it->second) {
	      if (white && it->second->to_ascii() == 'K') {
	        return it->first;
	      }else if(!white && it->second->to_ascii() == 'k') {
	        return it->first;
	      }
      }
    }
    throw Exception(" no king on board");
    return Position('A',1);
  }
  


  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  std::ostream& operator<<(std::ostream& os, const Board& board) {
    for(char r = '8'; r >= '1'; r--) {
      for(char c = 'A'; c <= 'H'; c++) {
	const Piece* piece = board(Position(c, r));
	if (piece) {
	  os << piece->to_ascii();
	} else {
	  os << '-';
	}
      }
      os << std::endl;
    }
    return os;
  }
}
