#include "constants.h"


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

/* function to set each spehere into valid board position */
static void set_board_position(int x, int y) {
    glTranslatef(x+1,y,2);
        glColor3f(0.5, 0.5, 0);
        glutSolidSphere(0.3,10,10);
    glTranslatef(-x-1,-y,-2);
}

static void draw_board() { 
    int i, j;

    for (i = 0; i < 67; i++)
        for (j = 0; j < 67; j++)
            if (board[i][j] == 2)
                set_board_position(i, j);
    
}
