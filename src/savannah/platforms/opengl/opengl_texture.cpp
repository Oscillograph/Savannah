#include <savannah/platforms/opengl/opengl_texture.h>

#ifdef SAVANNAH_PLATFORM_WINDOWS
	// for stb_image.h : Unicode filenames on Windows
	#define STBI_WINDOWS_UTF8
#endif
#define STB_IMAGE_IMPLEMENTATION
#include <external/stb_image.h>

namespace Savannah
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& filename)
		: m_Path(filename)
	{
		if (filename.size() > 0)
		{
			LoadTextureFromFile(m_Path);
		}
	}
	
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		if (m_TextureID)
			glDeleteTextures(1, &m_TextureID);
	}
	
	bool OpenGLTexture2D::LoadTextureFromFile(const std::string& filename)
	{
		// Load from file
		bool result = false;
		int imageWidth, imageHeight, channels;
//		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(filename.c_str(), &imageWidth, &imageHeight, &channels, 0);
		
		if (data)
		{
			uint32_t internalFormat = GL_RGB8;
			uint32_t dataFormat = GL_RGB8;
			
			if (channels == 4){
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			} else if (channels == 3){
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}
			
			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;
			
			if ((internalFormat == 0) && ( dataFormat == 0))
			{
				printf("Image format is not supported\n");
			} else {
				m_TextureID = 0;
				m_ImageSize = {imageWidth, imageHeight};
				
				// Create a OpenGL texture identifier
				glGenTextures(1, &m_TextureID);
				glBindTexture(GL_TEXTURE_2D, m_TextureID);
				
				// What to do if the texture doesn't fit the place
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				
				glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, imageWidth, imageHeight, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
				result = true;
			}
		} else {
			printf("Couldn't load a texture from \"%s\"\n", filename.c_str());
		}
		
		return result;
	}
	
	GLuint OpenGLTexture2D::GetID()
	{
		return m_TextureID;
	}
	
	iVec2 OpenGLTexture2D::GetSize()
	{
		return m_ImageSize;
	}
	
	void OpenGLTexture2D::SetData(void* data, uint32_t size) {
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_ImageSize.x, m_ImageSize.y, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
		//BSE_CORE_INFO("glTexImage2D successful");
	}
	
	void OpenGLTexture2D::Bind(uint32_t slot) const {
		// bind to slot 0
		// glBindTextureUnit(slot, m_RendererID);
		// glBindTexture(slot, m_RendererID);
		glActiveTexture(GL_TEXTURE0 + slot); // for batch texture renderer
		// glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		// BSE_INFO("Texture is bound to: {0}", GL_TEXTURE0 + slot);
	}
	
	void OpenGLTexture2D::Unbind(uint32_t slot) const {
		glActiveTexture(GL_TEXTURE0 + slot); // for batch texture renderer
		// glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		// BSE_INFO("Texture is unbound from: {0}", GL_TEXTURE0 + slot);
	}
}
