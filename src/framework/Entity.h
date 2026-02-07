//
//  Entity.hpp
//  ComputerGraphics
//
//  Created by Sara Gonzalez on 28/1/26.
//

#ifndef Entity_h
#define Entity_h

#include <stdio.h>
#include "mesh.h"
#include "framework.h"
#include "camera.h"
#include "image.h"


class Entity {
public:
    // ATTRIBUTES:
    Mesh* mesh;
    Matrix44 model;
    Color c;
    
    // METHODS
    Entity();
    void Render(Image* framebuffer, Camera* camera, const Color& c);
    void Update(float seconds_elapsed);
};


#endif /* Entity_h */
