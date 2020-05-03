#include "draw_objects.h"


#define POINT_SIZE 1
#define LINE_WIDTH 2


/* look at point */
#define EYEX 50
#define EYEY -50
#define EYEZ 50

#define CENTERX 0
#define CENTERY 0
#define CENTERZ 0

#define UPX 0
#define UPY 0
#define UPZ 1

extern int width, height;

static void on_display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(EYEX, EYEY, EYEZ, CENTERX, CENTERY, CENTERZ, UPX, UPY, UPZ);
    
		draw_axis();
    	draw_board();

    glutPostRedisplay();
    glutSwapBuffers();
}


static void on_keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27:   
        exit(0);
        break;
    }
    glutPostRedisplay();
}

static void on_reshape(int new_width, int new_height)
{
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
    
    glPointSize(POINT_SIZE);
    glLineWidth(LINE_WIDTH); 
}






