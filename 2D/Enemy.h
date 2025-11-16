#pragma once

#include <vector>
#include "dependente\glew\glew.h"
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

class Platform;
class Spike;

class Enemy {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    float size;
    float speed;
    float gravity;
    bool isGrounded;
    glm::vec4 color;
    int direction; // 1 = right, -1 = left

 
    float pauseTimer;

    Enemy(glm::vec3 pos, float sz = 0.08f);

   
    void update(const std::vector<Platform>& platforms, const std::vector<Spike>& spikes);

    void draw(GLuint transformLoc, GLuint colorLoc, glm::mat4 view) const;

    float getLeft() const;
    float getRight() const;
    float getBottom() const;
    float getTop() const;

private:
    void checkPlatformBoundaries(const std::vector<Platform>& platforms);
};