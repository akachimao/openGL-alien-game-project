
#pragma once
#include "dependente\glew\glew.h"
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

class Platform {
public:
    glm::vec3 position;
    float width;
    float height;
    glm::vec4 color;

    Platform(glm::vec3 pos, float w, float h, glm::vec4 col = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));

    void draw(GLuint transformLoc, GLuint colorLoc, glm::mat4 view);

    float getLeft() const;
    float getRight() const;
    float getTop() const;
    float getBottom() const;
};
