#define WALL_HIGH 2.5


void draw_wall_y_side(void);
void draw_wall_X_side(void);

void draw_wall_x_side() {
    glBegin(GL_POLYGON);
        glVertex3f(0,0,0);
        glNormal3f(0,1,0);
        glVertex3f(0,0,WALL_HIGH);
        glVertex3f(1,0,WALL_HIGH);
        glVertex3f(1,0,0);
    glEnd();
}

void draw_wall_y_side() {    
    glBegin(GL_POLYGON);
        glVertex3f(0,0,0);
        glNormal3f(1,0,0);
        glVertex3f(0,0,WALL_HIGH);
        glVertex3f(0,1,WALL_HIGH);
        glVertex3f(0,1,0);
    glEnd();                     
}

