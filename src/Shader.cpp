#include "Shader.h"

Shader::Shader(const char *vertexFile, const char *fragmentFile)
{
    std::cout << "Loading Shaders from: \n \t" << vertexFile << "\n \t" << fragmentFile << "\n";
    std::string vertexCode = getFileContents(vertexFile);
    std::string fragmentCode = getFileContents(fragmentFile);

    const char *vertexSource = vertexCode.c_str();
    const char *fragmentSource = fragmentCode.c_str();

    // Create a Vertex Shader Object
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach Vertex Shader source to the object
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    // Compile the vertex shader
    glCompileShader(vertexShader);
    printCompileErrors(vertexShader, "VERTEX");

    // Create a Fragment Shader Object
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach Fragment Shader source to the object
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    // Compile the fragment shader
    glCompileShader(fragmentShader);
    printCompileErrors(fragmentShader, "FRAGMENT");

    // Create Shader Program Object
    mId = glCreateProgram();
    // Attach the Vertex and Fragment Shaders to the Shader Program
    glAttachShader(mId, vertexShader);
    glAttachShader(mId, fragmentShader);
    // Link all the shaders together into the Shader Program
    glLinkProgram(mId);
    printCompileErrors(mId, "PROGRAM");

    // Delete the Vertex and Fragment Shader object because there are allready in the Shader Program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

const GLuint Shader::getId() const
{
    return mId;
}

void Shader::activateShader()
{
    glUseProgram(mId);
}

void Shader::printCompileErrors(unsigned int shader, const char *type)
{
    GLint hasCompiled;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);

            throw std::runtime_error("SHADER_COMPILATION_ERROR for: " + std::string(type));
        }
        else
        {
            std::cout << "Shader successfully compiled: " << type << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            throw std::runtime_error("SHADER_LINKING_ERROR");
        }
        else
        {
            std::cout << "Shader successfully linked" << std::endl;
        }
    }
}

void Shader::deleteShader()
{
    glDeleteProgram(mId);
}
