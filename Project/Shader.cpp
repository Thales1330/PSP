#include "Shader.h"
#include "Renderer.h"

#include <wx/msgdlg.h>

#include <fstream>
#include <sstream>
#include <string>


Shader::Shader(const std::string& filepath)
    : m_filepath(filepath), m_rendererID(0)
{
    ShaderSource source = ParseShader(filepath);
    m_rendererID = CreateShader(source.vertexShader, source.fragmentShader);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_rendererID));
}


unsigned int Shader::CompileShader(unsigned int type, const wxString& source) const
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    // Error handling
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = static_cast<char*>(alloca(length * sizeof(char))); // Heap allocation dynamically! =)
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        wxMessageBox(wxString::Format("Erro ao compilar o \"%s shader\" ='(\n%s", type == GL_VERTEX_SHADER ? "vertex" : "fragment", message));

        GLCall(glDeleteShader(id));
        return 0;
    }
    return id;
}

ShaderSource Shader::ParseShader(const wxString& filepath) const
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;

    std::ifstream stream(filepath.ToStdString());
    std::string line;
    std::stringstream ss[2];

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[static_cast<int>(type)] << line + std::string("\n");
        }
    }
    
    ShaderSource source;
    source.vertexShader = ss[0].str();
    source.fragmentShader = ss[1].str();

    return source;
}

unsigned int Shader::CreateShader(std::string& vertexShader, std::string& fragmentShader) const
{
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));

    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_rendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
    GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniformMatrix4fv(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniform1f(const std::string& name, const float& value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_rendererID, name.c_str()));
    if (location == -1) {
        wxMessageBox(wxString::Format("Atenção! uniform \"%s\" não encontrado", name));
    }
    
    m_uniformLocationCache[name] = location;
    return location;
}
