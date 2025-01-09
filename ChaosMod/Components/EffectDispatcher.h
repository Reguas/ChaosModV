#pragma once

#include "Component.h"

#include "Effects/Effect.h"
#include "Effects/EffectData.h"
#include "Effects/EffectIdentifier.h"
#include "Effects/EffectThreads.h"
#include "Effects/EnabledEffectsMap.h"

#include <array>
#include <list>
#include <memory>
#include <string_view>
#include <vector>

#define _NODISCARD [[nodiscard]]

using DWORD64 = unsigned long long;
using WORD    = unsigned short;
using BYTE    = unsigned char;

enum class ETwitchOverlayMode;

class EffectDispatcher : public Component
{
  private:
	struct ActiveEffect
	{
		EffectIdentifier m_EffectIdentifier;

		DWORD64 m_ullThreadId = 0;

		std::string m_szName;
		std::string m_szFakeName;

		float m_fTimer   = 0.f;
		float m_fMaxTime = 0.f;

		bool m_bHideText = true;

		ActiveEffect(const EffectIdentifier &effectIdentifier, RegisteredEffect *pRegisteredEffect,
		             const std::string &szName, const std::string &szFakeName, float fTimer)
		{
			m_EffectIdentifier          = effectIdentifier;
			m_szName                    = szName;
			m_szFakeName                = szFakeName;
			m_fTimer                    = fTimer;
			m_fMaxTime                  = fTimer;

			EEffectTimedType eTimedType = g_dictEnabledEffects.at(effectIdentifier).TimedType;

			m_ullThreadId               = EffectThreads::CreateThread(
                pRegisteredEffect, eTimedType != EEffectTimedType::Unk && eTimedType != EEffectTimedType::NotTimed);
		}
	};

	enum TravelledDistanceType
	{
		Distance,
		Displacement
	};

	std::array<BYTE, 3> m_rgTimerColor;
	std::array<BYTE, 3> m_rgTextColor;
	std::array<BYTE, 3> m_rgEffectTimerColor;

	bool m_bDisableDrawTimerBar          = false;
	bool m_bDisableDrawEffectTexts       = false;

	WORD m_usEffectSpawnTime             = 0;
	WORD m_usEffectTimedDur              = 0;
	WORD m_usEffectTimedShortDur         = 0;

	WORD m_usMetaEffectSpawnTime         = 0;
	WORD m_usMetaEffectTimedDur          = 0;
	WORD m_usMetaEffectShortDur          = 0;

	int m_iMaxRunningEffects             = 0;

	float m_fTimerPercentage             = 0.f;
	float m_fEffectsInnerSpacingMax      = .075f;
	float m_fEffectsInnerSpacingMin      = .030f;
	float m_fEffectsTopSpacingDefault    = .2f;
	float m_fEffectsTopSpacingWithVoting = .35f;

	std::vector<ActiveEffect> m_rgActiveEffects;
	std::vector<RegisteredEffect *> m_rgPermanentEffects;
	std::list<RegisteredEffect *> m_rgDispatchedEffectsLog;

	bool m_bEnableNormalEffectDispatch        = true;
	bool m_bEnableDistanceBasedEffectDispatch = false;

	float m_fDistanceToActivateEffect         = 500.f;
	Vector3 m_vSavedPosition                  = Vector3();
	TravelledDistanceType m_distanceType      = Distance;

	DWORD64 m_ullTimer                        = 0;

	bool m_bMetaEffectsEnabled                = true;
	float m_fMetaEffectTimerPercentage        = 0.f;

	bool m_bEnableTwitchVoting;
	ETwitchOverlayMode m_eTwitchOverlayMode;

	bool m_bSuspended = false;
	bool m_bPause = false;

  public:
	bool m_bPauseTimer              = false;

	bool m_bDispatchEffectsOnTimer  = true;

	float m_fFakeTimerBarPercentage = 0.f;

  protected:
	EffectDispatcher(bool bSuspended, const std::array<BYTE, 3> &rgTimerColor, const std::array<BYTE, 3> &rgTextColor,
	                 const std::array<BYTE, 3> &rgEffectTimerColor);

  private:
	void UpdateTimer(int iDeltaTime);
	void UpdateEffects(int iDeltaTime);
	void UpdateMetaEffects(int iDeltaTime);
	void UpdateTravelledDistance();
	float GetEffectTopSpace();

  public:
	virtual void OnModPauseCleanup() override;
	virtual void OnRun() override;

	void DrawTimerBar();
	void DrawEffectTexts();

	_NODISCARD bool ShouldDispatchEffectNow() const;

	_NODISCARD int GetRemainingTimerTime() const;

	void DispatchEffect(const EffectIdentifier &effectIdentifier, const char *szSuffix = nullptr,
	                    bool bAddToLog = true);
	void DispatchRandomEffect(const char *szSuffix = nullptr);

	void ClearEffect(const EffectIdentifier &effectId);
	void ClearEffects(bool bIncludePermanent = true);
	void ClearActiveEffects(const EffectIdentifier &exclude = EffectIdentifier());
	void ClearMostRecentEffect();

	float GetRemainingTimeForEffect(const EffectIdentifier &effectId);

	std::vector<RegisteredEffect *> GetRecentEffects(int distance, std::string_view ignoreEffect = {}) const;

	inline bool IsSuspended()
	{
		return m_bSuspended;
	}

	inline void SetPaused(bool value)
	{
		m_bPause = value;
	}

	inline bool IsDispatchingEffectsOnDistance()
	{
		return m_bEnableDistanceBasedEffectDispatch;
	}

	void Reset(bool bSuspended = false);
	void ResetTimer();

	void OverrideEffectName(std::string_view effectId, const std::string &szOverrideName);
	void OverrideEffectNameId(std::string_view effectId, std::string_view fakeEffectId);

	template <class T>
	requires std::is_base_of_v<Component, T>
	friend struct ComponentHolder;
};
