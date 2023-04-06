#include <stdafx.h>

#include "Info.h"
#include "SplashTexts.h"

void SplashTexts::OnModPauseCleanup()
{
	m_activeSplashes.clear();
}

void SplashTexts::OnRun()
{
	float fFrameTime = GET_FRAME_TIME();

	for (std::list<SplashText>::iterator it = m_activeSplashes.begin(); it != m_activeSplashes.end();)
	{
		DrawScreenText(it->m_szText, it->m_textPos, it->m_fScale, it->m_textColor, true);
		it->m_fTime -= fFrameTime;

		if (it->m_fTime <= 0)
		{
			it = m_activeSplashes.erase(it);
		}
		else
		{
			it++;
		}
	} 
}

void SplashTexts::ShowSplash(const std::string &szText, const ScreenTextVector &textPos, float fScale,
                             ScreenTextColor textColor, float fTime)
{
	m_activeSplashes.emplace_back(szText, textPos, fScale, textColor, fTime);
}

void SplashTexts::ShowInitSplash()
{
	ShowSplash(std::string("Chaos Mod v" MOD_VERSION " - special build for ")
	               + (g_Random.GetRandomInt(0, 4) == 4 ? "Matto" : "DarkViperAU")
	               + "\n\nSee credits.txt for a list of contributors",
	           { .25f, .3f }, .65f, { 60, 245, 190 });
#ifdef _DEBUG
	ShowSplash("DEBUG BUILD!", { .2f, .5f }, .7f, { 255, 0, 0 });
#endif
}

void SplashTexts::ShowTwitchVotingSplash()
{
	ShowSplash("Twitch Voting Enabled!", { .86f, .7f }, .8f, { 255, 100, 100 });
}

void SplashTexts::ShowClearEffectsSplash()
{
	ShowSplash("Effects Cleared!", { .86f, .86f }, .8f, { 255, 100, 100 });
}

void SplashTexts::ShowTwitchVotingRestartSplash()
{
	ShowSplash("Voting proxy has crashed and restarted", { .85f, .93f }, .4f, { 225, 67, 240 }, SPLASH_TEXT_DUR_SECS / 2);
}