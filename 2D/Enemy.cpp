#include "Enemy.h"
#include "Platform.h"
#include "Spike.h"
#include <algorithm>

Enemy::Enemy(glm::vec3 pos, float sz)
    : position(pos),
      velocity(0.0f, 0.0f, 0.0f),
      size(sz),
      speed(0.1f),
      gravity(9.81f),
      isGrounded(false),
      color(1.0f, 0.5f, 0.0f, 1.0f),
      direction(1),
      pauseTimer(0.0f) {
}

void Enemy::update(const std::vector<Platform>& platforms, const std::vector<Spike>& spikes) {
    const float dt = 0.016f; 

    if (pauseTimer > 0.0f) {
        pauseTimer -= dt;
        if (pauseTimer < 0.0f) pauseTimer = 0.0f;
    }

    
    if (!isGrounded) {
        velocity.y -= gravity * dt;
    }

    
    if (pauseTimer > 0.0f) {
        velocity.x = 0.0f;
    } else {
        velocity.x = direction * speed;
    }

  
    position += velocity * dt;

    
    isGrounded = false;

    for (const Platform& platform : platforms) {
        bool horizontalOverlap = getRight() > platform.getLeft() &&
                                 getLeft() < platform.getRight();

        if (horizontalOverlap) {
            // Landing from above
            if (velocity.y <= 0.0f &&
                getBottom() <= platform.getTop() &&
                getBottom() >= platform.getBottom()) {
                position.y = platform.getTop() + size / 2.0f;
                velocity.y = 0.0f;
                isGrounded = true;
            }
            // Hitting bottom of platform
            else if (velocity.y > 0.0f &&
                     getTop() >= platform.getBottom() &&
                     getTop() <= platform.getTop()) {
                position.y = platform.getBottom() - size / 2.0f;
                velocity.y = 0.0f;
            }
        }
    }

    // Spike collision: if enemy touches a spike, reverse direction, pause briefly and move slightly away
    for (const Spike& spike : spikes) {
        bool horizOverlap = getRight() > spike.getLeft() &&
                            getLeft() < spike.getRight();
        bool vertOverlap = getBottom() <= spike.getTop() &&
                           getTop() >= spike.getBottom();

        if (horizOverlap && vertOverlap) {
            // Reverse direction
            direction = -direction;

            // Small nudge to avoid sticking inside spike
            const float nudge = (size * 0.5f) + 0.02f;
            position.x += direction * nudge;

            // Briefly stop horizontal movement (stun) so it "stops and turns"
            pauseTimer = 0.15f; // 150 ms

            // Don't process more spikes this frame
            break;
        }
    }

    // Turn around at platform edges (only if not currently paused)
    if (pauseTimer <= 0.0f) {
        checkPlatformBoundaries(platforms);
    }
}

void Enemy::checkPlatformBoundaries(const std::vector<Platform>& platforms) {
    const float eps = 0.01f;

    for (const Platform& platform : platforms) {
        bool onPlatformVertically = getBottom() >= platform.getBottom() - eps &&
                                    getBottom() <= platform.getTop() + eps;
        bool horizontallyOverPlatform = getRight() > platform.getLeft() + eps &&
                                        getLeft() < platform.getRight() - eps;

        if (onPlatformVertically && horizontallyOverPlatform) {
            if (direction == -1 && getLeft() <= platform.getLeft() + eps) {
                direction = 1;
                position.x = platform.getLeft() + size / 2.0f + eps;
            } else if (direction == 1 && getRight() >= platform.getRight() - eps) {
                direction = -1;
                position.x = platform.getRight() - size / 2.0f - eps;
            }
            return;
        }
    }
}

void Enemy::draw(GLuint transformLoc, GLuint colorLoc, glm::mat4 view) const {
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, position);
    transform = glm::scale(transform, glm::vec3(size, size, 1.0f));
    transform = view * transform;

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glUniform4fv(colorLoc, 1, glm::value_ptr(color));
    // Uses the game's square VAO (bound before calling draw)
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

float Enemy::getLeft() const  { return position.x - size / 2.0f; }
float Enemy::getRight() const { return position.x + size / 2.0f; }
float Enemy::getBottom() const{ return position.y - size / 2.0f; }
float Enemy::getTop() const   { return position.y + size / 2.0f; }