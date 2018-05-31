#include "AudioSystem.hpp"
#include "../JobSystem/ArchiveManager.hpp"
#include "../JobSystem/MemoryPoolManager.hpp"

AudioSystem::AudioSystem()
{

}

AudioSystem::~AudioSystem()
{

}

void AudioSystem::Initialize()
{
	FMOD::System_Create(&m_system);
	m_system->init(32,FMOD_INIT_NORMAL,0);
	m_secondRemainingTillNextSound = 0.f;
}

FMOD::Sound* AudioSystem::CreateOrLoadSound(const char* fileName)
{
	map< const char*, FMOD::Sound* >::iterator it ;
	for( it = m_soundList.begin(); it != m_soundList.end(); ++it )
	{
		if(it->first == fileName)
		{
			return it->second;
		}
	}
	
	FMOD_CREATESOUNDEXINFO info;
	memset(&info, 0, sizeof(info));
	info.cbsize = sizeof(info);


	FMOD::Sound* newSound = nullptr;

	
	switch( ArchiveManager::s_loadMode )
	{
		case LOAD_FROM_DISK: m_system->createSound(fileName, FMOD_DEFAULT, 0, &newSound);
							 break;
		case LOAD_FROM_ARCHIVE: 
		{
			FileInArchiveInfo soundFile = ArchiveManager::LoadFileFromArchive( fileName );
			info.length = soundFile.fileSize;
			if( soundFile.fileSize != -1 )
			{
				m_system->createSound( (const char*)soundFile.fileContent, FMOD_OPENMEMORY, &info, &newSound );
			}
			break;
		}
		case PREFER_LOAD_FROM_DISK:
		{
			m_system->createSound(fileName, FMOD_DEFAULT, 0, &newSound);
			if( newSound == nullptr )
			{
				FileInArchiveInfo soundFile = ArchiveManager::LoadFileFromArchive( fileName );
				info.length = soundFile.fileSize;
				if( soundFile.fileSize != -1 )
				{
					m_system->createSound( (const char*)soundFile.fileContent, FMOD_OPENMEMORY, &info, &newSound );
				}
			}
			break;
		}
		case PREFER_LOAD_FROM_ARCHIVE:
		{
			FileInArchiveInfo soundFile = ArchiveManager::LoadFileFromArchive( fileName );
			info.length = soundFile.fileSize;
			if( soundFile.fileSize != -1 )
			{
				m_system->createSound( (const char*)soundFile.fileContent, FMOD_OPENMEMORY, &info, &newSound );
			}
			if( newSound == nullptr )
			{
				m_system->createSound(fileName, FMOD_DEFAULT, 0, &newSound);
			}
			break;
		}
	}
	
	m_soundList[ fileName ] = newSound;

	return newSound;
}

void AudioSystem::PlaySoundByName(const char* soundName, int volume, bool loop)
{
	FMOD::Sound* audioStream = CreateOrLoadSound(soundName);
	FMOD::Channel* audioChannel;
	
	m_system->playSound(FMOD_CHANNEL_FREE, audioStream, false, &audioChannel);

	audioChannel->setVolume( static_cast<float> ( volume ) );
	if(loop)
		audioChannel->setMode(FMOD_LOOP_NORMAL);
}


void AudioSystem::UpdateTimer(float elapsedTime)
{
	if(m_secondRemainingTillNextSound > 0.f)
		m_secondRemainingTillNextSound -= elapsedTime;
}

void AudioSystem::SetTimer(float /*playerSpeed*/)
{
	m_secondRemainingTillNextSound = ( static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/0.25f)) ) + 0.25f;

	m_secondRemainingTillNextSound = m_secondRemainingTillNextSound ;
}

void AudioSystem::Update()
{
	m_system->update();
}




