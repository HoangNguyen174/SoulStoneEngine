#include "FBO.hpp"
#include "GLRender.hpp"
#include "../Utilities/GameCommon.hpp"
#include "../JobSystem/MemoryPoolManager.hpp"
#include "../Render/GraphicManager.hpp"

FBO::FBO( const std::string& name )
{
	m_fboBufferDepthTextureID = 0;
	m_fboColorTextureID = 0;
	m_fboID = 0;
	m_name = name;
	m_width = WINDOW_PHYSICAL_WIDTH;
	m_height = WINDOW_PHYSICAL_HEIGHT;
	Initialize( m_width, m_height );
	GraphicManager::s_FBORegistry[ name ] = this;
}

FBO::FBO( const std::string& name, unsigned int width, unsigned int height )
{
	m_fboBufferDepthTextureID = 0;
	m_fboColorTextureID = 0;
	m_fboID = 0;
	m_width = width;
	m_height = height;
	m_name = name;
	Initialize( width, height );
	GraphicManager::s_FBORegistry[ name ] = this;
}

void FBO::Initialize( unsigned int width, unsigned int height )
{
	// Create color frame buffer texture
	glActiveTexture( GL_TEXTURE0 );
	glGenTextures( 1, &m_fboColorTextureID );
	glBindTexture( GL_TEXTURE_2D, m_fboColorTextureID );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGB, GL_FLOAT, NULL ); // NOTE: RGBA and UNSIGNED BYTE

	// Create depth frame buffer texture
	glActiveTexture( GL_TEXTURE1 );
	glGenTextures( 1, &m_fboBufferDepthTextureID );
	glBindTexture( GL_TEXTURE_2D, m_fboBufferDepthTextureID );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL ); // NOTE: DEPTH and UNSIGNED INT

	glGenFramebuffers( 1, &m_fboID );
	glBindFramebuffer( GL_FRAMEBUFFER, m_fboID );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboColorTextureID, 0 );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_fboBufferDepthTextureID, 0);
}

FBO* FBO::CreateOrGetFBOByName( const std::string& name )
{
	if( GetFBOByName( name  ) != nullptr )
	{
		return GraphicManager::s_FBORegistry[ name ];
	}
	else
	{
		FBO* fbo = new FBO( name );
		return fbo;
	}
}

FBO* FBO::CreateOrGetFBOByName( const std::string& name, unsigned int width, unsigned int height )
{
	if( GetFBOByName( name  ) != nullptr )
	{
		return GraphicManager::s_FBORegistry[ name ];
	}
	else
	{
		FBO* fbo = new FBO( name, width, height );
		return fbo;
	}
}

FBO* FBO::GetFBOByName( const std::string& name )
{
	auto found = GraphicManager::s_FBORegistry.find( name );
	if( found == GraphicManager::s_FBORegistry.end() )
		return nullptr;
	else
		return GraphicManager::s_FBORegistry[ name ];
}

void FBO::BindForWriting()
{
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_fboID );
}

void FBO::BindForReading()
{
	glBindFramebuffer( GL_READ_FRAMEBUFFER, m_fboID );
}
