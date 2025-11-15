#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "dependente\glew\glew.h"
#include "dependente\glfw\glfw3.h"
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

class Player {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    float size;
    float speed;
    float jumpForce;
    float gravity;
    bool isGrounded;
    glm::vec4 color;

    Player(glm::vec3 pos, float sz = 0.1f);

    void handleInput(GLFWwindow* window);
    void update();
    void draw(GLuint transformLoc, GLuint colorLoc, glm::mat4 view);

    float getLeft() const;
    float getRight() const;
    float getBottom() const;
    float getTop() const;
};