#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <time.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "SDL.h"
#include "glew.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "sprites.h"

#include "sparkles.h"


const int screenWidth = 640;
const int screenHeight = 640;

struct Character : pulic Sprite
{
    glm::vec2 bearing; //the bearing, or margins of the character
    glm::vec2 size; //the dimensions of the character
    GLuint advance; //the total width the character takes up, including the character width, the horizontal bearing, and the space from the next character
    Character(int stanW, int stanH)
    {

    }
};
std::map<char,Character> characters;
int main(int argc, char *argv[])
{
//std::cout <<"ASDF" << std::endl;
FT_Library  library;
FT_Init_FreeType(&library);

FT_Face face;
FT_New_Face(library, "alef.ttf",0,&face);
FT_Set_Pixel_Sizes(face,0,48);
for (GLubyte c = 0; c < 128; c++)
{
    // Load character glyph
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        continue;
    }
    // Generate texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
    );
    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Now store character for later use
    Character character = {
        texture,
        glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        face->glyph->advance.x
    };
    characters.insert(std::pair<GLchar, Character>(c, character));
}

FT_Done_Face(face);
FT_Done_FreeType(library);

    bool quit = false;
    SDL_Event e;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
SDL_Window* window = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,screenWidth, screenHeight,SDL_WINDOW_OPENGL);
SDL_GL_CreateContext(window);
glewExperimental = true;
glewInit();
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
glEnable( GL_BLEND );
glEnable(GL_DEBUG_OUTPUT);
int vertexShader = loadShaders("shaders/vertex/vertexShader.h",GL_VERTEX_SHADER);
int fragmentShader = loadShaders("shaders/fragment/fragmentShader.h", GL_FRAGMENT_SHADER);
program = glCreateProgram();
glAttachShader(program,vertexShader);
glAttachShader(program,fragmentShader);
glLinkProgram(program);

glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);

srand(time(NULL));

Sprite s(screenWidth, screenHeight,"square.png",true);
std::vector<glm::vec3> vec;

vec.push_back({0,0,0});
ParticleSystem sys;
loadParticles();
for (int i = 0; i < 1000; i ++)
{
    sys.addParticle(new Snowflake((float)(rand()%640), (float)(rand()%640)));
}


while(quit == false)
{    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
          quit = true;
        }
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(.5,.9,1,0);

    glUseProgram(program);
  // s.renderInstanced(vec,64,64);
    sys.render(snowflake);
   // snowflake.renderInstanced(vec,64,64);

//  s.renderInstanced(vec,64,64);
  //glBindVertexArray(VAO);
 //   glDrawElementsInstanced(GL_TRIANGLES,6,GL_UNSIGNED_INT,&indices,3);
    //s.render(0,0, 64, 64,M_PI/4);

    SDL_GL_SwapWindow(window);

}


}
