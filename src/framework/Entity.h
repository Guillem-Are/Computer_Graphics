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
    enum class eRenderMode {
            POINTCLOUD,
            WIREFRAME,
            TRIANGLES,
            TRIANGLES_INTERPOLATED
        };
    // ATTRIBUTES:
    Mesh* mesh;
    Image* texture = NULL;
    Matrix44 model;
    Color c;
    eRenderMode render_mode = eRenderMode::TRIANGLES_INTERPOLATED;
    bool use_texture = true;          // T
    bool use_occlusion = true;        //Z
    bool use_interpolated_uv = true;//C
    
    // METHODS
    Entity();
    void Render(Image* framebuffer, Camera* camera, FloatImage* zBuffer);
    void Update(float seconds_elapsed);
};


#endif /* Entity_h */
