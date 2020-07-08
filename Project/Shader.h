#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>
#include <wx/string.h>

#include "glm/glm.hpp"

struct ShaderSource
{
    std::string vertexShader = "";
    std::string fragmentShader = "";
};

class Shader
{
public:
    Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    //Set uniforms
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniform2f(const std::string& name, float v0, float v1);
    void SetUniformMatrix4fv(const std::string& name, const glm::mat4& matrix);
    void SetUniform1f(const std::string& name, const float& value);

protected:
    unsigned int GetUniformLocation(const std::string& name);
    unsigned int CompileShader(unsigned int type, const wxString& source) const;
    unsigned int CreateShader(std::string& vertexShader, std::string& fragmentShader) const;
    ShaderSource ParseShader(const wxString& filepath) const;

    unsigned int m_rendererID;
    wxString m_filepath;
    std::unordered_map<std::string, int> m_uniformLocationCache;
};

#endif // SHADER_H
