#pragma once

#include <glad/glad.h>
#include <string>

namespace Renderer {
	class Texture2D
	{
	public:
		Texture2D(const GLuint width, GLuint height, const unsigned char* data, 
				  const unsigned int channels = 4, 
				  const GLenum filter = GL_LINEAR, 
				  const GLenum wrapMode = GL_CLAMP_TO_EDGE);
		Texture2D() = delete;								// запрещаем обычный конструктор
		Texture2D(const Texture2D&) = delete;				// запрещаем копирующий конструктор
		Texture2D& operator=(const Texture2D&) = delete;	// запрещаем операцию присваивания
		Texture2D& operator=(Texture2D&& texture2d);		// move-operator, который позволяет не копировать всё содержимое памяти текстуры, а перемещать его
		Texture2D(Texture2D&& texture2d);

		~Texture2D(); // освобождаем память в видеокарте, которую выделили под текстуру

		void bind() const;									//подключает текстуру 

	private:
		GLuint m_ID;
		GLenum m_mode;
		unsigned int m_width;
		unsigned int m_height;
	};
}