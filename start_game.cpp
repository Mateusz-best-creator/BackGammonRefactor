#include <iostream>

// Modules for sleep_for function
#include <thread>
#include <chrono>

// All header files
#include "functions_definitions.h"
#include "AI.h"
#include "conio.h"
#include "interfaces.h"
#include "board.h"

bool first_AI(Board* table, Player* player_1, Player* player_2, bool& first_time, int& player_index, bool& start_a_new_game)
{
	AI1_make_move(table, player_1);
	if (check_if_player_won(player_1))
	{
		start_a_new_game = true;
		return true;
	}
	if (player_index == 1) { player_index = 2; }
	else { player_index = 1; }
	// Print state of the game to the file
	write_to_file_visually(table->table, player_1, player_2);
	write_to_file(table, player_1, player_2);
	return false;
}

bool second_AI(Board* table, Player* player_1, Player* player_2, bool& first_time, int& player_index, bool& start_a_new_game)
{
	AI2_make_move(table, player_2);
	if (check_if_player_won(player_2))
	{
		start_a_new_game = true;
		return true;
	}
	if (player_index == 1) { player_index = 2; }
	else { player_index = 1; }
	// Print state of the game to the file
	write_to_file_visually(table->table, player_1, player_2);
	write_to_file(table, player_1, player_2);
	return false;
}

void both_AI(Board* table, Player* player_1, Player* player_2, bool& first_time, int& player_index)
{
	// Show the board for 2 seconds in case AI vs AI
	if (player_1->name == "AI1" && player_2->name == "AI2")
	{
		print_table(table);
		print_removed_pawns_interface(player_1, player_2);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	print_table(table);

	// Print who start the game
	if (first_time)
	{
		first_time = false;
		print_which_player_begins(player_index);
	}
}

void P_case(int& zn, Player* p1, Player* p2, Board* table, bool& f, int& i, bool&s,
	bool&stop_playing)
{
	if (zn == 'P')
	{
		// If player typed "P" we will start a game in a loop
		do
		{
			both_AI(table, p1, p2, f, i);

			if (i == 1 && p1->name == "AI1")
			{
				if (first_AI(table, p1, p2, f, i, s))
					break;
				if (!first_AI(table, p1, p2, f, i, s))
					continue;
			}

			// Second IA bot makes a move
			if (i == 2 && p2->name == "AI2")
			{
				if (second_AI(table, p1, p2, f, i, s))
					break;
				if (!second_AI(table, p1, p2, f, i, s))
					continue;
			}

			stop_playing = play(p1, p2, table, i, s);
			// Change player_index
			if (i == 1) { i = 2; }
			else { i = 1; }

			print_player_index_to_file(i);

			if (!stop_playing)
			{
				// Print state of the game to the file
				write_to_file_visually(table->table, p1, p2);
				write_to_file(table, p1, p2);
			}
		} while (!stop_playing);
	}
}

void C_case(int& zn, Board* table)
{
	if (zn == 'C')
	{
		table->color++;
		if (table->color > 15)
		{
			table->color = 0;
		}
	}
}

void game(Board* table, Player* player_1, Player* player_2, int& zn, bool& first_time, int& player_index,
	bool& start_a_new_game, bool& stop_playing, bool& go_to_database)
{
	do
	{
		print_table(table);
		print_top_interface(player_1, player_2, false);
		zn = getch();

		C_case(zn, table);

		P_case(zn,player_1, player_2, table, first_time, player_index, start_a_new_game, stop_playing);

		if (zn == 'D')
		{
			go_to_database = true;
			break;
		}

		if (start_a_new_game)
		{
			clrscr();
			break;
		}
		go_to_database = false;
	} while (zn != 'Q' && zn != 'q');
}

bool ending(Board* table, bool& start_a_new_game, PlayersDatabase* database,
	Player* player_1, Player* player_2, bool& read_from_file, bool& go_to_database)
{
	_setcursortype(_NORMALCURSOR);
	// After the game free the memory to avoid memory leaks
	delete table;


	// After everything check if we want to start a new game
	if (start_a_new_game)
	{
		start_game(database, player_1, player_2, read_from_file);
		return true;
	}

	if (go_to_database)
	{
		database_functionality(database);
		return true;
	}

	if (!start_a_new_game)
	{
		// After the game free the memory
		//delete[] database;
		return true;
	}
	return false;
}

void init_env(Player* player_1, Player* player_2, Board* table, bool& read_from_file)
{
	// We need to do some preprocessing before we start a game
	player_1->pawn_char = 'B';
	player_2->pawn_char = 'R';
	player_1->player_index = 1;
	player_2->player_index = 2;

	// In case we read from file
	if (read_from_file)
	{
		table = load_table_from_file(player_1, player_2);
	}
	else
	{
		initialize_table(table);
	}
	read_from_file = false;
	print_table(table);

	// Print initial state of the game
	write_to_file_visually(table->table, player_1, player_2);
	write_to_file(table, player_1, player_2);
}

void start_game(PlayersDatabase* database, Player* player_1, Player* player_2, bool& read_from_file)
{

	int zn = 0;

	// This variable will indicate if user entered [P] to play for the first time
	bool first_time = true;

	// Initialize the board with default pawn places
	Board* table = new Board();

	// This index will keep track which player now makes a move
	int player_index = 1;

	// This boolean value will indicate if we want to stop the game
	bool stop_playing = false;

	// This value will indicate if we want to start a new game
	bool start_a_new_game = false;

	// This boolean value will indicate whether or not we want to go to our database interface
	bool go_to_database;

	init_env(player_1, player_2, table, read_from_file);

	game(table, player_1, player_2, zn, first_time, player_index, start_a_new_game, stop_playing, go_to_database);
	
	if (ending(table, start_a_new_game, database, player_1, player_2, read_from_file, go_to_database))
		return;
}