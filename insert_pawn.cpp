#include "functions_definitions.h"
#include "conio.h"
#include "board.h"

#include <iostream>
//nie zrobione
static int how_many_moves_can_make = 2;
static bool did_not_increment_moves = true;

void update_bar_pawns(char bar[BAR_SIZE], char player_sign, int pawns_on_bar)
{
	for (int i = 0; i < pawns_on_bar; ++i)
	{
		bar[i] = player_sign;
	}
}

bool insert_pawn(Board* table_s, int column_index, int row_index, const char player_sign,
	Player* player_1, Player* player_2, int start_column_index, bool& inserting_pawn_from_bar)
{
	int* dice1, * dice2;
	// Get the dice values
	if (player_sign == 'B')
	{
		dice1 = &player_1->dice1;
		dice2 = &player_1->dice2;
	}
	else
	{
		dice1 = &player_2->dice1;
		dice2 = &player_2->dice2;
	}

	if (*dice1 == *dice2 && did_not_increment_moves && *dice1 != 0 && *dice2 != 0)
	{
		how_many_moves_can_make = 4; did_not_increment_moves = false;
	}
	if (*dice1 != *dice2) { did_not_increment_moves = true; }

	// Check if we are able to make a move based on dice1 and dice2 values
	// We pass dice1 and dice2 by reference!
	int difference;
	if (inserting_pawn_from_bar)
	{
		if (player_sign == 'B')
		{
			difference = NUMBER_OF_COLUMNS - column_index;
			start_column_index = 24;
		}
		else
		{
			difference = column_index + 1;
			start_column_index = 0;
		}
	}
	else if (player_sign == 'B') { difference = start_column_index - column_index; }
	else { difference = column_index - start_column_index; }

	if ((column_index < start_column_index && player_sign == 'R' && !inserting_pawn_from_bar) ||
		(column_index > start_column_index && player_sign == 'B' && !inserting_pawn_from_bar))
	{
		gotoxy(TOP_LEFT_X_CORNER_COORDINATE, 31);
		std::cout << "You can't move back.";
		return false;
	}

	if (difference == 0)
	{
		// just continue
	}
	else if (difference != *dice1 + *dice2 && difference != *dice1 && difference != *dice2)
	{
		int dice_sum;
		if (how_many_moves_can_make == 3) { dice_sum = 3 * *dice1; }
		else if (how_many_moves_can_make == 4) { dice_sum = 4 * *dice2; }
		else { dice_sum = *dice1 + *dice2; }
		gotoxy(TOP_LEFT_X_CORNER_COORDINATE, 30);
		std::cout << "You cannot insert your pawn [" << player_sign << "] in " << column_index + 1 << " column.";
		gotoxy(TOP_LEFT_X_CORNER_COORDINATE, 31);
		std::cout << "You have " << dice_sum << " moves left";

		return false;
	}

	char enemy_player_sign = (player_sign == 'R') ? 'B' : 'R';

	int player_1_pawns_on_bar = 0;
	int player_2_pawns_on_bar = 0;

	for (int i = 0; i < BAR_SIZE; ++i)
	{
		if (table_s->player_1_bar[i] == 'B') { player_1_pawns_on_bar++; }
		if (table_s->player_2_bar[i] == 'R') { player_2_pawns_on_bar++; }
	}
	if (table_s->pawns[column_index][5] != 'E') { return false; }
	// Case when the entire column is empty
	if (table_s->pawns[column_index][0] == 'E')
	{
		table_s->pawns[column_index][0] = player_sign;
		// Check if all pawns of the player are in home, after inserting the pawn
		check_if_pawns_in_home(table_s, player_1, player_2, player_sign);
	}

	// zbicie piona
	else if (table_s->pawns[column_index][0] == enemy_player_sign && table_s->pawns[column_index][1] == 'E')
	{
		table_s->pawns[column_index][0] = player_sign;

		if (enemy_player_sign == 'R')
		{
			player_2_pawns_on_bar++;
			update_bar_pawns(table_s->player_2_bar, 'R', player_2_pawns_on_bar);
		}
		else
		{
			player_1_pawns_on_bar++;
			update_bar_pawns(table_s->player_1_bar, 'B', player_1_pawns_on_bar);
		}
		print_table(table_s); // Show the bar immediately

		// Check if all pawns of the player are in home, after inserting the pawn
		check_if_pawns_in_home(table_s, player_1, player_2, player_sign);
	}

	// Case when player insert the pawn (the same color)
	else if (table_s->pawns[column_index][0] == player_sign)
	{
		for (int i = 1; i < NUMBER_OF_ROWS_IN_COLUMN; ++i)
		{
			if (table_s->pawns[column_index][i] == 'E')
			{
				table_s->pawns[column_index][i] = player_sign;
				break;
			}
		}
		// Check if all pawns of the player are in home, after inserting the pawn
		check_if_pawns_in_home(table_s, player_1, player_2, player_sign);
	}

	// Case when player cannot insert the pawn
	else if (table_s->pawns[column_index][0] == enemy_player_sign)
	{
		gotoxy(TOP_LEFT_X_CORNER_COORDINATE, 30);
		std::cout << "You cannot insert your pawn [" << player_sign << "] in " << column_index + 1 << " column";
		return false; // false will indicate that we were unable to insert the pawn at given position
	}

	// Update dice values if we inserted the pawn properly
	if (difference != 0)
	{
		if (difference == *dice1)
		{
			if (how_many_moves_can_make > 2)
			{
				how_many_moves_can_make--;
			}
			else
			{
				*dice1 = 0;
			}
		}
		else if (difference == *dice2)
		{
			if (how_many_moves_can_make > 2)
			{
				how_many_moves_can_make--;
			}
			else
			{
				*dice2 = 0;
			}
		}
		else if (*dice1 + *dice2 == difference)
		{
			*dice1 = 0;
			*dice2 = 0;
			did_not_increment_moves = true;
		}
	}

	return true;
}