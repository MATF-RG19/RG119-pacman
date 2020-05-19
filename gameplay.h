#include "draw_objects.h"
#include "stdio.h"


#define POINT_SIZE 1
#define LINE_WIDTH 2


/* look at point */
#define EYEX 60
#define EYEY 30
#define EYEZ 30

#define CENTERX 30
#define CENTERY 20
#define CENTERZ 0

#define UPX 0
#define UPY 0
#define UPZ 1


extern int width, height;

int position[2];
int next_position[2];
int wanted_direction[2];
int game_timer;
int rot_ply = 0;
float anim_param;

static void on_keyboard(unsigned char key, int x, int y);
static void on_display(void);
static void on_reshape(int new_width, int new_height);
static void init_game(void);

 


static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:   
        exit(0);
        break;
    case 'A':
    case 'a':  
        //left
        wanted_direction[0] = 0;
        wanted_direction[1] = -1;
        break;
    case 'W':
    case 'w':   
        wanted_direction[0] = -1;
        wanted_direction[1] = 0;        
        //up
        break;
    case 'D':
    case 'd':   
        wanted_direction[0] = 0;
        wanted_direction[1] = 1;        
        //right
        break;
    case 'S':
    case 's':   
        wanted_direction[0] = 1;
        wanted_direction[1] = 0;     
        //down
        break;
    case 13:
        //start game
        break;
    }
    glutPostRedisplay();
}


static void on_display(void) {
    game_timer++;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(EYEX, EYEY, EYEZ, CENTERX, CENTERY, CENTERZ, UPX, UPY, UPZ);

        set_pacman_moves();
		draw_axis();
    	draw_board();
        draw_pacman(rot_ply);

    glutPostRedisplay();
    glutSwapBuffers();
}


static void on_reshape(int new_width, int new_height) {
    width = new_width;
    height = new_height;
    glViewport(0, 0, new_width, new_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) new_width / new_height, 0.1, 200);
}

static void init_game(void) {
    glClearColor(0.2, 0.3, 0.4, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_MULTISAMPLE);

    game_timer = 0;
    anim_param = 0;

    position[0] = 28;
    position[1] = 2;

    next_position[0] = 0;
    next_position[1] = 0;

    wanted_direction[0] = 0;
    wanted_direction[1] = 0;
    
    glPointSize(POINT_SIZE);
    glLineWidth(LINE_WIDTH); 
}






