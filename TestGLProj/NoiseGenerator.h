#ifndef NOISE_GENERATOR_H
#define NOISE_GENERATOR_H

#include <vector>
#include <glm/glm.hpp>

class NoiseGenerator {
public:
    std::vector<float> m_vertices;
    NoiseGenerator(int width);
    ~NoiseGenerator();

    std::vector<float> generatePerlinNoise();
    std::vector<float> generateVoronoi();
    void setSeed(int);
    void setOctaves(int);
    void setPersistence(float);
    void setAmplitude(float);
    void setLacunarity(float);
    void setFrequency(float);
    void setScale(float);

    void setVoronoiPointCount(int);
    void setVoronoiOctaves(int);
    void setVoronoiLacunarity(float);
    void setVoronoiGain(float);
    void setVoronoiJitter(float);
    void setVoronoiScale(float);
    void setVoronoiAmplitude(float);
    void setVoronoiSeed(int);

private:
    // Perlin Noise values
    int SEED = 0;
    int OCTAVES = 6;
    float PERSISTENCE;
    float AMPLITUDE;
    float LACUNARITY;
    float FREQUENCY;
    float HEIGHT;

    // Voronoi values
    int VORONOI_POINTS = 40;   // number of Voronoi points
    int VORONOI_OCTAVES = 4;   // number of octaves for noise
    float VORONOI_LACUNARITY = 50.0f;  // lacunarity for noise
    float VORONOI_GAIN = 0.4f;        // gain for noise
    float VORONOI_JITTER = 0.02f;
    float VORONOI_SCALE = 25.0f;      // scale of the noise
    float VORONOI_AMPLITUDE = 4.0f;
    int VORONOI_SEED;

    int m_width;
    float m_heightScale = 1.0f; // added member variable for scaling vertex heights

    float gradient(int, int, float, float);
};

#endif