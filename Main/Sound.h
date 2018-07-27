#pragma once
#include <SFML/Audio.hpp>
#include "TypeDefines.h"
using namespace sf;

namespace Tijss
{
	namespace Audio
	{
		class cSound
		{
		public:
			cSound(string fileName);
			~cSound();

			static void Init(string soundFolderPath)
			{
				m_SoundFolder = soundFolderPath + "\\";
			}

			// sound value getters and setters

			float getDuration()
			{
				/*return m_Buffer.getDuration().asSeconds;*/
				return 0;
			}

		private:
			SoundBuffer m_Buffer; // stores the sound data
			static string m_SoundFolder;

		public:
			sf::Sound m_Sound;		  // plays the sound
		};
	}
	


} //  namepsace snd