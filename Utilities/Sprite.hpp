#ifndef SPRITE_H
#define SPRITE_H
#include "GameCommon.hpp"
#include "SpriteAnimation.hpp"

class Sprite
{
	public:
		const Texture*					m_texture;
		const SpriteAnimation*			m_animation;
		bool							m_isBlending;
		bool							m_isAnimated;
		bool							m_isExpired;
		bool							m_isLooped;
		Vector2							m_position;
		float							m_orientaion;
		RGBColor						m_color;
		Vector2							m_scale;
		unsigned int					m_currentFrame;
		float							m_frameTime;
		float							m_frameTimeRemaining;
		float							m_timeForCurrentFrame;

	public:
		Sprite( bool isAnimated, bool isLooped );
		~Sprite();
		void Play();
		void Pause();
		void Stop();
		void Update(float elapsedTime);
		void Render();
		void SetScale( float x, float y );
		void SetPosition( const Vector2& pos );
		void SetLooped( bool looped );
		void SetFrameTime(float time);
		void SetAnimation( const SpriteAnimation& animation );
		void SetTexture( const std::string& texturePathName );
		Vector2 GetTopLeftCornerOfFrameIndex(int index);
		void SetOrientationDegree(float value);
};

#endif
