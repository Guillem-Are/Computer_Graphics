//
//  ParticleSystem.cpp
//  ComputerGraphics
//
//  Created by Guillem Arevalo Morell on 20/1/26.
//
#include "ParticleSystem.h"
#include "image.h"
#include <cstdlib>
#include <cmath>


void ParticleSystem::Init() {
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        particles[i].position = Vector2(rand() % 1500, rand() % 1000); // random position in screen
        float vy = 10.0f + static_cast<float>(rand() % 15); // random speed
        float vx = -3.0f + rand() % 6;
        particles[i].velocity = Vector2(vx, vy);
        
        particles[i].color = Color(rand() % 256, rand() % 256, rand() % 256);
 // random color
    
        particles[i].ttl = 1.0f + static_cast<float>(rand() % 100) / 100.0f;
        particles[i].inactive = false;
    }
}
void ParticleSystem::Update(float dt) {
    dt = std::min(dt, 0.033f);
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        Particle& p = particles[i];
        // Update position using velocity
        p.position.y += -p.velocity.y * dt;
       
        if (p.position.y < 0) {   //screen height
                    p.position.y = 1000;       // reset to top
                    p.position.x = rand() % 1500;  // random x
                }
                if (p.position.x < 0) p.position.x += 1500;
                if (p.position.x > 1500) p.position.x -= 1500;
    }
}
void ParticleSystem::Render(Image* framebuffer) {
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        Particle& p = particles[i];
        if (p.inactive) continue;

        int x = static_cast<int>(p.position.x);
        int y = static_cast<int>(p.position.y);

        //draw pixel
        framebuffer->SetPixel(x, y, p.color);
    }
}


