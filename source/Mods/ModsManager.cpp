// ******************************************************************************
// Filename:    ModsManager.cpp
// Project:     Vox
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 12/03/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "ModsManager.h"

#include <iostream>
#include "ini/INIReader.h"

#include <algorithm>

#include <fstream>
#include <ostream>
#include <iostream>
#include <string>
using namespace std;


ModsManager::ModsManager()
{
}

ModsManager::~ModsManager()
{
	ClearMods();
}

// Clearing
void ModsManager::ClearMods()
{
	for (unsigned int i = 0; i < m_vpMods.size(); i++)
	{
		delete m_vpMods[i];
		m_vpMods[i] = 0;
	}
	m_vpMods.clear();
}

// Loading
void ModsManager::LoadMods()
{
	string modsIniFile = "media/config/mods.ini";
	INIReader reader(modsIniFile);

	// Load the HUD textures mod
	string HUDTheme = reader.Get("HUD", "HUDTextures", "Default");
	AddMod(HUDTheme, false, false, false, true, false);
}

// Adding and removing active mods
void ModsManager::AddMod(string modName, bool gameplayMod, bool graphicsMod, bool soundMod, bool HUDMod, bool miscMod)
{
	Mod* pNewMod = new Mod();
	pNewMod->m_modName = modName;
	pNewMod->m_erase = false;
	pNewMod->m_gameplayMod = gameplayMod;
	pNewMod->m_graphicsMod = graphicsMod;
	pNewMod->m_soundMod = soundMod;
	pNewMod->m_HUDMod = HUDMod;
	pNewMod->m_miscMod = miscMod;
	m_vpMods.push_back(pNewMod);
}

bool needs_erasing(Mod* aMod)
{
	bool needsErase = aMod->m_erase;

	if (needsErase == true)
	{
		delete aMod;
	}

	return needsErase;
}

void ModsManager::RemoveMod(string modName, bool gameplayMod, bool graphicsMod, bool soundMod, bool HUDMod, bool miscMod)
{
	for (unsigned int i = 0; i < m_vpMods.size(); i++)
	{
		if (m_vpMods[i]->m_gameplayMod && gameplayMod ||
			m_vpMods[i]->m_graphicsMod && graphicsMod ||
			m_vpMods[i]->m_soundMod && soundMod ||
			m_vpMods[i]->m_HUDMod && HUDMod ||
			m_vpMods[i]->m_miscMod && miscMod)
		{
			if (m_vpMods[i]->m_modName == modName)
			{
				m_vpMods[i]->m_erase = true;
			}
		}
	}

	m_vpMods.erase(remove_if(m_vpMods.begin(), m_vpMods.end(), needs_erasing), m_vpMods.end());
}

// Accessors
int ModsManager::GetNumMods()
{
	return (int)m_vpMods.size();
}

Mod* ModsManager::GetMod(int index)
{
	return m_vpMods[index];
}

// HUD Theme
string ModsManager::GetHUDTextureTheme()
{
	for (unsigned int i = 0; i < m_vpMods.size(); i++)
	{
		if (m_vpMods[i]->m_HUDMod == true)
		{
			return m_vpMods[i]->m_modName;
		}
	}

	return "";
}