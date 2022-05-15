#include "SkyModule.h"

using namespace Sky::Modules;

SkyModule::SkyModule()
	: m_engine(), m_gameData(),
	m_isEnabled(false)
{
}

SkyModule::~SkyModule()
{
	// Don't do dis
	//if (m_name)
	//	delete[] m_name;
}

void SkyModule::Initialize(Engine& engine, GameData& gameData)
{
	m_engine = &engine;
	m_gameData = &gameData;

	OnInitialize();
}

void SkyModule::OnEnable()
{
}

void SkyModule::OnDisable()
{
}

void SkyModule::Toggle()
{
	// Call event first
	bool isEnabled = !m_isEnabled;
	if (isEnabled)
		OnEnable();
	else
		OnDisable();

	// Update variable
	m_isEnabled = isEnabled;
}

bool SkyModule::IsEnabled()
{
	return m_isEnabled;
}