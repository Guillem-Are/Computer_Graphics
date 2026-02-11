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
    if (!mesh)
        return;

    if (!use_occlusion && zBuffer)
        zBuffer->Fill(1e9f);
    std::vector<Vector3> vertices = mesh->GetVertices();
    const std::vector<Vector2>& uvs = mesh->GetUVs();
    int numVertices = int(vertices.size());
    
    std::vector<Vector3> projectedVertices;
    projectedVertices.resize(numVertices);
    
    for(int i=0; i < numVertices; i++){
        Vector3 world = model * vertices[i];
        Vector3 clip = camera->ProjectVector(world);
    
        projectedVertices[i] = clip;
    }
    
    for(int i=0; i +2 < numVertices; i +=3){
        Vector3 v0 = projectedVertices[i];
        Vector3 v1 = projectedVertices[i+1];
        Vector3 v2 = projectedVertices[i+2];
        
        
        if ((v0.x < -1 && v1.x < -1 && v2.x < -1) || (v0.x >  1 && v1.x >  1 && v2.x >  1) ||
            (v0.y < -1 && v1.y < -1 && v2.y < -1) || (v0.y >  1 && v1.y >  1 && v2.y >  1) ||
            (v0.z < -1 && v1.z < -1 && v2.z < -1) || (v0.z >  1 && v1.z >  1 && v2.z >  1)){
            continue;
        }
        
        
        
        int width = framebuffer->width;
        int height = framebuffer->height;
        

        int x0 = (int)((v0.x + 1.0f) * 0.5f * width);
        int y0 = (int)((1.0f + v0.y) * 0.5f * height);

        int x1 = (int)((v1.x + 1.0f) * 0.5f * width);
        int y1 = (int)((1.0f + v1.y) * 0.5f * height);

        int x2 = (int)((v2.x + 1.0f) * 0.5f * width);
        int y2 = (int)((1.0f + v2.y) * 0.5f * height);
        
        
        Vector3 p0((v0.x + 1.0f) * 0.5f * width,  (1.0f + v0.y) * 0.5f * height, v0.z);
        Vector3 p1((v1.x + 1.0f) * 0.5f * width,  (1.0f + v1.y) * 0.5f * height, v1.z);
        Vector3 p2((v2.x + 1.0f) * 0.5f * width,  (1.0f + v2.y) * 0.5f * height, v2.z);

                
        Color c0 = Color::RED;
        Color c1 = Color::GREEN;
        Color c2 = Color::BLUE;
        Vector2 uv0 = uvs[i];
        Vector2 uv1 = uvs[i + 1];
        Vector2 uv2 = uvs[i + 2];

                
        //framebuffer->DrawTriangleInterpolated(p0, p1, p2,Color::WHITE, Color::WHITE, Color::WHITE,zBuffer,texture,uv0, uv1, uv2);

                
        //   sTriangleInfo tri;
        //  tri.p0 = p0;
        //  tri.p1 = p1;
        //  tri.p2 = p2;

        //  tri.c0 = Color::WHITE;
        //   tri.c1 = Color::WHITE;
        //  tri.c2 = Color::WHITE;

        //   tri.uv0 = uv0;
        //  tri.uv1 = uv1;
     //   tri.uv2 = uv2;

       // tri.texture = texture;

       // framebuffer->DrawTriangleInterpolated(tri, zBuffer);
        if (render_mode == eRenderMode::WIREFRAME)
        {
            framebuffer->DrawLineDDA((int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, Color::WHITE);
            framebuffer->DrawLineDDA((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, Color::WHITE);
            framebuffer->DrawLineDDA((int)p2.x, (int)p2.y, (int)p0.x, (int)p0.y, Color::WHITE);
        }
        else if (render_mode == eRenderMode::TRIANGLES_INTERPOLATED)
       {
           sTriangleInfo tri;
           tri.p0 = p0;
           tri.p1 = p1;
           tri.p2 = p2;

           tri.c0 = c;
           tri.c1 = c;
           tri.c2 = c;

           if (use_interpolated_uv && !uvs.empty())
           {
               tri.uv0 = uvs[i];
               tri.uv1 = uvs[i + 1];
               tri.uv2 = uvs[i + 2];
           }
           else
           {
               tri.uv0 = Vector2(0, 0);
               tri.uv1 = Vector2(0, 0);
               tri.uv2 = Vector2(0, 0);
           }

           tri.texture = use_texture ? texture : NULL;

           framebuffer->DrawTriangleInterpolated(tri, use_occlusion ? zBuffer : NULL);
       }

    }
    
}


void Entity::Update(float seconds_elapsed)
{

    

    //Matrix44 rotation;
    //rotation.MakeRotationMatrix(seconds_elapsed, Vector3(0, 1, 0));

    //model = model * rotation;*/
    
    
   
    Matrix44 t;
    t.MakeTranslationMatrix(0.0, 0.5*sin(seconds_elapsed), 0.0);
    model = model * t;
    
}

