#include "queue.h"
#include "tile_game.h"
#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

#define VISITED_CAPACITY 1000000


static struct linked_list qlist = { .head = NULL };

static int is_empty()
 {
    return (qlist.head == NULL);
}

void enqueue(struct queue *q, struct game_state state) 
{
    size_t code = serialize(state);

    insert_at_tail(&qlist, code);
}

struct game_state dequeue(struct queue *q) 
{
    size_t code = remove_from_head(&qlist);

    return deserialize(code);
}


static int is_solved(struct game_state state)
 {
    int counter = 1;
    for (int i = 0; i < 4; i++)
     {
        for (int j = 0; j < 4; j++) 
        {
            if (i == 3 && j == 3) 
            {
                if (state.tiles[i][j] != 0)
                    return 0;
            } 
            else 
            {
                if (state.tiles[i][j] != counter)
                    return 0;
                counter++;
            }
        }
    }
    return 1;
}


static void generate_moves(struct game_state current, struct game_state moves[], int *num_moves) 
{
    int dirs[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
    *num_moves = 0;

    for (int i = 0; i < 4; i++) 
    {
        int new_r = current.empty_row + dirs[i][0];
        int new_c = current.empty_col + dirs[i][1];

        if (new_r >= 0 && new_r < 4 && new_c >= 0 && new_c < 4) 
        {
            struct game_state next = current;
            uint8_t temp = next.tiles[new_r][new_c];
            next.tiles[new_r][new_c] = next.tiles[current.empty_row][current.empty_col];
            next.tiles[current.empty_row][current.empty_col] = temp;
            next.empty_row = new_r;
            next.empty_col = new_c;
            next.num_steps = current.num_steps + 1;
            moves[(*num_moves)++] = next;
        }
    }
}

static int visited_contains(size_t visited[], int visited_count, size_t code) 
{
    for (int i = 0; i < visited_count; i++)
     {
        if (visited[i] == code)
            return 1;
    }

    return 0;
}

int number_of_moves(struct game_state start)
 {
    if (qlist.head != NULL)
     {
        free_list(qlist);

        qlist.head = NULL;
    }
    
    size_t *visited = malloc(VISITED_CAPACITY * sizeof(size_t));

    if (!visited) 
    {
        fprintf(stderr, "Memory allocation failed for visited states\n");

        exit(EXIT_FAILURE);
    }

    int visited_count = 0;
    
    enqueue(NULL, start);
    size_t start_code = serialize(start);
    visited[visited_count++] = start_code;
    
    while (!is_empty())
     {
        struct game_state current = dequeue(NULL);
        
        if (is_solved(current)) 
        {
            free(visited);
            free_list(qlist);
            qlist.head = NULL;
            return current.num_steps;
        }
        
        struct game_state moves[4];  
        int num_moves = 0;
        generate_moves(current, moves, &num_moves);
        
        for (int i = 0; i < num_moves; i++) 
        {
            size_t code = serialize(moves[i]);
            if (!visited_contains(visited, visited_count, code))
             {
                if (visited_count < VISITED_CAPACITY) 
                {
                    visited[visited_count++] = code;
                }

                enqueue(NULL, moves[i]);
            }
        }
    }
    
    free(visited);
    
    return -1;
}
