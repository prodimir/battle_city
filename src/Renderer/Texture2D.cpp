#include "Texture2D.h"

namespace Renderer {

	Texture2D::Texture2D(const GLuint width, GLuint height,
		const unsigned char* data,
		const unsigned int channels,
		const GLenum filter,			// помогает решить вопрос с потерей разрешения при увеличении текстуры(когда видны сами пиксели и картинка получается зазубренной). NEAREST делает лесинку
		const GLenum wrapMode) // определяет, как будет заполняться пространство выбранной текстурой, м.б. рипит, миррор и тд
		: m_width(width)									// сразу определим ширину и высоту
		, m_height(height)
	{
		switch (channels)
		{
		case 4:
			m_mode = GL_RGBA;
		case 3:
			m_mode = GL_RGB;
		default:
			m_mode = GL_RGBA;
			break;
		}
		glGenTextures(1, &m_ID);							// сгенерируем ID для текстуры
		glBindTexture(GL_TEXTURE_2D, m_ID);						// делаем текстуру m_ID текущей, то есть все изменения(параметры, команды) применяются для неё(забиндеть)
		glTexImage2D(GL_TEXTURE_2D, 0, m_mode, m_width, m_height, 0, m_mode, GL_UNSIGNED_BYTE, data); // второй параметр функции(level mipmaps) отвечает за сглаживание артефактов при удалении от текстуры. Связка с GL_TEXTURE_MAG_FILTER. Уровень 0 - текстура с обычным размером, 1 - уменьшенная в 2 раза и т.д. Оптимизация работы видеокарты

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode); // по оси х(условно)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode); // по оси y(условно)

		// у нас попиксельное изображение, картинка ни увеличиваться, ни уменьшаться не будет, поэтому можно филтрацию не использовать, но при работе с 3D-объектами фильтрация необходима
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter); // при увеличении текстуры 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter); // при удалении от объекта
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);	// убираем активную текстуру, чтобы текстуры друг друга не перебивали 
	}

	Texture2D& Texture2D::operator=(Texture2D&& texture2d)
	{
		glDeleteTextures(1, &m_ID);
		m_ID = texture2d.m_ID;
		texture2d.m_ID = 0; // чтобы не удалять два разы одну и ту же текстуру
		m_mode = texture2d.m_mode;
		m_width = texture2d.m_width;
		m_height = texture2d.m_height;

		return *this; // возвращаем текущую текстуру
	}

	Texture2D::Texture2D(Texture2D&& texture2d)
	{
		m_ID = texture2d.m_ID;
		texture2d.m_ID = 0; // чтобы не удалять два разы одну и ту же текстуру
		m_mode = texture2d.m_mode;
		m_width = texture2d.m_width;
		m_height = texture2d.m_height;
	}

	Texture2D::~Texture2D() // чтобы уничтожить текстуру, нужно удалить её с видеокарты
	{
		glDeleteTextures(1, &m_ID);
	}

	void Texture2D::bind() const // делаем нашу текстуру активной - текущей
	{
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}
}