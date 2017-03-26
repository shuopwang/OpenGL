//
//  main.c
//  robot
//
//  Created by shuopeng.wang on 2017/3/4.
//  Copyright © 2017年 shuopeng.wang. All rights reserved.
//

#include <OpenGL/OpenGL.h>
#include<GLUT/GLUT.h>
#include <stdlib.h>
#include <stdio.h>

#define SOLID 1
#define WIRE 2
#define ROTATE_SPEED 1.0
#define G 9.8
#define B 0.8 // Damping
int moveX,moveY;
int spinX = 0;
int spinY = 0;
int robot_position_x=0;
int robot_position_z=-10;
int robot_position_y=0;
double distance_robot=0;
int flag=0;
int m_boutton = 0;
int flaglfoot=1;
double distance_left;
double distance_right;

int left_leg_angle=0;
int right_leg_angle=0;
int leg_angle=0;
int flagrfoot=0;
float left_foot_x=1,left_foot_y=-9.2,left_foot_z=-10;
float right_foot_x=-1,right_foot_y=-9.2,right_foot_z=-10;


double distance_wall=0;
double distance_feet=0;
int start=0;
float cam_pos_x = -40;
float cam_pos_z = -20;
float cam_look_x=0;
float cam_look_z=-10;

float up_down = 0.0, left_right = -1.57;

float vect_x = 0.0;
float vect_z = 0.0;

double dt = 0.0002;


float wall_z=15;

float sphere_x =0;
float sphere_y = -9.2;
float sphere_z = -5;

float sphere_ix = 0;
float sphere_iy = -9.2;
float sphere_iz = -5;

// Vitesse de la sphère : t & t-1
float sphere_vx;
float sphere_vy;
float sphere_vz;

float sphere_ivx = 0;
float sphere_ivy = 0;
float sphere_ivz = 0;

// accélération de la sphère
float sphere_ax;
float sphere_ay;
float sphere_az;

float sphere_iax = 0.0;
float sphere_iay = 0.0;
float sphere_iaz = 0.0;

GLfloat mat_specularGREEN[] ={0.633, 0.727811, 0.633,1.0};
GLfloat mat_ambientGREEN[] ={0.0215, 0.1745, 0.0215,1.0};
GLfloat mat_diffuseGREEN[] ={0.07568, 0.61424, 0.07568,1.0};
GLfloat mat_shininessGREEN[] ={128.0};

GLfloat mat_specularWHITE[] ={255.0,255.0,255.0,1.0};
GLfloat mat_ambientWHITE[] ={255.0,255.0,255.0,1.0};
GLfloat mat_diffuseWHITE[] ={255.0,255.0,255.0,1.0};
GLfloat mat_shininessWHITE[] ={128.0 * 0.4};

GLfloat mat_specularYELLOW[] ={0.75,0.75,0.75,1.0};
GLfloat mat_ambientYELLOW[] ={1,1,0,1.0};
GLfloat mat_diffuseYELLOW[] ={0.50,0.50,0.50,1.0};
GLfloat mat_shininessYELLOW[] ={128.0};

GLfloat wall_specular[]={0.75,0.75,0.75,1.0};
GLfloat wall_ambient[]={0.5,0.5,0.5,1.0};
GLfloat wall_diffuse[]={0.50,0.50,0.50,1.0};
GLfloat wall_shiniess[]={128.0*0.6};

GLfloat mat_specularGRAY[] ={0.75,0.75,0.75,1.0};
GLfloat mat_ambientGRAY[] ={0.5,0.5,0.5,1.0};
GLfloat mat_diffuseGRAY[] ={0.50,0.50,0.50,1.0};
GLfloat mat_shininessGRAY[] ={128.0 * 0.6};

GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 0.0};   /*  composante diffuse */
GLfloat light_position[] = {0.0,10.0,10.0, 0.0};  /*  position */
void init(int Width,int Height) {
    //定义光源的颜色和位置
    // Couleur d'effacement du buffer de couleur
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    // paramètrage du Z-buffer
    //glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    
    // Activation de l'éclairage
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    
    
    // Activation du lissage
    glShadeModel(GL_SMOOTH);
    
    // Projection perceptive
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
    
    glMatrixMode(GL_MODELVIEW);
}

//画球
void drawBall(double R, double x, double y,double z, int MODE) {
    glPushMatrix();
    glTranslated(x,y,z);
    if (MODE == SOLID) {
        glutSolidSphere(R,20,20);
    } else if (MODE ==WIRE) {
        glutWireSphere(R,20,20);
    }
    glPopMatrix();
}


//画长方体
void drawSkewed(double l, double w, double h, double x, double y, double z, int MODE) {
    glPushMatrix();
    glScaled(l, w, h);
    glTranslated(x, y, z);
    if (MODE == SOLID) {
        glutSolidCube(1);
    } else if (MODE ==WIRE) {
        glutWireCube(1);
    }
    glPopMatrix();
}

void SetMaterial(GLfloat spec[], GLfloat amb[], GLfloat diff[], GLfloat shin[])
{
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, shin);
    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
}

void Draw3DSGrid(){
    SetMaterial(mat_specularGREEN, mat_ambientGREEN, mat_diffuseGREEN, mat_shininessGREEN);
    float i;
    for( i = -100; i <= 100; i += 1)
    {
        // Start drawing some lines
        glBegin(GL_LINES);
        
        // Do the horizontal lines (along the X)
        glVertex3f(-50, -11.2, i);
        glVertex3f(50, -11.2, i);
        
        // Do the vertical lines (along the Z)
        glVertex3f(i, -11.2, -50);
        glVertex3f(i, -11.2, 50);
        
        // Stop drawing lines
        glEnd();
    }
}
void DrawRobot(){
    SetMaterial(mat_specularYELLOW,mat_ambientYELLOW,mat_diffuseYELLOW,mat_shininessYELLOW);
    glRotated(spinX, 0, 1, 0);
    glRotated(spinY, 1, 0, 0);
    glTranslated(robot_position_x, robot_position_y, robot_position_z);
    //head
    drawBall(2, 0, 1, 0, SOLID);
    //body
    drawSkewed(5, 4.4, 3, 0, -0.75, 0, SOLID);

    //arm
    glPushMatrix();
    glTranslated(-2, -2.1, 0);
    glRotated(45, 0, 0, 1);
    drawSkewed(1, 3, 1, 3.5, -1.3, 0, SOLID);
    drawBall(1, 3.5, -6.4, 0, SOLID);
    glPopMatrix();
    glPushMatrix();
    glTranslated(2, -2.1, 0);
    glRotated(-45, 0, 0, 1);
    drawSkewed(1, 3, 1, -3.5, -1.3, 0, SOLID);
    drawBall(1, -3.5, -6.4, 0, SOLID);
    glPopMatrix();
 //   glRotated(left_leg_angle, 1, 0, 0);

    glPushMatrix();
    //leg and feet
    glTranslated(1, -6.2, 0);
    glRotated(left_leg_angle, 1, 0, 0);
    glTranslated(0, -3, 0);
    glPushMatrix();
        glScaled(1.2, 6, 2);
        glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
    
//    glRotated(right_leg_angle, 1, 0, 0);
    glPushMatrix();
    glTranslated(-1, -6.2, 0);
    glRotated(right_leg_angle, 1, 0, 0);
    glTranslated(0, -3, 0);
    glPushMatrix();
        glScaled(1.2, 6, 2);
        glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
}
void DrawWall(){
    
    SetMaterial(wall_specular, wall_ambient,wall_diffuse, wall_shiniess);
    glTranslated(0,0,wall_z);
    glScaled(10, 5, 1);
    glutSolidCube(5);
    
    
}
void display(void) {

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);

    glLoadIdentity();

    gluLookAt(cam_pos_x, 0, cam_pos_z, cam_look_x, 0.0, cam_look_z, 0.0, 1.0, 0.0);
    
    glPushMatrix();
    Draw3DSGrid();
    glPopMatrix();
    
    glPushMatrix();
    SetMaterial(mat_specularWHITE, mat_ambientWHITE,mat_diffuseWHITE, mat_shininessWHITE);
    glTranslated(sphere_x,sphere_y ,sphere_z );
    glutSolidSphere(1.5, 50, 50);
    glPopMatrix();
    
    glPushMatrix();
    DrawRobot();
    glPopMatrix();
    
    glPushMatrix();
    DrawWall();
    glPopMatrix();
    
    glutPostRedisplay();
    glutSwapBuffers();
}
//鼠标点击事件
void mouseClick(int btn, int state, int x, int y) {
    moveX = x;
    moveY = y;
    m_boutton=btn;
   
    
}

void move_camera(double speed)
{
    vect_x = cam_look_x - cam_pos_x;
    vect_z = cam_look_z - cam_pos_z;
    
    cam_pos_x += vect_x * speed;
    cam_pos_z += vect_z * speed;
    
    cam_look_x += vect_x * speed;
    cam_look_z += vect_z * speed;
}


/* Gestion de l'orientation de la caméra */
void rotate_camera(double speed)
{
    vect_x = cam_look_x - cam_pos_x;
    vect_z = cam_look_z - cam_pos_z;
    
    // Calculate the sine and cosine of the angle once
    double cosTheta = cos(speed);
    double sinTheta = sin(speed);
    
    double new_x = sqrt(pow(vect_x,2) + pow(vect_z,2)) * cosTheta;
    double new_z = sqrt(pow(vect_x,2) + pow(vect_z,2)) * sinTheta;
    
    cam_look_x = cam_pos_x + new_x;
    cam_look_z = cam_pos_z + new_z;
}
void Special_key(int key, int x, int y)
{
    
    switch (key)
    {
        case GLUT_KEY_RIGHT:
            left_right+= 0.03;
            rotate_camera(left_right);
            break;
            
        case GLUT_KEY_LEFT:
            left_right-= 0.03;
            rotate_camera(left_right);
            break;
            
        case GLUT_KEY_UP:
            move_camera(0.02);
            break;
            
        case GLUT_KEY_DOWN:
            move_camera(-0.02);
            break;
            
        default:
            break;
    }
    
    glutPostRedisplay();
    glutSwapBuffers();
    
}
//键盘事件
void keyPressed(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
            robot_position_x +=1;
            left_foot_x+=1;
            right_foot_x+=1;
            break;
        case 'd':
            robot_position_x -=1;
            left_foot_x-=1;
            right_foot_x-=1;
            break;
        case 'w':
            robot_position_z += 1;
            left_foot_z=robot_position_z;
            right_foot_z=robot_position_z;
            break;
        case 's':
            left_foot_z=robot_position_z;
            right_foot_z=robot_position_z;
            robot_position_z -= 1;
            break;
        case 'q':
            spinX -= 2;
            break;
        case 'e':
            spinX += 2;
            break;
        case 'i':
            robot_position_y+=10;
    }
    glutPostRedisplay();
}
// 鼠标移动事件
void mouseMove(int x, int y) {
    switch (m_boutton) {
        case GLUT_LEFT_BUTTON:
            if(flaglfoot)
            {
                right_leg_angle+=2;
                if(right_leg_angle>=60)
                    {
                        flaglfoot=0;
                    }
                if (right_leg_angle>0)
                {
                    right_foot_y=right_foot_y+6*0.0004;
                    right_foot_z=right_foot_z-6*0.0348;
                }
                if (right_leg_angle<0)
                {
                    right_foot_y=right_foot_y-6*0.0004;
                    right_foot_z=right_foot_z-6*0.0348;
                }
                
            }
            else
                {
                    right_leg_angle-=2;
                    if(right_leg_angle<=-60)
                        flaglfoot=1;
                    if(right_leg_angle>0)
                        {
                            right_foot_y=right_foot_y-6*0.0004;
                            right_foot_z=right_foot_z+6*0.0348;
                        }
                    if (right_leg_angle<0)
                    {
                        right_foot_y=right_foot_y+6*0.0004;
                        right_foot_z=right_foot_z+6*0.0348;
                    }
                }
            break;
            
        case GLUT_RIGHT_BUTTON:
            if(flagrfoot)
            {
                left_leg_angle+=2;
                if(left_leg_angle>=45)
                    flagrfoot=0;
                if (left_leg_angle>0)
                {
                    
                    left_foot_y=left_foot_y+6*0.0004;
                    left_foot_z=left_foot_z-6*0.0348;
                }
                if (left_leg_angle<0)
                {
                    left_foot_y=left_foot_y-6*0.0004;
                    left_foot_z=left_foot_z-6*0.0348;
                }
            }
            else
            {
                left_leg_angle-=2;
                if(left_leg_angle<=-45)
                    flagrfoot=1;
                if(left_leg_angle>0)
                {
                    left_foot_y=left_foot_y-6*0.0004;
                    left_foot_z=left_foot_z+6*0.0348;
                }
                if (left_leg_angle<0)
                {
                    left_foot_y=left_foot_y+6*0.0004;
                    left_foot_z=left_foot_z+6*0.0348;
                }
            }
            break;
    }
    if(abs(right_leg_angle)>abs(left_leg_angle))
        leg_angle=abs(right_leg_angle);
    else
        leg_angle=abs(left_leg_angle);
        
  //  printf("the left foot (x,y,z): %f, %f, %f",left_foot_x,left_foot_y,left_foot_z);
  //  printf("the right foot(x,y,z): %f, %f, %f",right_foot_x,right_foot_y,right_foot_z);
 //printf("the leg angle: %d",leg_angle);
    glutPostRedisplay();
    moveX = x;
    moveY = y;
}

void reshape(int Width, int Height) {
    if (Height==0)
        Height=1;
    
    glViewport(0, 0, Width, Height);
    
    // Projection perceptive
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
    
    glMatrixMode(GL_MODELVIEW);
}

void  detect_collision(){
    if(sphere_y < -10)
    {
        sphere_y = sphere_iy;
        sphere_x=sphere_ix;
        sphere_z=sphere_iz;
        sphere_vy = - sphere_vy;
    }
    distance_right=sqrt(pow((sphere_x-right_foot_x), 2)+pow((sphere_z-right_foot_z),2));
    distance_left=sqrt(pow((sphere_x-left_foot_x), 2)+pow((sphere_z-left_foot_z),2));
    distance_wall=sqrt(pow((wall_z-5-sphere_z), 2));
    distance_robot=sqrt(pow((sphere_x-robot_position_x),2)+pow((sphere_z-robot_position_z), 2));
    if (distance_wall<1)
    {
        sphere_y = sphere_iy;
        sphere_x=sphere_ix;
        sphere_z=sphere_iz;
        sphere_vx=-sphere_vx;
        sphere_vy=sphere_vy-170;
        sphere_vz=-sphere_vz;
    }
    if(distance_robot<1.5)
    {
        if ((robot_position_y-sphere_y)>0)
        {
            sphere_y = sphere_iy;
            sphere_x=sphere_ix;
            sphere_z=sphere_iz;
            sphere_vx=-sphere_vx;
            sphere_vy=-sphere_vy;
            sphere_vz=-sphere_vz;
        }
    }
if(distance_right<1.5||distance_left<1.5)
{
    if ((sphere_y-right_foot_y)<9.2)
    {
        if(left_leg_angle<2&&right_leg_angle<2)
        {
        sphere_vx=-sphere_vx;
        sphere_vy=-sphere_vy;
        sphere_vz=-sphere_vz;
        }
        if (abs(left_leg_angle)>2||abs(right_leg_angle)>2)
        {
            sphere_vx=-sphere_vx;
            sphere_vy=-sphere_vy;
            sphere_vz=-sphere_vz-(leg_angle);
        }

        sphere_y = sphere_iy;
        sphere_x=sphere_ix;
        sphere_z=sphere_iz;
    }
}
    
    if(sphere_vx==0&&sphere_vy==0&&sphere_vz==0)
    {
        start=0;
    }
}

void idle_function(){
    if (start==0)
    {
        distance_left=sqrt(pow((sphere_x-left_foot_x), 2)+pow((sphere_y-left_foot_y), 2)+pow((sphere_z-left_foot_z),2));
        distance_right=sqrt(pow((sphere_x-right_foot_x), 2)+pow((sphere_y-right_foot_y), 2)+pow((sphere_z-right_foot_z),2));
        
        if (distance_left<3)
        {
            //printf("left decte collision");
            start=1;
            if (abs(leg_angle)<2)
            {   flag=1;
                sphere_ivx=0;
                sphere_ivy=0;
                sphere_ivz=80;
                sphere_iax=0;
                sphere_iay=0;
                sphere_iaz=0;
            }
            else{
                flag=2;
                sphere_ivx=0;
                sphere_ivy=10*(leg_angle);
                sphere_ivz=15*(leg_angle);
                sphere_iax = 0.0;
                sphere_iay = -G;
                sphere_iaz = 0.0;
            }
        }
        if (distance_right<3)
        {
            start=1;
            if (abs(leg_angle)<2)
            {   flag=1;
                sphere_ivx=0;
                sphere_ivy=0;
                sphere_ivz=60;
                sphere_iax=0;
                sphere_iay=0;
                sphere_iaz=0;
            }
            else{
                flag=2;
            sphere_ivx=0;
            sphere_ivy=10*(leg_angle);
            sphere_ivz=15*(leg_angle);
            sphere_iax = 0.0;
            sphere_iay = -G;
            sphere_iaz = 0.0;
            }
        }
        
    }
    if (start>0)
    ////////////////////////////////////
    // Numerical integration
    {
    sphere_vx = sphere_ivx + ( sphere_iax * dt );
    sphere_vy = sphere_ivy + ( sphere_iay * dt );
    sphere_vz = sphere_ivz + ( sphere_iaz * dt );
    
    sphere_x = sphere_ix + ( sphere_vx * dt );
    sphere_y = sphere_iy + ( sphere_vy * dt );
//        printf("the sphere_z %f",sphere_z);
    sphere_z = sphere_iz + ( sphere_vz * dt );
    
    ////////////////////////////////////
    // Collision test
        detect_collision();
    ////////////////////////////////////
    // Acceleration calculation: ajouter la viscosité
        if(flag==2){
            sphere_ax =      - (sphere_vx * B);
    
            sphere_ay =  - G - (sphere_vy * B);
    
            sphere_az =      - (sphere_vz * B);
        }
        
        if (flag==1){
            sphere_ax =      - (sphere_vx * B);
            
            sphere_ay =      - (sphere_vy * B);
            
            sphere_az =      - (sphere_vz * B);
        }
        
    
    ////////////////////////////////////
    // System update
    
    sphere_iax = sphere_ax;
    sphere_iay = sphere_ay;
    sphere_iaz = sphere_az;
    
    sphere_ivx = sphere_vx;
    sphere_ivy = sphere_vy;
    sphere_ivz = sphere_vz;
    
    sphere_ix = sphere_x;
    sphere_iy = sphere_y;
    sphere_iz = sphere_z;
      
    ////////////////////////////////////
    }
    if (robot_position_y>0)
        robot_position_y-=0.005;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init(640, 480);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMove);
    glutSpecialFunc(Special_key);
    glutKeyboardFunc(keyPressed);
    glutIdleFunc(idle_function);
    glutMainLoop();
    return 0;
}
