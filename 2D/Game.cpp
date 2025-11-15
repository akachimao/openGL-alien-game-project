#include "Game.h"
#include "shader.hpp"
#include <thread>
#include <chrono>


Game::Game()
    : player(glm::vec3(0.0f, 0.3f, 0.0f)),
    cameraPos(0.0f, 0.0f, 0.0f),
    cameraSmooth(0.1f) {
    initWindow();
    initOpenGL();
    initGeometry();
    createPlatforms();
    createSpikes();
    createEnemies();
    createRockets();
}

Game::~Game() {
    cleanup();
}

void Game::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (game) {
        game->onWindowResize(width, height);
    }
}

void Game::onWindowResize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
    glViewport(0, 0, width, height);
}

void Game::initWindow() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(-1);
    }

    window = glfwCreateWindow(width, height, "2D Platformer", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Enable vsync for consistent frame timing
    glfwSwapInterval(1);
}

void Game::initOpenGL() {
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        exit(-1);
    }

    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    programID = LoadShaders("SimpleVertexShader.vertexshader",
        "SimpleFragmentShader.fragmentshader");

    if (programID == 0) {
        fprintf(stderr, "Failed to load shaders\n");
        glfwTerminate();
        exit(-1);
    }
    Spike::initMesh();
	Rocket::initMesh();

    // Cache uniform locations during initialization
    transformLoc = glGetUniformLocation(programID, "transform");
    colorLoc = glGetUniformLocation(programID, "color");

    if (transformLoc == -1 || colorLoc == -1) {
        fprintf(stderr, "Failed to get uniform locations\n");
        glDeleteProgram(programID);
        glfwTerminate();
        exit(-1);
    }
}

void Game::initGeometry() {
    GLfloat vertices[] = {
        0.5f,  0.5f, 0.0f,	// top right
        0.5f, -0.5f, 0.0f,	// bottom right
        -0.5f, -0.5f, 0.0f,	// bottom left
        -0.5f,  0.5f, 0.0f	// top left 
    };

    GLuint indices[] = {
        0, 3, 1,
        1, 3, 2,
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO to avoid accidental modifications
    glBindVertexArray(0);
}

void Game::createPlatforms() {
    platforms.reserve(10);  

  
    platforms.push_back(Platform(glm::vec3(0.0f, -0.5f, 0.0f), 5.0f, 0.2f)); 
    platforms.push_back(Platform(glm::vec3(1.5f, -0.1f, 0.0f), 1.0f, 0.2f)); 
    platforms.push_back(Platform(glm::vec3(2.8f, 0.3f, 0.0f), 1.2f, 0.2f));  
  
    platforms.push_back(Platform(glm::vec3(4.5f, 0.6f, 0.0f), 1.5f, 0.2f));
    platforms.push_back(Platform(glm::vec3(6.3f, 0.2f, 0.0f), 1.0f, 0.2f));
    platforms.push_back(Platform(glm::vec3(0.0f, 0.8f, 0.0f), 0.6f, 0.2f));
    platforms.push_back(Platform(glm::vec3(10.0f, -0.5f, 0.0f), 10.f, 0.2f));
    platforms.push_back(Platform(glm::vec3(0.7f, 0.3f, 0.0f), 0.6f, 0.2f));
	platforms.push_back(Platform(glm::vec3(-1.47f, -0.0f, 0.0f), 0.2f, 5.0f));
    platforms.push_back(Platform(glm::vec3(8.5f, -0.0f, 0.0f), 0.2f, 5.0f));
}

void Game::createSpikes() {
    spikes.reserve(8);

    spikes.push_back(Spike(glm::vec3(1.0f, -0.35f-player.size/2, 0.0f), 0.1f, 0.1f));//w//h
    spikes.push_back(Spike(glm::vec3(1.6f, 0.05-player.size/2, 0.0f), 0.1f, 0.1f));
    spikes.push_back(Spike(glm::vec3(1.7f, 0.05-player.size/2, 0.0f), 0.1f, 0.1f));
    spikes.push_back(Spike(glm::vec3(5.3f, -0.350f - player.size / 2, 0.0f), 0.1f, 0.1f));
    spikes.push_back(Spike(glm::vec3(5.4f, -0.350f - player.size / 2, 0.0f), 0.1f, 0.1f));
    spikes.push_back(Spike(glm::vec3(5.5f, -0.350f - player.size / 2, 0.0f), 0.1f, 0.1f));
    spikes.push_back(Spike(glm::vec3(5.6f, -0.350f - player.size / 2, 0.0f), 0.1f, 0.1f));
    spikes.push_back(Spike(glm::vec3(5.7f, -0.350f - player.size / 2, 0.0f), 0.1f, 0.1f));
}

void Game::createEnemies() {
    enemies.reserve(3); 

    
    enemies.push_back(Enemy(glm::vec3(2.78f, 0.45f-player.size/2, 0.0f)));
    enemies.push_back(Enemy(glm::vec3(0.0f, 0.950f-player.size/2, 0.0f)));
    enemies.push_back(Enemy(glm::vec3( 6.2f, 0.350f-player.size/2, 0.0f)));
    
    
    enemies[1].direction = -1;
}

void Game::createRockets() {
    rockets.reserve(1);
    rockets.push_back(Rocket(glm::vec3(8.0f, 0.0f-3*player.size/2, 0.0f), 0.18f, 0.45f, glm::vec4(0.9f,0.2f,0.2f,1.0f)));
}

void Game::checkCollisions() {
    player.isGrounded = false;

   
    for (Platform& platform : platforms) {
        
        bool overlapX = player.getRight() > platform.getLeft() && player.getLeft() < platform.getRight();
        bool overlapY = player.getTop() > platform.getBottom() && player.getBottom() < platform.getTop();

        if (!overlapX || !overlapY) continue;

        // penetration depths
        float penX = std::min(player.getRight(), platform.getRight()) - std::max(player.getLeft(), platform.getLeft());
        float penY = std::min(player.getTop(), platform.getTop()) - std::max(player.getBottom(), platform.getBottom());

        // Decide resolution axis by smallest penetration
        if (penX < penY) {
            
            if (player.position.x < platform.position.x) player.position.x -= penX;
            else player.position.x += penX;
            player.velocity.x = 0.0f;
        } else {

            const float eps = 0.01f;
            const float minDownVelocity = -0.05f; 

            bool wasAbove = prevPlayerBottom >= platform.getTop() - eps;
            bool isFallingEnough = player.velocity.y <= minDownVelocity;

            if (wasAbove || isFallingEnough) {
                // land on top
                player.position.y = platform.getTop() + player.size / 2.0f;
                player.velocity.y = 0.0f;
                player.isGrounded = true;
            } else {
              
                if (player.position.y < platform.position.y) {
                   
                    player.position.y = platform.getBottom() - player.size / 2.0f;
                } else {
                  
                    player.position.y = platform.getTop() + player.size / 2.0f + eps;
                    player.isGrounded = true;
                    player.velocity.y = 0.0f;
                }
            }
        }
    }

    // spikes collision 
    for (const Spike& spike : spikes) {
        bool horizontalOverlap = player.getRight() > spike.getLeft() &&
            player.getLeft() < spike.getRight();
        bool verticalOverlap = player.getBottom() <= spike.getTop() &&
            player.getTop() >= spike.getBottom();

        if (horizontalOverlap && verticalOverlap) {
            player.velocity = glm::vec3(0.0f);
            player.speed = 0.0f;
            player.jumpForce = 0.0f;
            player.color = glm::vec4(1.0f, 0.2f, 0.2f, 1.0f);
            break;
        }
    }

    // enemies collision 
    for (const Enemy& enemy : enemies) {
        bool horizontalOverlap = player.getRight() > enemy.getLeft() &&
            player.getLeft() < enemy.getRight();
        bool verticalOverlap = player.getBottom() <= enemy.getTop() &&
            player.getTop() >= enemy.getBottom();

        if (horizontalOverlap && verticalOverlap) {
            player.velocity = glm::vec3(0.0f);
            player.speed = 0.0f;
            player.jumpForce = 0.0f;
            player.color = glm::vec4(1.0f, 0.2f, 0.2f, 1.0f);
            break;
        }
    }

    // rockets collision 
    for (const Rocket& rocket : rockets) {
        bool horizontalOverlap = player.getRight() > rocket.getLeft() &&
            player.getLeft() < rocket.getRight();
        bool verticalOverlap = player.getBottom() <= rocket.getTop() &&
            player.getTop() >= rocket.getBottom();

        if (horizontalOverlap && verticalOverlap) {
            // Player reaches rocket
            player.velocity = glm::vec3(0.0f);
            player.speed = 0.0f;
            player.jumpForce = 0.0f;
            player.color = glm::vec4(0.2f, 0.4f, 1.0f, 1.0f);
            break;
        }
    }

    // lose condition 
    const float loseY = -1.0f;
    if (player.position.y < loseY) {
        player.velocity = glm::vec3(0.0f);
        player.speed = 0.0f;
        player.jumpForce = 0.0f;
        player.color = glm::vec4(1.0f, 0.2f, 0.2f, 1.0f);
    }
}

void Game::updateCamera() {
    cameraPos.x += (player.position.x - cameraPos.x) * cameraSmooth;
    
    float targetY = std::max(player.position.y, 0.30f);
    cameraPos.y += (targetY - cameraPos.y) * cameraSmooth;
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(programID);
    glBindVertexArray(vao);

    glm::mat4 view = glm::translate(glm::mat4(1.0f), -cameraPos);

    // Draw all platforms
    for (Platform& platform : platforms) {
        platform.draw(transformLoc, colorLoc, view);
    }

    // Draw spikes
    for (const Spike& spike : spikes) {
        spike.draw(transformLoc, colorLoc, view);
    }

    // Draw enemies
    for (const Enemy& enemy : enemies) {
        enemy.draw(transformLoc, colorLoc, view);
    }

    // Draw rockets
    for (const Rocket& rocket : rockets) {
        rocket.draw(transformLoc, colorLoc, view);
    }

    // Draw player
    player.draw(transformLoc, colorLoc, view);

    glBindVertexArray(0);  // Unbind VAO after rendering
}

void Game::run() {
    const double targetFPS = 60.0;
    const double targetFrameTime = 0.4 / targetFPS; // seconds per frame

    double lastPrintTime = 0.0;
    const double printInterval = 0.25; // seconds

    while (!glfwWindowShouldClose(window)) {
        double frameStart = glfwGetTime();

       
        prevPlayerBottom = player.getBottom();

        player.handleInput(window);
        player.update();

        // Update all enemies (now expects spikes too)
        for (Enemy& enemy : enemies) {
            enemy.update(platforms, spikes);
        }

        checkCollisions();
        updateCamera();
        render();

        // Print player coordinates 
        double now = glfwGetTime();
        if (now - lastPrintTime >= printInterval) {
            lastPrintTime = now;
            printf("Player position: x=%.3f, y=%.3f\n", player.position.x, player.position.y);
            if (!platforms.empty()) printf("platform 0 position: x=%.3f, y=%.3f\n", platforms[0].position.x, platforms[0].position.y);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        // --- Frame cap to targetFPS using glfwGetTime and sleeping ---
        double frameEnd = glfwGetTime();
        double frameDuration = frameEnd - frameStart;
        double timeToSleep = targetFrameTime - frameDuration;
        if (timeToSleep > 0.0) {
            // convert to microseconds to preserve precision
            long long sleepUs = static_cast<long long>(timeToSleep * 1e6);
            std::this_thread::sleep_for(std::chrono::microseconds(sleepUs));
        }
        
    }
}

void Game::cleanup() {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(programID);

    
    Spike::cleanupMesh();
    Rocket::cleanupMesh();
    glfwTerminate();
}