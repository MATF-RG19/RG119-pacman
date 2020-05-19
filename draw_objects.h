#include "constants.h"

extern int position[2];
extern int next_position[2];
extern int wanted_direction[2];
extern int game_timer;
extern int rot_ply;
extern float anim_param;





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

/* iterate through board matrix(0, 1, 2)
2 = sphere to draw */
static void draw_board() { 
    int i, j;

    for (i = 0; i < 67; i++)
        for (j = 0; j < 67; j++)
            if (board[i][j] == 2)
                set_board_position(i, j);
    
}

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
        board[position[0]][position[1]] = 1;
    }
}


static void set_pacman_moves() {
    if (board[position[0] + wanted_direction[0]][position[1] + wanted_direction[1]] > 0)
        update_values(1);

    if(game_timer % 5 == 0 && board[position[0] + next_position[0]][position[1] + next_position[1]] > 0) 
        update_values(0);
}

static void draw_pacman(int rot)
{   
    int mouth_angle = (game_timer % 40);


    glTranslatef(position[0],position[1],2);
        
        glRotatef(90+rot,0,0,1);
        
        glRotatef(-mouth_angle,0,1,0); 
        
        GLUquadric *newQuad = gluNewQuadric();
        glColor3f(0.7, 0.6, 0);
        gluDisk(newQuad,0,1.4,32,4);

        glPushMatrix();
            GLdouble plane0[] = {0,0,1,0};
            glEnable(GL_CLIP_PLANE0);
            glClipPlane(GL_CLIP_PLANE0, plane0);
                glColor3f(0.8, 0.9, 0);
                glutSolidSphere(1.5,20,20); 
            glDisable(GL_CLIP_PLANE0);
        glPopMatrix();
        glPushMatrix();
            glRotatef(2*mouth_angle,0,1,0);
            glColor3f(0.7, 0.6, 0);
            gluDisk(newQuad,0,1.4,32,4); 
            GLdouble plane1[] = {0,0,-1,0};
            glEnable(GL_CLIP_PLANE0);
            glClipPlane(GL_CLIP_PLANE0, plane1);
                glColor3f(0.8, 0.9, 0);
                glutSolidSphere(1.5,20,20); 
            glDisable(GL_CLIP_PLANE0);
        glPopMatrix();
        
        // Eyes
        glTranslatef(1.1,0.4,0.5);
            glColor3f(1, 1, 1);
            glutSolidSphere(.35,20,20);
            glTranslatef(0.2,0,0.1);
                glColor3f(0,0,0);
                glutSolidSphere(.18,10,10);
            glTranslatef(-0.2,0,-0.1);
        glTranslatef(0,-0.8,0);
            glColor3f(1, 1, 1);
            glutSolidSphere(0.35,20,20);
            glTranslatef(0.2,0,0.1);
                glColor3f(0,0,0);
                glutSolidSphere(0.18,10,10);
            glTranslatef(-0.2,0,-0.1);
        glTranslatef(-1.1,0.4,-0.5);

    glTranslatef(-position[0],-position[1],-2);

}
