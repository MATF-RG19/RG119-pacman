extern int game_timer;
extern int position[2];
extern int on_going;

static void draw_pacman(int rot) {   
    int mouth_angle = (game_timer % 20)*2 * on_going;

    glPushMatrix();
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

    glPopMatrix();

}
