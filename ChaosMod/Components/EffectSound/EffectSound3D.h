#pragma once

#include "Components/EffectSound/EffectSoundManager.h"

#include <miniaudio.h>
#include <scripthookv/inc/types.h>

#include <map>
#include <mutex>
#include <string>

class EffectSound3D : public EffectSoundManager
{
	ma_engine m_maEngine;
	DWORD64 m_SoundIdCounter = 1;

	struct Sound
	{
		ma_sound Handle;
		EffectSoundPlayOptions PlayOptions;
	};
	std::map<DWORD64, Sound> m_Sounds;
	std::mutex m_SoundsMutex;
	bool m_IsStopping = false;
	std::thread m_PauseSoundsThread;

  public:
	EffectSound3D();

  private:
	void FreeSounds();

  public:
	virtual void OnModPauseCleanup() override;
	virtual void OnRun() override;
	virtual DWORD64 HandleSound(const std::string &soundFile) override;
	virtual void SetSoundOptions(DWORD64 soundId, const EffectSoundPlayOptions &soundPlayOptions) override;
};