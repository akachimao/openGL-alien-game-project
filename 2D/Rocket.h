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

    void initMesh();
    void cleanupMesh();

    void draw(GLuint transformLoc, GLuint colorLoc, glm::mat4 view) const;

    float getLeft() const;
    float getRight() const;
    float getTop() const;
    float getBottom() const;

	// launch control
    void startLaunch(float height = 10.0f); 
    void update(float dt);                  // advance animation by dt seconds
    bool isLaunching() const { return launching; }

private:
    // simple shared mesh
    static GLuint s_rocketVao;
    static GLuint s_rocketVbo;
    static GLuint s_rocketIbo;
    static bool   s_rocketInitialized;

    // launch state
    bool launching = false;
    float launchTargetY = 0.0f;
    float launchSpeed = 1.5f; // world units per second 
};