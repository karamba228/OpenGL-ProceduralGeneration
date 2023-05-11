#define _USE_MATH_DEFINES
#include "NoiseGenerator.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include <vector>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include "FastNoiseLite.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <cmath>

FastNoiseLite noise;

NoiseGenerator::NoiseGenerator(int width) 
    : m_width(width)
{
    m_vertices.resize(m_width * m_width * 3);
}

NoiseGenerator::~NoiseGenerator() {
    
}

std::vector<float> NoiseGenerator::generatePerlinNoise() {
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    std::vector<float> noise_map(m_width * m_width);
    int index = 0;

    for (int y = 0; y < m_width; y++) {
        for (int x = 0; x < m_width; x++) {
            float noise_value = 0.0;
            float freq = FREQUENCY;
            float amp = AMPLITUDE;

            // Loop through octaves and calculate noise value for each
            for (int i = 0; i < OCTAVES; i++) {
                noise.SetFrequency(freq);
                noise.SetSeed(SEED + i); // Use different seed for each octave

                // Calculate noise value for current octave and add it to the total noise value
                noise_value += noise.GetNoise((float)x, (float)y) * amp;

                // Update frequency and amplitude for next octave
                freq *= LACUNARITY;
                amp *= PERSISTENCE;
            }

            // Normalize noise value to [-1, 1] range and store in noise map
            noise_map[index++] = noise_value * HEIGHT;
        }
    }

    return noise_map;
}

void NoiseGenerator::setSeed(int value) {
    SEED = value;
}
void NoiseGenerator::setOctaves(int value) {
    OCTAVES = value;
}
void NoiseGenerator::setPersistence(float value) {
    PERSISTENCE = value;
}
void NoiseGenerator::setAmplitude(float value) {
    AMPLITUDE = value;
}
void NoiseGenerator::setLacunarity(float value) {
    LACUNARITY = value;
}
void NoiseGenerator::setFrequency(float value) {
    FREQUENCY = value;
}
void NoiseGenerator::setScale(float value) {
    HEIGHT = value;
}


std::vector<float> NoiseGenerator::generateVoronoi()
{
    srand(VORONOI_SEED);
    // Generate Voronoi points with jitter
    std::vector<glm::vec2> points(VORONOI_POINTS);
    float jitter = VORONOI_JITTER * m_width;
    for (int i = 0; i < VORONOI_POINTS; i++) {
        float x = (rand() % m_width) - (m_width / 2);
        float y = (rand() % m_width) - (m_width / 2);
        x += (rand() % (int)jitter) - (jitter / 2);
        y += (rand() % (int)jitter) - (jitter / 2);
        points[i] = glm::vec2(x, y);
    }

    // Generate noise
    std::vector<float> noise(m_width * m_width);
    float maxVal = 0.0f;
    for (int i = 0; i < VORONOI_POINTS; i++) {
        maxVal += pow(VORONOI_GAIN, VORONOI_OCTAVES - 1) / (1 + VORONOI_OCTAVES - 1);
    }
    for (int octave = 0; octave < VORONOI_OCTAVES; octave++) {
        float frequency = pow(VORONOI_LACUNARITY, octave);
        float amplitude = pow(VORONOI_GAIN, octave) / (1 + octave);

        for (int x = 0; x < m_width; x++) {
            for (int y = 0; y < m_width; y++) {
                float nx = (float)x / (float)m_width - 0.5f;
                float ny = (float)y / (float)m_width - 0.5f;

                float noiseValue = 0.0f;

                for (int i = 0; i < VORONOI_POINTS; i++) {
                    glm::vec2 p = (points[i] + glm::vec2((rand() % (int)jitter) - (jitter / 2),
                        (rand() % (int)jitter) - (jitter / 2))) / glm::vec2(m_width, m_width);
                    glm::vec2 dp = glm::vec2(nx, ny) - p;
                    float dist = glm::length(dp);

                    noiseValue += exp(-dist * dist * VORONOI_SCALE * frequency) * amplitude;
                }

                noise[x + y * m_width] += noiseValue / maxVal;
            }
        }
    }

    // Normalize the noise to the range [0, 1] and apply gain
    float minValue = *std::min_element(noise.begin(), noise.end()) / VORONOI_AMPLITUDE;
    float maxValue = *std::max_element(noise.begin(), noise.end()) / VORONOI_AMPLITUDE;
    float range = maxValue - minValue;
    float gain = pow(VORONOI_GAIN, 1.0f / range);
    std::transform(noise.begin(), noise.end(), noise.begin(),
        [=](float value) { return pow((value - minValue) / range, gain); });

    return noise;
}

void NoiseGenerator::setVoronoiPointCount(int value) {
    VORONOI_POINTS = value;
}
void NoiseGenerator::setVoronoiOctaves(int value) {
    VORONOI_OCTAVES = value;
}
void NoiseGenerator::setVoronoiLacunarity(float value) {
    VORONOI_LACUNARITY = value;
}
void NoiseGenerator::setVoronoiGain(float value) {
    VORONOI_GAIN = value;
}
void NoiseGenerator::setVoronoiJitter(float value) {
    VORONOI_JITTER = value;
}
void NoiseGenerator::setVoronoiScale(float value) {
    VORONOI_SCALE = value;
}
void NoiseGenerator::setVoronoiAmplitude(float value) {
    VORONOI_AMPLITUDE = value;
}
void NoiseGenerator::setVoronoiSeed(int value) {
    VORONOI_SEED = value;
}