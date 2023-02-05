// Minesweeper
// By: Benjamin Yee
// Email: yeebenja@umich.edu
// LinkedIn: https://www.linkedin.com/in/yeebenja
// GitHub: https://github.com/yeebenja

#include <iostream>
#include <string>
#include "assert.h"
#include <cassert>
#include <random>

using namespace std;

// Exceptions
class Improper_Dimensions {};
class Improper_Number_of_Mines {};
class Invalid_Arguements {};

// Minesweeper Class
class Minesweeper_Game {
private:
	struct Tile {	// Note: Tile is just "plain old data". No need
					// to use dynamic programming
		bool is_revealed;	// Indicates if tile has been revealed of not
		bool has_mine;		// If true, then tile contains a mine
		int number_of_nearby_mines; // Indicates number nearby mines in all directions
									// N, NE, E, SE, S, SW, W, NW
		
		// Tile pointers that point to other tiles around current tile
		Tile* north;
		Tile* northeast;
		Tile* east;
		Tile* southeast;
		Tile* south;
		Tile* southwest;
		Tile* west;
		Tile* northwest;
	};
	Tile* board[1000];	// MAX SIZE = 1000, change as needed
						// Note: board[] is an array of Tile pointers
	Tile** board_start; // Tile pointer to start of board
	int width;
	int height;
	int total_number_of_tiles;
	int number_of_mines;
	bool hit_mine;

public:

	// Default Constructor (14x18 board with 40 mines)
	Minesweeper_Game() : width(14), height(18), number_of_mines(40), 
		total_number_of_tiles(14*18), hit_mine(false) {
		board_start = board;
	}

	// Custom Constructor: Specify width and height. Number of mines default 20
	Minesweeper_Game(int width_in, int height_in) : 
		width(width_in), height(height_in), total_number_of_tiles(width_in * height_in),
		number_of_mines(20), hit_mine(false) {
		board_start = board;
		try {
			Improper_Dimensions error_1;
			if(width_in <= 1 || height_in <= 1) throw error_1;
		}
		catch (const Improper_Dimensions& obj_1) {
			cout << "Error: Improper Dimensions. Ending Program." << endl;
		}
	}

	// Custom Constructor: Specify width, height, and number of mines
	Minesweeper_Game(int width_in, int height_in, int number_of_mines_in) :
		width(width_in), height(height_in), number_of_mines(number_of_mines_in),
		total_number_of_tiles(width_in * height_in), hit_mine(false) {
		board_start = board;
		try {
			Improper_Dimensions error_1;
			Improper_Number_of_Mines error_2;
			if (width_in <= 1 || height_in <= 1) throw error_1;
			if (number_of_mines_in <= 0 || number_of_mines_in > width_in * height_in) throw error_2;
		}
		catch (const Improper_Dimensions& obj_1) {
			cout << "Error: Improper dimensions. Ending program." << endl;
		}
		catch (const Improper_Number_of_Mines& obj_2) {
			cout << "Error: Improper number of mines: Ending program." << endl;
		}
	}

	// Destructor
	~Minesweeper_Game() {
		// Make sure to delete all dynamically allocated
		// tiles in board[]
		for (size_t i = 0; i < total_number_of_tiles; ++i) delete board[i];
	}
	
	// EFFECTS: Runs game of Minesweeper
	void run_game(void) {
		make_board();
		place_mines();
		run_game_helper();
	}

	// EFFECTS: Places mines for testing and debugging
	// For debugging and testing purposes only
	void place_mines_debug(void) {
		// 15 22 34 42 53 60 62 67 68 74
		board[15]->has_mine = true;
		board[22]->has_mine = true;
		board[34]->has_mine = true;
		board[42]->has_mine = true;
		board[53]->has_mine = true;
		board[60]->has_mine = true;
		board[62]->has_mine = true;
		board[67]->has_mine = true;
		board[68]->has_mine = true;
		board[74]->has_mine = true;

		// Update the number of nearby mines for each tile
		for (size_t i = 0; i < total_number_of_tiles; ++i) {
			look_for_mines_nearby(board[i]);
		}
		// Uncomment for debugging purposes
		//reveal_mines();
	}

	// EFFECTS: Output location of all mines
	// For debugging and testing purposes only
	void reveal_mines(void) {
		for (size_t i = 0; i < total_number_of_tiles; ++i) {
			if (board[i]->has_mine == true) {
				Tile* current_tile = board[i];
				int current_row = row_index(current_tile);
				int current_col = column_index(current_tile);
				cout << "Mine placed at row " << current_row << " col " << current_col;
				cout << " / tile " << tile_index(current_row, current_col) << endl;
			}
		}
	}

	// EFFECTS: Takes in user input and reveals tile
	// If the tile is a mine, then game over
	// If the tile isn't a mine, then run reveal function and take in
	// more input from user.
	void run_game_helper(void) {
		while (hit_mine == false) {
			if (check_if_win() == true) break;
			print_board();
			pick_tile();
		}
		if (hit_mine == true) {
			print_board_when_game_over();
			cout << "Game Over! Hit Mine!" << endl;
		}
		if (check_if_win() == true) {
			print_board_when_game_over();
			cout << "You Won!" << endl;
		}
	}

	// EFFECTS: Checks user has revealed all tiles don't have mines, if so
	// then returns true. Otherwise, returns false.
	bool check_if_win(void) {
		for (size_t i = 0; i < total_number_of_tiles; ++i) {
			if (board[i]->has_mine) continue;
			else if (board[i]->has_mine == false && board[i]->is_revealed == false) return false;
		}
		return true;
	}

	// EFFECTS: User inputs row and column. Then reveals the tiles
	void pick_tile(void) {
		// Notes: If the user picks a tile that is already revealed, loop back
		// until user picks a tile that isn't already revealed
		int row_chosen, column_chosen;
		while (1) {
			cout << "Input row: ";
			cin >> row_chosen;
			cout << "Input col: ";
			cin >> column_chosen;
			int i = row_chosen * width + column_chosen;
			if (valid_dimensions_chosen(row_chosen, column_chosen)) {
				// Check if tile was already chosen
				if (Tile_at(row_chosen, column_chosen)->is_revealed) {
					cout << "Tile already revealed. Try again." << endl;
					continue;
				}
				else break;
			}
			else cout << "Invalid dimensions" << endl;
		}
		// Here, we can assume the user has chosen a valid tile that has not
		// yet been revealed
		Tile* tile_chosen = Tile_at(row_chosen, column_chosen);

		// Reveal tile_chosen
		tile_chosen->is_revealed = true;

		// Check if tile is a mine, if so then game over
		if (tile_chosen->has_mine) {
			hit_mine = true;
			return;
		}

		// Then run recursive helper function
		tile_reveal_recursive_func(tile_chosen);
	}
		
	// EFFECTS: Given a tile, will reveal all other tiles in all directions it until
	// tile have more than one nearby mine
	void tile_reveal_recursive_func(Tile* current_tile) {
		// -----dubugging-----
		Tile** temp = board_start;
		while (*temp != current_tile) ++temp;
		int board_index = temp - board_start; // For debugging purposes
		// -------------------
		current_tile->is_revealed = true;

		// Base Case: Stop when current_tile's number_of_nearby_mines > 0
		if (current_tile->number_of_nearby_mines > 0) {
			current_tile->is_revealed = true;
			return;
		}
		// Recursive Case: Look north, east, south, west, northwest, northeast
		// southwest, and southeast
		if (current_tile->north) { 
			if (current_tile->north->is_revealed == false) {
				tile_reveal_recursive_func(current_tile->north);
			}
		}
		if (current_tile->east) {
			if (current_tile->east->is_revealed == false) {
				tile_reveal_recursive_func(current_tile->east);
			}
		}
		if (current_tile->south) {
			if (current_tile->south->is_revealed == false) {
				tile_reveal_recursive_func(current_tile->south);
			}
		}
		if (current_tile->west) {
			if (current_tile->west->is_revealed == false) {
				tile_reveal_recursive_func(current_tile->west);
			}
		}
		// Only reveal north, south, east, and west
		// Do not call recursive function in directions northeast,
		// northwest, southeast, and southwest
	}

	// EFFECTS: Returns true if arguements row and col are in bounds of
	// the game board
	bool valid_dimensions_chosen(int row, int col) {
		if (row >= 0 && row < height && col >= 0 && col < width) return true;
		return false;
	}


	// EFFECTS: Prints each tile on the board
	// If the tile hasn't been revealed, tile is "?"
	// If the tile has been revealed, then tile is the number of nearby mines
	// but if the number of nearby mines is 0, then tile is " "
	void print_board(void) {

		// Print column
		cout << " " << " " << " ";
		for (int col = 0; col < width; ++col) {
			if (col < 10) cout << col << "  ";
			else cout << col << " "; 
		}
		cout << endl;
		for (int row = 0; row < height; ++row) {
			/*cout << row << " ";*/
			if (row < 10) cout << row << " " << " ";
			else cout << row << " ";
			for (int col = 0; col < width; ++col) {
				Tile* current_tile = Tile_at(row, col);
				// If the tile hasn't been revealed, tile is "?"
				if (current_tile->is_revealed == false) cout << "?" << " " << " ";
				// If the tile has been revealed, then tile is the number of nearby mines
				// but if the number of nearby mines is 0, then tile is " "
				else {
					if (current_tile->number_of_nearby_mines == 0) cout << " " << "  ";
					else cout << current_tile->number_of_nearby_mines << "  ";
				}
			}
			cout << endl;
		}
	}

	// EFFECTS: Prints board when game is either won or lost
	// i.e. reveals entire board including mines
	void print_board_when_game_over(void) {

		// Print column
		cout << " " << " " << " ";
		for (int col = 0; col < width; ++col) {
			if (col < 10) cout << col << "  ";
			else cout << col << " ";
		}
		cout << endl;
		for (int row = 0; row < height; ++row) {
			if (row < 10) cout << row << " " << " ";
			else cout << row << " ";
			for (int col = 0; col < width; ++col) {
				Tile* current_tile = Tile_at(row, col);
				// If the tile has a mine, print X
				if (current_tile->has_mine == true) cout << "X" << " " << " ";
				else {
					if (current_tile->number_of_nearby_mines == 0) cout << " " << "  ";
					else cout << current_tile->number_of_nearby_mines << "  ";
				}
			}
			cout << endl;
		}
	}

	// EFFECTS: Creates board based on dimensions (width and height)
	// Each Tile is initilized to have no mine, no nearby tiles with mines, and not revealed
	// The place_mines() function will be responsible for placing mines after
	// make_board() is called
	void make_board(void) {
		// First, make the tiles in the board
		for (size_t i = 0; i < total_number_of_tiles; ++i) {
			board[i] = new Tile;
		}
		// Next, make assign directional tiles within each tile
		for (int i = 0; i < total_number_of_tiles; ++i) {
			Tile* temp = board[i];
			int current_row = row_index(board[i]);
			int current_col = column_index(board[i]);
			// Case 1: Corner Tile Case
			if (is_corner_tile(board[i])) {
				// Top Left Case
				if (row_index(temp) == 0 && column_index(temp) == 0) {
					temp->north = nullptr;
					temp->northeast = nullptr;
					temp->east = Tile_at(current_row, current_col + 1);
					temp->southeast = Tile_at(current_row + 1, current_col + 1);
					temp->south = Tile_at(current_row + 1, current_col);
					temp->southwest = nullptr;
					temp->west = nullptr;
					temp->northwest = nullptr;
				}
				// Top Right Case
				else if (row_index(temp) == 0 && column_index(temp) == width - 1) {
					temp->north = nullptr;
					temp->northeast = nullptr;
					temp->east = nullptr;
					temp->southeast = nullptr;
					temp->south = Tile_at(current_row + 1, current_col);
					temp->southwest = Tile_at(current_row + 1, current_col - 1);
					temp->west = Tile_at(current_row, current_col - 1);
					temp->northwest = nullptr;
				}
				// Bottom Left Case
				else if (row_index(temp) == height - 1 && column_index(temp) == 0) {
					temp->north = Tile_at(current_row - 1, current_col);
					temp->northeast = Tile_at(current_row - 1, current_col + 1);
					temp->east = Tile_at(current_row, current_col + 1);
					temp->southeast = nullptr;
					temp->south = nullptr;
					temp->southwest = nullptr;
					temp->west = nullptr;
					temp->northwest = nullptr;
				}
				// Bottom Right Case
				else if (row_index(temp) == height - 1 && column_index(temp) == width - 1) {
					temp->north = Tile_at(current_row - 1, current_col);
					temp->northeast = nullptr;
					temp->east = nullptr;
					temp->southeast = nullptr;
					temp->south = nullptr;
					temp->southwest = nullptr;
					temp->west = Tile_at(current_row, current_col - 1);
					temp->northwest = Tile_at(current_row - 1, current_col - 1);
				}
				// For debugging purposes
				else assert(false);
			}
			// Case 2: Edge Tile Case (NOT Corner Tile)
			else if (is_edge_tile(board[i])) {
				// Top Case
				if (row_index(temp) == 0) {
					temp->north = nullptr;
					temp->northeast = nullptr;
					temp->east = Tile_at(current_row, current_col + 1);
					temp->southeast = Tile_at(current_row + 1, current_col + 1);
					temp->south = Tile_at(current_row + 1, current_col);
					temp->southwest = Tile_at(current_row + 1, current_col - 1);
					temp->west = Tile_at(current_row, current_col - 1);
					temp->northwest = nullptr;
				}
				// Left Case
				else if (column_index(temp) == 0) {
					temp->north = Tile_at(current_row - 1, current_col);
					temp->northeast = Tile_at(current_row - 1, current_col + 1);
					temp->east = Tile_at(current_row, current_col + 1);
					temp->southeast = Tile_at(current_row + 1, current_col + 1);
					temp->south = Tile_at(current_row + 1, current_col);
					temp->southwest = nullptr;
					temp->west = nullptr;
					temp->northwest = nullptr;
				}
				// Right Case
				else if (column_index(temp) == width - 1) {
					temp->north = Tile_at(current_row - 1, current_col);
					temp->northeast = nullptr;
					temp->east = nullptr;
					temp->southeast = nullptr;
					temp->south = Tile_at(current_row + 1, current_col);
					temp->southwest = Tile_at(current_row + 1, current_col - 1);
					temp->west = Tile_at(current_row, current_col - 1);
					temp->northwest = Tile_at(current_row - 1, current_col - 1);
				}
				// Bottom Case
				else if (row_index(temp) == height - 1) {
					temp->north = Tile_at(current_row - 1, current_col);
					temp->northeast = Tile_at(current_row - 1, current_col + 1);
					temp->east = Tile_at(current_row, current_col + 1);
					temp->southeast = nullptr;
					temp->south = nullptr;
					temp->southwest = nullptr;
					temp->west = Tile_at(current_row, current_col - 1);
					temp->northwest = Tile_at(current_row - 1, current_col - 1);
				}
				// For debugging purposes
				else assert(false);
			}
			// Case 3: Middle Tile Case
			else {
				temp->north = Tile_at(current_row - 1, current_col);
				temp->northeast = Tile_at(current_row - 1, current_col + 1);
				temp->east = Tile_at(current_row, current_col + 1);
				temp->southeast = Tile_at(current_row + 1, current_col + 1);
				temp->south = Tile_at(current_row + 1, current_col);
				temp->southwest = Tile_at(current_row + 1, current_col - 1);
				temp->west = Tile_at(current_row, current_col - 1);
				temp->northwest = Tile_at(current_row - 1, current_col - 1);
			}
		}
		// Upon making board, all tiles initially have no mines and is not revealed
		// We will call another function when randomly placing the mines
		for (size_t i = 0; i < total_number_of_tiles; ++i) {
			board[i]->has_mine = false;
			board[i]->is_revealed = false;
			board[i]->number_of_nearby_mines = 0;
		}
	}
	// EFFECTS: Returns a random integer in range [0, x] inclusive
	int random_0_to_x(int x) const {
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> distribution_obj(0, x); // distribution in range [0, x]
		// ^^^ Change as needed ^^^
		return distribution_obj(rng);
	}

	// EFFECTS: Randomly places mines on the board
	void place_mines(void) {
		// We can edit this function as we're developing it
		int mines_placed = 0;
		while (mines_placed != number_of_mines) {
			int x = random_0_to_x(total_number_of_tiles - 1);
			// Look first if tile already has a mine, if so skip it
			if (board[x]->has_mine) {
				continue;
			}
			board[x]->has_mine = true;
			++mines_placed;
		}
		// Update the number of nearby mines for each tile
		for (size_t i = 0; i < total_number_of_tiles; ++i) {
			look_for_mines_nearby(board[i]);
		}
		// Uncomment for debugging purposes
		//reveal_mines();
	}
	// EFFECTS: Given a Tile pointer, looks for mines nearby the current tile
	// and calculates number of nearby mines around the current tile
	void look_for_mines_nearby(Tile* current_tile) {
		int x = 0;
		if (current_tile->north) if (current_tile->north->has_mine) ++x;
		if (current_tile->northeast) if (current_tile->northeast->has_mine) ++x;
		if (current_tile->east) if (current_tile->east->has_mine) ++x;
		if (current_tile->southeast) if (current_tile->southeast->has_mine) ++x;
		if (current_tile->south) if (current_tile->south->has_mine) ++x;
		if (current_tile->southwest) if (current_tile->southwest->has_mine) ++x;
		if (current_tile->west) if (current_tile->west->has_mine) ++x;
		if (current_tile->northwest) if (current_tile->northwest->has_mine) ++x;

		current_tile->number_of_nearby_mines = x;
	}
	// EFFECTS: Returns true if tile is a corner tile
	bool is_corner_tile(Tile* temp) const {
		if((row_index(temp) == 0 && column_index(temp) == 0) ||
		   (row_index(temp) == 0 && column_index(temp) == width - 1) ||
		   (row_index(temp) == height - 1 && column_index(temp) == 0) ||
		   (row_index(temp) == height - 1 && column_index(temp) == width - 1)) return true;
		return false;
	}

	// EFFECTS: Returns true if tile is an edge tile and not a
	// corner tile
	bool is_edge_tile(Tile* temp) const {
		// Note: Edge NOT Corner
		if (is_corner_tile(temp)) return false;
		else if (row_index(temp) == 0 || column_index(temp) == 0 ||
			column_index(temp) == width - 1 || row_index(temp) == height - 1) return true;
		else return false;
	}

	// EFFECTS: Returns pointer to tile given row and column
	Tile* Tile_at(int row, int col) {
		return board[row * width + col];
	}

	// EFFECTS: Given row and column indexes, returns tile index
	// Example, if board is 10x10:
	// Row 0 Col 0 = Tile 0
	// Row 0 Col 5 = Tile 5
	// Row 9 Col 9 = Tile 99
	int tile_index(int row, int col) {
		return row * width + col;
	}

	// EFFECTS: Returns row index given pointer to tile
	// Note: row_index function is O(n) (this is super slow)
	int row_index(Tile* tile_ptr) const {

		Tile** temp = board_start;
		while (*temp != tile_ptr) ++temp;

		int board_index = temp - board_start;
		int row = (board_index - column_index(tile_ptr)) / width;
		return row; 
	}

	// EFFECTS: Returns column index given pointer to tile
	// Note: column_index function is O(n) (this is super slow)
	int column_index(Tile* tile_ptr) const {
		Tile** temp = board_start;
		while (*temp != tile_ptr) ++temp;

		int board_index = temp - board_start;
		int column = board_index % width;
		return column;
	}
};


// Main function
int main(int argc, char** argv) {
	
	try {
		// Arguements: exec, width, height, number of mines

		// If argc == 1, then use default constructor
		if (argc == 1) {
			Minesweeper_Game inst1;
			inst1.run_game();
		}
		// If argc == 3, then use second custom constuctor
		else if (argc == 3) {
			int width = atoi(argv[1]);
			int height = atoi(argv[2]);
			Minesweeper_Game inst2(width, height);
			inst2.run_game();
		}
		// If argc == 4, then use third custom constructor
		else if (argc == 4) {
			int width = atoi(argv[1]);
			int height = atoi(argv[2]);
			int number_of_mines = atoi(argv[3]);
			Minesweeper_Game inst3(width, height, number_of_mines);
			inst3.run_game();
		}
		else {
			Invalid_Arguements error_instance1;
			throw error_instance1;
		}
	}
	catch (const Invalid_Arguements &obj3) {
			cout << "Invalid arguements" << endl;
	}
}