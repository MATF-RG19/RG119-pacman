#define POINT_SIZE 1
#define LINE_WIDTH 2

extern int width, height;

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

static void on_display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(50, -10, 50, 0, 0, 0, 0, 0, 1);
    
		draw_axis();
    
    glutPostRedisplay();
    glutSwapBuffers();
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




