
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

    GLint prevVao = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prevVao);

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, position);
    transform = glm::scale(transform, glm::vec3(width, height, 1.0f));
    transform = view * transform;

    glBindVertexArray(s_rocketVao);

 
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glUniform4fv(colorLoc, 1, glm::value_ptr(color));

    
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

    glBindVertexArray((GLuint)prevVao);
}

float Rocket::getLeft() const { return position.x - width / 2.0f; }
float Rocket::getRight() const { return position.x + width / 2.0f; }
float Rocket::getTop() const { return position.y + height / 2.0f; }
float Rocket::getBottom() const { return position.y - height / 2.0f; }