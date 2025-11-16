#pragma once

#include <vector>
#include "dependente\glew\glew.h"
#include "dependente\glfw\glfw3.h"
#include "dependente\glm\glm.hpp"
#include "Player.h"
#include "Platform.h"
#include "Spike.h"
#include "Enemy.h" 
#include "Rocket.h"

class Game {
private:
    GLFWwindow* window;
    int width = 1920;
    int height = 1080;
    GLuint programID;
    GLuint vao, vbo, ibo;

    GLint transformLoc, colorLoc;  

    Player player;
    std::vector<Platform> platforms;
    std::vector<Spike> spikes;     // added spikes
    std::vector<Enemy> enemies;   // vector to hold enemies
    std::vector<Rocket> rockets;  // vector to hold rockets
    glm::vec3 cameraPos;
    float cameraSmooth;

    // track player's previous bottom y so collisions can use previous state
    /*float prevPlayerBottom = 0.0f;*/

    void initWindow();
    void initOpenGL();
    void initGeometry();
    void createPlatforms();
    void createSpikes();           // create spikes on bottom platform
    void createEnemies();         // method to create enemies
    void createRockets();
    void checkCollisions();
    void updateCamera();
    void render();
    void cleanup();

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

public:
    Game();
    ~Game();
    void run();
    void onWindowResize(int newWidth, int newHeight);
};
