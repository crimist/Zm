// Zm.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Helpers.h"
#include "Offsets.h"
#include "Zm.h"
#include "Math.h"

void Zm::Dump() {
	Offsets::Ammo *ammo = (Offsets::Ammo *)OFFSET_AMMO;
	Offsets::PlayerInfo *player = (Offsets::PlayerInfo *)OFFSET_PLAYERINFO;

	Console::Value("Wep1", ammo->Wep1);
	Console::Value("Wep1Akimbo", ammo->Wep1Akimbo);
	Console::Value("Wep2", ammo->Wep2);
	Console::Value("Wep2Akimbo", ammo->Wep2Akimbo);
	Console::Value("Wep3", ammo->Wep3);
	Console::Value("WepGrenadeLthl", ammo->WepGrenadeLthl);
	Console::Value("WepGrenadeTtcl", ammo->WepGrenadeTtcl);

	Console::Value("Pos0", player->Position[0]);
	Console::Value("Pos1", player->Position[1]);
	Console::Value("Pos2", player->Position[2]);
	Console::Value("LastJump", player->LastJump);
	Console::Value("ADS", player->ADS);
	Console::Value("Angle0", player->Angle[0]);
	Console::Value("Angle1", player->Angle[1]);
	Console::Value("Money", player->Money);
	Console::Value("Kills", player->Kills);
	Console::Value("Revives", player->Revives);
	Console::Value("Downs", player->Downs);
	Console::Value("Headshots", player->Headshots);

	//ListAllZombies(); // Worse old method I found first
	ListAllZombies2();
}

void Zm::ListAllZombies() {
	/* Notes: 
		ZCount is slower to update than memory because it waits
			until the zombies have crawled out of the ground
	Pattern:
	0x21C59D0
	+0x4	0x21C59D4 // i = 0
	+0x31C	0x21C5CF0 // i = 1
	+0x318	0x21C6008 // i = 2
	+0x4	0x21C600C
	+0x318	0x21C6324
	+0x4	0x21C6328
	+0x318	0x21C6640
	+0x4	0x21C6644
	??*/

	//int ZCount = 0x2330388; // Zombie count
	//int ZBase = 0x21C59D0; // Lowest zombie address aka base
	//int *ZCount = (int *)Offsets::ZCount;
	//int address = Offsets::ZBase;
	int *ZCount = (int *)0x2330388;
	int address = 0x21C59D0;
	bool is4 = true;

	std::cout << *ZCount << std::endl;

	for (int i = 0; i < *ZCount; i++) {
		int *health = (int *)address;

		if (*health > 0) // Alive
			std::cout << "[" << i << "] " << std::hex << address << std::dec << " " << *health << std::endl;
		else { // Dead
			*ZCount = *ZCount + 1; // Make it find more zombies
			std::cout << "Dead" << std::endl;
		}

		if (i == 1) {
			address = address + 0x31C;
			continue;
		}
		else if (is4 == true)
			address = address + 0x4;
		else
			address = address + 0x318;

		is4 = !is4;
	}
}

void Zm::ListAllZombies2() {
	Offsets::gentity_t *self = GetGentity(0);

	for (int i = 0; i < 1024; i++) {
		Offsets::gentity_t *ent = GetGentity(i);
		if (ent->Health > 0 && ent->ClientNum < 100 && (ent->Type == EntityType::ZOMBIE || ent->Type == EntityType::PLAYER)) {
			Console::Value("Client#", ent->ClientNum);
			Console::Value("Position0", ent->Position[0]);
			Console::Value("Position1", ent->Position[1]);
			Console::Value("Position2", ent->Position[2]);
			Console::Value("Type", ent->Type);
			Console::Value("Health", ent->Health);
			Console::Value("Team", ent->TeamInfo->Team);

			//if (ent->Type == EntityType::PLAYER) {
			//	std::cout << "PlayerInfo Gravity: " << ent->PlayerInfo->Gravity << std::endl;
			//	std::cout << "PlayerInfo Weapon2Clip: " << ent->PlayerInfo->Weapon2Clip << std::endl;
			//	std::cout << "PlayerInfo Headshots: " << ent->PlayerInfo->Headshots << std::endl;
			//	std::cout << "PlayerInfo Headshots&: " << &ent->PlayerInfo->Headshots << std::endl;
			//	std::cout << "PlayerInfo Kills: " << ent->PlayerInfo->Kills << std::endl;
			//	std::cout << "PlayerInfo Kills&: " << &ent->PlayerInfo->Kills << std::endl;
			//	std::cout << "PlayerInfo Score: " << ent->PlayerInfo->Score << std::endl;
			//	std::cout << "PlayerInfo Score&: " << &ent->PlayerInfo->Score << std::endl;
			//	std::cout << "PlayerInfo LethalAmmo: " << ent->PlayerInfo->LethalAmmo << std::endl;
			//	std::cout << "PlayerInfo LethalAmmo&: " << &ent->PlayerInfo->LethalAmmo << std::endl;
			//	std::cout << "PlayerInfo Name: " << ent->PlayerInfo->Name << std::endl;
			//	std::cout << "PlayerInfo Name&: " << &ent->PlayerInfo->Name << std::endl;
			//	std::cout << "PlayerInfo Weapon1Clip: " << ent->PlayerInfo->Weapon1Clip << std::endl;
			//}

			Console::Value("Distance", Math::VecDistance(self->Position, ent->Position));
		}
	}
}