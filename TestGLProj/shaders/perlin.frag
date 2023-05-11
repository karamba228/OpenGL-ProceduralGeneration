#version 440 core

in vec2 vTexCoord;
out vec4 FragColor;

uniform vec2 uResolution;
uniform int uOctaves;
uniform float uPersistence;
uniform float uAmplitude;
uniform float uLacunarity;
uniform float uFrequency;
uniform float uNoiseScale;
uniform int uSeed;

float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}

float noise(in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    vec2 u = f*f*(3.0-2.0*f);
    return mix(mix(random(i + vec2(0.0, 0.0)), random(i + vec2(1.0, 0.0)), u.x),
               mix(random(i + vec2(0.0, 1.0)), random(i + vec2(1.0, 1.0)), u.x), u.y);
}

void main() {
    vec2 st = vTexCoord * uNoiseScale;
    float value = 0.0;
    float amplitude = uAmplitude;
    float frequency = uFrequency;

    for (int i = 0; i < uOctaves; i++) {
        value += noise(st * frequency) * amplitude;
        st *= uLacunarity;
        amplitude *= uPersistence;
        frequency *= 2.0;
    }

    value = (value + 1.0) / 2.0;

    FragColor = vec4(vec3(value), 1.0);
}