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
    
    Button b;
    
    b.image.LoadPNG("images/clear.png");
    b.type = BTN_CLEAR;
    b.position = Vector2(20, 20);
    buttons.push_back(b);
    
    b.image.LoadPNG("images/load.png");
    b.type = BTN_LOAD;
    b.position = Vector2(70 , 20);
    buttons.push_back(b);
    
    b.image.LoadPNG("images/save.png");
    b.type = BTN_SAVE;
    b.position = Vector2(120, 20);
    buttons.push_back(b);
    
    b.image.LoadPNG("images/eraser.png");
    b.type = BTN_ERASER;
    b.position = Vector2(170, 20);
    buttons.push_back(b);
    
    b.image.LoadPNG("images/line.png");
    b.type = BTN_LINE;
    b.position = Vector2(220, 20);
    buttons.push_back(b);
    
    b.image.LoadPNG("images/rectangle.png");
    b.type = BTN_RECT;
    b.position = Vector2(270, 20);
    buttons.push_back(b);
    
    b.image.LoadPNG("images/triangle.png");
    b.type = BTN_TRIANGLE;
    b.position = Vector2(320, 20);
    buttons.push_back(b);
    
    b.image.LoadPNG("images/white.png");
    b.type = BTN_WHITE;
    b.position = Vector2(420, 20);
    buttons.push_back(b);
    
    b.image.LoadPNG("images/blue.png");
    b.type = BTN_BLUE;
    b.position = Vector2(470, 20);
    buttons.push_back(b);
    
    b.image.LoadPNG("images/cyan.png");
    b.type = BTN_CYAN;
    b.position = Vector2(520, 20);
    buttons.push_back(b);
    
    b.image.LoadPNG("images/red.png");
    b.type = BTN_RED;
    b.position = Vector2(570, 20);
    buttons.push_back(b);
    
    b.image.LoadPNG("images/yellow.png");
    b.type = BTN_YELL;
    b.position = Vector2(620, 20);
    buttons.push_back(b);
    
    b.image.LoadPNG("images/pencil.png");
    b.type = BTN_PENCIL;
    b.position = Vector2(370, 20);
    buttons.push_back(b);
    
    
    particleSystem.Init();
}

// Render one frame
void Application::Render(void)
{
    // ...
    /*
    framebuffer.DrawLineDDA(50, 100, 500, 200, Color(225, 0, 150));
    framebuffer.DrawRect(400, 250, 300, 400, Color(50, 150, 250), borderWidth, true, Color(0, 200, 250));
    framebuffer.DrawTriangle(Vector2(700,200), Vector2(1100,300),Vector2(875, 500), Color(200, 100, 250), true, Color(0, 150,250));
    particleSystem.Render(&framebuffer);
     */
    
    if (currentMode == 1){
        // TOOLBAR BACKGROUND:
        framebuffer.DrawRect(0, 0, window_width, 75, Color(170, 200, 255), borderWidth, true, Color(170, 200, 255));
        // TOOLBAR IMAGES (WITH BUTTONS)
        for (Button& b : buttons){
            b.Draw(framebuffer);
        }
    }
    else if (currentMode == 2){
        framebuffer.Fill(Color::BLACK);
        particleSystem.Render(&framebuffer);
    }
    
    framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{
    if (currentMode == 2) particleSystem.Update(seconds_elapsed);
    else{
        if (isDrawing && (currentTool == TOOL_PENCIL || currentTool == TOOL_ERASER)) {
           if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) {
               Color c = currentColor;
               if (currentTool == TOOL_ERASER) c = Color:: BLACK;
               framebuffer.DrawLineDDA(prevMouse.x, prevMouse.y, mouse_position.x, mouse_position.y, c);
               prevMouse = mouse_position;
           }
        }
    }
}

//keyboard press event
void Application::OnKeyPressed( SDL_KeyboardEvent event)
{
    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
    switch(event.keysym.sym) {
        case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
        case SDLK_PLUS: borderWidth++; break;
        case SDLK_MINUS:if (borderWidth > 1) borderWidth--;  break;
        case SDLK_1: currentMode = 1; framebuffer.Fill(Color::BLACK); break;
        case SDLK_2: currentMode = 2; framebuffer.Fill(Color::BLACK); break;
        case SDLK_f: fillShapes = !fillShapes; break;
    }
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
    if (currentMode != 1) return;
    if (event.button == SDL_BUTTON_LEFT) {
        for (Button& b : buttons) {
            if (b.IsMouseInside(mouse_position)) {
                if(b.type == BTN_PENCIL) {
                    currentTool = TOOL_PENCIL;
                }
                else if (b.type == BTN_LINE){
                    currentTool = TOOL_LINE;
                }
                else if (b.type == BTN_RECT){
                    currentTool = TOOL_RECT;
                }
                else if (b.type == BTN_TRIANGLE){
                    currentTool = TOOL_TRIANGLE;
                }
                else if (b.type == BTN_ERASER){
                    currentTool = TOOL_ERASER;
                }
                else if (b.type == BTN_WHITE){
                    currentColor = Color:: WHITE;
                }
                else if (b.type == BTN_BLUE){
                    currentColor = Color:: BLUE;
                }
                else if (b.type == BTN_CYAN){
                    currentColor = Color:: CYAN;
                }
                else if (b.type == BTN_RED){
                    currentColor = Color:: RED;
                }
                else if (b.type == BTN_YELL){
                    currentColor = Color:: YELLOW;
                }
                else if (b.type == BTN_CLEAR){
                    framebuffer.Fill(Color::BLACK);
                    framebuffer.DrawRect(0, 0, window_width, 75, Color(170, 200, 255), borderWidth, true, Color(170, 200, 255));
                    for (Button& btn : buttons) {
                        btn.Draw(framebuffer);
                    }
                }
                else if (b.type == BTN_LOAD){
                    framebuffer.LoadPNG("images/fruits.png");
                }
                else if (b.type == BTN_SAVE){
                    framebuffer.SaveTGA("images/img.tga");
                }
                   
                return;
            }
        }
        isDrawing = true;
        startMouse = mouse_position;
        prevMouse = mouse_position;
    }
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
    if (currentMode != 1) return;
    if (event.button == SDL_BUTTON_LEFT && isDrawing) {
        endMouse = mouse_position;
                
        Color drawColor = (currentTool == TOOL_ERASER) ? Color::BLACK : currentColor;
        
        // Draw the final shape
        if (currentTool == TOOL_LINE){
            framebuffer.DrawLineDDA(startMouse.x, startMouse.y, endMouse.x, endMouse.y, drawColor);
        }
        else if (currentTool == TOOL_RECT) {
            int w = endMouse.x - startMouse.x;
            int h = endMouse.y - startMouse.y;
            int rectX = startMouse.x;
            int rectY = startMouse.y;
            
            if (w < 0) {
                rectX = endMouse.x;
                w = -w;
            }
            if (h < 0) {
                rectY = endMouse.y;
                h = -h;
            }
            framebuffer.DrawRect(rectX, rectY, w, h, drawColor, borderWidth, fillShapes, drawColor);
        }
        else if (currentTool == TOOL_TRIANGLE) {
            framebuffer.DrawTriangle(startMouse, endMouse, Vector2(startMouse.x + (endMouse.x - startMouse.x), startMouse.y + (endMouse.y - startMouse.y) / 2), drawColor, fillShapes, drawColor);
        }
        
        isDrawing = false;
    }
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    if (!isDrawing) return;
    
    currentMouse = mouse_position;
    
    if (currentTool == TOOL_PENCIL){
        framebuffer.DrawLineDDA(prevMouse.x, prevMouse.y, currentMouse.x, currentMouse.y, currentColor);
        prevMouse = currentMouse;
    }
    else if (currentTool == TOOL_ERASER) {
        framebuffer.DrawLineDDA(prevMouse.x, prevMouse.y, currentMouse.x, currentMouse.y, Color::BLACK);
        prevMouse = currentMouse;
    }

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


