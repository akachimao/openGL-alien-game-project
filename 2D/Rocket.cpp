#include "Rocket.h"
#include "dependente\glm\gtc\type_ptr.hpp"

// Static definitions
GLuint Rocket::s_rocketVao = 0;
GLuint Rocket::s_rocketVbo = 0;
GLuint Rocket::s_rocketIbo = 0;
bool   Rocket::s_rocketInitialized = false;

Rocket::Rocket(glm::vec3 pos, float w, float h, glm::vec4 col)
    : position(pos), width(w), height(h), color(col) {
}

void Rocket::initMesh() {
    if (s_rocketInitialized) return;

    const GLfloat vertices[] = {
        -0.25f, -0.5f, 0.0f,
         0.25f, -0.5f, 0.0f,
        -0.25f,  0.0f, 0.0f,
         0.25f,  0.0f, 0.0f,
         0.00f,  0.5f, 0.0f
    };

    const GLuint indices[] = {
        0, 2, 1,
        1, 2, 3,
        2, 4, 3
    };

    glGenVertexArrays(1, &s_rocketVao);
    glGenBuffers(1, &s_rocketVbo);
    glGenBuffers(1, &s_rocketIbo);

    glBindVertexArray(s_rocketVao);

    glBindBuffer(GL_ARRAY_BUFFER, s_rocketVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_rocketIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    s_rocketInitialized = true;
}

void Rocket::cleanupMesh() {
    if (!s_rocketInitialized) return;
    if (s_rocketIbo) { glDeleteBuffers(1, &s_rocketIbo); s_rocketIbo = 0; }
    if (s_rocketVbo) { glDeleteBuffers(1, &s_rocketVbo); s_rocketVbo = 0; }
    if (s_rocketVao) { glDeleteVertexArrays(1, &s_rocketVao); s_rocketVao = 0; }
    s_rocketInitialized = false;
}

void Rocket::draw(GLuint transformLoc, GLuint colorLoc, glm::mat4 view) const {
    if (!s_rocketInitialized) return;

    glm::vec4 grayBody = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);    // Gray body
    glm::vec4 redParts = glm::vec4(0.9f, 0.2f, 0.2f, 1.0f);    // Red nose & fins

    // We'll use the game's global square VAO (already bound in Game::render)
    // No need to bind our own VAO

    // === ROCKET BODY (gray rectangle in center) ===
    glm::mat4 body = glm::mat4(1.0f);
    body = glm::translate(body, position);
    body = glm::scale(body, glm::vec3(width * 0.5f, height * 0.65f, 1.0f));
    body = view * body;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(body));
    glUniform4fv(colorLoc, 1, glm::value_ptr(grayBody));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // === NOSE/TOP (red square at top) ===
    glm::mat4 nose = glm::mat4(1.0f);
    nose = glm::translate(nose, position + glm::vec3(0.0f, height * 0.4f, 0.0f));
    nose = glm::scale(nose, glm::vec3(width * 0.3f, height * 0.25f, 1.0f));
    nose = view * nose;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(nose));
    glUniform4fv(colorLoc, 1, glm::value_ptr(redParts));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // === LEFT FIN (red square at bottom left) ===
    glm::mat4 leftFin = glm::mat4(1.0f);
    leftFin = glm::translate(leftFin, position + glm::vec3(-width * 0.35f, -height * 0.35f, 0.0f));
    leftFin = glm::scale(leftFin, glm::vec3(width * 0.25f, height * 0.2f, 1.0f));
    leftFin = view * leftFin;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(leftFin));
    glUniform4fv(colorLoc, 1, glm::value_ptr(redParts));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // === RIGHT FIN (red square at bottom right) ===
    glm::mat4 rightFin = glm::mat4(1.0f);
    rightFin = glm::translate(rightFin, position + glm::vec3(width * 0.35f, -height * 0.35f, 0.0f));
    rightFin = glm::scale(rightFin, glm::vec3(width * 0.25f, height * 0.2f, 1.0f));
    rightFin = view * rightFin;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(rightFin));
    glUniform4fv(colorLoc, 1, glm::value_ptr(redParts));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

float Rocket::getLeft() const { return position.x - width / 2.0f; }
float Rocket::getRight() const { return position.x + width / 2.0f; }
float Rocket::getTop() const { return position.y + height / 2.0f; }
float Rocket::getBottom() const { return position.y - height / 2.0f; }

void Rocket::startLaunch(float height) {
    if (!launching) {
        launching = true;
        launchTargetY = position.y + height;
    }
}

void Rocket::update(float dt) {
    if (!launching) return;

    position.y += launchSpeed * dt;

    if (position.y >= launchTargetY) {
        position.y = launchTargetY;
        launching = false;
    }
}