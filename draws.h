static void draw_board() { 
    for (i = 0; i < 67; i++){
        for (j = 0; j < 67; j++){
            if (map_mat[i][j] == 2){
                printf("%d, %d\n",i,j);
                glTranslatef(i+1,j,2);
                    glColor3f(1, 0, 0);
                    glutSolidSphere(.2,10,10);
                glTranslatef(-i-1,-j,-2);
            }
        }
    }
    
}
