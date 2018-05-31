#include "SpriteAnimation.hpp"
#include "GameCommon.hpp"

SpriteAnimation::SpriteAnimation( const std::string& textureFilePath, int numFrameWide, int numFrameHeight, int startSeqIndex, int endSeqIndex )
	: m_numFrameWide( numFrameWide ),
	  m_numFrameHeight( numFrameHeight )
{
	m_texture = Texture::CreateOrGetTexture( textureFilePath );

	float texCoordsWidth = 1.f / m_numFrameWide;
	float texCoordsHeight = 1.f / m_numFrameHeight;

	for( int i = startSeqIndex; i <= endSeqIndex; i++ )
	{
		Vector2i tileCoords = ConvertTileIndexToTileCoords2D( i, m_numFrameWide, m_numFrameHeight );

		FrameTexCoords texCoords;
		texCoords.topLeft = Vector2( tileCoords.x * texCoordsWidth, tileCoords.y * texCoordsHeight );
		texCoords.bottomRight = Vector2( ( tileCoords.x + 1 ) * texCoordsWidth, ( tileCoords.y + 1 ) * texCoordsHeight );

		m_frameTexcoords.push_back( texCoords );
	}	
}

const Texture* SpriteAnimation::GetSpriteSheet() const
{
	return m_texture;
}

unsigned int SpriteAnimation::GetAnimationSize() const
{
	return m_frameTexcoords.size();
}

FrameTexCoords SpriteAnimation::GetTexCoordsAtFrame( unsigned int frame ) const
{
	if( frame > m_frameTexcoords.size() - 1 || frame < 0 )
		return m_frameTexcoords[0];

	return m_frameTexcoords[ frame ];
}