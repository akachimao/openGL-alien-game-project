#pragma once
#include "Platform.h"

Platform::Platform(glm::vec3 pos, float w, float h, glm::vec4 col)
    : position(pos), width(w), height(h), color(col) {
}

void Platform::draw(GLuint transformLoc, GLuint colorLoc, glm::mat4 view) {
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, position);
    transform = glm::scale(transform, glm::vec3(width, height, 1.0f));
    transform = view * transform;

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glUniform4fv(colorLoc, 1, glm::value_ptr(color));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

float Platform::getLeft() const {
    return position.x - width / 2;
}

float Platform::getRight() const {
    return position.x + width / 2;
}

float Platform::getTop() const {
    return position.y + height / 2;
}

float Platform::getBottom() const {
    return position.y - height / 2;
}