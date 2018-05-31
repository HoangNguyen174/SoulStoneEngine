#include "GBuffer.hpp"
#include "../Utilities/GameCommon.hpp"
#include "../Render/GraphicManager.hpp"

GBuffer::GBuffer( const std::string& name )
{
	m_fboID = 0;
	m_name = name;
	m_width = WINDOW_PHYSICAL_WIDTH;
	m_height = WINDOW_PHYSICAL_HEIGHT;
	for( int i = 0; i < GBUFFER_NUM_TEXTURES; i++ )
	{
		m_textureIDs[ i ] = 0;
	}
	Initialize();
}

void GBuffer::Initialize()
{
	glGenFramebuffers( 1, &m_fboID );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_fboID );

	glGenTextures( GBUFFER_NUM_TEXTURES, m_textureIDs );
	for( unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++ )
	{
		glBindTexture( GL_TEXTURE_2D, m_textureIDs[i] );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL );
		glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textureIDs[i], 0 );
	}
	
	// Create depth frame buffer texture
	glGenTextures(1, &m_depthTextureID );
	glBindTexture( GL_TEXTURE_2D, m_depthTextureID );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, m_width, m_height, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL );
	glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTextureID, 0);

	// Create final color frame buffer texture
	glGenTextures( 1, &m_finalTextureID );
	glBindTexture( GL_TEXTURE_2D, m_finalTextureID );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL );
	glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_finalTextureID, 0);
}

void GBuffer::BindForReading()
{
	glBindFramebuffer( GL_READ_FRAMEBUFFER, m_fboID );
}

void GBuffer::BindForWriting()
{
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_fboID );
}

void GBuffer::SetReadBuffer( GBufferTextureType type )
{
	glReadBuffer( GL_COLOR_ATTACHMENT0 + type );
}

void GBuffer::ClearFinalTexture()
{
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_fboID );
	glDrawBuffer( GL_COLOR_ATTACHMENT4 );
	glClear( GL_COLOR_BUFFER_BIT );
}

void GBuffer::BindForGeometryPass()
{
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_fboID );

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, 
							 GL_COLOR_ATTACHMENT1, 
							 GL_COLOR_ATTACHMENT2,
							 GL_COLOR_ATTACHMENT3 };

	glDrawBuffers( ARRAY_SIZE_IN_ELEMENTS( drawBuffers ), drawBuffers );
}

void GBuffer::BindForStencilPass()
{
	glDrawBuffer( GL_NONE );
}

void GBuffer::BindForLightPass()
{
	glDrawBuffer( GL_COLOR_ATTACHMENT4 );
}

void GBuffer::BindForFinalPass()
{
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
	glBindFramebuffer( GL_READ_FRAMEBUFFER, m_fboID );
	glReadBuffer( GL_COLOR_ATTACHMENT4 );
}