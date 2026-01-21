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
        particles[i].position = Vector2(rand() % 1280, rand() % 720); // random position in screen
        float speed = 10.0f + static_cast<float>(rand() % 15); // random speed
        particles[i].velocity = Vector2(0.0f, speed);
        
        particles[i].color = Color(rand() % 256, rand() % 256, rand() % 256);
 // random color
       // particles[i].acceleration = 30.0f;   // no acceleration for now
        particles[i].ttl = 1.0f + static_cast<float>(rand() % 100) / 100.0f;
        particles[i].inactive = false;
    }
}
void ParticleSystem::Update(float dt) {
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        Particle& p = particles[i];

        if (p.inactive) continue;

        // Update position using velocity
        p.position.x += p.velocity.x * dt;
        p.position.y += p.velocity.y * dt;

        // Optional: apply acceleration
       // p.velocity.x += p.velocity.x * p.acceleration * dt;
      //  p.velocity.y += p.velocity.y * p.acceleration * dt;


        // Decrease TTL
        //p.ttl -= dt;
        //if (p.ttl <= 0.0f) {
          //  p.inactive = true; // particle expired
       // }
        if (p.position.y > 720) {   // screen height
                    p.position.y = 0;       // reset to top
                    p.position.x = rand() % 1280;  // random x
                }
                if (p.position.x < 0) p.position.x += 1280;
                if (p.position.x > 1280) p.position.x -= 1280;
    }
}
void ParticleSystem::Render(Image* framebuffer) {
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        Particle& p = particles[i];
        if (p.inactive) continue;

        int x = static_cast<int>(p.position.x);
        int y = static_cast<int>(p.position.y);

        // Draw pixel (you can improve to draw circles)
        framebuffer->SetPixel(x, y, p.color);
    }
}
