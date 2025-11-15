#include "Player.h"

Player::Player(glm::vec3 pos, float sz)
    : position(pos), size(sz), speed(0.01f), jumpForce(0.04f),
    gravity(-0.001f), isGrounded(false),
    velocity(0.0f, 0.0f, 0.0f),
    color(0.5f, 0.0f, 0.5f, 1.0f) {
}

void Player::handleInput(GLFWwindow* window) {
    // Horizontal movement
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position.x -= speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position.x += speed;
    }

    // Jump
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && isGrounded) {
        velocity.y = jumpForce;
        isGrounded = false;
    }
}

void Player::update() {
    
    velocity.y += gravity;
    position.y += velocity.y;
}

void Player::draw(GLuint transformLoc, GLuint colorLoc, glm::mat4 view) {
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, position);
    transform = glm::scale(transform, glm::vec3(size, size, 1.0f));
    transform = view * transform;

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glUniform4fv(colorLoc, 1, glm::value_ptr(color));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

float Player::getLeft() const {
    return position.x - size / 2;
}

float Player::getRight() const {
    return position.x + size / 2;
}

float Player::getBottom() const {
    return position.y - size / 2;
}

float Player::getTop() const {
    return position.y + size / 2;
}