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
    
    // 2D textures
    texture = new Texture();
    texture->Load("images/fruits.png");
    textureTask3 = new Texture();
    textureTask3->Load("images/image_task3.png");
    
    // Quad for 2D tasks
    quad = new Mesh();
    quad->CreateQuad();
    
    // 3D mesh
    Mesh* mesh = new Mesh();
    mesh->LoadOBJ("meshes/lee.obj");
    
    // Lee face texture (shared by all entities)
    Texture* leeTexture = new Texture();
    leeTexture->Load("textures/lee_color_specular.tga");
    
    // Set up the different materials
    raster_material = new Material();
    raster_material->shader = Shader::Get("shaders/raster.vs", "shaders/raster.fs");
    raster_material->color_texture = leeTexture;

    gouraud_material = new Material();
    gouraud_material->shader = Shader::Get("shaders/gouraud.vs", "shaders/gouraud.fs");
    gouraud_material->color_texture = leeTexture;

    phong_material = new Material();
    phong_material->shader = Shader::Get("shaders/phong.vs", "shaders/phong.fs");
    phong_material->color_texture = leeTexture;

    // Init entities (initialized to raster materials)
    Entity* e1 = new Entity();
    e1->mesh = mesh;
    e1->material = raster_material;
    t.MakeTranslationMatrix(0, 0, 8);
    s.MakeScaleMatrix(4, 4, 3);
    e1->model = t * s;
    entities.push_back(e1);
    
    Entity* e2 = new Entity();
    e2->mesh = mesh;
    e2->material = raster_material;
    t.MakeTranslationMatrix(-2, -1, 9);
    r.MakeRotationMatrix(45.0 * DEG2RAD, Vector3(0, 1, 0));
    s.MakeScaleMatrix(6, 6, 4);
    e2->model = t * r * s;
    entities.push_back(e2);
    
    Entity* e3 = new Entity();
    e3->mesh = mesh;
    e3->material = raster_material;
    t.MakeTranslationMatrix(1, -1.5, 11);
    r.MakeRotationMatrix(35.0 * DEG2RAD, Vector3(0, -1, 0));
    s.MakeScaleMatrix(4, 4, 3);
    e3->model = t * r * s;
    entities.push_back(e3);
    
    
    // Set up scene uniform data
    uniform_data.ambient_light = Vector3(0.1f, 0.1f, 0.1f);
    uniform_data.camera_position = camera->eye;

    // Add one light to the scene
    sLight light1;
    light1.position = Vector3(0.0f, 5.0f, 20.0f);
    light1.diffuse  = Vector3(1.0f, 1.0f, 1.0f);
    uniform_data.lights.push_back(light1);
    
    sLight light2;
    light2.position = Vector3(-5.0f,5.0f,5.0f);
    light2.diffuse  = Vector3(1.0f,0.0f,0.0f);

    uniform_data.lights.push_back(light2);
    
    
}

// Render one frame
void Application::Render(void)
{
    // clear the screen before we draw anything
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (!lab5){
        if (currentTask != 4)
        {
            glDisable(GL_DEPTH_TEST);  // 2D quad doesn't need depth
            
            shader = Shader::Get("shaders/quad.vs", currentFS.c_str()); // Load the shader pair for the current task/subtask
            if (!shader || !quad) return;
            
            shader->Enable();
            
            // Both tasks 2 and 3 need a texture
            if (currentTask == 2 || currentTask == 3)
            {
                Texture* currentTexture = (currentTask == 2) ? texture : textureTask3;
                currentTexture->Bind();  // fruits.png bound to unit 0
                shader->SetUniform1("u_texture", 0);
                shader->SetUniform1("u_time", time);
            }
            
            shader->SetUniform1("u_aspect_ratio", float(window_width) / float(window_height));
            
            quad->Render();
            shader->Disable();
        }
        else  // Task 4: GPU rasterization of 3D mesh
        {
            glEnable(GL_DEPTH_TEST);

            uniform_data.viewprojection  = camera->GetViewProjectionMatrix();
            uniform_data.camera_position = camera->eye;

            for (Entity* e : entities){
                e->Render(uniform_data);  // <-- was e->Render(camera), now fixed
            }
        }
        
    }
    else // -------- LAB 5 --------
        {
            glEnable(GL_DEPTH_TEST);

            uniform_data.viewprojection  = camera->GetViewProjectionMatrix();
            uniform_data.camera_position = camera->eye;

            // multipass lighting
            int numLights = std::min(uniform_data.num_lights, (int)uniform_data.lights.size());

            for (int i = 0; i < numLights; i++)
            {
                if (i == 0)
                {
                    glDisable(GL_BLEND); // first light normal render
                }
                else
                {
                    glEnable(GL_BLEND);  // add next lights
                    glBlendFunc(GL_ONE, GL_ONE);
                }

                uniform_data.current_light = uniform_data.lights[i];

                for (Entity* e : entities)
                    e->Render(uniform_data);
            }

            glDisable(GL_BLEND);
        }
    }

// Called after render
void Application::Update(float seconds_elapsed)
{
    entities[0]->Update(seconds_elapsed);
    time += seconds_elapsed;
}

//keyboard press event
void Application::OnKeyPressed( SDL_KeyboardEvent event)
{
    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
    switch(event.keysym.sym) {
        case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
           
        case SDLK_1:
            if (lab5)
                uniform_data.num_lights = 1;
            else
                currentTask = 1;
            break;

        case SDLK_2:
            if (lab5)
                uniform_data.num_lights = 2;
            else
                currentTask = 2;
            break;

        case SDLK_3:
            if (lab5)
                uniform_data.num_lights = 3;
            else
                currentTask = 3;
            break;

        case SDLK_4:
            if (lab5)
                uniform_data.num_lights = 4;
            else
                currentTask = 4;
            break;

                    // --- Lab4 Subtasks ---
        case SDLK_a: currentSubtask = 0; break;
        case SDLK_b: currentSubtask = 1; break;
        case SDLK_c:
            if (lab5)
                phong_material->use_color_texture = !phong_material->use_color_texture;
            else
                currentSubtask = 2;
            break;
        case SDLK_d: currentSubtask = 3; break;
        case SDLK_e: currentSubtask = 4; break;
        case SDLK_f: currentSubtask = 5; break;

                    // --- Lab selection ---
        case SDLK_l: lab5 = !lab5; break;

                    // --- Lab5 shading selection ---
        case SDLK_g:
            for (Entity* e : entities)
                e->material = gouraud_material;
            break;

        case SDLK_p:
            for (Entity* e : entities)
                e->material = phong_material;
            break;

                    // --- Lab5 Phong textures toggles ---
        case SDLK_s: phong_material->use_specular_texture = !phong_material->use_specular_texture; break;

        case SDLK_n: phong_material->use_normal_texture = !phong_material->use_normal_texture; break;
                }

                // --- Always rebuild shader filename for Lab4 tasks ---
        if (currentTask >= 1 && currentTask <= 3) {
            char letter = 'a' + currentSubtask;
            currentFS = std::string("shaders/task") + std::to_string(currentTask) + letter + ".fs";
            std::cout << "Loading shader: " << currentFS << std::endl;
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


