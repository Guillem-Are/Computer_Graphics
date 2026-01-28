//
//  Entity.cpp
//  ComputerGraphics
//
//  Created by Sara Gonzalez on 28/1/26.
//

#include "Entity.h"

void Entity::Render(Image* framebuffer, Camera* camera, const Color& c)
{
    // Get the vertices of the mesh and iterate through them
    std::vector<Vector3> vertices = mesh->GetVertices();
    
    for(Vector3 v : vertices){
        
        // Transform the vertices from local to world space using the Entity’s model matrix
        // Project each of the world space vertices to clip space positions using your current camera
        
        // Make sure to render only the projected triangles that lay inside the cube
        // Before drawing each of the triangle lines, convert the clip space positions to screen space using the framebuffer width and height.
    }
    
}


void Entity::Update(float seconds_elapsed)
{
    
}
