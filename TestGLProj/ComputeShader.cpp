#define GL_COMPUTE_SHADER 0x91B9
#include "ComputeShader.h"

#include <cstdio>    // for stderr and fprintf
#include <cstdlib>   // for NULL
#include <GL/glew.h> // for GL_COMPUTE_SHADER
#include <GL/gl.h>   // for OpenGL functions and types

ComputeShader::ComputeShader()
    : m_computeShaderId(0), m_computeProgramId(0)
{
}

ComputeShader::~ComputeShader()
{
    cleanup();
}

bool ComputeShader::InitializeFromFile(const char* computeFilename)
{
    // Read the compute shader source code from file
    std::string computeSourceCode;
    std::ifstream computeFile(computeFilename);
    if (computeFile.is_open()) {
        std::stringstream buffer;
        buffer << computeFile.rdbuf();
        computeSourceCode = buffer.str();
        computeFile.close();
    }
    else {
        std::cerr << "Error: failed to open compute shader file " << computeFilename << std::endl;
        return false;
    }

    return CreateShader(computeSourceCode);
}

bool ComputeShader::CreateShader(std::string computeSourceCode)
{
    // Compile the compute shader
    m_computeShaderId = glCreateShader(GL_COMPUTE_SHADER);
    const char* computeCodePtr = computeSourceCode.c_str();
    glShaderSource(m_computeShaderId, 1, &computeCodePtr, NULL);
    glCompileShader(m_computeShaderId);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(m_computeShaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(m_computeShaderId, sizeof(infoLog), NULL, infoLog);
        std::cerr << "Error: failed to compile compute shader:\n" << infoLog << std::endl;
        return false;
    }

    // Create the compute program and attach the shader
    m_computeProgramId = glCreateProgram();
    glAttachShader(m_computeProgramId, m_computeShaderId);
    glLinkProgram(m_computeProgramId);

    // Check for link errors
    glGetProgramiv(m_computeProgramId, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetProgramInfoLog(m_computeProgramId, sizeof(infoLog), NULL, infoLog);
        std::cerr << "Error: failed to link compute program:\n" << infoLog << std::endl;
        return false;
    }

	return true;
}

void ComputeShader::cleanup()
{
	if (m_computeProgramId != 0) {
		glDeleteProgram(m_computeProgramId);
		m_computeProgramId = 0;
	}
}

// TODO: Implement methods for setting input and output buffers
// TODO: Implement method for dispatching the compute shader
