#include "AI.h"
#include <iostream>
#include "conio.h"

void c_from_bar(Board* table_s, Player* player_AI, char AI_pawn_char)
{
	/*
		Checks whether or not we are able to take pawn out of the board
	*/
	AI1_insert_pawn_from_bar(table_s, player_AI, player_AI->dice1 + player_AI->dice2, AI_pawn_char);
	AI1_insert_pawn_from_bar(table_s, player_AI, player_AI->dice1, AI_pawn_char);
	AI1_insert_pawn_from_bar(table_s, player_AI, player_AI->dice2, AI_pawn_char);
}

bool c_if_home(Board* table_s, Player* player_AI, char AI_pawn_char, int& i, int& j, int& dice_sum)
{
	if (check_if_all_pawns_in_home(table_s, 'B'))
	{
		player_AI->can_take_pawns_from_the_board = true;
		AI1_take_pawns_off_the_board(table_s, player_AI, AI_pawn_char);
		if (player_AI->dice1 == 0 && player_AI->dice2 == 0) { return true; }
	}
	else
	{
		player_AI->can_take_pawns_from_the_board = false;
	}
	AI1_move_pawn(table_s, player_AI, i, j, dice_sum, AI_pawn_char);
	return false;
}

void loop_through_board(Board* table_s, Player* player_AI, char AI_pawn_char, int dice_sum)
{
	for (int i = NUMBER_OF_COLUMNS - 1; i >= 0; i--)
	{
		for (int j = NUMBER_OF_ROWS_IN_COLUMN - 1; j >= 0; j--)
		{
			c_from_bar(table_s, player_AI, AI_pawn_char);
			if (player_AI->dice1 == 0 && player_AI->dice2 == 0) { return; }

			// Update dice_sum variable
			dice_sum = player_AI->dice1 + player_AI->dice2;
			if (c_if_home(table_s, player_AI, AI_pawn_char, i, j, dice_sum))
				return;
		}
	}
}

// This function will make a move as an AI, if we are unable to make any move, than we return false
void AI1_make_move(Board* table_s, Player* player_AI)
{
	int dice1, dice2;
	get_random_numbers(&dice1, &dice2);
	int dice_sum = dice1 + dice2;
	player_AI->dice1 = dice1;
	player_AI->dice2 = dice2;

	const char AI_pawn_char = player_AI->pawn_char;
	loop_through_board(table_s, player_AI, AI_pawn_char, dice_sum);

}