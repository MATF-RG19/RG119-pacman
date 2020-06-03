/* THIS HEADER CONTAINTS GAME FUNCTIONALITY */

#include <math.h>
#include <time.h>

#include "draw_objects.h"
#include "stdio.h"
#include "image.h"

extern int width, height;

#define WIN_MESSAGE "WIN! PRESS ENTER TO PLAY AGAIN"
#define LOSE_MESSAGE "GAME OVER! PRESS ENTER TO TRY AGAIN"
#define GAME_WIN 3890

/* path to bmp files */
static char* FILENAME1 = "../textures/intro.bmp";
static char* FILENAME2 = "../textures/floor.bmp"; 

static float matrix[16];

// textures 
static GLuint names[3];

int position[2];
int next_position[2];
int wanted_direction[2];
int game_timer;
int rot_ply = 0;
float anim_param;
int on_going;
int ready = 1;
int score;

static int ghosts_dir[6] = {0,1,1,0,0,-1};
static int view_param;

/* define functions */ 
static void on_keyboard(unsigned char key, int x, int y);
static void on_display(void);
static void on_reshape(int new_width, int new_height);
static void on_timer(int value);
static void init_game(void);
static void set_light_and_material(void);

void text(char *t, int h, int w);
void set_camera_and_view(void);
float min_distance_to_another(int ghost, int x, int y);
void move_ghost(int ghost);
void load_images(void);
void set_textures(void);
void generate_text(int indicator);
static void draw_end_screen(void);

/*** COPIED FROM RG #7 ****/
void set_textures() {
    // intro game texture
    if(on_going == 0) {
        glEnable(GL_TEXTURE_2D); 
        glBindTexture(GL_TEXTURE_2D, names[1]);
        glBegin(GL_QUADS);
            glNormal3f(1,0,0);

            glTexCoord2f(0,0);
            glVertex3f(100,-16,-9);
            
            glTexCoord2f(1,0); 
            glVertex3f(100,16,-9);
            
            glTexCoord2f(1,1); 
            glVertex3f(100,16,9);
            
            glTexCoord2f(0,1); 
            glVertex3f(100,-16,9);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // floor texture
    glEnable(GL_TEXTURE_2D); 
    glBindTexture(GL_TEXTURE_2D, names[0]);
    glBegin(GL_QUADS);
        glNormal3f(0,0,1);
        
        glTexCoord2f(0,0);
        glVertex3f(0,0,0);
        
        glTexCoord2f(11,0); 
        glVertex3f(0,66,0);;
        
        glTexCoord2f(11,11); 
        glVertex3f(62,66,0);
        
        glTexCoord2f(0,11); 
        glVertex3f(62,0,0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}

/* display given text on the screen [h, w] */
void text(char* t, int h, int w) {   
    glDisable(GL_LIGHTING); 
    glColor3f(0, 0.8, 0.9);
    glPushMatrix();
        glMatrixMode(GL_PROJECTION); 
        GLdouble matrix[16];
        glGetDoublev(GL_PROJECTION_MATRIX, matrix);
        glLoadIdentity();
        glOrtho(0, width, 0, height, -3, 3);
        
        glMatrixMode(GL_MODELVIEW);
        
        glLoadIdentity();
        glRasterPos2f(h,w); 

        int i;
        for(i = 0; t[i]; i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int) t[i]); 
        }        
        glMatrixMode(GL_PROJECTION);        
        glLoadMatrixd(matrix); 
        glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void generate_text(int indicator) {
    char score_text[30] = " ";
    char message_text[60] = " ";


    if(indicator != 0) 
        sprintf(score_text, "SCORE: %d ", score);
    if(indicator == 2)
        sprintf(message_text, WIN_MESSAGE);
    if(indicator == -1)
        sprintf(message_text, LOSE_MESSAGE);

    text(message_text, 50, height-50);
    text(score_text, width-150, height-50);
}

/*** COPIED FROM RG #7 ****/
void load_images() {
    Image * image;
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    image = image_init(0, 0);

    glGenTextures(3,names);
    image_read(image, FILENAME1);
    glBindTexture(GL_TEXTURE_2D, names[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, 
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    image_read(image, FILENAME2);
    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 
                    0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);
    image_done(image);
}

/* set light and material */
static void set_light_and_material(void) {
    GLfloat light_position[] = {60, 80, 20, 0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    GLfloat ambient[] = {0.2, 0.2, 0.2, 1};
    GLfloat diffuse[] = {1, 1, 1, 1};
    GLfloat specular[] = {0, 0, 0, 1};
    GLfloat ambient_coeff[] = {0.1, 0.1, 0.1, 1};
    GLfloat diffuse_coeff[] = {0, 0, 0, 1};
    GLfloat specular_coeff[] = {1, 1, 1, 1};
    GLfloat shininess = 0;
       
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeff);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

/* BEGIN CALLBACK FUNCTIONS SECTION */
static void on_timer(int value) {
    if (value != TIMER_ID) return;
    game_timer++;
    glutPostRedisplay();
    
    ready = 0;
    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);   
}

static void on_keyboard(unsigned char key, int x, int y) {   
    switch (key) {
    case 27:   
        exit(0);
        break;
    case 13:
        if (on_going == 0){ // intro
            on_going = 1;
            game_timer = 0;
        }
        if (on_going == -1 || on_going == 2){
            on_going = 0; // new game
            init_game();
        }
        break;
    /* change camera view */
    case 'Q':
    case 'q':
        if (view_param < 20 && on_going == 1)
            view_param += 2; // UP
        break;
    case 'E':
    case 'e':
        if (view_param > 0 && on_going == 1)
            view_param -= 2; // DOWN
        break;
        
    }
    if (on_going == 1 && game_timer > 80){
        switch (key) {
        /* MOVE PLAYER */
        case 'A':
        case 'a':  
            // LEFT
            wanted_direction[0] = 0;
            wanted_direction[1] = -1;
            break;
        case 'W':
        case 'w':  
            // UP 
            wanted_direction[0] = -1;
            wanted_direction[1] = 0;        
            break;
            // RIGHT
        case 'D':
        case 'd':   
            wanted_direction[0] = 0;
            wanted_direction[1] = 1;        
            break;
        case 'S':
        case 's':   
            // DOWN
            wanted_direction[0] = 1;
            wanted_direction[1] = 0;        
            break;
        }
    }
    glutPostRedisplay();
}



static void on_display(void) {
    game_timer++;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    set_camera_and_view();
    set_textures();

    if(on_going == 1) {
        set_pacman_moves(ready);

        /* allow moving to the other side of the board */
        if(position[1] < 1 ) 
            position[1] = 65;
        else if(position[1] > 65)
            position[1] = 1;
        

        /* if game on, draw ghost one by one after some time */
        if(game_timer > 100 && game_timer % 3 == 0)
            move_ghost(2);

        if(game_timer > 240 && game_timer % 3 == 0)
            move_ghost(1);
        
        if (game_timer > 300 && game_timer % 3 == 0)
            move_ghost(3);
    }
	// set ghosts to right position
    draw_ghost(ghosts_position[0],ghosts_position[1],1);
    draw_ghost(ghosts_position[2],ghosts_position[3],2); 
    draw_ghost(ghosts_position[4],ghosts_position[5],3);

    draw_wall_and_map();
    draw_pacman(rot_ply);

    int i = 0;
    for(; i < 3; i++) {
        // check if pacman and ghost collide
        if(sqrt(pow(position[0]-ghosts_position[i*2],2) + pow(position[1]-ghosts_position[i*2+1],2)) < 3){
            position[0] = ghosts_position[i*2];
            position[1] = ghosts_position[i*2+1];
            on_going = -1;
    }   
        // turn ghost in walking mode
        ghosts_look[i] = ghosts_dir[2*i+1] * 90;
        ghosts_look[i] = ghosts_dir[2*i] == -1 ? 180 : ghosts_look[i];
    }

    if(score >= GAME_WIN)
        on_going = 2;

    generate_text(on_going);
    ready = 1;

    glutPostRedisplay();
    glutSwapBuffers();
}

void set_camera_and_view() {
    if(on_going == 1 && game_timer < 80)
        anim_param = game_timer;
    // Seting camera and view parametars    
    float x0 = on_going != 0 ? 84 - anim_param/3 + view_param : 115;
    float y0 = on_going != 0 ? 33 : 0;
    float z0 = on_going != 0 ? 26 + anim_param/4: 0;
    float x1 = on_going != 0 ? 35 : 0;
    float y1 = on_going != 0 ? 33 : 0;
    float z1 = on_going != 0 ? 14 - anim_param/4 + view_param/2 : 0;
    
    gluLookAt(x0, y0, z0, x1, y1, z1, 0, 0, 1);
}

static void on_reshape(int new_width, int new_height) {
    width = new_width;
    height = new_height;
    glViewport(0, 0, new_width, new_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) new_width / new_height, 1, 100);
}

/* END CALLBACK FUNCTIONS SECTION */


/* function to calculate minimum distance to another ghost using Manhattan */
float min_distance_to_another(int ghost, int x, int y) {
    int g_x = ghost - 1;
    int g1_x = (g_x + 1) % 3;
    int g2_x = (g_x + 2) % 3;
    
    float dist1 = abs(ghosts_position[g_x*2]-ghosts_position[g1_x*2]+x) + abs(ghosts_position[g_x*2+1]-ghosts_position[g1_x*2+1]+y);
    float dist2 = abs(ghosts_position[g_x*2]-ghosts_position[g2_x*2]+x) + abs(ghosts_position[g_x*2+1]-ghosts_position[g2_x*2+1]+y);
    
    return dist1 < dist2 ? dist1 : dist2;
}

void move_ghost(int ghost) {
    int x = ghosts_position[ghost*2-2];
    int y = ghosts_position[ghost*2-1];
    int count = 0;    
    int new_x[4] = {0,0,0,0};
    int new_y[4] = {0,0,0,0};
    int new_pos_x, new_pos_y;
    
    /* check all adjacent positions */
    if(board[x+1][y] > 0 && min_distance_to_another(ghost, 1, 0) > 3){
        new_x[count]++;
        count++;
    }    
    if(board[x-1][y] > 0 && min_distance_to_another(ghost, -1, 0) > 3){
        new_x[count]--;
        count++;
    }
    if(board[x][y+1] > 0 && min_distance_to_another(ghost, 0, 1) > 3){
        new_y[count]++;
        count++;
    }
    if(board[x][y-1] > 0 && min_distance_to_another(ghost, 0, -1) > 3 ){
        new_y[count]--;
        count++;
    }
    // if there are no available positions stay at the current one */
    if(count == 0) {
        new_pos_x = x;
        new_pos_y = y;
    }
    else { // generate new random position
        clock_t cl = clock();
        double r = cl;
        srand(cl);
    
        int rand_coef = rand() % count;
        new_pos_x = x + new_x[rand_coef];
        new_pos_y = y + new_y[rand_coef];
    }
    // if there is no barrier set new position
    if(count < 3 && board[x + ghosts_dir[ghost*2-2]][y + ghosts_dir[ghost*2-1]] > 0 
        && min_distance_to_another(ghost, ghosts_dir[ghost*2-2], ghosts_dir[ghost*2-1]) > 3 ){
        new_pos_x = x + ghosts_dir[ghost*2-2];
        new_pos_y = y + ghosts_dir[ghost*2-1];
        
    }
      else if(new_pos_x == x && new_pos_y == y) {
        return;
    }
    else { // change gost direction
        ghosts_dir[ghost*2-2] = new_pos_x - x;
        ghosts_dir[ghost*2-1] = new_pos_y - y;
    }
    // set new position if everything OK
    ghosts_position[ghost*2-2] = new_pos_x;
    ghosts_position[ghost*2-1] = new_pos_y;
}

/* initalize parameters for start */
static void init_game(void) {
    glClearColor(0.1, 0.1, 0.1, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_MULTISAMPLE);

    set_light_and_material();
    
    reset_board();
    game_timer = 0;
    anim_param = 0;
    on_going = 0;
    view_param = 14;
    score = -10;

    position[0] = 28;
    position[1] = 2;

    int i;
    for(i = 0;i<2;i++) {
    next_position[i] = 0;
    wanted_direction[i] = 0;
    }

    for(i = 0;i<3;i++)
        ghosts_position[i] = 28;

    ghosts_position[3] = 33;
    ghosts_position[4] = 28;
    ghosts_position[5] = 38;
    
    load_images();
}


static void draw_end_screen(){

    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, names[1]);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
  
        gluOrtho2D(0, width, 0, height);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex2f(0,0);

            glTexCoord2f(0, 1);
            glVertex2f(0, height);

            glTexCoord2f(1, 1);
            glVertex2f(width, height);

            glTexCoord2f(1, 0);
            glVertex2f(width, 0);
        glEnd();
        
        glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    glMatrixMode(GL_MODELVIEW);

    glDisable(GL_TEXTURE_2D);



}