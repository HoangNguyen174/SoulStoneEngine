#include "Sprite.hpp"
#include "../JobSystem/MemoryPoolManager.hpp"
#include "../Render/GraphicManager.hpp"

Sprite::Sprite( bool isAnimated, bool isLooped )
	: m_animation( nullptr ),
	  m_isAnimated( isAnimated ),
	  m_isLooped( isLooped ),
	  m_currentFrame(0),
	  m_texture( nullptr ),
	  m_scale( Vector2( 1.f, 1.f ) ),
	  m_orientaion( 0.f ),
	  m_color( RGBColor::White() )
{
}

Sprite::~Sprite()
{
	delete m_texture;
	m_texture = nullptr;
}

void Sprite::Play()
{
	m_isAnimated = true;
}

void Sprite::Pause()
{
	m_isAnimated = false;
}

void Sprite::Stop()
{
	m_isAnimated = false;
	m_currentFrame = 0;
}

void Sprite::SetTexture( const std::string& texturePathName )
{
	m_texture = Texture::CreateOrGetTexture( texturePathName );
}

void Sprite::SetFrameTime(float time)
{
	m_frameTime = time;
	m_frameTimeRemaining = m_frameTime;
}

void Sprite::SetAnimation( const SpriteAnimation& animation )
{
	m_animation = &animation;
	m_texture = animation.GetSpriteSheet();
	m_currentFrame = 0;
}

void Sprite::SetPosition( const Vector2& pos )
{
	m_position = pos;
}

void Sprite::SetScale(float x, float y)
{
	m_scale.x = x;
	m_scale.y = y;
}

void Sprite::Update(float elapsedTime)
{
	if( m_isAnimated && m_animation )
	{
		m_frameTimeRemaining -= elapsedTime;
		if( m_frameTimeRemaining <= 0.f && m_currentFrame < m_animation->GetAnimationSize() )
		{
			m_currentFrame++;
			m_frameTimeRemaining = m_frameTime;
		}
		if( m_currentFrame >= m_animation->GetAnimationSize() )
		{
			m_isLooped ? m_currentFrame = 0 : m_currentFrame ;
		}
	}
}

void Sprite::Render()
{
	GraphicManager::s_render->ActiveTexture( GL_TEXTURE0 );

	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glDisable( GL_DEPTH_TEST );

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	if( m_animation )
		glBindTexture( GL_TEXTURE_2D, m_animation->GetSpriteSheet()->m_openglTextureID );
	else
	{
		if( m_texture )
			glBindTexture( GL_TEXTURE_2D, m_texture->m_openglTextureID );
	}

	glColor4f( m_color.m_red, m_color.m_green, m_color.m_blue, m_color.m_alpha );

	glPushMatrix();
	glTranslatef( m_position.x, m_position.y, 0.f );
	glRotatef( m_orientaion, 0.f, 0.f, 1.f);
	glScalef( m_scale.x, m_scale.y, 0.f);

	glBegin( GL_QUADS );
	{
		if( m_animation )
		{
			Vector2 topLeft = m_animation->GetTexCoordsAtFrame( m_currentFrame ).topLeft;
			Vector2 bottomRight = m_animation->GetTexCoordsAtFrame( m_currentFrame ).bottomRight;

			glTexCoord2f( topLeft.x, topLeft.y );
			glVertex2f( -.5f, .5f );
			glTexCoord2f( topLeft.x, bottomRight.y );
			glVertex2f( -.5f, -.5f );
			glTexCoord2f( bottomRight.x, bottomRight.y );
			glVertex2f( .5f, -.5f );
			glTexCoord2f( bottomRight.x, topLeft.y );
			glVertex2f( .5f, .5f );
		}
		else
		{
			glTexCoord2f( 0.f, 1.f );
			glVertex2f( -.5f, -.5f );
			glTexCoord2f( 1.f, 1.f );
			glVertex2f( .5f, -.5f );
			glTexCoord2f( 1.f, 0.f );
			glVertex2f( .5f, .5f );
			glTexCoord2f( 0.f, 0.f );
			glVertex2f( -.5f, .5f );
		}
	}
	glEnd();
	glPopMatrix();
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}


