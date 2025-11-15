#pragma once

#include "dependente\glew\glew.h"
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

class Spike {
public:
    glm::vec3 position;
    float width;
    float height;
    glm::vec4 color;

    Spike(glm::vec3 pos, float w, float h, glm::vec4 col = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));

    void draw(GLuint transformLoc, GLuint colorLoc, glm::mat4 view) const;

    float getLeft() const;
    float getRight() const;
    float getTop() const;
    float getBottom() const;

    
    static void initMesh();
    static void cleanupMesh();
};