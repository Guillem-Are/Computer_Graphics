#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"
#include "ParticleSystem.h"
Application::Application(const char* caption, int width, int height)
{
    this->window = createWindow(caption, width, height);

    int w,h;
    SDL_GetWindowSize(window,&w,&h);

    this->mouse_state = 0;
    this->time = 0.f;
    this->window_width = w;
    this->window_height = h;
    this->keystate = SDL_GetKeyboardState(nullptr);

    this->framebuffer.Resize(w, h);
}

Application::~Application()
{
}

void Application::Init(void)
{
    std::cout << "Initiating app..." << std::endl;
    particleSystem.Init();
}

// Render one frame
void Application::Render(void)
{
    // ...
    framebuffer.Fill(Color(0, 0, 0));
    framebuffer.DrawLineDDA(50, 100, 500, 200, Color(225, 0, 150));
    framebuffer.DrawRect(400, 250, 300, 400, Color(50, 150, 250), borderWidth, true, Color(0, 200, 250));
    framebuffer.DrawTriangle(Vector2(700,200), Vector2(1100,300),Vector2(875, 500), Color(200, 100, 250), true, Color(0, 150, 250));
    particleSystem.Render(&framebuffer);
    framebuffer.Render();
    
}

// Called after render
void Application::Update(float seconds_elapsed)
{
    particleSystem.Update(seconds_elapsed);
}

//keyboard press event
void Application::OnKeyPressed( SDL_KeyboardEvent event)
{
    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
    switch(event.keysym.sym) {
        case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
        case SDLK_PLUS: borderWidth++; break;
        case SDLK_MINUS:if (borderWidth > 1) borderWidth--;  break;
    }
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {

    }
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {

    }
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
    float dy = event.preciseY;

    // ...
}

void Application::OnFileChanged(const char* filename)
{
    Shader::ReloadSingleShader(filename);
}


