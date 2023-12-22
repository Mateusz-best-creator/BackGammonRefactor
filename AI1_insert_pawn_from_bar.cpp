#include "AI.h"
#include "conio.h"
#include "board.h"

bool check_dices(Player* player_AI)
{
	if (player_AI->dice1 == 0 && player_AI->dice2 == 0) { return true; }
}

void count_pawns(int& pawns_on_bar, Board* table_s, char AI_pawn_char)
{
	for (size_t i = 0; i < NUMBER_OF_ROWS_IN_COLUMN; ++i)
	{
		if (table_s->player_1_bar[i] == AI_pawn_char) { pawns_on_bar++; }
	}
}

void loop_through_columns(Board* table_s, int dice_value, char AI_pawn_char, Player* player_AI, int player_AI_pawns_on_bar)
{
	for (size_t i = 0; i < NUMBER_OF_ROWS_IN_COLUMN - 1; ++i)
	{
		if (table_s->pawns[NUMBER_OF_COLUMNS - dice_value][i] == 'E')
		{
			table_s->pawns[NUMBER_OF_COLUMNS - dice_value][i] = AI_pawn_char;
			if (dice_value == player_AI->dice1 + player_AI->dice2) { player_AI->dice1 = 0; player_AI->dice2 = 0; }
			else if (dice_value == player_AI->dice1) { player_AI->dice1 = 0; }
			else if (dice_value == player_AI->dice2) { player_AI->dice2 = 0; }
			table_s->player_1_bar[player_AI_pawns_on_bar - 1] = 'E';
			player_AI_pawns_on_bar--;
			break;
		}
	}
}

void move_to_the_board(Board* table_s, int player_AI_pawns_on_bar, int dice_value, Player* player_AI, char AI_pawn_char)
{
	if (player_AI_pawns_on_bar > 0)
	{
		if (table_s->pawns[NUMBER_OF_COLUMNS - dice_value][0] == 'E' ||
			(table_s->pawns[NUMBER_OF_COLUMNS - dice_value][0] == AI_pawn_char &&
				table_s->pawns[NUMBER_OF_COLUMNS - dice_value][NUMBER_OF_ROWS_IN_COLUMN - 1] != AI_pawn_char))
		{
			loop_through_columns(table_s, dice_value, AI_pawn_char, player_AI, player_AI_pawns_on_bar);
		}
		print_table(table_s);
	}
}

void AI1_insert_pawn_from_bar(Board* table_s, Player* player_AI, int dice_value, char AI_pawn_char)
{
	if (check_dices(player_AI)) { return; }
	int player_AI_pawns_on_bar = 0;
	count_pawns(player_AI_pawns_on_bar, table_s, AI_pawn_char);
	
	move_to_the_board(table_s, player_AI_pawns_on_bar, dice_value, player_AI, AI_pawn_char);
	// Moving the pawn from the bar to the board
}