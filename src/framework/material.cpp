//
//  material.cpp
//  ComputerGraphics
//
//  Created by Sara Gonzalez on 7/3/26.
//

#include "material.h"

void Material::Enable(const sUniformData& uniformData)
{
    if (!shader) return;

    shader->Enable();

    // Matrices
    shader->SetMatrix44("u_model",uniformData.model);
    shader->SetMatrix44("u_viewprojection", uniformData.viewprojection);

    // Camera
    shader->SetUniform3("u_camera_position", uniformData.camera_position);

    // Ambient light
    shader->SetUniform3("u_ambient_light", uniformData.ambient_light);

    // First light
    if (!uniformData.lights.empty()) {
        shader->SetUniform3("u_light_position", uniformData.lights[0].position);
        shader->SetUniform3("u_light_color", uniformData.lights[0].diffuse);
    }

    // Material properties
    shader->SetUniform3("u_Ka", Ka);
    shader->SetUniform3("u_Kd", Kd);
    shader->SetUniform3("u_Ks", Ks);
    shader->SetUniform1("u_shininess", shininess);
    
    shader->SetUniform1("u_use_color_texture",    use_color_texture    ? 1 : 0);
    shader->SetUniform1("u_use_specular_texture", use_specular_texture ? 1 : 0);
    shader->SetUniform1("u_use_normal_texture",   use_normal_texture   ? 1 : 0);


    // Color texture on slot 0
    if (color_texture) {
        color_texture->Bind();
        shader->SetUniform1("u_color_texture", 0);
    }

    // Normal texture on slot 1
    if (normal_texture) {
        glActiveTexture(GL_TEXTURE1);
        normal_texture->Bind();
        shader->SetUniform1("u_normal_texture", 1);
        glActiveTexture(GL_TEXTURE0);
    }
    
    
}

void Material::Disable()
{
    if (shader)
        shader->Disable();
}
