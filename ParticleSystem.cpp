//
//  ParticleSystem.cpp
//  ComputerGraphics
//
//  Created by Guillem Arevalo Morell on 20/1/26.
//
#include "ParticleSystem.h"
#include "Image.h"

void ParticleSystem::Init() {
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        particles[i].position = Vector2(rand() % 800, rand() % 600); // random position in screen
        float angle = static_cast<float>(rand() % 360) * 3.14159f / 180.0f; // random direction
        float speed = 50.0f + static_cast<float>(rand() % 100); // random speed
        particles[i].velocity = Vector2(cos(angle) * speed, sin(angle) * speed);
        particles[i].color = Color(rand() % 256, rand() % 256, rand() % 256, 255); // random color
        particles[i].acceleration = 0.0f;   // no acceleration for now
        particles[i].ttl = 1.0f + static_cast<float>(rand() % 100) / 100.0f; // 1-2 seconds
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
        p.velocity.x += p.velocity.x * p.acceleration * dt;
        p.velocity.y += p.velocity.y * p.acceleration * dt;

        // Decrease TTL
        p.ttl -= dt;
        if (p.ttl <= 0.0f) {
            p.inactive = true; // particle expired
        }
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
