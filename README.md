# QT_3d_mouse_ray-tracing
A gui without any opengl matrixes. 3d mouse ray tracing included

The red dot is the 3D intersection with the mouse. 

For ray tracing the mouse pointer is implemented as a line in the z axis direction.
Each of the 3d cube lines are calculated in time from 0 to 1, to get the points on the cube lines with a certain resolution.
This way of calculation is also needed for 3d splines. So the procedure is in line with more complex shapes.

Each calculated point of the cube within 0.2mm near the z direction mouse pointer line is a intersection.

Look at the code. It's written without opengl matixes. We don't need them !!

![1](https://raw.githubusercontent.com/grotius-cnc/QT_3d_mouse_ray-tracing/master/qt_opengl_pic.png)

Coded in Linux :
Qt Creator 4.8.2
Based on Qt 5.9.8 (GCC 5.3.1 20160406 (Red Hat 5.3.1-6), 64 bit)
Built on Mar 22 2019 
From revision 04aac85e26
