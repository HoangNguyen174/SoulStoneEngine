#ifndef TEXTURE_H
#define TEXTURE_H
#include "Vector2.hpp"
#include "string"

#undef new
#include <map>
//#define new new( __FILE__, __LINE__ )

#include <Windows.h>
#include <gl/gl.h>
#include "../Render/glext.h"
#include "Vector2i.hpp"

class Texture
{
	private:	
		Texture(const std::string& imageFilePath);

	public:
		Vector2i									  m_size;
		unsigned char								  *m_imageData;
		int											  m_openglTextureID;
		static std::map<std::string, Texture*>	      s_textureRegistry;
		
	public:
		Texture();
		static Texture *GetTextureByName(const std::string& imageFilePath);
		static Texture *CreateOrGetTexture(const std::string& imageFilePath);
};


#endif