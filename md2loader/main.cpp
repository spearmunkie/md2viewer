#include<iostream>
#include "MD2.h"

using namespace std;

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP     16

// This is our SDL surface
SDL_Surface *surface;
MD2Model myModel,model2;
GLfloat lX,lY,lZ;
// function to release/destroy our resources and restoring the old desktop
void Quit( int returnCode )
{
    SDL_Quit( );
    exit( returnCode );
}

int resizeWindow( int width, int height )
{
    GLfloat ratio;
    if ( height == 0 )
        height = 1;
    ratio = ( GLfloat )width / ( GLfloat )height;
    glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    gluPerspective(60.0f, ratio, 0.1f, 200.0f );
    glMatrixMode( GL_MODELVIEW );

    glLoadIdentity( );

    /* Initialize OpenGL context */
    glEnable( GL_TEXTURE_2D );
    glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
    glShadeModel (GL_SMOOTH);

    glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
        model2.loadTexture("tris.bmp");
    myModel.loadTexture("weapon.bmp");
    return( TRUE );
}

/* function to handle key press events */
void handleKeyPress( SDL_keysym *keysym )
{
    switch ( keysym->sym )
	{
	case SDLK_ESCAPE:
	    /* ESC key was pressed */
	    Quit( 0 );
	    break;
	case SDLK_F1:
	    /* F1 key was pressed
	     * this toggles fullscreen mode
	     */
	    SDL_WM_ToggleFullScreen( surface );
	    break;
	default:
	    break;
	}

    return;
}

/* general OpenGL initialization function */
int initGL( GLvoid )
{

    GLfloat lightpos[] = { 5.0f, 10.0f, 0.0f, 1.0f };
	lX=0.0;lY=0.0;lZ=0.0;
    /* Initialize OpenGL context */
    glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
    glShadeModel (GL_SMOOTH);

    glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);

    glLightfv (GL_LIGHT0, GL_POSITION, lightpos);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-40,40,-40,40,-40,40);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    if (myModel.ReadMD2Model("weapon.md2"))
    if (model2.ReadMD2Model("tris.md2"))
        return( TRUE );
    return false;
}

/* Here goes our drawing code */
int drawGLScene( GLvoid )
{
    static int n = 0; /* The current frame */
    static float interp = 0.0;
    static float interp2 = 0.0;
    static double curent_time = 0;
    static double last_time = 0;


    /* Clear The Screen And The Depth Buffer */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


    /* Move Left 1.5 Units And Into The Screen 6.0 */

    glRotatef(lX,1.0,0.0,0.0);
	glRotatef(lY,0.0,1.0,0.0);
	glRotatef(lZ,0.0,0.0,1.0);

    last_time = curent_time;
    curent_time = (double)SDL_GetTicks() / 1000.0;

    interp += 10 * (curent_time - last_time);
    interp2 += 10 * (curent_time - last_time);

    myModel.Animate(0,myModel.getEnd()-1,interp);
    model2.Animate(0,model2.getEnd()-1,interp2);

    glTranslatef (0.0f, 0.0f, -100.0f);
    glRotatef (-90.0f, 1.0, 0.0, 0.0);
    glRotatef (-90.0f, 0.0, 0.0, 1.0);

    myModel.draw(interp);
    model2.draw(interp2);

    /* Draw it to the screen */
    SDL_GL_SwapBuffers( );
    return( TRUE );
}

int main( int argc, char **argv )
{

    /* Flags to pass to SDL_SetVideoMode */
    int videoFlags;
    /* main loop variable */
    int done = FALSE;
    /* used to collect events */
    SDL_Event event;
    /* this holds some info about our display */
    const SDL_VideoInfo *videoInfo;
    /* whether or not the window is active */
    int isActive = TRUE;

    /* initialize SDL */
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
	    fprintf( stderr, "Video initialization failed: %s\n",
		     SDL_GetError( ) );
	    Quit( 1 );
	}

    /* Fetch the video info */
    videoInfo = SDL_GetVideoInfo( );

    if ( !videoInfo )
	{
	    fprintf( stderr, "Video query failed: %s\n",
		     SDL_GetError( ) );
	    Quit( 1 );
	}

    /* the flags to pass to SDL_SetVideoMode */
    videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
    videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
    videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */

    /* This checks to see if surfaces can be stored in memory */
    if ( videoInfo->hw_available )
	videoFlags |= SDL_HWSURFACE;
    else
	videoFlags |= SDL_SWSURFACE;

    /* This checks if hardware blits can be done */
    if ( videoInfo->blit_hw )
	videoFlags |= SDL_HWACCEL;

    /* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    /* get a SDL surface */
    surface = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
				videoFlags );

    /* Verify there is a surface */
    if ( !surface )
	{
	    fprintf( stderr,  "Video mode set failed: %s\n", SDL_GetError( ) );
	    Quit( 1 );
	}

    /* initialize OpenGL */
    initGL( );

    /* resize the initial window */
    resizeWindow( SCREEN_WIDTH, SCREEN_HEIGHT );

    /* wait for events */
    while ( !done )
	{
	    /* handle the events in the queue */

	    while ( SDL_PollEvent( &event ) )
		{
		    switch( event.type )
			{
			case SDL_ACTIVEEVENT:
			    /* Something's happend with our focus
			     * If we lost focus or we are iconified, we
			     * shouldn't draw the screen
			     */
			    if ( event.active.gain == 0 )
				isActive = FALSE;
			    else
				isActive = TRUE;
			    break;
			case SDL_VIDEORESIZE:
			    /* handle resize event */
			    surface = SDL_SetVideoMode( event.resize.w,
							event.resize.h,
							16, videoFlags );
			    if ( !surface )
				{
				    fprintf( stderr, "Could not get a surface after resize: %s\n", SDL_GetError( ) );
				    Quit( 1 );
				}
			    resizeWindow( event.resize.w, event.resize.h );
			    break;
			case SDL_KEYDOWN:
			    /* handle key presses */
			    handleKeyPress( &event.key.keysym );
			    break;
			case SDL_QUIT:
			    /* handle quit requests */
			    done = TRUE;
			    break;
			default:
			    break;
			}
		}

	    /* draw the scene */
	    if ( isActive )
		drawGLScene( );
	}

    /* clean ourselves up and exit */
    Quit( 0 );

    /* Should never get here */
    return( 0 );
}
