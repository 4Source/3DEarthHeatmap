#include "Shader.h"

Shader::Shader(const char *vertexFile, const char *fragmentFile)
{
    GLuint vertexShader;
    GLuint fragmentShader;

    try
    {
        // Get Vertex Shader file content
        std::cout << "Loading Shader from: \t" << vertexFile << "\n";
        std::string vertexCode = getFileContents(vertexFile);
        const char *vertexSource = vertexCode.c_str();

        // Create a Vertex Shader Object
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        // Attach Vertex Shader source to the object
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        // Compile the vertex shader
        glCompileShader(vertexShader);
        printCompileErrors(vertexShader, "VERTEX");

        // Get Fragment Shader file content
        std::cout << "Loading Shader from: \t" << fragmentFile << "\n";
        std::string fragmentCode = getFileContents(fragmentFile);
        const char *fragmentSource = fragmentCode.c_str();

        // Create a Fragment Shader Object
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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
    catch (const std::exception &e)
    {
        // Delete the Shader
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        std::cout << e.what() << "\n";
    }
}

Shader::Shader(const char *vertexFile, const char *fragmentFile, const char *geometryFile)
{
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint geometryShader;

    try
    {
        // Get vertex Shader file content
        std::cout << "Loading Shader from: \t" << vertexFile << "\n";
        std::string vertexCode = getFileContents(vertexFile);
        const char *vertexSource = vertexCode.c_str();

        // Create a vertex Shader Object
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        // Attach vertex Shader source to the object
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        // Compile the vertex shader
        glCompileShader(vertexShader);
        printCompileErrors(vertexShader, "VERTEX");

        // Get fragment Shader file content
        std::cout << "Loading Shader from: \t" << fragmentFile << "\n";
        std::string fragmentCode = getFileContents(fragmentFile);
        const char *fragmentSource = fragmentCode.c_str();

        // Create a fragment Shader Object
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        // Attach fragment Shader source to the object
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        // Compile the fragment shader
        glCompileShader(fragmentShader);
        printCompileErrors(fragmentShader, "FRAGMENT");

        // Get Geometry Shader file content
        std::cout << "Loading Shader from: \t" << geometryFile << "\n";
        std::string geometryCode = getFileContents(geometryFile);
        const char *geometrySource = geometryCode.c_str();

        // Create a geometry Shader Object
        geometryShader = glCreateShader(GL_FRAGMENT_SHADER);
        // Attach geometry Shader source to the object
        glShaderSource(geometryShader, 1, &geometrySource, NULL);
        // Compile the geometry shader
        glCompileShader(geometryShader);
        printCompileErrors(geometryShader, "GEOMETRY");

        // Create Shader Program Object
        mId = glCreateProgram();
        // Attach the Shaders to the Shader Program
        glAttachShader(mId, vertexShader);
        glAttachShader(mId, fragmentShader);
        glAttachShader(mId, geometryShader);
        // Link all the shaders together into the Shader Program
        glLinkProgram(mId);
        printCompileErrors(mId, "PROGRAM");

        // Delete the Shader object because there are allready in the Shader Program
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteShader(geometryShader);
    }
    catch (const std::exception &e)
    {
        // Delete the Shader
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteShader(geometryShader);

        std::cout << e.what() << "\n";
    }
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
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            // Read out the length of the log and create log buffer
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            char infoLog[maxLength];

            glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog);
            throw std::runtime_error("SHADER_COMPILATION_ERROR for: " + std::string(type) + "\n" + std::string(infoLog));
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
            // Read out the length of the log and create log buffer
            GLint maxLength = 0;
            glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            char infoLog[maxLength];

            glGetProgramInfoLog(shader, maxLength, &maxLength, infoLog);
            throw std::runtime_error("SHADER_LINKING_ERROR\n" + std::string(infoLog));
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

void Shader::sendUniform(const char *uniform, GLuint value)
{
    // Shader needs to be activated before changing the value of a uniform
    activateShader();
    // Sets the value of the uniform
    glUniform1i(glGetUniformLocation(getId(), uniform), value);
}

void Shader::sendUniform(const char *uniform, bool value)
{
    // Shader needs to be activated before changing the value of a uniform
    activateShader();
    // Sets the value of the uniform
    glUniform1i(glGetUniformLocation(getId(), uniform), value);
}

void Shader::sendUniform(const char *uniform, GLfloat value)
{
    // Shader needs to be activated before changing the value of a uniform
    activateShader();
    // Sets the value of the uniform
    glUniform1f(glGetUniformLocation(getId(), uniform), value);
}

void Shader::sendUniform(const char *uniform, glm::vec3 vector)
{
    // Shader needs to be activated before changing the value of a uniform
    activateShader();
    // Sets the value of the uniform
    glUniform3f(glGetUniformLocation(getId(), uniform), vector.x, vector.y, vector.z);
}

void Shader::sendUniform(const char *uniform, glm::vec4 vector)
{
    // Shader needs to be activated before changing the value of a uniform
    activateShader();
    // Sets the value of the uniform
    glUniform4f(glGetUniformLocation(getId(), uniform), vector.x, vector.y, vector.z, vector.w);
}

void Shader::sendUniform(const char *uniform, glm::mat4 matrix)
{
    // Shader needs to be activated before changing the value of a uniform
    activateShader();
    // Sets the value of the uniform
    glUniformMatrix4fv(glGetUniformLocation(getId(), uniform), 1, GL_FALSE, glm::value_ptr(matrix));
}