/*  
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "Entity.h"
#include "mesh.h"      
#include "shader.h"
#include "texture.h"

class Application
{
    
public:

	// Window

	SDL_Window* window = nullptr;
	int window_width;
	int window_height;
    int borderWidth = 5;
	float time;
    FloatImage zbuffer;

    
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
    bool isLeftMousePressed = false;
    bool isRightMousePressed = false;
    
    Mesh* quad;
    Shader* shader;
    Texture* texture;
    Texture* textureTask3 = nullptr;

    int currentTask = 1;
    int currentSubtask = 0; // 0=a, 1=b, 2=c, 3=d, 4=e, 5=f
    std::string currentFS = "shaders/task1f.fs";
   
    
    
    std::vector<Entity*> entities;
    Camera* camera;
    
    

	void OnKeyPressed(SDL_KeyboardEvent event);
	void OnMouseButtonDown(SDL_MouseButtonEvent event);
	void OnMouseButtonUp(SDL_MouseButtonEvent event);
	void OnMouseMove(SDL_MouseButtonEvent event);
	void OnWheel(SDL_MouseWheelEvent event);
	void OnFileChanged(const char* filename);

	// CPU Global framebuffer
	// Image framebuffer;

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
		//this->framebuffer.Resize(width, height);
	}

	Vector2 GetWindowSize()
	{
		int w,h;
		SDL_GetWindowSize(window,&w,&h);
		return Vector2(float(w), float(h));
	}
};

