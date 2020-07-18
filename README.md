# Computer_Graphics-Shadows

## Team:

    Shreeya Bharat Nelekar - 2017A7PS0093H
        Department of Computer Science
    Gunpreet Kaur - 2017A7PS1573H
        Department of Computer Science
    Sriram Kodey - 2018A4PS0671H
        Department of Mechanical Engineering

## About the Project:

### Algorithm:

#### Z-Buffer visible surface algorithm

    The depth values i.e. z-values are stored for each pixel in the scene. While rendering the objects, their z-values are compared with the stored z-values to decide whether they are visible or hidden behind something with lesser z-value. The algorithm implicitly implements radix sorts in x and y. The z-values need not be sorted beforehand for all the objects. As the rendering goes on the z-values are compared while drawing the scene. For this purpose, double depth buffers are used. One buffer in which this all computation takes place and the other which is used for projection finally, by swapping the two buffers. This is done to avoid flickering effect while rendering the scene. So, any complex scene can be handled by the algorithm. As the key of the algorithm is based on “Comparison of z-values”.

#### Casting Shadows

    Shadows are cast by exploiting the z-values, which are being computed by the Z-buffer visible surface algorithm. As quoted in the paper, “Williams, L., "Casting Curved Shadows on Curved Surfaces", Computer Graphics, vol.12, no. 3, pp270-4, 1978”

    The algorithm is as follows:
        A view of the scene is constructed from the point of view of the light source. Only the Z values and not the shading values need be computed and stored.
        A view of the scene is then constructed from the point of view of the observer's eye. A linear transformation exists which maps X,Y,Z points the observer's view into X,Y,Z coordinates in the light source view.As each point is generated in the observer's view, it is transformed into the computed view in the light source space and tested for visibility to the light source before computing its shading value. If the point is not visible to the light source, it is in shadow and is shaded accordingly.

### Implementation:

The OpenGL libraries GL/glut.h and GL/gl.h have been used for implementation purposes.

Various functions used in sequence are as follows:

    glEnable( GL_DEPTH_TEST )

    do depth comparisons and update the depth buffer( The soul of Z-Buffer Visible Surface Algorithm )

    glEnable( GL_COLOR_MATERIAL )

    enable color, else black and white image is formed

    glEnable( GL_LIGHT0 )

    enable light 0 (maximum of 8 lights can be incorporated, as per GL library)

    glEnable(GL_LIGHTING)

    enable lighting dark and light patches depending on light's position wrt object

    glEnable(GL_BLEND) & glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA)

    blend the computed fragment color values with the values in the color buffers, this is used to blend the light gray [RGBA: 00000070] colour with the existing object colour to highlight and form a realistic shadow
    glLightfv(GLenum light,GLenum pname, const GLfloat *params)

#### To set parameters of light

    glLightfv( GL_LIGHT0, GL_POSITION, g_light_pos )-Set the position of light

    glDrawBuffer(GLenum buf)

    When colors are written to the frame buffer, they are written into the color buffers specified by glDrawBuffer.
    glDrawBuffer( GL_NONE )- disables drawing into color buffer, this is done while rendering the scene from light’s point of view, wherein the colours need not be filled as only depth values matter.
    glDrawPixels(GLsizei width,GLsizei height,GLenum format,GLenum type,const void * data) write a block of pixels to the frame buffer, for drawing shadows.

    glReadPixels( GLint x,GLint y,GLsizei width,GLsizei height,GLenum format,GLenum type,GLvoid *pixels)

    reads a block of pixels from the framebuffer into user defined data structure, both in case of light and observer. These filled in data structures are then used for rendering shadows by comparing z-values, and determining whether a point is not visible to the light or not. If it is visible to the light, shadow is cast.

    gluUnProject(GLdouble winx,GLdouble winy,GLdouble winz,const GLdouble modelMatrix[16],const GLdouble projMatrix[16],const GLint viewport[4],GLdouble *objx,GLdouble *objy,GLdouble *objz)

    maps window coordinates to object coordinates.

    winx- The x window coordinate to be mapped.

    objx- The computed x object coordinate.

This was used to get world coordinates from observer’s object coordinates.

    gluProject(GLdouble objx,GLdouble objy,GLdouble objz,const GLdouble modelMatrix[16],const GLdouble projMatrix[16],const GLint viewport[4],GLdouble *winx,GLdouble *winy,GLdouble *winz)

    transforms the specified object coordinates into window coordinates using modelMatrix, projMatrix, and viewport. The result is stored in winx, winy, and winz.

This was used to get light view screen coordinate and depth.

    glEnable(GL_NORMALIZE)

    This was very essential as scaling the objects as we draw them interferes with the lighting properties. Therefore, it is essential to normalize them afterwards.

### Results:

The shadow is cast only in the field of view of the light source. So, care must be taken for the field of view of the observer’s to be encompassed within the field of view of light. This is important as transformation between two image spaces is carried out to determine shadow. By default, the objects that lie outside the field of view of the light are illuminated. Shadow is formed only for objects lying in view-volume of light source.

<p align="center">
  <img src="https://github.com/Shreeya1699/Computer_Graphics-Shadows/blob/master/images/image2.png" width="200" title="The image shows the local axes. Red (X), Green(Y), Blue(Z).">
</p>


The light is black square on the wall, and shadows can be observed with respect to it.

a. From the observer’s point of view.
<p align="center">
  <img src="https://github.com/Shreeya1699/Computer_Graphics-Shadows/blob/master/images/image1.png" width="600" title="From the observer's point of view, whose eye is at (4,6,4) wrt to local axes as shown in the figure of coordinate axes above.">
</p>

b. From the light’s point of view.
<p align="center">
  <img src="https://github.com/Shreeya1699/Computer_Graphics-Shadows/blob/master/images/image3.png" width="600" title="From the light’s point of view. Hence, everything is illuminated in this picture and no shadows can be seen, as it is from light’s point of view. It is located at (0,3.6,-2.4) wrt to local axes as shown in the image of the coordinates above.">
</p>


### Self-Shadowing and how to avoid it-

When transformation is carried out from a point on the surface in the observer's view-space to the light source’s view-space, in ideal case, it should remain the part of the surface it was part of. But, due to imprecision of machine arithmetic and quantization of Z-buffer surfaces, it doesn’t exactly fall at a place it should have. So, a bias is added from the z-value after transforming it into the light space. This bias added ensures that points project in front of themselves, and thus do not shadow themselves. This problem is more evident in objects with curved surfaces.As a surface curves smoothly away from the light, however, it must ultimately shadow itself.A smooth surface shadowing itself in a shallow curve may switch from light to dark on the strength of a least significant bit in the Z-buffer. Worse, it may switch back and forth as the quantizing error beats with the sampling grid, producing a vivid moire. This is ensured by the function glReadPixels’s type field:

"GL_DEPTH_COMPONENT" - Depth values are read from the depth buffer. Each component is converted to floating point such that the minimum depth value maps to 0.0 and the maximum value maps to 1.0. Each component is then multiplied by GL_DEPTH_SCALE, added to GL_DEPTH_BIAS, and finally clamped to the range [0,1].

