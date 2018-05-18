#include "stdafx.h"
#include "Offsets.h"

HWND Offsets::SelfWnd = FindWindow(NULL, L"Call of Duty®: Black Ops II - Zombies");
Offsets::ScreenSize *Offsets::Screen = reinterpret_cast<Offsets::ScreenSize *>(0x11A554C);
Offsets::entity_t *Offsets::Ent = reinterpret_cast<Offsets::entity_t *>(OFFSET_ENTITY);
Offsets::gentity_t *Offsets::GEnt = reinterpret_cast<Offsets::gentity_t *>(OFFSET_GENTITY_PLAYER);
Vector3 *Offsets::Pos = reinterpret_cast<Vector3 *>(OFFSET_POSITION);