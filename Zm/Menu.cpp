#include "stdafx.h"
#include "Menu.h"
#include "Vectors.h"
#include "Offsets.h"
#include "Helpers.h"
#include "Math.h"

Menu* Menu::Instance;
Menu::Menu() {
	this->oToggle = false;
	Offsets::gentity_t *self = GetGentity(0);
	this->oWeapon = self->PlayerInfo->Weapon1ID;
}
Menu::~Menu() {}

void Menu::Toggle() {
	this->oToggle = !this->oToggle;
}

void Menu::Draw() {
	if (this->oToggle) {
		ImGui::Begin("Zm Menu");

		ImGui::Checkbox("ESP", &oESP); 
		ImGui::Checkbox("Aimbot", &oAimbot); 
		ImGui::Checkbox("Unreal Aimbot", &oUnrealAimbot);
		ImGui::Checkbox("Crosshair", &oCrosshair);
		ImGui::Checkbox("Health", &oHealth);
		ImGui::Checkbox("Unlimited Ammo", &oAmmo);
		bool setAmmo = ImGui::Button("Set Unlimited Ammo");

		ImGui::InputInt("Speed", &oSpeed);
		bool setSpeed = ImGui::Button("Set Speed");
		bool resetSpeed = ImGui::Button("Reset Speed");

		ImGui::InputInt("Weapon ID (Add weap list later)", &oWeapon);
		bool setWeapon = ImGui::Button("Set Weapon");

		bool god = ImGui::Button("Become God");

		ImGui::InputInt("Zombie HP", &oZmHP);
		bool setHP = ImGui::Button("Set Zombies HP");

		ImGui::InputText("Name", oName, IM_ARRAYSIZE(oName));
		bool setName = ImGui::Button("Set Name");

		bool detach = ImGui::Button("Detach (F10)"); // make this do shit

		if (setSpeed) {
			int *speed = reinterpret_cast<int *>(0x29F1820);
			*speed = oSpeed;
		}
		if (resetSpeed) {
			int *speed = reinterpret_cast<int *>(0x29F1820);
			*speed = 190;
		}
		if (setWeapon) {
			Offsets::gentity_t *self = GetGentity(0);
			self->PlayerInfo->Weapon1ID = (WeaponID)oWeapon;
		}
		if (setAmmo) {
			Offsets::gentity_t *self = GetGentity(0);
			static int originalAmmo1;
			static int originalAmmo2;
			if (oAmmo) { // If unlimited
				originalAmmo1 = self->PlayerInfo->Weapon1Clip;
				originalAmmo2 = self->PlayerInfo->Weapon2Clip;

				self->PlayerInfo->Weapon1Clip = 9999;
				self->PlayerInfo->Weapon2Clip = 9999;
			} else {
				self->PlayerInfo->Weapon1Clip = originalAmmo1;
				self->PlayerInfo->Weapon2Clip = originalAmmo2;
			}
		}
		if (god) {
			int *health = reinterpret_cast<int *>(OFFSET_HEALTH);
			int *maxHealth = reinterpret_cast<int *>(OFFSET_MAXHEALTH);
			int *score = reinterpret_cast<int *>(OFFSET_SCORE);
			*maxHealth = 7000;
			*health = 7000;
			*score = 60000;
		}
		if (setHP) {
			Console::Log("Omae wa mou shindeiru");
			for (int i = 2; i < 1024; i++) {
				Offsets::gentity_t *ent = GetGentity(i);
				if (ent->Type == EntityType::ZOMBIE) {
					ent->Health = oZmHP;
				}
			}
		}
		if (setName) {
			Offsets::gentity_t *self = GetGentity(0);
			strcpy(self->PlayerInfo->Name, oName);
		}

		ImGui::End();
	}
}

Menu *Menu::GetInstance() {
	if (!Instance)
		Instance = new Menu();

	return Instance;
}

