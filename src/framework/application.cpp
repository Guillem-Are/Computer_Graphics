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
    
    Matrix44 t, r, s;
    t.SetIdentity(); r.SetIdentity(); s.SetIdentity();
    camera = new Camera();
    camera->SetPerspective(45.0 * DEG2RAD, float(window_width)/float(window_height), 0.1, 100.0);
    camera->LookAt(Vector3(0,0,15), Vector3(0,0,0), Vector3(0,1,0));
    
    Mesh* mesh = new Mesh();
    mesh->LoadOBJ("meshes/lee.obj");
    
    Image* leeTexture = new Image();
    leeTexture->LoadTGA("textures/lee_color_specular.tga", false);

    
    Entity* e1 = new Entity();
    e1->mesh = mesh;
    e1->texture = leeTexture;
    t.MakeTranslationMatrix(0, 0, 8);
    s.MakeScaleMatrix(4, 4, 2);
    e1->model = t*s;
    entities.push_back(e1);
    
    
    Entity* e2 = new Entity();
    e2->mesh = mesh;
    e2->texture = leeTexture;
    e2->c = Color::BLUE;
    s.MakeScaleMatrix(6, 6, 2);
    t.MakeTranslationMatrix(-2, -1, 9);
    r.MakeRotationMatrix(45.0*DEG2RAD, Vector3(0,1,0));
    e2->model = t*s*r;
    entities.push_back(e2);
    
    
    Entity* e3 = new Entity();
    e3->mesh = mesh;
    e3->texture = leeTexture;
    e3->c = Color::RED;
    t.MakeTranslationMatrix(1, -1.5, 11);
    r.MakeRotationMatrix(35.0*DEG2RAD, Vector3(0,-1,0));
    s.MakeScaleMatrix(6, 6, 3);
    e3->model = t*s*r;
    entities.push_back(e3);
    framebuffer.Resize(window_width, window_height);
    zbuffer.Resize(window_width, window_height);

}

// Render one frame
void Application::Render(void)
{
    framebuffer.Fill(Color::BLACK);
    zbuffer.Fill(1e9f);

    for(Entity* e : entities){
        if(e && e->mesh != NULL){
            e->Render(&framebuffer, camera, &zbuffer);
        }
        if (numEntities == 1) break;
    }

    framebuffer.Render();
}


// Called after render
void Application::Update(float seconds_elapsed)
{
    //entities[0]->Update(seconds_elapsed);
}

//keyboard press event
void Application::OnKeyPressed( SDL_KeyboardEvent event)
{
    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
    switch(event.keysym.sym) {
        case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
        case SDLK_1: numEntities = 1; break;
        case SDLK_2: numEntities = 2; break;
        case SDLK_f: currentProperty = 2; break;
        case SDLK_n: currentProperty = 1; break;
        case SDLK_v: currentProperty = 3; break;
        case SDLK_PLUS:
            if (currentProperty == 1)
            {
                camera->near_plane++;
                camera->UpdateProjectionMatrix();
            }
            else if (currentProperty == 2)
            {
                camera->far_plane++;
                camera->UpdateProjectionMatrix();
            }
            else if (currentProperty == 3)
            {
                camera->fov++;
                camera->UpdateProjectionMatrix();
            }
            break;
        case SDLK_MINUS:
            if (currentProperty == 1 )
            {
                camera->near_plane--;
                camera->UpdateProjectionMatrix();
            }
            else if(currentProperty==2)
            {
                camera->far_plane--;
                camera->UpdateProjectionMatrix();
            }
            else if (currentProperty == 3)
            {
                camera->fov--;
                camera->UpdateProjectionMatrix();
            }
            break;
        case SDLK_t:
            for (Entity* e : entities)
                e->use_texture = !e->use_texture;
        break;

        case SDLK_z:
            for (Entity* e : entities)
                e->use_occlusion = !e->use_occlusion;
            break;

        case SDLK_c:
            for (Entity* e : entities)
                e->use_interpolated_uv = !e->use_interpolated_uv;
            break;
            
    }
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
    float sensitivity = 0.001;
    if (isLeftMousePressed) {
        
        // Calculate how much to rotate based on mouse movement
        float angleY = mouse_delta.x * sensitivity;  // Move mouse horizonatlly --> rotating around y-axis
        float angleX = -mouse_delta.y * sensitivity;  // Move mouse vertically --> rotating around x-axis
        
        // rotate around Y axis
        camera->Rotate(angleY, Vector3(0,1,0));
        
        
        // Get the right vector (x axis for camera current position, the vector we rotate around)
        Vector3 forward = camera->center - camera->eye; // direction camera is looking at
        forward.Normalize();
        Vector3 right = camera->up.Cross(forward);
        right.Normalize();
        
        // rotate around X axis
        camera->Rotate(angleX, right);
        

        camera->UpdateViewMatrix();
    }
        

    else if (isRightMousePressed) {
        
        // Calculate camera's local axis
        Vector3 forward = camera->center - camera->eye;
        forward.Normalize();
        
        Vector3 right = camera->up.Cross(forward);
        right.Normalize();
        
        Vector3 up = forward.Cross(right);

        
        // Calculate movement
        Vector3 movement = right * (mouse_delta.x * sensitivity) + up * (mouse_delta.y * sensitivity);
        
        // Move both eye and center
        //camera->eye = camera->eye + movement;
        camera->center = camera->center + movement;
        
        camera->UpdateViewMatrix();
    }
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
    float dy = event.preciseY; // scroll direction
    float zoomSpeed = 0.5;
        
    // Get direction from eye to center
    Vector3 direction = camera->center - camera->eye;
    float currentDistance = direction.Length();
    direction.Normalize();
    
    if (dy > 0) {
        // Scroll UP - zoom IN (get closer)
        float newDistance = currentDistance - zoomSpeed*dy;
        
        // Don't zoom too close (minimum distance)
        if (newDistance > 0.5) {
            camera->eye = camera->center - direction * newDistance;
        }
    }
    else if (dy < 0) {
        // Scroll DOWN - zoom OUT (get farther)
        float newDistance = currentDistance - zoomSpeed*dy;
        
        // Don't zoom too far (maximum distance)
        if (newDistance < 50.0f) {
            camera->eye = camera->center - direction * newDistance;
        }
    }
    
    camera->UpdateViewMatrix();
}

void Application::OnFileChanged(const char* filename)
{
    Shader::ReloadSingleShader(filename);
}


