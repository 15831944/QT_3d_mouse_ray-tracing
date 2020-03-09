#include "opengl.h"
#include <math.h>
#include <QToolTip>
#include <QKeyEvent>
#include <QCursor>
#include <QString>
#include <mainwindow.h>
#include <iostream>
#include <variable.h>
#include <matrix_3d.h>

using namespace std;
double aspect;

#define myqDebug() qDebug() << fixed << qSetRealNumberPrecision(4)

opengl::opengl(QWidget *parent)
    : QOpenGLWidget(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(20);
}

opengl::~opengl()
{

}

void opengl::initializeGL()
{
    //glut init for displaying solid cone,box etc.
    char *myargv [1];
    int myargc=1;
    glutInit(&myargc, myargv);

    aspect = double(this->size().width())/double(this->size().height());
}

void opengl::resizeGL(int w, int h)
{
    aspect = double(this->size().width())/double(this->size().height());
}

void opengl::paintGL()
{  
    //draw static origin
    glColor4f(1,0,0,1);//x axis
    glBegin(GL_LINES);
    glVertex3d(xorigin,yorigin*aspect,zorigin);
    glVertex3d(xorigin+0.1,yorigin*aspect,zorigin);
    glEnd();

    glColor4f(0,1,0,1);//y axis
    glBegin(GL_LINES);
    glVertex3d(xorigin,yorigin*aspect,zorigin);
    glVertex3d(xorigin,(yorigin+0.1)*aspect,zorigin);
    glEnd();

    glColor4f(0,0,1,1);//z axis
    glBegin(GL_LINES);
    glVertex3d(xorigin,yorigin*aspect,zorigin);
    glVertex3d(xorigin,yorigin*aspect,zorigin+0.1);
    glEnd();

    //line inputs feeding matrix in main view
    vector<line3d> line_array;
    //origin
    line_array.push_back({0,0,0,0.2,0,0,1,0,0,1});
    line_array.push_back({0,0,0,0,0.2,0,0,1,0,1});
    line_array.push_back({0,0,0,0,0,0.2,0,0,1,1});

    //cube front
    line_array.push_back({0,0,0,0.5,0,0,1,0,1,1});
    line_array.push_back({0.5,0,0,0.5,0.5,0,1,1,1,1});
    line_array.push_back({0.5,0.5,0,0,0.5,0,1,1,1,1});
    line_array.push_back({0,0.5,0,0,0,0,1,1,1,1});
    //cube back
    line_array.push_back({0,0,-0.5,0.5,0,-0.5,1,1,1,1});
    line_array.push_back({0.5,0,-0.5,0.5,0.5,-0.5,1,1,1,1});
    line_array.push_back({0.5,0.5,-0.5,0,0.5,-0.5,1,1,1,1});
    line_array.push_back({0,0.5,-0.5,0,0,-0.5,1,1,1,1});
    //cube connection lines front to back face
    line_array.push_back({0,0,0,0,0,-0.5,1,1,1,1});
    line_array.push_back({0.5,0,0,0.5,0,-0.5,1,1,1,1});
    line_array.push_back({0.5,0.5,0,0.5,0.5,-0.5,1,1,1,1});
    line_array.push_back({0,0.5,0,0,0.5,-0.5,1,1,1,1});

    //to catch the line id by mouse ray tracing, expand vector<line3d> with a id number..
    //i preferr to use a array[][] that loops in the main view. Each array item contains line id, line coordinates, colors, etc.
    //this is the easyest way. For splines you need to store the spline coordinates into a list and pass the whole list to the view.

    //point inputs feeding matrix in main view
    vector<point3d> point_array;
    point_array.push_back({0.25,0.25,-0.25});

    //main view
    for(int i=0; i<line_array.size(); i++){

        //translate
        mat4d ps_tr = translate_3d(line_array.at(i).xs,line_array.at(i).ys,line_array.at(i).zs,xmove,ymove,zmove);
        mat4d pe_tr = translate_3d(line_array.at(i).xe,line_array.at(i).ye,line_array.at(i).ze,xmove,ymove,zmove);

        //rotate
        double distxs=ps_tr.x-xorigin; //specify rotation base point
        double distys=ps_tr.y-yorigin;
        double distzs=ps_tr.z-zorigin;
        double distxe=pe_tr.x-xorigin;
        double distye=pe_tr.y-yorigin;
        double distze=pe_tr.z-zorigin;

        mat4d ps_ro = rotate_3d(distxs,distys,distzs,xrotate,yrotate,zrotate);
        mat4d pe_ro = rotate_3d(distxe,distye,distze,xrotate,yrotate,zrotate);

        ps_ro.x+=xorigin; //after rotation, bring points back to offset coordinates
        ps_ro.y+=yorigin;
        ps_ro.z+=zorigin;
        pe_ro.x+=xorigin;
        pe_ro.y+=yorigin;
        pe_ro.z+=zorigin;

        //scale
        mat4d ps_sc = scale_3d(ps_ro.x,ps_ro.y,ps_ro.z,xscale,yscale*aspect,zscale);
        mat4d pe_sc = scale_3d(pe_ro.x,pe_ro.y,pe_ro.z,xscale,yscale*aspect,zscale);

        glColor4f(line_array.at(i).r,line_array.at(i).g,line_array.at(i).b,line_array.at(i).a);
        glBegin(GL_LINES);
        glVertex3d(ps_sc.x,ps_sc.y,ps_sc.z);
        glVertex3d(pe_sc.x,pe_sc.y,pe_sc.z);
        glEnd();

        //check for mouse intersections with the lines.. ray tracing !!!
        for(double i=0; i<1; i+=0.01){
            point3d p = line_time(ps_sc.x,ps_sc.y,ps_sc.z,pe_sc.x,pe_sc.y,pe_sc.z,i);
            cout<<"px:"<<p.x<<endl;
            cout<<"py:"<<p.y<<endl;
            cout<<"pz:"<<p.z<<endl;

            //look for mouse - 3d line intersection
            point3d LineStart, LineEnd, Point;
            float Distance;

            LineStart.x=mx;
            LineStart.y=my;
            LineStart.z=-1;
            LineEnd.x=mx;
            LineEnd.y=my;
            LineEnd.z=1;
            Point.x=p.x;
            Point.y=p.y/aspect;
            Point.z=p.z;

            DistancePointLine(&Point, &LineStart, &LineEnd, &Distance);
            dist=Distance;
            if(Distance<0.01){
                //draw the point
                glPointSize(4);
                glColor4f(1,0,0,1);
                glBegin(GL_POINTS);
                glVertex3d(p.x,p.y,p.z);
                glEnd();
            }
        }
    }
    line_array.clear();

    for(int i=0; i<point_array.size(); i++){

        //translate..
        mat4d p_tr = translate_3d(point_array.at(i).x,point_array.at(i).y,point_array.at(i).z,xmove,ymove,zmove);

        //rotate..

        double distxs=p_tr.x-xorigin;
        double distys=p_tr.y-yorigin;
        double distzs=p_tr.z-zorigin;

        mat4d p_ro = rotate_3d(distxs,distys,distzs,xrotate,yrotate,zrotate);

        p_ro.x+=xorigin;
        p_ro.y+=yorigin;
        p_ro.z+=zorigin;

        //scale..
        mat4d p_sc = scale_3d(p_ro.x,p_ro.y,p_ro.z,xscale,yscale*aspect,zscale);

        //check if point is near the mouse line http://paulbourke.net/geometry/pointlineplane/source.c
        point3d LineStart, LineEnd, Point;
        float Distance;

        LineStart.x=mx;
        LineStart.y=my;
        LineStart.z=-1;
        LineEnd.x=mx;
        LineEnd.y=my;
        LineEnd.z=1;
        Point.x=p_sc.x;
        Point.y=p_sc.y/aspect;
        Point.z=p_sc.z;

        DistancePointLine(&Point, &LineStart, &LineEnd, &Distance);
        dist=Distance;
        if(Distance<0.01){
            glPointSize(8);
            glColor4f(1,0,0,1);//red
        } else {
            glPointSize(4);
            glColor4f(0,0,1,1);//blue
        }
        //draw the point
        glBegin(GL_POINTS);
        glVertex3d(p_sc.x,p_sc.y,p_sc.z);
        glEnd();
    }
    point_array.clear();

}

void opengl::wheelEvent(QWheelEvent *event)
{

}

void opengl::mouseMoveEvent(QMouseEvent *event)
{
    //true scaled mouse coordinates, will follow the scale matrix value's
    mx = (2.0f * event->x()) / this->width() - 1.0f;
    my = (1.0f - (2.0f * event->y()) / this->height())/aspect;
    mz = 0.0f;

    //preview mouse coordinates with a product scale off 1. (scale back to orginal coordinates)
    ui_mx=mx*(1/xscale);
    ui_my=my*(1/yscale);
    ui_mz=mz*(1/zscale);
}

void opengl::mousePressEvent(QMouseEvent *event)
{

}

void opengl::mouseReleaseEvent(QMouseEvent *event)
{

}

























