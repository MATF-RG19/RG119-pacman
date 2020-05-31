#include "draw_wall.h"
#include "constants.h"
#include "draw_pacman.h"
#include "draw_ghosts.h"
#include <math.h>

extern int position[2];
extern int next_position[2];
extern int wanted_direction[2];
extern int game_timer;
extern int rot_ply;
extern float anim_param;
extern int score;

static void draw_line(int x, int y, int z, int height);
static void draw_axis(void);
static void update_values(int indicator);
static void set_pacman_moves(int ready);

void set_normal_and_vertex(float u, float v, float r);
void draw_pillar(float from, float to);
void draw_wall_and_map(void);

// function to draw axis
static void draw_line(int x, int y, int z, int length) {
    glBegin(GL_LINES);
        glColor3f(x,y,z);
        glVertex3f(0,0,0);
        glVertex3f(length * x, length * y, length * z);
    glEnd();
}

static void draw_axis() {
    int length = 100;

    draw_line(1, 0, 0, length);
    draw_line(0, 1, 0, length);
    draw_line(0, 0, 1, length);
}


/* PACMAN MOVES SECTION */

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
        if(board[position[0]][position[1]] == 2) score += 10;
        if(board[position[0]][position[1]] == 3) score += 100;

        board[position[0]][position[1]] = 1;
    }
}

static void set_pacman_moves(int ready) {
    if (board[position[0] + wanted_direction[0]][position[1] + wanted_direction[1]] > 0)
        update_values(1);

    if(ready == 0 && board[position[0] + next_position[0]][position[1] + next_position[1]] > 0) 
        update_values(0);
}

/* WALL AND MAP SECTION */

void set_normal_and_vertex(float u, float v, float r) {
    glNormal3f(sin(v),cos(v),0);
    glVertex3f(r*sin(v),r*cos(v),u);
}

void draw_pillar(float from, float to) {
    float l = 2.2;
    float r = 1;
    float u, v;
    glPushMatrix();
    for (u = 0; u < l-.35; u += 0.1) {
        glBegin(GL_TRIANGLE_STRIP);
        for (v = from; v <= to + EPS; v += PI / 20) {
            set_normal_and_vertex(u, v, r);
            set_normal_and_vertex(u + PI / 20, v,r);
        }
        glEnd();
    }
    glPopMatrix();
}

void draw_wall_and_map() {
    
    int i, j;
    
    for (i = 0; i < 67; i++){
        for (j = 0; j < 67; j++){
            
            float wave = sin(game_timer/5 + i + j)/5+1;
            
            switch (board[i][j]){
            case 2 :
                glTranslatef(i,j,wave);
                    glColor3f(1, 1, 0.3);
                    glutSolidSphere(.3,10,10);
                glTranslatef(-i,-j,-wave);
                break;
                
            case 3 :
                glTranslatef(i,j,2);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(.5,10,10);
                glTranslatef(-i,-j,-2);
                break;
                
            case 9 :
                glTranslatef(i,j,0);
                    glColor3f(0.2, 0, 0.8);
                    draw_wall_y_side();
                glTranslatef(-i,-j,0);
                break;
            case 8 :
                glTranslatef(i,j,0);
                    glColor3f(0.2, 0, 0.8);
                    draw_wall_x_side();
                glTranslatef(-i,-j,0);
                break;
            
            case 7 :
                glTranslatef(i-1,j+1,0);
                    glColor3f(0.2, 0, 0.8);
                    draw_pillar(PI/2,PI);
                glTranslatef(-i+1,-j-1,0);
                break;
            case 6 :
                glTranslatef(i-1,j-1,0);
                    glColor3f(0.2, 0, 0.8);
                    draw_pillar(0,PI/2);
                glTranslatef(-i+1,-j+1,0);
                break;
            case 5 :
                glTranslatef(i+1,j-1,0);
                    glColor3f(0.2, 0, 0.8);
                    draw_pillar(3*PI/2, 2*PI);
                glTranslatef(-i-1,-j+1,0);
                break;    
            case 4 :
                glTranslatef(i+1,j+1,0);
                    glColor3f(0.2, 0, 0.8);
                    draw_pillar(PI,3*PI/2);
                glTranslatef(-i-1,-j-1,0);
                break;
                
            }
        }
    }
    
    
}



