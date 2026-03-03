#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"


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
    //this->framebuffer.Resize(w, h);
}

Application::~Application()
{
}

void Application::Init(void)
{
    std::cout << "Initiating app..." << std::endl;
    
    Matrix44 t, r, s;
    t.SetIdentity(); r.SetIdentity(); s.SetIdentity();
    camera = new Camera();
    camera->SetPerspective(45.0 * DEG2RAD, float(window_width)/float(window_height), 0.1, 100.0);
    camera->LookAt(Vector3(0,0,15), Vector3(0,0,0), Vector3(0,1,0));
    
    Mesh* mesh = new Mesh();
    mesh->LoadOBJ("meshes/lee.obj");
    texture = new Texture();
    texture->Load("images/fruits.png"); // make sure path is correct
    texture = new Texture();
    texture->Load("Imagesfcb/.png");
    
    //Image* leeTexture = new Image();
    //leeTexture->LoadTGA("textures/lee_color_specular.tga", false);

    
    Entity* e1 = new Entity();
    e1->mesh = mesh;
    //e1->texture = leeTexture;
    t.MakeTranslationMatrix(0, 0, 8);
    s.MakeScaleMatrix(4, 4, 3);
    e1->model = t*s;
    entities.push_back(e1);
    
    
    Entity* e2 = new Entity();
    e2->mesh = mesh;
    //e2->texture = leeTexture;
    e2->c = Color::BLUE;
    s.MakeScaleMatrix(6, 6, 4);
    t.MakeTranslationMatrix(-2, -1, 9);
    r.MakeRotationMatrix(45.0*DEG2RAD, Vector3(0,1,0));
    e2->model = t*r*s;
    entities.push_back(e2);
    
    
    Entity* e3 = new Entity();
    e3->mesh = mesh;
    //e3->texture = leeTexture;
    e3->c = Color::RED;
    t.MakeTranslationMatrix(1, -1.5, 11);
    r.MakeRotationMatrix(35.0*DEG2RAD, Vector3(0,-1,0));
    s.MakeScaleMatrix(4, 4, 3);
    e3->model = t*r*s;
    entities.push_back(e3);
    
    //framebuffer.Resize(window_width, window_height);
    //zbuffer.Resize(window_width, window_height);
    
    quad = new Mesh();
    quad->CreateQuad();

}

// Render one frame
void Application::Render(void)
{
    if (currentTask != 4)
    {
        shader = Shader::Get("shaders/quad.vs", currentFS.c_str());
        if (!shader) {
                   std::cerr << "Error: shader is null! Check currentFS: " << currentFS << std::endl;
                   return;
               }

        if (!quad) {
                   std::cerr << "Error: quad is null!" << std::endl;
                   return;
               }
        shader->Enable();
        texture->Bind();
        shader->SetUniform1("u_aspect_ratio", float(window_width) / float(window_height));
        quad->Render();
        shader->Disable();
    }
    else
    {
        
    }
}


// Called after render
void Application::Update(float seconds_elapsed)
{
    entities[0]->Update(seconds_elapsed);
}

//keyboard press event
void Application::OnKeyPressed( SDL_KeyboardEvent event)
{
    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
    switch(event.keysym.sym) {
        case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
      
        case SDLK_1: currentTask = 1; break;
        case SDLK_2: currentTask = 2; break;
        case SDLK_3: currentTask = 3; break;
        case SDLK_4: currentTask = 4; break;

        //case SDLK_a: currentSubtask = 0; currentFS = "shaders/task" + std::to_string(currentTask) + "a.fs"; break;
            //case SDLK_b: currentSubtask = 1; currentFS = "shaders/task" + std::to_string(currentTask) + "b.fs"; break;
            //case SDLK_c: currentSubtask = 2; currentFS = "shaders/task" + std::to_string(currentTask) + "c.fs"; break;
            //case SDLK_d: currentSubtask = 3; currentFS = "shaders/task" + std::to_string(currentTask) + "d.fs"; break;
            //case SDLK_e: currentSubtask = 4; currentFS = "shaders/task" + std::to_string(currentTask) + "e.fs"; break;
            //case SDLK_f: currentSubtask = 5; currentFS = "shaders/task" + std::to_string(currentTask) + "f.fs"; break;
        case SDLK_a: currentSubtask = 0; break;
        case SDLK_b: currentSubtask = 1; break;
        case SDLK_c: currentSubtask = 2; break;
        case SDLK_d: currentSubtask = 3; break;
        case SDLK_e: currentSubtask = 4; break;
        case SDLK_f: currentSubtask = 5; break;
     }

     // 🔥 Always rebuild shader filename after any change
     char letter = 'a' + currentSubtask;

     currentFS = std::string("shaders/task") +
                 std::to_string(currentTask) +
                 letter + ".fs";

     std::cout << "Loading shader: " << currentFS << std::endl;
    
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {
        isLeftMousePressed = true;
    }
    else if (event.button == SDL_BUTTON_RIGHT) {
        isRightMousePressed = true;
    }
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {
        isLeftMousePressed = false;
    }
    else if (event.button == SDL_BUTTON_RIGHT) {
        isRightMousePressed = false;
    }
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    float sensitivity;
    if (isLeftMousePressed) {
        sensitivity = 0.001;
        // Calculate how much to rotate based on mouse movement
        float angleY = -mouse_delta.x * sensitivity;   // Move mouse horizonatlly --> rotating around y-axis
        float angleX = -mouse_delta.y * sensitivity;  // Move mouse vertically   --> rotating around x-axis
        
        // Get current direction from center to eye
        Vector3 direction = camera->eye - camera->center;
        float distance = direction.Length(); // We keep the intial distance from eye to center (don't change it when moving)
        
        // Rotate around Y axis (horizontal mouse movement)
        Matrix44 ry;
        ry.MakeRotationMatrix(angleY, Vector3(0, 1, 0));
        direction = ry * direction;
        
        // Rotate around X axis (vertical mouse movement)
        // Get the right vector (x axis for camera current position, the vector we rotate around)
        Vector3 forward = camera->center - camera->eye;
        forward.Normalize();
        Vector3 right = camera->up.Cross(forward);
        right.Normalize();
        
        Matrix44 rx;
        rx.MakeRotationMatrix(angleX, right);
        direction = rx * direction;
        
        direction.Normalize();
        camera->eye = camera->center + (direction * distance);
        
        camera->UpdateViewMatrix();
    }
        

    else if (isRightMousePressed) {
        float sensitivity = 0.01;
        // Calculate camera's local axis
        Vector3 forward = camera->center - camera->eye;
        forward.Normalize();
        
        Vector3 right = camera->up.Cross(forward);
        right.Normalize();
        
        Vector3 up = forward.Cross(right);

        
        // Calculate movement
        Vector3 movement = right * (mouse_delta.x * sensitivity) + up * (mouse_delta.y * sensitivity);
        
        // Move both eye and center
        camera->eye = camera->eye + movement;
        camera->center = camera->center + movement;
        
        camera->UpdateViewMatrix();
    }
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
    float dy = event.preciseY;
    float zoomSpeed = 0.2;
    
    Vector3 direction = camera->center - camera->eye;
    float currentDistance = direction.Length();
    direction.Normalize();
    
    // Your better version - zoom amount proportional to scroll
    float newDistance = currentDistance + zoomSpeed * dy;
    
    // Clamp to reasonable range
    //if (newDistance > 0.5 && newDistance < 50.0) {
        camera->eye = camera->center - direction * newDistance;
        camera->UpdateViewMatrix();
    //}
    
    camera->UpdateViewMatrix();
}

void Application::OnFileChanged(const char* filename)
{
    Shader::ReloadSingleShader(filename);
}


