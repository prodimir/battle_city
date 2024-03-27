#pragma once
#include <glad/glad.h>

#include <string>
namespace Renderer {
	class ShaderProgram {
	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		~ShaderProgram();
		bool isCompiled() const {return m_isCompiled; }
		void use() const;
		void setInt(const std::string& name, const GLint value);	// идентификатор текстуры один шейдер, потому что uniform в fragment.txt

		// prohibitions

		ShaderProgram() = delete; // can't use empty ShaderProgram
		ShaderProgram(ShaderProgram&) = delete;	// can't call shader from another shader, as they will have same identificators
		ShaderProgram& operator=(const ShaderProgram&) = delete; // can't equal two shaders, as they will have same identificators
		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;
	private:
		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
		bool m_isCompiled = false;	// flag of compile and link
		GLuint m_ID = 0;
	};
}

