# This Makefile was modified so that glut would work correctly on RedHat 9
# (there was an issue with glut not working properly on RedHat 9 and
# "-L/usr/X11R6/lib -lXi -lXmu" had to be added to the gcc line as a
# workaround). If you are trying to setup Mesa and glut for anything else then
# RedHat 9 try removing that part and seeing if it works without that.

project:project.c
	gcc project.c -o project -L/usr/X11R6/lib -lXi -lXmu -I/usr/local/include/GL/ /usr/lib/libGL.so.1 /usr/lib/libglut.so.3 /usr/lib/libGLU.so.1 -lm
