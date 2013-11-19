#include <GL/gl.h>
#include <GL/glu.h>
#include<GL/glext.h>
#include "SDL.h"
#include "SDL/SDL_opengl.h"
#include "SDL/SDL_image.h"
#include<fstream>
#include<iostream>
// magic number "IDP2" or 844121161
#define MD2_IDENT				(('2'<<24) + ('P'<<16) + ('D'<<8) + 'I')

// model version
#define	MD2_VERSION				8
using namespace std;

// MD2 header
struct MD2Header
{
    int ident;                  // magic number: "IDP2"
    int version;                // version: must be 8

    int skinwidth;              // texture width
    int skinheight;             // texture height

    int frameSize;              // size in bytes of a frame

    int numSkins;              // number of skins
    int numVertices;           // number of vertices per frame
    int numST;                 // number of texture coordinates
    int numTris;               // number of triangles
    int numGlcmds;             // number of opengl commands
    int numFrames;             // number of frames

    //these make loading the md2 file much easier
    //if we ever want something, just seek with these values
    int offsetSkins;           // offset skin data
    int offsetST;              // offset texture coordinate data
    int offsetTris;            // offset triangle data
    int offsetFrames;          // offset frame data
    int offsetGlcmds;          // offset OpenGL command data
    int offsetEnd;             // offset end of file
};
typedef float Vec3f[3];

// Texture name
struct MD2Skin
{
  char name[64];
};

//Texture coords
struct MD2TexCoord
{
  short s;
  short t;
};

// Triangle info
struct MD2Triangle
{
  unsigned short vertex[3];
  unsigned short st[3];
};

// Compressed vertex
struct MD2Vertex
{
  unsigned char v[3];
  unsigned char normalIndex;
};

// Model frame
struct MD2Frame
{
  Vec3f scale;
  Vec3f translate;
  char name[16];
  MD2Vertex *verts;
};

// GL command packet
struct MD2Glcmd
{
  float s;
  float t;
  int index;
};

class MD2Model
{
    public:
        MD2Model();
        ~MD2Model();
        		//Switches to the given animation
		void Animate (int start, int end,float &interp);
		//Draws the current state of the animated model.
		void draw(float interp);
		//Loads an MD2Model from the specified file.  Returns false if there was
		//an error loading it.
		bool ReadMD2Model(char* fileName);
		void loadTexture(char* name);
		int getEnd();
    private:
        MD2Header header;

        MD2Skin *skins;
        MD2TexCoord *texcoords;
        MD2Triangle *triangles;
        MD2Frame *frames;
        int *glcmds;
        int frame;

        GLuint texture;
};
