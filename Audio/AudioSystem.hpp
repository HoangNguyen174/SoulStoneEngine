#ifndef AUDIO_SYSTEMH
#define AUDIO_SYSTEMH

#include "../ThirdParty/fmod/fmod.hpp"
#include "../Utilities/MathUtilities.hpp"
#include "../JobSystem/ArchiveManager.hpp"
#include <time.h>
#include <string.h>

#undef new
#include <vector>
#include <map>
//#define new new( __FILE__, __LINE__ )

#pragma comment( lib, "fmodex_vc.lib" )

class AudioSystem
{
	public:
		FMOD::System*					m_system;
		FMOD::Sound*					m_walkingOnGrass;
		float							m_secondRemainingTillNextSound;
		map<const char*,FMOD::Sound*>	m_soundList;
		
	public:
		AudioSystem();
		~AudioSystem();
		void Initialize();
		FMOD::Sound* CreateOrLoadSound(const char* soundName);
		void PlaySoundByName(const char* soundName, int volume, bool loop);
		void UpdateTimer(float elapsedTime);
		void SetTimer(float playerSpeed);
		void Update();
};
#endif