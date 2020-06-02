/* HEADER FILE WHICH CONTAINTS FUNCTIONS TO DRAW WALL */
#define WALL_HIGH 2.5
#define DIAMETER 1.0


void draw_wall_y_side(void);
void draw_wall_X_side(void);
void draw_pillar(int angle);

void draw_wall_x_side() {
    glPushMatrix();
        glTranslatef(0.5, 0, 1.1);
        glScalef(1, 0.4, WALL_HIGH);
        glutSolidCube(1);
    glPopMatrix();    
}

void draw_wall_y_side() {    
    glPushMatrix();
        glTranslatef(0, 0.5, 1.1);
        glScalef(0.4, 1, WALL_HIGH);
        glutSolidCube(1);
    glPopMatrix();                    
}

/* draw 25% of cylinder which represents roundend wall section 
input parameter describes z-axis rotation angle */
void draw_pillar(int angle) {
    GLdouble plane0[] = {0,-1,0,0};
    GLdouble plane1[] = {1,0,0,0};

    glPushMatrix();
    glRotatef(angle*90,0,0,1);
    GLUquadric *quad = gluNewQuadric();
        glEnable(GL_CLIP_PLANE0);
        glClipPlane(GL_CLIP_PLANE0, plane0);
        glEnable(GL_CLIP_PLANE0 + DIAMETER);
        glClipPlane(GL_CLIP_PLANE0 + DIAMETER, plane1);
        gluCylinder(quad, DIAMETER + 0.2,  DIAMETER + 0.2,  WALL_HIGH,  12,  1);
        gluQuadricOrientation(quad, GLU_INSIDE);
        gluCylinder(quad, DIAMETER - 0.2,  DIAMETER - 0.2,  WALL_HIGH,  12,  1);
        gluQuadricOrientation(quad, GLU_OUTSIDE);
        glPushMatrix();
            glTranslatef(0, 0, WALL_HIGH);
            gluDisk(quad, DIAMETER - 0.2, DIAMETER + 0.2, 12,1);
        glPopMatrix();
        glDisable(GL_CLIP_PLANE0 + DIAMETER);
        glDisable(GL_CLIP_PLANE0);
    glPopMatrix();
}


