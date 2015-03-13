#include <iostream>
#include <fstream>

#include "GL/glew.h"

#include "SDL.h"
#include "SDL_opengl.h"

#include "glm/glm.hpp"

const float vertices[] = {
     0.0f,  0.5f, // Vertex 1 (X, Y)
     0.5f, -0.5f, // Vertex 2 (X, Y)
    -0.5f, -0.5f  // Vertex 3 (X, Y)
};

void checkGlError() {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "GL error: " << error << std::endl;
    }
}

void loadGfx() {
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLint status;

    std::ifstream vifs("../shaders/vertex.glsl");
    std::string vertex((std::istreambuf_iterator<char>(vifs)), (std::istreambuf_iterator<char>()));
    const GLchar *vertexSource = (const GLchar *)vertex.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, 0);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        std::cout << "Shader Compile Failed!" << std::endl;
        char buffer[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
        std::cout << buffer << std::endl;
        exit(1);
    }

    std::ifstream fifs("../shaders/fragment.glsl");
    std::string fragment((std::istreambuf_iterator<char>(fifs)), (std::istreambuf_iterator<char>()));
    const GLchar *fragmentSource = (const GLchar *)fragment.c_str();

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, 0);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        std::cout << "Shader Compile Failed!" << std::endl;
        char buffer[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
        std::cout << buffer << std::endl;
        exit(1);
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glBindFragDataLocation(shaderProgram, 0, "outColor");

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

void cleanUpGfx() {

}

void renderFrame() {
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void runMainLoop(SDL_Window* window) {
    checkGlError();
    checkGlError();
    checkGlError();
    std::cout << "Load GFX" << std::endl;
    loadGfx();
    checkGlError();

    SDL_Event windowEvent;
    while (true)
    {
        checkGlError();
        
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT) break;
            if (windowEvent.type == SDL_KEYUP &&
                windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
        }

        renderFrame();
        SDL_GL_SwapWindow(window);
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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_Window* window = SDL_CreateWindow("space-game", 100, 100, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    glewExperimental = GL_TRUE;
    glewInit();

    checkGlError();
    std::cout << "Run Main Loop" << std::endl;

    runMainLoop(window);

    SDL_GL_DeleteContext(context);

    SDL_Quit();

    return 0;
}