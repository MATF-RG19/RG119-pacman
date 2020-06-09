#include <GL/glut.h>
// include game functionality header
#include "../headers/gameplay.h"

int width, height;


int main(int argc, char **argv)
{
	//setup window parameters
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(1920,1080);
    glutInitWindowPosition(32, 16);
    glutCreateWindow(argv[0]);
    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);

    // callback functions
    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_keyboard);
    glutDisplayFunc(on_display);
    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);   

    init_game();
    
    glutMainLoop();
    
    return 0;
}

