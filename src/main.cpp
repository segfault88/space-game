#include <iostream>
#include <fstream>

#define GLEW_STATIC
#include "GL/glew.h"

#include "SDL.h"
#include "SDL_opengl.h"

#include "glm/glm.hpp"

const float vertices[] = {
     0.0f,  0.5f, // Vertex 1 (X, Y)
     0.5f, -0.5f, // Vertex 2 (X, Y)
    -0.5f, -0.5f  // Vertex 3 (X, Y)
};

void checkGlError(const char* file, GLuint line) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "GL error! " << file << ":" << line << " - " << error << std::endl;
    }

}

void checkGlError(const char* file, const uint32_t line, const uint32_t frame) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "GL error! Frame: " << frame << " " << file << ":" << line << " - " << error << std::endl;
    }
}

GLuint loadShader(const std::string& filename, const GLuint type) {
    
    GLint status;
    
    std::ifstream fstream(filename);
    std::string shaderString((std::istreambuf_iterator<char>(fstream)), (std::istreambuf_iterator<char>()));
    const GLchar *shaderSourcePtr = (const GLchar *)shaderString.c_str();
    
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSourcePtr, 0);
    glCompileShader(shader);
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        std::cout << "Shader Compile Failed! Shader:" << filename << std::endl;
        char buffer[1024];
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        std::cout << buffer << std::endl;
        exit(1);
    }
    
    return shader;
}

void loadGfx() {
    std::cout << "Load GFX" << std::endl;
    
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    checkGlError(__FILE__, __LINE__);
    
    // TODO: figure out paths :(
    GLuint vertexShader = loadShader("/Users/malcolm/Projects/space-game/shaders/vertex.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader = loadShader("/Users/malcolm/Projects/space-game/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
    
    checkGlError(__FILE__, __LINE__);
    
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glBindFragDataLocation(shaderProgram, 0, "outColor");

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    
    checkGlError(__FILE__, __LINE__);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);

    checkGlError(__FILE__, __LINE__);
}

void cleanUpGfx() {

}

void renderFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void runMainLoop(SDL_Window* window) {
    loadGfx();

    uint32_t frame = 0;
    SDL_Event windowEvent;
    while (true)
    {
        checkGlError(__FILE__, __LINE__, frame);
        
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT) break;
            if (windowEvent.type == SDL_KEYUP &&
                (windowEvent.key.keysym.sym == SDLK_ESCAPE || windowEvent.key.keysym.sym == SDLK_q)) break;
        }

        renderFrame();
        SDL_GL_SwapWindow(window);
        ++frame;
    }

    cleanUpGfx();
}

int main()
{
    std::cout << "Space Game!" << std::endl;

    // Setup some stuff, open a window and jump into the main loop

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Unable to init SDL: " << SDL_GetError() << std::endl;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_Window* window = SDL_CreateWindow("space-game", 100, 100, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    glewExperimental = GL_TRUE;
    glewInit();
    
    glClearColor(0.2f, 0.2f, 0.2f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    checkGlError(__FILE__, __LINE__);
    std::cout << "Run Main Loop" << std::endl;

    runMainLoop(window);

    SDL_GL_DeleteContext(context);

    SDL_Quit();

    return 0;
}