#include <math.h>

#include "draw_wall.h"
#include "constants.h"
#include "draw_pacman.h"
#include "draw_ghosts.h"

// switching ghost colors
int activate_colors = 0;
float time_activated = 20;

extern int position[2];
extern int next_position[2];
extern int wanted_direction[2];
extern int game_timer;
extern int rot_ply;
extern float anim_param;
extern int score;


static void update_values(int indicator);
static void set_pacman_moves(int ready);
void draw_wall_and_map(void);


static void set_pacman_moves(int ready) {
    // move player
    if (board[position[0] + wanted_direction[0]][position[1] + wanted_direction[1]] > 0)
        update_values(1);

    // update current position and score
    if(ready == 0 && board[position[0] + next_position[0]][position[1] + next_position[1]] > 0) 
        update_values(0);
}

/* move pacman and/or set new current position and score */
static void update_values(int indicator) {
    if(indicator) {
        next_position[0] = wanted_direction[0];
        next_position[1] = wanted_direction[1];

        rot_ply =- wanted_direction[0]*90;
        rot_ply += wanted_direction[1] == -1 ? 180:0;
    }
    else {
        position[0] += next_position[0];
        position[1] += next_position[1];
        if(board[position[0]][position[1]] == 2) score += 10; // small coin
        if(board[position[0]][position[1]] == 3) {score += 100; activate_colors = 1; time_activated = 20;} // big coin
        board[position[0]][position[1]] = 1;
    }
}
 
/* draw game board and coins using board definition from constants.h */
void draw_wall_and_map() {
    int i, j;
    for (i = 0; i < 67; i++){
        for (j = 0; j < 67; j++){
            float wave = sin(game_timer/5 + i + j)/5+1;  

            switch (board[i][j]){
            case 2 : // small coin
                glTranslatef(i,j,wave);
                    glColor3f(1, 1, 0.3);
                    glutSolidSphere(0.3 ,10 ,10);
                glTranslatef(-i,-j,-wave);
                break;
            case 3 : // big coin
                glTranslatef(i,j,2);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(0.5, 10, 10);
                glTranslatef(-i,-j,-2);
                break;
            /* roundend wall section */
            case 4 :
                glTranslatef(i+1,j+1,0);
                    glColor3f(0.1, 0.8, 0.9);
                    draw_pillar(3);
                glTranslatef(-i-1,-j-1,0);
                break;    
            case 5 :
                glTranslatef(i+1,j-1,0);
                    glColor3f(0.1, 0.8, 0.9);
                    draw_pillar(2);
                glTranslatef(-i-1,-j+1,0);
                break;
            case 6 :
                glTranslatef(i-1,j-1,0);
                    glColor3f(0.1, 0.8, 0.9);
                    draw_pillar(1);
                glTranslatef(-i+1,-j+1,0);
                break;
            case 7 :
                glTranslatef(i-1,j+1,0);
                    glColor3f(0.1, 0.8, 0.9);
                    draw_pillar(0);
                glTranslatef(-i+1,-j-1,0);
                break;    
            /* wall section */ 
            case 8 :
                glTranslatef(i,j,0);
                    glColor3f(0.1, 0.8, 0.9);
                    draw_wall_x_side();
                glTranslatef(-i,-j,0);
                break;
            case 9 : 
                glTranslatef(i,j,0);
                    glColor3f(0.1, 0.8, 0.9);
                    draw_wall_y_side();
                glTranslatef(-i,-j,0);
                break;      
            }
        }
    }    
}



