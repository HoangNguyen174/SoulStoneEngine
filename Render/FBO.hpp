#ifndef FBO_H
#define FBO_H
#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <string>

class FBO
{
	public:
		unsigned int		m_fboID;
		unsigned int		m_fboColorTextureID;
		unsigned int		m_fboBufferDepthTextureID;
		unsigned int		m_width;
		unsigned int		m_height;
		std::string			m_name;	

	private:
		FBO( const std::string& name );
		FBO( const std::string& name, unsigned int width, unsigned int height );
		~FBO() {}
		void Initialize();
		void Initialize( unsigned int width, unsigned int height );

	public:
		static FBO*	GetFBOByName( const std::string& name );
		static FBO* CreateOrGetFBOByName( const std::string& name );
		static FBO* CreateOrGetFBOByName( const std::string& name, unsigned int width, unsigned int height );
		void BindForWriting();
		void BindForReading();
};

#endif;