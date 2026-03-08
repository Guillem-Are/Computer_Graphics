//
//  material.hpp
//  ComputerGraphics
//
//  Created by Sara Gonzalez on 7/3/26.
//

#ifndef material_h
#define material_h


#include <stdio.h>
#include <vector>
#pragma once
#include "shader.h"
#include "texture.h"
#include "framework.h"

struct sLight {
    Vector3 position;
    Vector3 diffuse;  // renamed from color to diffuse for consistency
};

struct sUniformData {
    Matrix44 model;
    Matrix44 viewprojection;
    Vector3  ambient_light;
    Vector3  camera_position;
    std::vector<sLight> lights;   // vector, not raw pointer
    bool is_first_pass = true;
    sLight current_light;
    int num_lights = 1;
};

class Material {
public:
    Shader*  shader = nullptr;
    Texture* color_texture  = nullptr;
    Texture* normal_texture = nullptr;

    Vector3 Ka = Vector3(0.1f, 0.1f, 0.1f);
    Vector3 Kd = Vector3(0.8f, 0.8f, 0.8f);
    Vector3 Ks = Vector3(0.5f, 0.5f, 0.5f);
    float shininess = 32.0f;

    bool use_color_texture  = true;
    bool use_normal_texture = false;
    bool use_specular_texture = false;

    void Enable(const sUniformData& uniformData);
    void Disable();
};

#endif /* material_h */
