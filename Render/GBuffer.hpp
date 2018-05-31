#ifndef G_BUFFER_H
#define G_BUFFER_H
#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <string>
#include "FBO.hpp"

enum GBufferTextureType { GBUFFER_TEXTURE_TYPE_POSITION = 0,
						  GBUFFER_TEXTURE_TYPE_NORMAL,
						  GBUFFER_TEXTURE_TYPE_DIFFUSE,
						  GBUFFER_TEXTURE_TYPE_NORMAL_VIEW,
// 						  GBUFFER_TEXTURE_TYPE_TANGENT,
// 						  GBUFFER_TEXTURE_TYPE_BITANGENT,
						  GBUFFER_NUM_TEXTURES };
							
class GBuffer
{
	public:
		unsigned int		m_textureIDs[ GBUFFER_NUM_TEXTURES ];
		unsigned int		m_depthTextureID;
		unsigned int		m_finalTextureID;
		unsigned int		m_fboID;
		unsigned int		m_width;
		unsigned int		m_height;
		std::string			m_name;	

	public:
		GBuffer( const std::string& name );
		~GBuffer() {}
		void Initialize();
		void BindForWriting();
		void BindForReading();
		void SetReadBuffer( GBufferTextureType type );
		void ClearFinalTexture();
		void BindForGeometryPass();
		void BindForStencilPass();
		void BindForLightPass();
		void BindForFinalPass();
};

#endif