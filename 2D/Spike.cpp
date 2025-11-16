#include "Spike.h"


static GLuint s_spikeVao = 0;
static GLuint s_spikeVbo = 0;
static GLuint s_spikeIbo = 0;
static bool s_spikeMeshInitialized = false;

Spike::Spike(glm::vec3 pos, float w, float h, glm::vec4 col)
    : position(pos), width(w), height(h), color(col) {
}

void Spike::initMesh() {
    if (s_spikeMeshInitialized) return;

    
    const GLfloat vertices[] = {
        -0.75f, 0.0f, 0.0f,  
         -0.5f, 0.5f, 0.0f,  
		 -0.25f,  0.0f, 0.0f,
			0.0f, 0.5f, 0.0f, 
			0.25f, 0.0f, 0.0f,
			0.5f, 0.5f, 0.0f,
			0.75f, 0.0f, 0.0f, 
            0.0f,0.0f,0.0f
        
    };

    const GLuint indices[] = { 
        0, 1, 2,
		2,3, 4,
		4,5,6,
        0,6,7
    
    };

    glGenVertexArrays(1, &s_spikeVao);
    glGenBuffers(1, &s_spikeVbo);
    glGenBuffers(1, &s_spikeIbo);

    glBindVertexArray(s_spikeVao);

    glBindBuffer(GL_ARRAY_BUFFER, s_spikeVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_spikeIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    s_spikeMeshInitialized = true;
}

void Spike::cleanupMesh() {
    if (!s_spikeMeshInitialized) return;

    if (s_spikeIbo) { glDeleteBuffers(1, &s_spikeIbo); s_spikeIbo = 0; }
    if (s_spikeVbo) { glDeleteBuffers(1, &s_spikeVbo); s_spikeVbo = 0; }
    if (s_spikeVao) { glDeleteVertexArrays(1, &s_spikeVao); s_spikeVao = 0; }

    s_spikeMeshInitialized = false;
}

void Spike::draw(GLuint transformLoc, GLuint colorLoc, glm::mat4 view) const {
    // Ensure mesh exists (safe: will initialize only once)
    if (!s_spikeMeshInitialized) initMesh();

    
    GLint prevVao = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prevVao);

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, position);
    transform = glm::scale(transform, glm::vec3(width, height, 1.0f));
    transform = view * transform;

    
    glBindVertexArray(s_spikeVao);
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glUniform4fv(colorLoc, 1, glm::value_ptr(color));
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    
    glBindVertexArray((GLuint)prevVao);
}

float Spike::getLeft() const {
    return position.x - width / 2;
}

float Spike::getRight() const {
    return position.x + width / 2;
}

float Spike::getTop() const {
    return position.y + height / 2;
}

float Spike::getBottom() const {
    return position.y - height / 2;
}
