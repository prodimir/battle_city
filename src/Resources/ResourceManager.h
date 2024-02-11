#pragma once

#include <string>
#include <memory>
#include <map>

namespace Renderer 
{
	class ShaderProgram;
}



class ResourceManager {
public:
	ResourceManager(const std::string& executablePath); // executablePath is path to battle_city.exe file
	~ResourceManager() = default;

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	std::shared_ptr<Renderer::ShaderProgram> loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(const std::string& shaderName);
	
	//load textures
	void loadTexture(const std::string& textureName, const std::string& texturePath);

private:
	std::string getFileString(const std::string& relativeFilePath) const;
	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramMap;
	ShaderProgramMap m_shaderPrograms;

	std::string m_path;
};