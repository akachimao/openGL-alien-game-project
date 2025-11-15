
#pragma once

#include "dependente\glew\glew.h"
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

class Rocket {
public:
    glm::vec3 position;
    float width;
    float height;
    glm::vec4 color;

    Rocket(glm::vec3 pos, float w = 0.2f, float h = 0.45f, glm::vec4 col = glm::vec4(0.9f,0.1f,0.1f,1.0f));

    void draw(GLuint transformLoc, GLuint colorLoc, glm::mat4 view) const;

    float getLeft() const;
    float getRight() const;
    float getTop() const;
    float getBottom() const;

    
    static void initMesh();
    static void cleanupMesh();

private:
    
    static GLuint s_rocketVao;
    static GLuint s_rocketVbo;
    static GLuint s_rocketIbo;
    static bool s_rocketInitialized;
};