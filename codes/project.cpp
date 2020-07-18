/**
* @file         project.cpp
* @brief       	The program that draws a 3D scene along with shadows.   
* @author       Gunpreet Kaur, Shreeya Nelekar and Sriram Kodey
* @include      OpenGL libraries <GL/glut.h> and <GL/gl.h> to implement OpenGL functions and custom header file "Components.h" for drawing objects in the scene.
*/
#include <GL/glut.h> 
#include <GL/gl.h>
#include<bits/stdc++.h>
#include "Components.h"

GLfloat lightPosition[4] = {0.0f,3.6f,-2.4f,1.0f};
GLfloat observerPosition[4] = {4.0f,6.0f,4.0f,1.0f};
int width = 800;
int height = 600;
GLdouble modelviewMatrixLight[16];
GLdouble modelviewMatrixObserver[16];
GLdouble projectionMatrix[16];
GLint viewport[4];
GLfloat* depthBufferLight = new GLfloat[width * height];
GLfloat* depthBufferObserver = new GLfloat[width * height];
void make_shadows();
void draw_scene();
void display();
void init();

/**
*\bFunction: main
*<br><br>
*\bBrief Description: driver function which sets the display mode, assigns the routine to be called while displaying,sets the window size and title and most importantly calls the glutMainLoop() function
*<br><br>
*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("scene with shadows");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
return 0;
}

/**
*\bFunction: init
*<br><br>
*\bBrief Description: function performs the required initializations.
*<br><br>
*/
void init()
{
	glShadeModel( GL_SMOOTH );
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//set background colour to black
    glEnable( GL_DEPTH_TEST );//do depth comparisons and update depth buffer,which has been initialized in glutInitDisplayMode,else no shadows will be formed, it is soul of z- buffer algo
    glEnable( GL_COLOR_MATERIAL );// enable color, else black and white image is formed;
    glEnable( GL_LIGHT0 );//enable light 0
    glEnable(GL_LIGHTING);// enable lighting dark and light patches depending on light's position wrt object
    glEnable(GL_BLEND);//blend the computed fragment color values with the values in the color buffers.
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}
/**
*\bFunction: display
*<br><br>
*\bBrief Description: function that performs following things-
*<br><br>
*\ba) renders scene from point of view of light by disabling colour, and stores z-buffer values. 
*<br><br>
*\bb) renders scene from point of view of observer.
*<br><br>
*\bc) calls the make_shadows function.
*/
void display()
{
	GLint buffer = GL_NONE;
    glGetIntegerv( GL_DRAW_BUFFER, &buffer );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );// clear the color and depth buffer

    glPushMatrix( );
    	glLightfv( GL_LIGHT0, GL_POSITION,lightPosition);//set the position of light 0
    	//switch to viewpoint of light
    	glPushMatrix();
        	glDrawBuffer( GL_NONE );// disable drawing into color buffer
        	glLoadIdentity();
	        	glMatrixMode( GL_PROJECTION );
	    		glLoadIdentity( );
	    		gluPerspective( 60.0, (GLfloat)width / (GLfloat)height, 1.0, 100.0 );
	    		glMatrixMode( GL_MODELVIEW );
	    		glLoadIdentity( );
	        	gluLookAt( lightPosition[0], lightPosition[1], lightPosition[2], 0, 0, 0, 0, 1, 0 );// set the camera to the viewpoint of the light
        	draw_scene( );
        	glReadPixels( 0, 0, width, height, GL_DEPTH_COMPONENT,GL_FLOAT,depthBufferLight);
        	glGetDoublev( GL_MODELVIEW_MATRIX, modelviewMatrixLight );
        	glDrawBuffer( (GLenum)buffer );
    	glPopMatrix( );
    	glClear( GL_DEPTH_BUFFER_BIT );//clear the depth buffer, from light's view point depth buffer was filled, which has been copied into depthBufferLight
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity( );
		gluPerspective( 60.0, (GLfloat)width/ (GLfloat)height, 1.0, 100.0 );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity( );
    	gluLookAt(observerPosition[0], observerPosition[1],observerPosition[2],0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );//observer
    		draw_scene( );
    		make_shadows( );
    glPopMatrix( );
    
    glFlush( );
    glutSwapBuffers( );
}
/**
*\bFunction: draw_scene
*<br><br>
*\bBrief Description: function that sets position,size and colour of various objects in the scene.
*<br><br>
*/	
void draw_scene()
{
	glRotatef(0,0,1,0); 
	glRotatef(0,1,0,0); 
	glScalef(1,1,1); 

    //light
    glPointSize(8);
    glBegin(GL_POINTS);
    glColor3f(1,1,0);
    glVertex4f(lightPosition[0],lightPosition[1],lightPosition[2],1.0f);
    glEnd();
    //other objects in the scene
	glPushMatrix();
		glEnable(GL_NORMALIZE);
		draw_axes();  
		draw_floor();  
		draw_wallframes();
	 	draw_allchairs();

		glPushMatrix();
			glTranslatef(2,-0.35,-2.2);
	  		draw_surface(0.8,1.2,0.5,0.2,0.2,0.2,0.2);
	  		glTranslatef(0,0.65,0);
	  		draw_surface(0.8,0.1,0.5,0.8,0.5,0.3,0.4);
		glPopMatrix();
	 	draw_eatables();
	glPopMatrix();
}
/**
*\bFunction: make_shadows
*<br><br>
*\bBrief Description: function that performs following things- 
*<br><br>
*\ba) extracts the modelview, project, and viewport matrices involved when viewed by the observer.
*<br><br>
*\bb) goes through every pixel(x,y) in frame buffer,gets depth(z) in the projected scene from observer's point of view
*<br><br>
*\bc) extracts world coordinates(before projection) from the object coordinates which have been extracted above in step b), using gluUnProject
*<br><br>
*\bd) transforms these world coordinates into light screen coordinates, using gluProject
*<br><br>
*\be) already saved z-buffer value while rendering scene from light's point of view is compared with z component of light screen coordinates obtained in step d). If at that point on screen, there is something ahead of it i.e. z-buffer value is less than screen z under consideration, that z on the screen is in shadow then, and it (x,y,z) as per observer's screen coordinates is then shaded accordingly.
*<br><br>
*/
void make_shadows()
{
    
    GLdouble objx, objy, objz,winx, winy, winz,depthObserver,depthLight;
    GLuint pixel = { 0x00000070};// color of pixels in shadow
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);//Specifies the byte alignment requirements for the start of each pixel row in memory

    //get observer's viewpoint, modelview, projection matrices
    glGetDoublev( GL_MODELVIEW_MATRIX,modelviewMatrixObserver );
    glGetDoublev( GL_PROJECTION_MATRIX,projectionMatrix);
    glGetIntegerv( GL_VIEWPORT, viewport );
    glReadPixels( 0, 0, width, height, GL_DEPTH_COMPONENT,GL_FLOAT, depthBufferObserver);

    // set the project matrix to orthographic
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
      glLoadIdentity();
      gluOrtho2D(0.0, (GLfloat)width, 0.0, (GLfloat)height);
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
		glLoadIdentity();
	    // go through every pixel in frame buffer
	    for( int y = 0; y < height; y++ )
	        for( int x = 0; x < width; x++ )
	        {
	            depthObserver = depthBufferObserver[ y * width + x ]; //depth at pixel. We will consider only what is visible to the observer at first place.
	            gluUnProject( x, y,depthObserver,modelviewMatrixObserver,projectionMatrix, viewport,&objx, &objy, &objz );// get world coordinates from x, y, depth
	            gluProject( objx, objy, objz, modelviewMatrixLight,projectionMatrix,viewport, &winx, &winy, &winz );//get light view screen coordinate and depth
	            
	            if(winx>=width||winy>=height||winx<0||winy<0)// make sure within the screen
	                continue;
	            depthLight= depthBufferLight[GLint(winy) *width + GLint(winx)];//get the depth value from the light

	            if( (winz-depthLight) >0.005)//checking if pixel is visible to the light source, as light source is at z=0 in the view volume.
	            {
	                glRasterPos2i( x, y );
	                glDrawPixels( 1, 1, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, &pixel );
	            }
	        }
      glPopMatrix();
	glMatrixMode(GL_PROJECTION);
    glPopMatrix();
 	glMatrixMode(GL_MODELVIEW);
}
	
    


  