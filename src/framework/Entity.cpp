//
//  Entity.cpp
//  ComputerGraphics
//
//  Created by Sara Gonzalez on 28/1/26.
//

#include "Entity.h"


Entity::Entity()
{
    mesh = NULL;
    model.SetIdentity();
    c = Color::WHITE;
}


void Entity::Render(Image* framebuffer, Camera* camera, FloatImage* zBuffer)
{
    // Get the vertices of the mesh and iterate through them
    std::vector<Vector3> vertices = mesh->GetVertices();
    int numVertices = int(vertices.size());
    
    std::vector<Vector3> projectedVertices;
    projectedVertices.resize(numVertices);
    
    for(int i=0; i < numVertices; i++){
        
        // Transform the vertices from local to world space using the Entity’s model matrix
        Vector3 world = model * vertices[i];
        // Project each of the world space vertices to clip space positions using your current camera
        Vector3 clip = camera->ProjectVector(world);
    
        projectedVertices[i] = clip;
    }
    
    for(int i=0; i +2 < numVertices; i +=3){
        // 3 vertices to make a triangle --> AIXO VA AIXI O HAURIA DE SER TOTS AMB TOTS I ANAR FENT DUN EN UN??
        Vector3 v0 = projectedVertices[i];
        Vector3 v1 = projectedVertices[i+1];
        Vector3 v2 = projectedVertices[i+2];
        
        // Make sure to render only the projected triangles that lay inside the cube
        if ((v0.x < -1 && v1.x < -1 && v2.x < -1) || (v0.x >  1 && v1.x >  1 && v2.x >  1) ||
            (v0.y < -1 && v1.y < -1 && v2.y < -1) || (v0.y >  1 && v1.y >  1 && v2.y >  1) ||
            (v0.z < -1 && v1.z < -1 && v2.z < -1) || (v0.z >  1 && v1.z >  1 && v2.z >  1)){
            continue;
        }
        
        // Before drawing each of the triangle lines, convert the clip space positions to screen space using the framebuffer width and height.
        
        int width = framebuffer->width;
        int height = framebuffer->height;
        

        int x0 = (int)((v0.x + 1.0f) * 0.5f * width);
        int y0 = (int)((1.0f + v0.y) * 0.5f * height);

        int x1 = (int)((v1.x + 1.0f) * 0.5f * width);
        int y1 = (int)((1.0f + v1.y) * 0.5f * height);

        int x2 = (int)((v2.x + 1.0f) * 0.5f * width);
        int y2 = (int)((1.0f + v2.y) * 0.5f * height);
        
        // Draw(fill) the triangle
        Vector3 p0((v0.x + 1.0f) * 0.5f * width,  (1.0f + v0.y) * 0.5f * height, v0.z);
        Vector3 p1((v1.x + 1.0f) * 0.5f * width,  (1.0f + v1.y) * 0.5f * height, v1.z);
        Vector3 p2((v2.x + 1.0f) * 0.5f * width,  (1.0f + v2.y) * 0.5f * height, v2.z);

                // Assign colors (example: you can change to per-vertex color)
        Color c0 = Color::RED;
        Color c1 = Color::GREEN;
        Color c2 = Color::BLUE;

                // Draw the triangle with barycentric interpolation and Z-buffer
        framebuffer->DrawTriangleInterpolated(p0, p1, p2, c0, c1, c2, zBuffer);
            }
    
}


void Entity::Update(float seconds_elapsed)
{

    
    /*// ROTATION:
    Matrix44 rotation;
    rotation.MakeRotationMatrix(seconds_elapsed, Vector3(0, 1, 0));

    model = model * rotation;*/
    
    
    // TRANSLATION:
    Matrix44 t;
    t.MakeTranslationMatrix(0.0, 0.5*sin(seconds_elapsed), 0.0);
    model = model * t;
    
}
