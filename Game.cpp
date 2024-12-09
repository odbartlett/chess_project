#include <cassert>
#include "Board.h"
#include "Game.h"
#include "King.h"
#include "Piece.h"

namespace Chess
{
	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	Game::Game() : is_white_turn(true) {
		// Add the pawns
		for (int i = 0; i < 8; i++) {
			board.add_piece(Position('A' + i, '1' + 1), 'P');
			board.add_piece(Position('A' + i, '1' + 6), 'p');
		}

		// Add the rooks
		board.add_piece(Position( 'A'+0 , '1'+0 ) , 'R' );
		board.add_piece(Position( 'A'+7 , '1'+0 ) , 'R' );
		board.add_piece(Position( 'A'+0 , '1'+7 ) , 'r' );
		board.add_piece(Position( 'A'+7 , '1'+7 ) , 'r' );

		// Add the knights
		board.add_piece(Position( 'A'+1 , '1'+0 ) , 'N' );
		board.add_piece(Position( 'A'+6 , '1'+0 ) , 'N' );
		board.add_piece(Position( 'A'+1 , '1'+7 ) , 'n' );
		board.add_piece(Position( 'A'+6 , '1'+7 ) , 'n' );

		// Add the bishops
		board.add_piece(Position( 'A'+2 , '1'+0 ) , 'B' );
		board.add_piece(Position( 'A'+5 , '1'+0 ) , 'B' );
		board.add_piece(Position( 'A'+2 , '1'+7 ) , 'b' );
		board.add_piece(Position( 'A'+5 , '1'+7 ) , 'b' );

		// Add the kings and queens
		board.add_piece(Position( 'A'+3 , '1'+0 ) , 'Q' );
		board.add_piece(Position( 'A'+4 , '1'+0 ) , 'K' );
		board.add_piece(Position( 'A'+3 , '1'+7 ) , 'q' );
		board.add_piece(Position( 'A'+4 , '1'+7 ) , 'k' );
	}


    Game::Game(const Game& copy) {

	  //set turn bool to the same value
	  is_white_turn = copy.turn_white();

	  //add all the pieces to the board
	  for (char c = 'A'; c < 'I'; c++) {
	    for (char r = '1'; r < '9'; r++) {
	      if (copy.board(Position(c,r)) != nullptr) {
			board.add_piece(Position(c, r), copy.board(Position(c, r))->to_ascii());
	      }
	    }
	  }
	}
  

  Game::~Game() {
    //iterate through all positions
    for (char c = 'A'; c < 'I'; c++) {
        for (char r = '1'; r < '9'; r++) {
	  // delete piece if it exists
	        if (board(Position(c,r)) != nullptr) {
	      		delete board(Position(c,r));
	      	}
	}
    }
  }
  
  void Game::make_move(const Position& start, const Position& end) {

	  //check that start pos is on board
	  if (!(start.first <= 'H' && start.first >= 'A' && start.second <= '8' && start.second >= '1')) {
	    throw Exception("start position is not on board\n");
	  }
	  //check that end pos is on board
	  if (!(end.first <= 'H' && end.first >= 'A' && end.second <= '8' && end.second >= '1')) {
            throw Exception("end position is not on board\n");
          }
	  if (board(start) == nullptr) {
	    throw Exception("no piece at start position");
	  }
	  if (is_white_turn != board(start)->is_white()) {
	    throw Exception("piece color and turn do not match");
	  }
	  // non-capture move
	  if(board(end) == nullptr) {
	    if (!(board(start)->legal_move_shape(start, end))) {
	      throw Exception("illegal move shape");
	    }
	  } else  {
	    if (board(end)->is_white() == is_white_turn) {
	      throw Exception("cannot capture own piece");
	    }
	    if (!(board(start)->legal_capture_shape(start, end))) {
	      throw Exception("illegal capture shape");
	    }
	  }
	  if(check_path(start, end, board(start))) {
	    throw Exception("path is not clear");
	  }
	  // check if move results in exposing own king
	  Game game_copy(*this);
	  game_copy.board.move_piece(start, end);
	  game_copy.promote_pawn(end);
	  if (game_copy.in_check(is_white_turn)) {
	    throw Exception("move exposes check");
	  }
	
	board.move_piece(start, end);
	promote_pawn(end);
	is_white_turn = !is_white_turn;
	  
	}

	bool Game::in_check(const bool& white) const {

	  //get the position of the king
		Position king_pos = board.find_king_pos(white);

		//iterate through pieces
		for (char c = 'A'; c < 'I'; c++) {
		  for (char r = '1'; r < '9'; r++) {
		       	
			Position p(c, r);
				
				//check the pieces of the opposite color if they can capture the king
				if (board(p) != nullptr && board(p)->is_white() != white) {
				  if (board(p)->legal_capture_shape(p, king_pos) && 
				  !check_path(p, king_pos, board(p))) {
						return true;
					}
				}
			}
		}
		return false;
	}


	bool Game::in_mate(const bool& white) const {
	  if (!in_check(white)) {return false;}

	  //make copy of board for possible moves
	  Game copy(*this);

	  //iterate through pieces via positions on the board
	  for (char c = 'A'; c < 'I'; c++) {
	     for (char r = '1'; r < '9'; r++) {
	       //check that each position has a piece
	       Position p(c,r);
	       if(copy.board(p) != nullptr) {
		   //check that the piece has the same color as whose move it is
		   if(copy.board(p)->is_white() == white) {
		     //iterate through possible moves
		     	for (char i = 'A'; i < 'I'; i++) {
	    	        for (char j = '1'; j < '9'; j++) {

			   //try and catch block that returns false only if a valid move can be made
			      try {
		      		copy.make_move(Position(c, r), Position(i, j));
		      		return false;
		    	      }
		    	      catch (Exception& e) {
		     		continue;
			      }
			 	}
		      }
		   }
		 }
	    }
	  }
	  return true;
	}

	bool Game::in_stalemate(const bool& white) const {
	  if (in_check(white)) {return false;}

	  //make copy of board for possible moves
	  Game copy(*this);

	  //iterate through pieces via positions on the board
	  for (char c = 'A'; c < 'I'; c++) {
	     for (char r = '1'; r < '9'; r++) {
	       //check that each position has a piece
	       Position p(c,r);
	       if(copy.board(p) != nullptr) {
		   //check that the piece has the same color as whose move it is
		   if(copy.board(p)->is_white() == white) {
		     //iterate through possible moves
		     	for (char i = 'A'; i < 'I'; i++) {
	    	        for (char j = '1'; j < '9'; j++) {

			   //try and catch block that returns false only if a valid move can be made
			      try {
		      		copy.make_move(Position(c, r), Position(i, j));
		      		return false;
		    	      }
		    	      catch (Exception& e) {
		     		continue;
			      }
			 	}
		      }
		   }
		 }
	     }
	  }
	  return true;
	}

    // Return the total material point value of the designated player
    int Game::point_value(const bool& white) const {
    	int sum = 0;

      	//iterate through pieces
      	for (char c = 'A'; c < 'I'; c++) {
	  for (char r = '1'; r < '9'; r++) {

	    Position p(c, r);

	    if (board(p) == nullptr) {continue;}

	    //if else to match the color of the turn
	    if (white) {
	      //add if it matches
	      if (board(p)->is_white()) {
	    	 sum += board(p)->point_value();
	      }
	    } else {
	      //add if it matches
	      if (!(board(p)->is_white())) {
	    	 sum += board(p)->point_value();
	      }
	    }
	  }
	}
	  	
        return sum;
    }


    std::istream& operator>> (std::istream& is, Game& game) {
     char piece;
     // iterate through positions on board
	for (char r = '8'; r > '0'; r--) {
	  for (char c = 'A'; c < 'I'; c++) {
	    
	    if (game.board(Position(c,r)) != nullptr) {
	      game.board.remove_piece(Position(c, r)); // remove piece
	    }
	    if (!(is >> piece)) {
	      throw Exception(" invalid/no board");
	    }
	    if (piece == '-') {
	      continue;
	    }
	    else {
	      game.board.add_piece(Position(c, r), piece); // add piece
	    }
	  }
	}
	// determine current turn
	if (!(is >> piece)) {
	  throw Exception(" no turn specified");
	}
	else {
	  if(piece == 'w') {
	    game.is_white_turn = true;
	  }
	  else if(piece == 'b') {
	    game.is_white_turn = false;
	  }
	  else {
		std::cout << piece;
	    throw Exception(" invalid turn designator");
	  }
	}
	return is;
    }

    /////////////////////////////////////
    // DO NOT MODIFY THIS FUNCTION!!!! //
    /////////////////////////////////////
	std::ostream& operator<< (std::ostream& os, const Game& game) {
		// Write the board out and then either the character 'w' or the character 'b',
		// depending on whose turn it is
		return os << game.board << (game.turn_white() ? 'w' : 'b');
	}


  bool Game::check_path(const Position& start, const Position& end, const Piece* piece) const {
    if (piece->to_ascii() == 'n' || piece->to_ascii() == 'N') {
      return false;
    }
    
    int row_diff = end.second - start.second;
    int col_diff = end.first - start.first;

    if (abs(row_diff) <= 1 && abs(col_diff) <= 1) {
      return false;
    }
    
    //moving along a single row or column
    if (row_diff == 0 || col_diff == 0) {
      //case moving along a row
      if (row_diff == 0) {
	//move right
	if (col_diff > 0) {
	  char r = end.first;
	  for (char c = start.second + 1; c < end.second; c++) {
	    if (board(Position(c, r)) != nullptr) {
	      return true;
	    } else {
	      continue;
	    }
	  }
	}
	//move left
	if (col_diff < 0) {
	  char r = end.second;
	  for (char c = start.first - 1; c > end.first; c--) {
	    if (board(Position(c, r)) != nullptr) {
              return true;
	    } else {
              continue;
            }
	  }
	}
      }
    
      //moving up
      if (col_diff == 0) {
	if (row_diff > 0) {
	  char c = end.first;
	  for (char r =	start.second + 1; r < end.second; r++) {
	    if (board(Position(c, r)) != nullptr) {
              return true;
	    } else {
              continue;
            }
	  }
	}
	//moving down 
	if (row_diff < 0) {
	  char c = end.first;
	  for (char r = start.second - 1; r > end.second; r--) {
	    if (board(Position(c, r)) != nullptr) {
              return true;
            } else {
              continue;
            }
	  }
	}
      }
    }
    //moving diagonally
    else {
      //moving up right
	if (row_diff > 0 && col_diff > 0) {
	char c = start.first + 1;
	for (char r = start.second + 1; r < end.second; r++) {
	  	if (board(Position(c, r)) != nullptr) {
            return true;
    	} else {
	      	c++; 
        	continue;
        }
	}
    }
	//moving up left
    if (row_diff > 0 && col_diff < 0) {
	char c = start.first - 1;
        for (char r = start.second + 1; r < end.second; r++) {
          if (board(Position(c, r)) != nullptr) {
              return true;
            } else {
              c--;
              continue;
            }
        }
      }
      //moving down right
      if (row_diff < 0 && col_diff > 0) {
		char c = start.first + 1;
        for (char r = start.second - 1; r < end.second; r--) {
          if (board(Position(c, r)) != nullptr) {
              return true;
            } else {
              c++;
              continue;
            }
        }
      }
	  //moving down left
    if (row_diff < 0 && col_diff < 0) {
	char c = start.first - 1;
        for (char r = start.second - 1; r < end.second; r--) {
          if (board(Position(c, r)) != nullptr) {
              return true;
            } else {
              c--;
              continue;
            }
        }
      }
    }
	return false;
  }


    void Game::promote_pawn(const Position& end) {
    const Piece* piece = board(end);
	
    if(piece->to_ascii() == 'P' && end.second == '8') {
      board.remove_piece(end);
      board.add_piece(end, 'Q');
    }
    else if(piece->to_ascii() == 'p' && end.second == '1') {
      board.remove_piece(end);
      board.add_piece(end, 'q');
    }
  }
}

