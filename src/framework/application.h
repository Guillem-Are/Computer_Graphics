/*  
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "ParticleSystem.h"
#include "Button.h"

class Application
{
public:

	// Window

	SDL_Window* window = nullptr;
	int window_width;
	int window_height;
    int borderWidth = 5;
    ParticleSystem particleSystem;
	float time;

	// Input
	const Uint8* keystate;
	int mouse_state; // Tells which buttons are pressed
	Vector2 mouse_position; // Last mouse position
	Vector2 mouse_delta; // Mouse movement in the last frame
    bool isDrawing = false;
    Vector2 startMouse;
    Vector2 currentMouse;
    Vector2 prevMouse;
    Vector2 endMouse;

    
    int TOOL_PENCIL = 0;
    int TOOL_LINE = 1;
    int TOOL_RECT = 2;
    int TOOL_TRIANGLE = 3;
    int TOOL_ERASER = 4;
    
    int BTN_PENCIL = 0;
    int BTN_LINE = 1;
    int BTN_RECT = 2;
    int BTN_TRIANGLE = 3;
    int BTN_ERASER = 4;
    int BTN_CLEAR = 5;
    int BTN_LOAD = 6;
    int BTN_SAVE = 7;
    int BTN_WHITE = 8;
    int BTN_BLUE = 9;
    int BTN_CYAN = 10;
    int BTN_RED = 11;
    int BTN_YELL = 12;
    
    std:: vector<Button> buttons;
    
    int currentTool = TOOL_PENCIL;
    Color currentColor = Color::WHITE;
    
    int currentMode = 1;
    bool fillShapes = false;

	void OnKeyPressed(SDL_KeyboardEvent event);
	void OnMouseButtonDown(SDL_MouseButtonEvent event);
	void OnMouseButtonUp(SDL_MouseButtonEvent event);
	void OnMouseMove(SDL_MouseButtonEvent event);
	void OnWheel(SDL_MouseWheelEvent event);
	void OnFileChanged(const char* filename);

	// CPU Global framebuffer
	Image framebuffer;

	// Constructor and main methods
	Application(const char* caption, int width, int height);
	~Application();

	void Init( void );
	void Render( void );
	void Update( float dt );

	// Other methods to control the app
	void SetWindowSize(int width, int height) {
		glViewport( 0,0, width, height );
		this->window_width = width;
		this->window_height = height;
		this->framebuffer.Resize(width, height);
	}

	Vector2 GetWindowSize()
	{
		int w,h;
		SDL_GetWindowSize(window,&w,&h);
		return Vector2(float(w), float(h));
	}
};
