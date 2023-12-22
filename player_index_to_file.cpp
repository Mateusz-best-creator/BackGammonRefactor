#include "functions_definitions.h"
#include "stdio.h"

void print_player_index_to_file(int index)
{
    FILE* file;

    // Open the file using fopen_s
    if (fopen_s(&file, "player_index_to_file.txt", "w") != 0) {
        return;
    }

    // Write an index to the file
    fprintf(file, "%d", index);

    // Close the file
    fclose(file);
}