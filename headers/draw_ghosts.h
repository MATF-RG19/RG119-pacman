#include <math.h>
#include <time.h>
#include <stdio.h>

/* changing ghost colors */
extern int activate_colors;
extern float time_activated;


extern int game_timer;

static int ghosts_look[3] = {90,180,-90};

// array of ghost colors R,G,B
int ghosts_color[3] = {1,2,3};




void draw_eyes(void);
void draw_eyebrows(void);
void draw_mouth(void);
void draw_ghost_face(void);
void draw_ghost(int x, int y, int c);

// draw ghost eyes
void draw_eyes() {
    glTranslatef(1.2,0.5,0.5);
        glColor3f(1, 1, 1);
        glutSolidSphere(.4,20,20);
        glTranslatef(0.3,0,0.1);
        glColor3f(0,0,0);
        glutSolidSphere(.15,10,10);
        glTranslatef(-0.3,0,-0.1);
        glTranslatef(0,-1,0);
            glColor3f(1, 1, 1);
            glutSolidSphere(0.4,20,20);
            glTranslatef(0.3,0,0.1);
                glColor3f(0,0,0);
                glutSolidSphere(.15,10,10);
        glTranslatef(-0.3,0,-0.1);
    glTranslatef(-1.2,0.5,-0.5);
}

void draw_eyebrows() {
    float eybrows_height = 0.9 + cos(game_timer/6)/20;
    glPushMatrix();
        glTranslatef(1.4,0.5,eybrows_height); 
        glRotatef(30,1,0,1);
        glColor3f(0.3,0,0.2);
        glScalef(2,7,1);
        glutSolidCube(0.1);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(1.4,-0.5,eybrows_height); 
        glRotatef(-30,1,0,1);
        glColor3f(0.3,0,0.2);
        glScalef(2,7,1);
        glutSolidCube(0.1);
    glPopMatrix();
}

void draw_mouth() {
    float mouth_height = -0.5 + sin(game_timer/6)/10;

        glPushMatrix();
        glRotatef(22,0,0,1);
        glPushMatrix();
            glTranslatef(2,-0.2,mouth_height); 
            glRotatef(50,1,0,0);
            glColor3f(0.3,0,0.2);
            glScalef(2,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(2,0.2,mouth_height); 
            glRotatef(-50,1,0,0);
            glColor3f(0.3,0,0.2);
            glScalef(2,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(2,-0.2,mouth_height); 
            glRotatef(50,1,0,0);
            glColor3f(0.3,0,0.2);
            glScalef(2,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(2,0.2,mouth_height); 
            glRotatef(-50,1,0,0);
            glColor3f(0.3,0,0.2);
            glScalef(2,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
        
        glPushMatrix();
        glRotatef(-22,0,0,1);
        glPushMatrix();
            glTranslatef(2,-0.2,mouth_height); 
            glRotatef(50,1,0,0);
            glColor3f(0.3,0,0.2);
            glScalef(2,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(2,0.2,mouth_height); 
            glRotatef(-50,1,0,0);
            glColor3f(0.3,0,0.2);
            glScalef(3,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
        glPopMatrix();
        
    glPopMatrix();
}

void draw_ghost_face() {
    glPushMatrix();
        draw_eyes();
        draw_eyebrows();
        draw_mouth();
    glPopMatrix();
}

// draw ghost body as cylinder
void draw_ghost_body() {
    float l = 1.6;
    float r = 1.7;
    float u, v;
        
    glPushMatrix();
        glTranslatef(0,0,-1.6);
        glRotatef(game_timer/2,0,0,1); 
        
        GLUquadric *quad = gluNewQuadric();
        gluCylinder(quad, r,  r,  l,  40,  40);
        int i = 1;
        glBegin(GL_TRIANGLE_STRIP);
        for(v = 0; v <= 2*PI + EPS; v += PI/10) {
            i++;
            glNormal3f(sin(v),cos(v),0);
            glVertex3f(r*sin(v),r*cos(v),0);
                
            glNormal3f(sin(v),cos(v),0);
            glVertex3f(r*sin(v),r*cos(v),-(i%2)-0);
        }
        glEnd();        
    glPopMatrix();  
}


void draw_ghost(int x,int y, int c) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    

    /* set ghost color: 1 = red, 2 = green, 3 = blue */
    switch (ghosts_color[c-1]){
        case 0 :
            glColor4f(0.8,0.8,1,0.9);
            break;
        case 1 :
            glColor4f(0.9,0,0,0.9); break;
        case 2 :
            glColor4f(0,0.9,0,0.9); break;
        case 3 :
            glColor4f(0,0,0.9,0.9); break;
    }

    
    /* if pacman ate big coin */
    if(time_activated <= 0) 
        activate_colors = 0;

    clock_t cl = clock();
    double r = cl;
    srand(cl);
    int activate_c = rand() % 2;

    if(activate_colors == 1 && activate_c == 1) {
        glColor4f(0.8, 0.7, 0.8, 0.9);
        time_activated-=0.05;
    }

    glPushMatrix();
        glTranslatef(x,y,2.4);
        glRotatef(ghosts_look[c-1],0,0,1);
        glPushMatrix();
            GLdouble plane0[] = {0,0,1,0};
            glEnable(GL_CLIP_PLANE0);
            glClipPlane(GL_CLIP_PLANE0, plane0);
            glutSolidSphere(1.7,40,40); 
            glDisable(GL_CLIP_PLANE0);
    glPopMatrix();
    
    draw_ghost_body();
    draw_ghost_face();
                
}

