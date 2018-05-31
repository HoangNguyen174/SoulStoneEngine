#pragma once
#include "Texture.hpp"
#include "Vector2i.hpp"
#include <vector>

struct FrameTexCoords
{
	Vector2 topLeft;
	Vector2 bottomRight;
};

class SpriteAnimation
{
	private:
		const Texture*				m_texture;
		std::vector<FrameTexCoords>	m_frameTexcoords;
		int							m_numFrameWide;
		int							m_numFrameHeight;

	public:
		SpriteAnimation( const std::string& textureFilePath, int numFrameWide, int numFrameHeight, int startSeqIndex, int endSeqIndex );
		~SpriteAnimation();
		const Texture* GetSpriteSheet() const;
		FrameTexCoords GetTexCoordsAtFrame( unsigned int frame ) const;
		unsigned int GetAnimationSize() const; 
};