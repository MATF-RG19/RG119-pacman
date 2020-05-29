#include "draw_objects.h"
#include "stdio.h"
#include <math.h>
#include <time.h>

#include "image.h"


#define POINT_SIZE 1
#define LINE_WIDTH 2


/* look at point */
#define EYEX 60
#define EYEY 30
#define EYEZ 40

#define CENTERX 40
#define CENTERY 30
#define CENTERZ 0

#define UPX 0
#define UPY 0
#define UPZ 1


extern int width, height;


static char* FILENAME2 = "../floor.bmp"; 
static float matrix[16]; 
static GLuint names[3];


int position[2];
int next_position[2];
int wanted_direction[2];
int game_timer;
int rot_ply = 0;
float anim_param;
int on_going;
static int ghosts_dir[6] = {0,1,1,0,0,-1};

int ready = 1;

static int view_param;
 



static void on_keyboard(unsigned char key, int x, int y);
static void on_display(void);
static void on_reshape(int new_width, int new_height);
static void on_timer(int value);
static void init_game(void);
static void set_light_and_material(void);

void set_camera_and_view(void);
float min_distance_to_another(int ghost, int x, int y);
void move_ghost(int ghost);

void load_images(void);


void load_images() {
    Image * image;
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    image = image_init(0, 0);

    glGenTextures(3,names);
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

static void set_light_and_material(void) {
    GLfloat light_position[] = {-100, 100, 100, 0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    GLfloat ambient[] = { 0.2, 0.2, 0.2, 1 };
    GLfloat diffuse[] = { 0.8, 0.9, 0.8, 1 };
    GLfloat specular[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat ambient_coeff[] = { 0.5, 0.4, 0.3, 1 };
    GLfloat diffuse_coeff[] = { 1, 1, 1, 1 };
    GLfloat specular_coeff[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat shininess = 50;
       
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
        if (on_going == 0){ // Game intro
            on_going = 1;
            game_timer = 0;
        }
        if (on_going == -1 || on_going == 2){
            on_going = 1; // Start new game
            init_game();
        }
        break;
    case 'Q':
    case 'q':
        if (view_param < 20 && on_going == 1)
            view_param += 2; // Move camera up
        break;
    case 'E':
    case 'e':
        if (view_param > 0 && on_going == 1)
            view_param -= 2; // Move camera down
        break;
        
    }
    
    
    if (on_going == 1 && game_timer > 80){
        switch (key) {
        case 'A':
        case 'a':  
            // Go left
            wanted_direction[0] = 0;
            wanted_direction[1] = -1;
            break;
        case 'W':
        case 'w':   
            wanted_direction[0] = -1;
            wanted_direction[1] = 0;        
            // Go up
            break;
        case 'D':
        case 'd':   
            wanted_direction[0] = 0;
            wanted_direction[1] = 1;        
            // Go right
            break;
        case 'S':
        case 's':   
            wanted_direction[0] = 1;
            wanted_direction[1] = 0;        
            //Go down
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

    // Set floor texture
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
    

    if(on_going == 1) {
        set_pacman_moves(ready);

        if(position[1] < 1 ) 
            position[1] = 65;
        else if(position[1] > 65)
            position[1] = 1;
        
        if(game_timer > 100 && game_timer % 3 == 0)
            move_ghost(2);

        if(game_timer > 240 && game_timer % 3 == 0)
            move_ghost(1);
        
        if (game_timer > 300 && game_timer % 3 == 0)
            move_ghost(3);
    }
	
    //draw_axis();
    //draw_board();
    draw_ghost(ghosts_position[0],ghosts_position[1],1);
    draw_ghost(ghosts_position[2],ghosts_position[3],2); 
    draw_ghost(ghosts_position[4],ghosts_position[5],3);
    draw_wall_and_map();
    draw_pacman(rot_ply);

    int i = 0;
    
    for (; i < 3; i++) {
        if(sqrt(pow(position[0]-ghosts_position[i*2],2) + pow(position[1]-ghosts_position[i*2+1],2)) < 3){
            position[0] = ghosts_position[i*2];
            position[1] = ghosts_position[i*2+1];
            on_going = -1;
    }
        ghosts_look[i] = ghosts_dir[2*i+1] * 90;
        ghosts_look[i] = ghosts_dir[2*i] == -1 ? 180 : ghosts_look[i];
    }

    ready = 1;
 
    glutPostRedisplay();
    glutSwapBuffers();
}


static void on_reshape(int new_width, int new_height) {
    width = new_width;
    height = new_height;
    glViewport(0, 0, new_width, new_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) new_width / new_height, 1, 100);
}

static void init_game(void) {
    glClearColor(0.2, 0.3, 0.4, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_MULTISAMPLE);

    set_light_and_material();

    game_timer = 0;
    anim_param = 0;
    on_going = 1;
    view_param = 14;

    position[0] = 28;
    position[1] = 2;

    next_position[0] = 0;
    next_position[1] = 0;

    wanted_direction[0] = 0;
    wanted_direction[1] = 0;

    ghosts_position[0] = 28;
    ghosts_position[1] = 28;
    ghosts_position[2] = 28;
    ghosts_position[3] = 33;
    ghosts_position[4] = 28;
    ghosts_position[5] = 38;
    
    load_images();    
}

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
    
    if (board[x + 1][y] > 0 && min_distance_to_another(ghost, 1, 0) > 3){
        new_x[count]++;
        count++;
    } 
        
    if (board[x - 1][y] > 0 && min_distance_to_another(ghost, -1, 0) > 3){
        new_x[count]--;
        count++;
    }
    
    if (board[x][y + 1] > 0 && min_distance_to_another(ghost, 0, 1) > 3){
        new_y[count]++;
        count++;
    }
    
    if (board[x][y - 1] > 0 && min_distance_to_another(ghost, 0, -1) > 3 ){
        new_y[count]--;
        count++;
    }
        
    if (count == 0) {
        new_pos_x = x;
        new_pos_y = y;
    }
    else {
        clock_t cl = clock();
        double r = cl;
        srand( cl );
    
        int rand_coef = rand() % count;
        new_pos_x = x + new_x[rand_coef];
        new_pos_y = y + new_y[rand_coef];
    }
    
    if (count < 3 && board[x + ghosts_dir[ghost*2-2]][y + ghosts_dir[ghost*2-1]] > 0 
        && min_distance_to_another(ghost, ghosts_dir[ghost*2-2], ghosts_dir[ghost*2-1]) > 3 ){
        new_pos_x = x + ghosts_dir[ghost*2-2];
        new_pos_y = y + ghosts_dir[ghost*2-1];
        
    }
      else if (new_pos_x == x && new_pos_y == y) {
        return;
    }
    else {
        ghosts_dir[ghost*2-2] = new_pos_x - x;
        ghosts_dir[ghost*2-1] = new_pos_y - y;
    }
    
    ghosts_position[ghost*2-2] = new_pos_x;
    ghosts_position[ghost*2-1] = new_pos_y;
}

void set_camera_and_view() {

if (on_going == 1 && game_timer < 80)
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


