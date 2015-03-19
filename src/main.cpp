#include <iostream>
#include <fstream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <FreeImage.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    
    0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
};

uint32_t screenX = 800, screenY = 600;
GLuint vertexShader = 0, fragmentShader = 0, shaderProgram = 0;
GLuint vao = 0, vbo = 0, texture = 0;
GLint uniTrans = 0;

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

GLuint loadTexture(const std::string& filename) {
    FIBITMAP *pImage = FreeImage_Load(FreeImage_GetFileType(filename.c_str(), 0), filename.c_str());
    
    GLuint t;
    glGenTextures(1, &t);
    
    glBindTexture(GL_TEXTURE_2D, t);
    
    uint32_t width = FreeImage_GetWidth(pImage);
    uint32_t height = FreeImage_GetHeight(pImage);
    uint32_t bpp = FreeImage_GetBPP(pImage);
    
    std::cout << "loading texture: " << filename << ", width: " << width << ", height: " << height << ", bpp: " << bpp << std::endl;
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height,
                 0, GL_BGR, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));
    
    FreeImage_Unload(pImage);
    
    return t;
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
    
    glEnable(GL_DEPTH_TEST);
    
    checkGlError(__FILE__, __LINE__);
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    checkGlError(__FILE__, __LINE__);
    
    // TODO: figure out paths :(
    vertexShader = loadShader("/Users/malcolm/Projects/space-game/shaders/vertex.glsl", GL_VERTEX_SHADER);
    fragmentShader = loadShader("/Users/malcolm/Projects/space-game/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
    
    checkGlError(__FILE__, __LINE__);
    
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glBindFragDataLocation(shaderProgram, 0, "outColor");

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    
    checkGlError(__FILE__, __LINE__);
    
    // Specify the layout of the vertex data
    
    uniTrans = glGetUniformLocation(shaderProgram, "model");
    
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    
    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    
    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    
    
    glm::mat4 view = glm::lookAt(glm::vec3(1.2f, 1.2f, 1.2f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 0.0f, 1.0f));
    GLint uniView = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

    // see proj in renderFrame
    
    checkGlError(__FILE__, __LINE__);
    
    texture = loadTexture("/Users/malcolm/Projects/space-game/assets/kitty.jpg");
    
    checkGlError(__FILE__, __LINE__);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    checkGlError(__FILE__, __LINE__);
}

void cleanUpGfx() {
    glDetachShader(shaderProgram, fragmentShader);
    fragmentShader = 0;
    glDetachShader(shaderProgram, vertexShader);
    vertexShader = 0;
    glDeleteShader(shaderProgram);
    shaderProgram = 0;
    
    glDeleteTextures(1, &texture);
    texture = 0;
    
    glDeleteBuffers(1, &vbo);
    vao = 0;
    
    glDeleteVertexArrays(1, &vao);
    vao = 0;
}

void renderFrame(const float rotation) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)screenX / (float)screenY, 1.0f, 10.0f);
    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
    
    glm::mat4 trans;
    trans = glm::rotate(trans, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::rotate(trans, glm::radians(rotation * 2.f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void runMainLoop(SDL_Window* window) {
    loadGfx();

    uint32_t frame = 0;
    float rotation = 0.f;
    
    SDL_Event windowEvent;
    while (true)
    {
        checkGlError(__FILE__, __LINE__, frame);
        
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_WINDOWEVENT && windowEvent.window.event == SDL_WINDOWEVENT_RESIZED) {
                screenX = windowEvent.window.data1;
                screenY = windowEvent.window.data2;
            }
            if (windowEvent.type == SDL_QUIT) break;
            if (windowEvent.type == SDL_KEYUP &&
                (windowEvent.key.keysym.sym == SDLK_ESCAPE || windowEvent.key.keysym.sym == SDLK_q)) break;
            
            if (windowEvent.type == SDL_KEYDOWN) {
                switch (windowEvent.key.keysym.sym) {
                    case SDLK_UP:
                        std::cout << "Up!" << std::endl;
                        break;
                    case SDLK_LEFT:
                        std::cout << "Left!" << std::endl;
                        break;
                    case SDLK_RIGHT:
                        std::cout << "Right!" << std::endl;
                        break;
                    case SDLK_DOWN:
                        std::cout << "Down!" << std::endl;
                        break;
                }
            }
        }

        
        rotation += 1.25f;
        renderFrame(rotation);
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

    SDL_Window* window = SDL_CreateWindow("space-game", 100, 100, screenX, screenY, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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