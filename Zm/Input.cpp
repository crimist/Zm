#include "stdafx.h"
#include "Input.h"
#include "Helpers.h"
#include "Vectors.h"
#include "Math.h"
#include "Game.h"

Input::Input() {}
Input::~Input() {}

void Input::Thread() { // Have NumLock on
	Console::Log("Input::Thread() started");

	Vector3 oldPos;
	SHORT keystate; // while will assume int32 otherwise because cpp

	while (1) {
		if ((keystate = GetKeyState(VK_F9) & 0x8000)) { // F9 teleport to nearest ent on cursur
			Offsets::gentity_t *self = GetGentity(0);
			POINT curs;
			GetCursorPos(&curs);
			int closeID = 0;
			POINT closest = { 0xFFFF, 0xFFFF }; // I hope no one has a display over 65535x65535 pixels

			float viewMatrix[16];
			int v = 0;
			for (int i = 0; i < 64; i = (i + 0x4)) {
				float *base = reinterpret_cast<float *>(0x033F0400 + i);
				viewMatrix[v++] = *base;
			}

			for (int i = 20; i < 200; i++) { // Ignore ourselves with the starting 1
				Offsets::gentity_t *ent = GetGentity(i);
				if (ent->Health > 0 && ent->Type == EntityType::ZOMBIE) {
					Vector3 screen;
					Vector3 pos;
					pos.x = ent->Position[0];
					pos.y = ent->Position[1];
					pos.z = ent->Position[2];

					if (Math::WorldToScreen(pos, screen, viewMatrix)) {
						if (curs.x - screen.x < closest.x) {
							closest.x = curs.x - screen.x;
							closeID = ent->ClientNum;
						}
					}
				}
			}

			Offsets::gentity_t *ent = GetGentity(closeID);

			oldPos.x = Offsets::Pos->x;
			oldPos.y = Offsets::Pos->y;
			oldPos.z = Offsets::Pos->z;
			Offsets::Pos->x = ent->Position[0];
			Offsets::Pos->y = ent->Position[1];
			Offsets::Pos->z = ent->Position[2];
			Console::Log("Teleported");
		}
		if ((keystate = GetKeyState(VK_F8) & 0x8000)) { // Go back to old position
			Offsets::Pos->x = oldPos.x;
			Offsets::Pos->y = oldPos.y;
			Offsets::Pos->z = oldPos.z;
		}
		if ((keystate = GetKeyState(VK_F7) & 0x8000)) { // Register tag j_head
			int ret = Game::RegisterTag("j_head");
			Console::Value("ret", ret);
		}
		if ((keystate = GetKeyState(VK_F6) & 0x8000)) { // Get client wep1 id
			Offsets::gentity_t *self = GetGentity(0);
			Console::Value("self->PlayerInfo->Weapon1ID", self->PlayerInfo->Weapon1ID);
		}
		if ((keystate = GetKeyState(VK_F4) & 0x8000)) { // Set all zombies health to 1 = 1 hit die
			Console::Log("F4: Setting zombie health");
			for (int i = 2; i < 1024; i++) {
				Offsets::gentity_t *ent = GetGentity(i);
				if (ent->Type == EntityType::ZOMBIE) {
					ent->Health = 1;
				}
			}
		}

		Sleep(50);
	}
}