//
//  main.cpp
//  grav
//
//  Created by XuFangzhou on 17/3/28.
//  Copyright © 2017年 XuFangzhou. All rights reserved.
//

#include <iostream>


#include <GLUT/glut.h>
#include <OpenGL/gl.h>
//#include <GL/glut.h>

#include <cstdio>

#include "gravity.hpp"

const int SamplingTimer = 1;
const int samplingrate = 40; //in milliseconds
const int iterationnum = 10;
const double PI = 3.14159265358979323846264338327950288419716939937510;

void drawSphere(double x, double y, double z, double r)
{/*
  double theta;
  glBegin(GL_LINE_LOOP);
  for (theta = 0; theta < 2 * PI; theta += 0.01)
		glVertex2f(x + r * cos(theta), y + r * sin(theta));
  glEnd();*/
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidSphere(r, 25, 25);
    glPopMatrix();
}
void drawBodies(BodyX *body, int n)
{
    int i;
    glColor3f(1.0, 1.0, 1.0);
    for (i = 0; i < n; i++)
    {
        drawSphere(body[i].c.x, body[i].c.y, body[i].c.z, body[i].size);
        //printf("draw (%lf,%lf)\n", body[i].c.x, body[i].c.y);
    }
}
void drawAxes()
{
    double axislength = 10.0;
    glBegin(GL_LINES);
    glVertex3f(-axislength, 0.0, 0.0);
    glVertex3f(axislength, 0.0, 0.0);
    glVertex3f(0.0, -axislength, 0.0);
    glVertex3f(0.0, axislength, 0.0);
    glVertex3f(0.0, 0.0, -axislength);
    glVertex3f(0.0, 0.0, axislength);
    glEnd();
}
void display();
void sample(int value)
{
    int i;
    //printf("sample\n");
    for (i = 0; i < iterationnum; i++)
        //iterateOcttree(body);
        iterate2(body);
    /*
     for (i = 0; i < n; i++)
     printf("body %d (%lf,%lf,%lf)\n", i, body[i].c.x, body[i].c.y, body[i].c.z);*/
    display();
    glutTimerFunc(samplingrate, sample, SamplingTimer);
}
double rotatex = 0.0, rotatey = 0.0;
const double rotatestep = 1;
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFrontFace(GL_CW);
    glPushMatrix();
    glRotatef(rotatex, 1.0, 0.0, 0.0);
    glRotatef(rotatey, 0.0, 1.0, 0.0);
    drawAxes();
    drawBodies(body, n);
    glPopMatrix();
    glFlush();
}
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-9.0, 9.0, -9.0, 9.0, -5.0, 5.0);
    display();
}
int px, py;
void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        px = x;
        py = y;
    }
}
void mousemove(int x, int y)
{
    int dx = x - px, dy = y - py;
    px = x;
    py = y;
    //if (button == GLUT_LEFT_BUTTON)
    {
        rotatex += dy * rotatestep;
        rotatey += dx * rotatestep;
        printf("rotate x=%lf, y=%lf\n", rotatex, rotatey);
    }
}

void Initial(void);
int main(int argc, const char * argv[]) {
    input(body);
//    glutInit(&argc, &argv);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Gravity");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutTimerFunc(samplingrate, sample, SamplingTimer);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mousemove);
    
    Initial();
    glutMainLoop();
    return 0;
}



void Initial(void)
{
    GLfloat mat_ambient[]={0.2f,0.2f,0.2f,1.0f};
    GLfloat mat_diffuse[]={0.8f,0.8f,0.8f,1.0f};
    GLfloat mat_specular[]={1.0f,1.0f,1.0f,1.0f};
    GLfloat mat_shininess[]={50.0f};
    GLfloat light0_diffuse[]={0.0f,0.0f,1.0f,1.0f};
    GLfloat light0_position[]={1.0f,1.0f,1.0f,0.0f};
    GLfloat light1_ambient[]={0.2f,0.2f,0.2f,1.0f};
    GLfloat light1_diffuse[]={1.0f,0.0f,0.0f,1.0f};
    GLfloat light1_specular[]={1.0f,0.6f,0.6f,1.0f};
    GLfloat light1_position[]={-2.0f,-2.0f,1.0f,0.0f};
    GLfloat spot_direction[]={1.0f,1.0f,-1.0f};           //定义材质属性
    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);      //指定材质的环境反射光反射系数
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);      //指定材质的漫反射光反射系数
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);    //指定材质的镜面反射光反射系数
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);  //指定材质的镜面发射指数值
    //light0为漫反射的蓝色点光源
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);     //指定漫反射光成分
    glLightfv(GL_LIGHT0,GL_POSITION,light0_position);   //设置光源的位置  //light1为红色聚光光源
    glLightfv(GL_LIGHT1,GL_AMBIENT,light1_ambient);     //指定环境光成分
    glLightfv(GL_LIGHT1,GL_DIFFUSE,light1_diffuse);     //指定漫反射光成分
    glLightfv(GL_LIGHT1,GL_SPECULAR,light1_specular);   //指定镜面光成分
    glLightfv(GL_LIGHT1,GL_POSITION,light1_position);   //指定光源位置
    glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,30.0);            //指定聚光截止角
    glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,spot_direction);//指定聚光灯的方向
    glEnable(GL_LIGHTING);  //启用光源
    glEnable(GL_LIGHT0);    //启用0号光源
    //glEnable(GL_LIGHT1);    //启用1号光源
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f,1.0f,1.0f,1.0f);    //
    glShadeModel(GL_FLAT); //设置明暗处理模式
}

