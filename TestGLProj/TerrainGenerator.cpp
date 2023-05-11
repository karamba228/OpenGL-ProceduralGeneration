#include "TerrainGenerator.h"
#include "Camera.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

//glm library
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <algorithm>

TerrainGenerator::TerrainGenerator(Shader* shader, int width, Shader* perlinShader)
    : m_shader(shader),
    m_perlinShader(perlinShader),
    m_width(width),
    m_span(width),
    m_vao(0),
    m_vbo(0),
    m_ibo(0),
    m_nbo(0)
    //m_fbo(0)
{
    glGenVertexArrays(1, &m_vao); // Generate vertex array object and bind it.
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo); // Generate vertex buffer object and bind it.
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glGenBuffers(1, &m_ibo); // Generate index buffer object and bind it.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glGenBuffers(1, &m_nbo); // Generate normals buffer object.

    //glGenFramebuffers(1, &m_fbo);
    //glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    //glGenTextures(1, &texture);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    //// Set texture parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //// Allocate memory for the texture
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, width, 0, GL_RGBA, GL_FLOAT, nullptr);

    //// Unbind framebuffer
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindVertexArray(m_vao);
}

void TerrainGenerator::render(glm::mat4 ModelView, glm::mat4 Projection)
{
    // Check if the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer is not complete!" << std::endl;
    }

    // Bind Perlin noise shader program
    //m_perlinShader->Activate();

    //// Set Perlin noise shader uniforms
    //m_perlinShader->SetUniform("uResolutionLoc", glm::vec2(m_span, m_width));
    //m_perlinShader->SetUniform("uNoiseFrequencyLoc", noiseFrequency);
    //m_perlinShader->SetUniform("uNoiseScaleLoc", noiseScale);

    //m_perlinShader->SetUniform("uOctaves", OCTAVES);
    //m_perlinShader->SetUniform("uPersistence", PERSISTENCE);
    //m_perlinShader->SetUniform("uAmplitude", AMPLITUDE);
    //m_perlinShader->SetUniform("uLacunarity", LACUNARITY);
    //m_perlinShader->SetUniform("uFrequency", FREQUENCY);
    //m_perlinShader->SetUniform("uNoiseScale", HEIGHT);
    //m_perlinShader->SetUniform("uSeed", SEED);

    //m_perlinShader->DeActivate();

    //// Bind FBO.
    //glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    //// Attach the noise texture to the framebuffer object.
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    //// Unbind the framebuffer object.
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //// Create a new texture object and copy the generated texture into it.
    //glBindTexture(GL_TEXTURE_2D, texture);
    //glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, m_span, m_width, 0);

    // Bind the shader program and set its uniforms.
    m_shader->Activate();
    m_shader->SetUniform("Projection", Projection);
    m_shader->SetUniform("ModelView", ModelView);
    m_shader->SetUniform("lightPosition", glm::vec4(50.0, 10.0, 50.0, 1.0));
    m_shader->SetUniform("uTexture", 0);

    //// Bind the texture to texture unit 0.
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, texture);

    // Bind the vertex array object and its buffers.
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableVertexAttribArray((*m_shader)["vertexPosition"]);
    glVertexAttribPointer((*m_shader)["vertexPosition"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_nbo);
    glEnableVertexAttribArray((*m_shader)["vertexNormal"]);
    glVertexAttribPointer((*m_shader)["vertexNormal"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

    // Draw using indices.
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind vertex array object.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Unbind shader.
    //glBindTexture(GL_TEXTURE_2D, 0);
    m_shader->DeActivate();
}

void TerrainGenerator::generateLevelPlane()
{
    // Clear any previous data
    m_vertices.clear();
    m_indices.clear();

    // Bind the VAO
    glBindVertexArray(m_vao);

    // Create a flat plane with a size x size grid of vertices
    for (int z = 0; z < m_span; z++)
    {
        for (int x = 0; x < m_width; x++)
        {
            // Set the position of the vertex
            float xPos = (float)x;
            float yPos = 0.0f;
            float zPos = (float)z;

            // Set the texture coordinates of the vertex
            float u = (float)x / (float)m_span;
            float v = (float)z / (float)m_width;

            // Add the vertex to the list of vertices
            m_vertices.push_back(glm::vec3(xPos, yPos, zPos));
        }
    }

    // Create the index buffer for the plane
    for (int z = 0; z < m_span - 1; z++)
    {
        for (int x = 0; x < m_width - 1; x++) 
        {
            // Calculate the indices of the four vertices that make up the current quad
            int topLeft = (z * (m_width)) + x;
            int topRight = topLeft + 1;
            int bottomLeft = ((z + 1) * (m_width)) + x;
            int bottomRight = bottomLeft + 1;

            // Add the indices to the list of indices
            m_indices.push_back(topLeft);
            m_indices.push_back(bottomLeft);
            m_indices.push_back(topRight);
            m_indices.push_back(topRight);
            m_indices.push_back(bottomLeft);
            m_indices.push_back(bottomRight);
        }
    }

    m_adjList.resize(m_vertices.size());
    normals.resize(m_vertices.size(), glm::vec3(0.0f));
    m_sediment.resize(m_vertices.size(), 0.0f);

    // Iterate over all triangles in the mesh
    for (int i = 0; i < m_indices.size(); i += 3) {
        // Get the indices of the three vertices of the triangle
        int i0 = m_indices[i];
        int i1 = m_indices[i + 1];
        int i2 = m_indices[i + 2];

        // Add neighboring vertices for each vertex in the triangle
        m_adjList[i0].push_back(i1);
        m_adjList[i0].push_back(i2);
        m_adjList[i1].push_back(i0);
        m_adjList[i1].push_back(i2);
        m_adjList[i2].push_back(i0);
        m_adjList[i2].push_back(i1);
    }

    updateBuffers();
}

void TerrainGenerator::applyHeightMap(const std::vector<float>& heightMap, float heightScale)
{
    if (heightMap.empty()) {
        // Reset the map to be flat
        for (int z = 0; z < m_span; z++) {
            for (int x = 0; x < m_width; x++) {
                int index = z * m_width + x;
                m_vertices[index].y = 0.0f;
            }
        }
    }
    else {
        if (heightMap.size() != m_width * m_span) {
            // The size of the height map doesn't match the size of the plane
            return;
        }

        // Loop through every vertex and apply the corresponding height value from the height map
        for (int z = 0; z < m_span; z++)
        {
            for (int x = 0; x < m_width; x++)
            {
                // Calculate the index of the current vertex
                int index = z * m_width + x;

                // Get the height value from the height map and apply the height scale
                float height = heightMap[index] * heightScale;

                // Update the y position of the vertex
                m_vertices[index].y = height;
            }
        }
    }

    // Update the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);

    updateBuffers();
}

glm::vec3 TerrainGenerator::calculateSlope(glm::vec3& pos, glm::vec3& normal) {
    // Compute the slope by projecting the normal onto the horizontal plane
    glm::vec3 slope = normal - glm::dot(normal, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec3(0.0f, 1.0f, 0.0f);
    return glm::normalize(slope);
}

glm::vec3 TerrainGenerator::calculateMaxSlopeDir(glm::vec3& slope) {
    // Compute the direction of maximum slope as the direction of steepest descent
    glm::vec3 maxSlopeDir = -glm::normalize(slope);

    return maxSlopeDir;
}

void TerrainGenerator::thermalErosion(int numIterations, float talusAngle, float dt)
{
    // Perform thermal erosion algorithm
    for (int i = 0; i < numIterations; i++)
    {
        // Select a random point on the terrain
        int index = rand() % m_vertices.size();
        glm::vec3& pos = m_vertices[index];

        // Calculate slope and direction of maximum slope at that point
        glm::vec3& normal = normals[index];
        glm::vec3 slope = calculateSlope(pos, normal);
        glm::vec3 maxSlopeDir = calculateMaxSlopeDir(slope);

        // Calculate the amount of material to move
        float amountToMove = dt * pow(glm::dot(slope, maxSlopeDir), 2.0);

        // Check if the slope angle is greater than the talus angle
        if (glm::degrees(glm::acos(glm::dot(slope, maxSlopeDir))) > talusAngle)
        {
            // Move material downslope to neighboring points
            for (int j = 0; j < m_adjList[index].size(); j++)
            {
                int neighborIndex = m_adjList[index][j];
                glm::vec3& neighborPos = m_vertices[neighborIndex];

                // Calculate the direction from the current point to the neighboring point
                glm::vec3 direction = glm::normalize(neighborPos - pos);

                // Calculate the amount of material to move to the neighboring point
                float amountMoved = amountToMove * glm::dot(direction, maxSlopeDir);

                // Update the height of the current and neighboring points
                pos -= amountMoved * maxSlopeDir;
                neighborPos += amountMoved * maxSlopeDir;
            }
        }

        // Update the VBO and NBO buffers after every 100 iterations
        if (i % 100 == 0)
        {
            updateBuffers();
        }
    }
}

void TerrainGenerator::updateBuffers()
{
    // Calculate the normals for each triangle and store them in the buffer.
    for (int i = 0; i < m_indices.size(); i += 3) {
        glm::vec3 p1 = m_vertices[m_indices[i]];
        glm::vec3 p2 = m_vertices[m_indices[i + 1]];
        glm::vec3 p3 = m_vertices[m_indices[i + 2]];

        glm::vec3 normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));

        normals[m_indices[i]] += normal;
        normals[m_indices[i + 1]] += normal;
        normals[m_indices[i + 2]] += normal;
    }

    // Normalize the normals and upload them to the buffer.
    for (int i = 0; i < normals.size(); ++i) {
        normals[i] = glm::normalize(normals[i]);
    }

    // Bind the VAO and VBO.
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);
    // Bind the normals buffer to index 1.
    glBindBuffer(GL_ARRAY_BUFFER, m_nbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    // Bind the EBO and upload the index data.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);
}
