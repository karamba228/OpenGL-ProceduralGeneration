#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Shader.h"

class TerrainGenerator {
public:
    struct Vertex {
        Vertex() : Position(0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 1.0f) {}
        glm::vec3 Position;
        glm::vec3 Normal;
    };

    TerrainGenerator(Shader*, int, Shader*);
    ~TerrainGenerator() {}

    void render(glm::mat4, glm::mat4);

    void generateLevelPlane();
    void applyHeightMap(const std::vector<float>& heightMap, float heightScale);

    glm::vec3 calculateMaxSlopeDir(glm::vec3&);
    glm::vec3 calculateSlope(glm::vec3&, glm::vec3&);
    void thermalErosion(int numIterations, float talusAngle, float dt);

    //std::vector<float> calculateFlowRates(float, std::vector<glm::vec3>&);
    //void TerrainGenerator::simulateWaterErosion(float, std::vector<glm::vec3>&, float, float, std::vector<float>&);
    //float TerrainGenerator::calculateSedimentCapacity(float, float, float, float, float);
    //void TerrainGenerator::simulateSedimentTransport(float, float, float, float, std::vector<float>&, std::vector<float>&);
    //void hydraulicErosion(int, float, float, float, float, float);

    glm::vec3 calculateNormal(int index);
    void updateBuffers();

    GLuint getMeshPlaneVAO() const {
        return m_vao;
    }

    void setSeed(int);
    void setOctaves(int);
    void setPersistence(float);
    void setAmplitude(float);
    void setLacunarity(float);
    void setFrequency(float);
    void setScale(float);

    // The dimensions of the terrain.
    int m_width; // The number of vertices along the x-axis.
    int m_span;  // The number of vertices along the z-axis.

private:
    // Shader used for rendering.
    Shader* m_shader;
    Shader* m_perlinShader;

    // Vertex array object and buffer objects used to store vertex and index data.
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;
    GLuint m_nbo;
    //GLuint m_fbo;
    //GLuint texture;

    float noiseFrequency = 1.0f;
    float noiseScale = 0.2f;

    // Vertex and index data.
    std::vector<glm::vec3> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<glm::vec3> normals;
    std::vector<std::vector<int>> m_adjList;
    std::vector<float> m_sediment;

    // Perlin Noise values
    int SEED = 0;
    int OCTAVES = 6;
    float PERSISTENCE;
    float AMPLITUDE;
    float LACUNARITY;
    float FREQUENCY;
    float HEIGHT;

    /*int k;
    int m;
    float g;
    float sedimentDensity = 2000.0f;
    float waterDensity = 1000.0f;*/
};
#endif // TERRAINGENERATOR_H