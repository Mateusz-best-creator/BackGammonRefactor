#include "AI.h"
#include "board.h"

void c_pawns_2(int& player_1_pawns_on_bar, int& player_AI_pawns_on_bar, Board* table_s)
{
	for (int i = 0; i < BAR_SIZE; ++i)
	{
		if (table_s->player_1_bar[i] == 'B') { player_1_pawns_on_bar++; }
		if (table_s->player_2_bar[i] == 'R') { player_AI_pawns_on_bar++; }
	}
}

bool e_col_2(Board* table_s, int i, int j, int dice_value, const char AI_pawn_char)
{
	// Empty column
	if (table_s->pawns[i + dice_value][0] == 'E')
	{
		table_s->pawns[i][j] = 'E';
		table_s->pawns[i + dice_value][0] = AI_pawn_char;
		return true;
	}
}

bool r_pawn_2(Board* table_s, int i, int j, int dice_value, char AI_pawn_char, int& player_1_pawns_on_bar)
{
	// Zbicie piona
	if (table_s->pawns[i + dice_value][0] == 'B' && table_s->pawns[i + dice_value][1] == 'E')
	{
		table_s->pawns[i][j] = 'E';
		table_s->pawns[i + dice_value][0] = AI_pawn_char;
		table_s->player_1_bar[player_1_pawns_on_bar] = 'B';
		player_1_pawns_on_bar++;
		print_table(table_s); // Show new bar immediately
		return true;
	}
}

bool i_pawn_2(Board* table_s, int i, int j, int dice_value, char AI_pawn_char)
{
	// Column is arleady occupied by AI pawns and we are able to insert the pawn
	if (table_s->pawns[i + dice_value][0] == AI_pawn_char &&
		table_s->pawns[i + dice_value][NUMBER_OF_ROWS_IN_COLUMN - 1] != AI_pawn_char) // Case when column is fully occupied
		{
			for (size_t k = 0; k < NUMBER_OF_ROWS_IN_COLUMN; ++k)
			{
				if (table_s->pawns[i + dice_value][k] == 'E')
				{
					table_s->pawns[i + dice_value][k] = 'R';
					table_s->pawns[i][j] = 'E';
					return true;
				}
			}
	}
}

bool AI2_check_insert_conditions(Board* table_s, Player* player_AI, int i, int j, const char AI_pawn_char, int dice_value)
{
	int player_1_pawns_on_bar = 0;
	int player_AI_pawns_on_bar = 0;

	c_pawns_2(player_1_pawns_on_bar, player_AI_pawns_on_bar, table_s);

	// Empty column
	if (e_col_2(table_s, i, j, dice_value, AI_pawn_char))
		return true;

	if (r_pawn_2(table_s, i, j, dice_value, AI_pawn_char, player_1_pawns_on_bar))
		return true;

	if (i_pawn_2(table_s, i, j, dice_value, AI_pawn_char))
		return true;

	return false;
}