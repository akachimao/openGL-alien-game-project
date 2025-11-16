#include "Player.h"

Player::Player(glm::vec3 pos, float sz)
    : position(pos),
    velocity(0.0f, 0.0f, 0.0f),
    size(sz),
    speed(0.01f),
    jumpForce(0.04f),
    gravity(-0.001f),
    isGrounded(false),
    color(0.7f, 0.7f, 0.7f, 1.0f)
{
}

void Player::handleInput(GLFWwindow* window) {
    // horizontal movement
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position.x -= speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position.x += speed;
    }
    // jump
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
    glm::vec4 lightGray = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);  // Astronaut body color
    glm::vec4 blackVisor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);  // Visor color

    // if the player dies from collision we use the death color for entire astronaut
    if (color.r > 0.9f && color.g < 0.3f) {  // Red (death)
        lightGray = color;
        blackVisor = color;
    }
    else if (color.b > 0.9f && color.r < 0.3f) {  // Blue (win)
        lightGray = color;
        blackVisor = color;
    }

    
    // left leg
    glm::mat4 leftLeg = glm::mat4(1.0f);
    leftLeg = glm::translate(leftLeg, position + glm::vec3(-size * 0.15f, -size * 0.6f, 0.0f));
    leftLeg = glm::scale(leftLeg, glm::vec3(size * 0.25f, size * 0.4f, 1.0f));
    leftLeg = view * leftLeg;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(leftLeg));
    glUniform4fv(colorLoc, 1, glm::value_ptr(lightGray));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // right leg
    glm::mat4 rightLeg = glm::mat4(1.0f);
    rightLeg = glm::translate(rightLeg, position + glm::vec3(size * 0.15f, -size * 0.6f, 0.0f));
    rightLeg = glm::scale(rightLeg, glm::vec3(size * 0.25f, size * 0.4f, 1.0f));
    rightLeg = view * rightLeg;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(rightLeg));
    glUniform4fv(colorLoc, 1, glm::value_ptr(lightGray));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //body
    glm::mat4 torso = glm::mat4(1.0f);
    torso = glm::translate(torso, position + glm::vec3(0.0f, -size * 0.1f, 0.0f));
    torso = glm::scale(torso, glm::vec3(size * 0.6f, size * 0.7f, 1.0f));
    torso = view * torso;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(torso));
    glUniform4fv(colorLoc, 1, glm::value_ptr(lightGray));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

   
    // left arm
    glm::mat4 leftArm = glm::mat4(1.0f);
    leftArm = glm::translate(leftArm, position + glm::vec3(-size * 0.4f, -size * 0.05f, 0.0f));
    leftArm = glm::scale(leftArm, glm::vec3(size * 0.2f, size * 0.5f, 1.0f));
    leftArm = view * leftArm;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(leftArm));
    glUniform4fv(colorLoc, 1, glm::value_ptr(lightGray));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // right arm
    glm::mat4 rightArm = glm::mat4(1.0f);
    rightArm = glm::translate(rightArm, position + glm::vec3(size * 0.4f, -size * 0.05f, 0.0f));
    rightArm = glm::scale(rightArm, glm::vec3(size * 0.2f, size * 0.5f, 1.0f));
    rightArm = view * rightArm;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(rightArm));
    glUniform4fv(colorLoc, 1, glm::value_ptr(lightGray));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // head
    glm::mat4 head = glm::mat4(1.0f);
    head = glm::translate(head, position + glm::vec3(0.0f, size * 0.4f, 0.0f));
    head = glm::scale(head, glm::vec3(size * 0.45f, size * 0.45f, 1.0f));
    head = view * head;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(head));
    glUniform4fv(colorLoc, 1, glm::value_ptr(lightGray));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // visor
    glm::mat4 visor = glm::mat4(1.0f);
    visor = glm::translate(visor, position + glm::vec3(0.0f, size * 0.4f, 0.0f));
    visor = glm::scale(visor, glm::vec3(size * 0.35f, size * 0.2f, 1.0f));
    visor = view * visor;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(visor));
    glUniform4fv(colorLoc, 1, glm::value_ptr(blackVisor));
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
