//header1

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h> 
#include <GL/gl.h>
#include<bits/stdc++.h>



using namespace std;

void draw_axes()
{
  glLineWidth(2.0); 
  glBegin(GL_LINES); 
    glColor3f(1,0,0);  
    glVertex3f(0,0,0); 
    glVertex3f(.15,0,0); 
    glColor3f(0,1,0);  
    glVertex3f(0,0,0); 
    glVertex3f(0,.15,0); 
    glColor3f(0,0,1); 
    glVertex3f(0,0,0); 
    glVertex3f(0,0,.15); 
  glEnd(); 
  glLineWidth(1.0); 
}
void draw_floor()
{
  glColor3f(.0,.8,0); 
  glPushMatrix(); 
  glScalef(5, 0.01, 5); 
  glBegin(GL_QUADS); 
  glNormal3f(-1,0,1); 
  glColor3d(0.87,0.721,0.5289); 
  glVertex3f(-0.5, 0, 0.5);
  glNormal3f(1,0,1); 
  glColor3d(0.87,0.721,0.5289); 
  glVertex3f(0.5, 0, 0.5);
  glNormal3f(1,0,-1);
  glColor3d(0.87,0.721,0.5289); 
  glVertex3f(0.5, 0 , -0.5);
  glNormal3f(-1, 0, -1);
 glColor3d(0.87,0.721,0.5289); 
  glVertex3f(-0.5, 0, -0.5);
  glEnd(); 
  // glutSolidCube(2.0);
  glPopMatrix(); 
}

//
//  draw a wall 
//
void draw_surface(float x,float y, float z, float c1, float c2, float c3, float c4)
{
  glColor3f(c1,c2,c3); 
  glPushMatrix(); 
  glScalef(x,y,z); 
  glutSolidCube(1.0);
  glPopMatrix(); 
}

// draw a table leg
void draw_leg(float xt, float yt, float zt,float sx,float sy,float sz)
{
  glPushMatrix();
  glTranslatef(xt, yt, zt); 
  glScalef(sx,sy,sz); 
  glutSolidCube(1.0); 
  glPopMatrix(); 
}
void draw_table(float sx,float sy,float sz,float c1, float c2, float c3, float c4,
				float legx, float legy,float legz ,float slegx,float slegy,float slegz) 
{
  glColor3f(c1,c2,c3); 

  glPushMatrix(); 
  glScalef(sx,sy,sz); 
  glutSolidCube(1.0); 
  glPopMatrix(); 

  draw_leg(legx,-legy, -legz,slegx,slegy,slegz); 
  draw_leg(legx,-legy, legz,slegx,slegy,slegz); 
  draw_leg(-legx,-legy, -legz,slegx,slegy,slegz); 
  draw_leg(-legx,-legy, legz,slegx,slegy,slegz); 
}
void draw_circle(float x1,float y1,float radius,float level, float r,float g,float b)
{
  //wrt table, adjusting x and z coordinates
  // x1=x1-1;
  // y1=y1-1;
	glColor3f(r,g,b);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x1,level,y1);
	float x2,y2,angle;
	for (angle=1.0f;angle<361.0f;angle+=0.2)
	{
	    x2 = x1+sin(angle)*radius;
	    y2 = y1+cos(angle)*radius;
	    glVertex3f(x2,level,y2);
	}
	glEnd();
}
void draw_plates(float x1,float y1,float radius,float level, float r,float g,float b){
    glColor3f(0.5,0.5,0); 
  GLUquadricObj *p = gluNewQuadric(); 
 // gluQuadricDrawStyle(p, GLU_LINE); 
  glPushMatrix(); 
  glTranslatef(x1,.06,y1);
  glRotatef(-90, 1, 0, 0); 
  glScalef(radius,radius,0.03);  
  gluCylinder(p, 1, 1.2, 1, 30, 30); 
  glPopMatrix(); 
    draw_circle(x1,y1,radius,level,r,g,b);
   
}
void draw_teapot() 
{
  glColor3f(0.6,0.2,0.2); 
  glutSolidTeapot(0.6); 
}

void draw_base(float sx, float sy, float sz, float c1, float c2, float c3,
				float up,float base, float height) 
{
	glColor3f(c1,c2,c3); 
  GLUquadricObj *p = gluNewQuadric(); 
 // gluQuadricDrawStyle(p, GLU_LINE); 
  glPushMatrix(); 
  glScalef(sx,sy,sz);  
  gluCylinder(p, up,base,height, 30, 30); 
  glPopMatrix(); 
}

void draw_doughnuts()
{
    glColor3f(1,1,0);
    glutSolidTorus(0.04,0.08,20,20);
}

void draw_chair(float sx,float sy,float sz,float c1, float c2, float c3, float c4,
				float legx, float legy,float legz ,float slegx,float slegy,float slegz, int direction){

	draw_table(sx,sy,sz,c1,c2,c3,c4,legx,legy,legz,slegx,slegy, slegz);
	if(direction==0){ //chair on right
		glPushMatrix();
			glRotatef(-95,0,0,1);
			glTranslatef(-sz/2,sx/2,0);
			draw_surface(sx,sy,sz,c1+0.2,c2,c3,c4);
			glTranslatef(0,-0.02,0);
			draw_surface(0.8*sx,0.8*sy,0.8*sz,c1,1,1,c4);
		glPopMatrix();

	}
	if(direction==1){ //chair on back
		glPushMatrix();
			glRotatef(95,1,0,0);
			glTranslatef(0,sz/2,-sx/2);
			draw_surface(sx,sy,sz,c1+0.2,c2,c3,c4);
			glTranslatef(0,-0.02,0);
			draw_surface(0.8*sx,0.8*sy,0.8*sz,c1,1,1,c4);
		glPopMatrix();

	}
	if(direction==2){ //chair on left
		glPushMatrix();
			glRotatef(95,0,0,1);
			glTranslatef(sz/2,sx/2,0);
			draw_surface(sx,sy,sz,c1+0.2,c2,c3,c4);
			glTranslatef(0,-0.02,0);
			draw_surface(0.8*sx,0.8*sy,0.8*sz,c1,1,1,c4);
		glPopMatrix();

	}
	if(direction==3){ //chair on front
		glPushMatrix();
			glRotatef(-95,1,0,0);
			glTranslatef(0,sz/2,sx/2);
			draw_surface(sx,sy,sz,c1+0.2,c2,c3,c4);
			glTranslatef(0,-0.02,0);
			draw_surface(0.8*sx,0.8*sy,0.8*sz,c1,1,1,c4);
		glPopMatrix();

	}
	
}
void draw_wallframes(){
	glPushMatrix(); 
	  glTranslatef(-2.5,2,0); 
	  glRotatef(90, 0, 0, 1); 
	  draw_surface(4, 0.01, 5,0.7,0,0,0); 
	  draw_surface(2, 0.03, 2.5,0,0,0,0);
	  draw_surface(1.8, 0.04, 2.3,1,1,1,1);
	  glPopMatrix();

	  glPushMatrix(); 
	  glTranslatef(0,2,-2.5); 
	  glRotatef(90, 1, 0, 0);
	  glRotatef(90, 0, 1, 0); 
	  draw_surface(4, 0.01, 5,.6,.6,1,1); 
	  glPushMatrix();
	  	glTranslatef(1.2,0,1.2);
	  	 draw_surface(1, 0.03,1.3,0,0,0,0);
	 	 draw_surface(0.8, 0.04, 1.1,1,1,1,1);
	 	glPopMatrix();
	 	draw_surface(1, 0.03,1.3,0,0,0,0);
	 	 draw_surface(0.8, 0.04, 1.1,1,1,1,1);
	 glPushMatrix();
	  	glTranslatef(-1.2,0,-1.2);
	  	 draw_surface(1, 0.03,1.3,0,0,0,0);
	 	 draw_surface(0.8, 0.04, 1.1,1,1,1,1);
	 	glPopMatrix();
	  glPopMatrix();

}
void draw_allchairs(){
	glPushMatrix(); 
  		glTranslatef(0, 1, 0);      
  		draw_table(2.5, 0.1, 2.5,.5,.5, .5,1,1.20,0.5,1.20,0.1, 1, 0.1);
  		glPushMatrix();
  			glTranslatef(1,-.4,0);
  			draw_chair(0.7,.08,0.7,1,0.5,0.5,0.5,.32,0.3,0.32,0.05,0.6,0.05,0);
  		glPopMatrix();

  		glPushMatrix();
  			glTranslatef(0,-.4,1);
  			draw_chair(0.7,.08,0.7,1,0.5,0.5,0.5,.32,0.3,0.32,0.05,0.6,0.05,1);
  		glPopMatrix();

  		glPushMatrix();
  			glTranslatef(-1,-.4,0);
  			draw_chair(0.7,.08,0.7,1,0.5,0.5,0.5,.32,0.3,0.32,0.05,0.6,0.05,2);
  		glPopMatrix();

  		glPushMatrix();
  			glTranslatef(0,-.4,-1);
  			draw_chair(0.7,.08,0.7,1,0.5,0.5,0.5,.32,0.3,0.32,0.05,0.6,0.05,3);
  		glPopMatrix();

}

void draw_eatables(){

// -----------------Drawing plates---------------------------
  		draw_plates(0.7,0,0.3,0.07,0.7,0.7,0.5); 
  		glPushMatrix();
  			glTranslatef(0.7,0.07,-0.32);
  			glPushMatrix();
  			glRotatef(-50,0,1,0);
  			glTranslatef(0.4,0,-0.25);
  			draw_circle(0,0.48,0.12,0.02, 1,1,1);
 			draw_circle(0,0.48,0.05,0.03, 1,1,0);
 			draw_circle(0,0.35,0.12,0.02, 1,1,1);
			draw_circle(0,0.35,0.05,0.03, 1,1,0);
			glPopMatrix();
 			glTranslatef(-0.15,0,0.3);
 		    glRotatef(-90, 1, 0, 0); 
		    draw_doughnuts();
		    glTranslatef(0.20,0.15,0);
		    draw_doughnuts();
      	glPopMatrix();

  		draw_plates(0,0.7,0.3,0.07, 0.7,0.7,0.5);
  		glPushMatrix();
  			glTranslatef(0,0.07,0.3);
  			draw_circle(0,0.48,0.12,0.02, 1,1,1);
 			draw_circle(0,0.48,0.05,0.03, 1,1,0);

			draw_circle(0,0.35,0.12,0.02, 1,1,1);
			draw_circle(0,0.35,0.05,0.03, 1,1,0);
  		glPopMatrix();

 //omlets
  		draw_plates(-0.2,-0.8,0.25,0.07, 0.2,0.2,0.2);
  		glPushMatrix();
  			glTranslatef(-0.2,0.07,-1.2);
  			draw_circle(0,0.48,0.12,0.02, 1,1,1);
 			draw_circle(0,0.48,0.05,0.03, 1,1,0);

			draw_circle(0,0.35,0.12,0.02, 1,1,1);
			draw_circle(0,0.35,0.05,0.03, 1,1,0);
  		glPopMatrix();

// square plates
  		glPushMatrix();
  			glTranslatef(0.18,0.07,-0.42);
  			glRotatef(-30,0,1,0);
  			draw_surface(0.3,0.01,0.6,0,05,05,1);

  			glPushMatrix();
  				glTranslatef(-0.15,0.03,0);
  				glRotatef(125,0,0,1);
  				draw_surface(0.05,0.01,0.6,1,1,1,1);
  			glPopMatrix();

  			glPushMatrix();
  				glTranslatef(0.17,0.03,0);
  				glRotatef(55,0,0,1);
  				draw_surface(0.05,0.01,0.6,1,1,1,1);
  			glPopMatrix();

  			glPushMatrix();
  				glTranslatef(0,0.03,-0.31);
  				glRotatef(55,1,0,0);
  				draw_surface(0.3,0.01,0.05,1,1,1,1);
  			glPopMatrix();

  			glPushMatrix();
  				glTranslatef(0,0.03,0.31);
  				glRotatef(125,1,0,0);
  				draw_surface(0.3,0.01,0.05,1,1,1,1);
  			glPopMatrix();

  			glPushMatrix();
     	        glRotatef(-90, 1, 0, 0); 
		        draw_doughnuts();
      		glPopMatrix();

      		glPushMatrix();
      			glTranslatef(0,0,0.18);
     	        glRotatef(-90, 1, 0, 0); 
		        draw_doughnuts();
      		glPopMatrix();

      		glPushMatrix();
      			glTranslatef(0,0,-0.18);
     	        glRotatef(-90, 1, 0, 0); 
		        draw_doughnuts();
      		glPopMatrix();
  		glPopMatrix();

 // draw cup saucer
  		draw_plates(0,0.1,0.13,0.07, 0.8,0.8,0.8);
  		glPushMatrix();
  			glTranslatef(0,0.07,0.1);
  			glRotatef(-90,1,0,0);
  			draw_base(0.08,.08,0.12,1,1,1,0.8,1,1);
  			glPushMatrix();
	  				glRotatef(90,1,0,0);
	  				draw_circle(0,0,.073,0.10,0.9,0.3,0.1); 
	  			glPopMatrix();
  		glPopMatrix();

  		draw_plates(-0.25,-0.1,0.13,0.07, 0.8,0.8,0.8);
  		glPushMatrix();
  			glTranslatef(-0.25,0.07,-0.10);
  			glRotatef(-90,1,0,0);
  			draw_base(0.08,.08,0.12,1,1,1,0.8,1,1);
  			glPushMatrix();
	  				glRotatef(90,1,0,0);
	  				draw_circle(0,0,.073,0.10,0.9,0.3,0.1); 
	  			glPopMatrix();
  		glPopMatrix();

//----------------------------------------------------------

// draw napkins
		glPushMatrix();
			glTranslatef(0.7,0.07,0.7);
			//draw glass
			glPushMatrix();
				glRotatef(-90,1,0,0);
				draw_base(0.08,.08,0.25,0.5,0.2,0.6,0.8,1,1);
				glPushMatrix();
	  				glRotatef(90,1,0,0);
	  				draw_circle(0,0,.0765,0.20,0.8,0.2,0.1); 
	  			glPopMatrix();
			glPopMatrix();

			draw_surface(0.36,0.01,0.36, 0.2,0.2,0.4,0.7);
			glPushMatrix();
				glTranslatef(-0.18,0,0);
				draw_surface(0.05,0.02,0.36,0.2,0.3,0.5,0.1); 
				glTranslatef(0.36,0,0);	
				draw_surface(0.05,0.02,0.36,0.2,0.3,0.5,0.1);
				glTranslatef(-0.18,0,-0.18);
				draw_surface(0.36,0.02,0.05,0.2,0.3,0.5,0.1);
				glTranslatef(0,0,0.36);
				draw_surface(0.36,0.02,0.05,0.2,0.3,0.5,0.1);
			glPopMatrix();	
			glTranslatef(0.1,0,0);
			glRotatef(-5,0,1,0);
			draw_surface(0.36,0.01,0.36, 0.2,0.2,0.4,0.7);
			glPushMatrix();
				glTranslatef(-0.18,0,0);
				draw_surface(0.05,0.02,0.36,0.2,0.3,0.5,0.1); 
				glTranslatef(0.36,0,0);	
				draw_surface(0.05,0.02,0.36,0.2,0.3,0.5,0.1);
				glTranslatef(-0.18,0,-0.18);
				draw_surface(0.36,0.02,0.05,0.2,0.3,0.5,0.1);
				glTranslatef(0,0,0.36);
				draw_surface(0.36,0.02,0.05,0.2,0.3,0.5,0.1);
			glPopMatrix();
  		glPopMatrix();

  		glPushMatrix();
			glTranslatef(0.7,0.07,-0.7);
			
			draw_surface(0.36,0.01,0.36, 0.2,0.2,0.4,0.7);
			glPushMatrix();
				glTranslatef(-0.18,0,0);
				draw_surface(0.05,0.02,0.36,0.2,0.3,0.5,0.1); 
				glTranslatef(0.36,0,0);	
				draw_surface(0.05,0.02,0.36,0.2,0.3,0.5,0.1);
				glTranslatef(-0.18,0,-0.18);
				draw_surface(0.36,0.02,0.05,0.2,0.3,0.5,0.1);
				glTranslatef(0,0,0.36);
				draw_surface(0.36,0.02,0.05,0.2,0.3,0.5,0.1);
			glPopMatrix();	
			glTranslatef(0.1,0,0);
			glRotatef(-5,0,1,0);
			draw_surface(0.36,0.01,0.36, 0.2,0.2,0.4,0.7);
			glPushMatrix();
				glTranslatef(-0.18,0,0);
				draw_surface(0.05,0.02,0.36,0.2,0.3,0.5,0.1); 
				glTranslatef(0.36,0,0);	
				draw_surface(0.05,0.02,0.36,0.2,0.3,0.5,0.1);
				glTranslatef(-0.18,0,-0.18);
				draw_surface(0.36,0.02,0.05,0.2,0.3,0.5,0.1);
				glTranslatef(0,0,0.36);
				draw_surface(0.36,0.02,0.05,0.2,0.3,0.5,0.1);
			glPopMatrix();
  		glPopMatrix();

  		glPushMatrix();
			glTranslatef(-0.8,0.07,0.7);
			glPushMatrix();
				glRotatef(-90,1,0,0);
				draw_base(0.08,.08,0.25,0.5,0.2,0.6,0.8,1,1);
				glPushMatrix();
	  				glRotatef(90,1,0,0);
	  				draw_circle(0,0,.0765,0.20,1,1,1); 
	  			glPopMatrix();
			glPopMatrix();
			draw_surface(0.36,0.01,0.36, 0.2,0.2,0.4,0.7);
			glPushMatrix();
				glTranslatef(-0.18,0,0);
				draw_surface(0.05,0.02,0.36,0.2,0.3,0.5,0.1); 
				glTranslatef(0.36,0,0);	
				draw_surface(0.05,0.02,0.36,0.2,0.3,0.5,0.1);
				glTranslatef(-0.18,0,-0.18);
				draw_surface(0.36,0.02,0.05,0.2,0.3,0.5,0.1);
				glTranslatef(0,0,0.36);
				draw_surface(0.36,0.02,0.05,0.2,0.3,0.5,0.1);
			glPopMatrix();	
			glTranslatef(0.1,0,0);
			glRotatef(-5,0,1,0);
			draw_surface(0.36,0.01,0.36, 0.2,0.2,0.4,0.7);
			glPushMatrix();
				glTranslatef(-0.18,0,0);
				draw_surface(0.05,0.02,0.36,0.2,0.3,0.5,0.1); 
				glTranslatef(0.36,0,0);	
				draw_surface(0.05,0.02,0.36,0.2,0.3,0.5,0.1);
				glTranslatef(-0.18,0,-0.18);
				draw_surface(0.36,0.02,0.05,0.2,0.3,0.5,0.1);
				glTranslatef(0,0,0.36);
				draw_surface(0.36,0.02,0.05,0.2,0.3,0.5,0.1);
			glPopMatrix();
  		glPopMatrix();

//draw glasses

//------------------Draw containers------------------------

  		glPushMatrix();          
			glRotatef(-90, 1, 0, 0); 
			glTranslatef(-1,0.5,0);   
			draw_base(0.15,0.15,0.25,0.5,0.5,0,1,1,1); 
			glPushMatrix();
	  			glRotatef(90,1,0,0);
	  			draw_circle(0,0,.15,0.25,0.5,0.5,0); 
	  			draw_circle(0,0,.15,0,0.5,0.5,0); 
	  		glPopMatrix();
			glTranslatef(0,0,.25);
			draw_base(0.10,0.10,0.03,1,0.5,0,1,1,1);  
			glPushMatrix();
	  			glRotatef(90,1,0,0);
	  			draw_circle(0,0,.10,0.03,0.5,0,0); 
	  		glPopMatrix();              
		glPopMatrix();

		glPushMatrix();          
			glRotatef(-90, 1, 0, 0); 
			glTranslatef(-0.6,0.5,0);   
			draw_base(0.15,0.15,0.25,0.5,0.5,0,1,1,1); 
			glPushMatrix();
	  			glRotatef(90,1,0,0);
	  			draw_circle(0,0,.15,0.25,0.5,0.5,0); 
	  			draw_circle(0,0,.15,0,0.5,0.5,0); 
	  		glPopMatrix();
			glTranslatef(0,0,.25);
			draw_base(0.10,0.10,0.03,1,0.5,0,1,1,1);  
			glPushMatrix();
	  			glRotatef(90,1,0,0);
	  			draw_circle(0,0,.10,0.03,0.5,0,0); 
	  		glPopMatrix();              
		glPopMatrix(); 

		glPushMatrix();          
			glRotatef(-90, 1, 0, 0); 
			glTranslatef(-0.8,0.2,0);   
			draw_base(0.15,0.15,0.25,0.5,0.5,0,1,1,1); 
			glPushMatrix();
	  			glRotatef(90,1,0,0);
	  			draw_circle(0,0,.15,0.25,0.5,0.5,0); 
	  			draw_circle(0,0,.15,0,0.5,0.5,0); 
	  		glPopMatrix();
			glTranslatef(0,0,.25);
			draw_base(0.10,0.10,0.03,1,0.5,0,1,1,1);  
			glPushMatrix();
	  			glRotatef(90,1,0,0);
	  			draw_circle(0,0,.10,0.03,0.5,0,0); 
	  		glPopMatrix();              
		glPopMatrix(); 
//------------------------------------------------------------------------------------

//--------Drawing Teapot--------------------------------------------------------
		glPushMatrix();
			glTranslatef(-0.5,0.24, 0.23);        // position a teapot at (0,0.2,0) relative to the table 
			glScalef(.4, .4, .4);         // the size of the teapot is 0.4x0.4x0.4
			draw_teapot();
		glPopMatrix(); 
//------------------------------------------------------------- 
	                
}
