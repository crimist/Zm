#include "stdafx.h"
#include "Menu.h"
#include "Vectors.h"
#include "Offsets.h"
#include "Helpers.h"
#include "Math.h"

Menu *Menu::Instance;
Menu::Menu() {
	//this->oToggle = false;
	Offsets::gentity_t *self = GetGentity(0);
	this->oWeaponDropdown = self->PlayerInfo->Weapon1ID;
	strcpy(this->oName, self->PlayerInfo->Name);
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
		ImGui::Checkbox("Triggerbot", &oTrigger);
		ImGui::InputInt("Triggerbot shot delay", &oTriggerDelay);
		ImGui::Checkbox("Crosshair", &oCrosshair);
		ImGui::Checkbox("Health", &oHealth);
		bool setAmmo = ImGui::Checkbox("Unlimited Ammo", &oAmmo);
		ImGui::Checkbox("ImGUI Debug", &oDebug);

		bool speedChanged = ImGui::InputInt("Speed", &oSpeed);
		bool resetSpeed = ImGui::Button("Reset speed to default (190)");

		bool godMode = ImGui::Button("Set 7000 HP & 60000 Score");

		bool hpChanged = ImGui::InputInt("Set Zombie HP", &oZmHP);

		bool setName = ImGui::InputText("Name", oName, IM_ARRAYSIZE(oName));

		ImGui::Checkbox("Change Weapon2?", &oWeaponChange2);
		bool wepChanged = ImGui::ListBox("Weapons", &oWeaponDropdown, Offsets::WeaponList, IM_ARRAYSIZE(Offsets::WeaponList), 5);

		if (wepChanged) {
			Console::Value("oWeaponDropdown", oWeaponDropdown);
			Offsets::gentity_t *self = GetGentity(0);
			if (oWeaponChange2)
				self->PlayerInfo->Weapon2ID = Offsets::WepLookup[oWeaponDropdown];
			else
				self->PlayerInfo->Weapon1ID = Offsets::WepLookup[oWeaponDropdown];
		}

		if (speedChanged) {
			int *speed = reinterpret_cast<int *>(OFFSET_SPEED);
			*speed = oSpeed;
		}
		if (resetSpeed) {
			int *speed = reinterpret_cast<int *>(OFFSET_SPEED);
			*speed = 190;
			oSpeed = *speed;
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
		if (godMode) {
			int *health = reinterpret_cast<int *>(OFFSET_HEALTH);
			int *maxHealth = reinterpret_cast<int *>(OFFSET_MAXHEALTH);
			int *score = reinterpret_cast<int *>(OFFSET_SCORE);
			*maxHealth = 7000;
			*health = 7000;
			*score = 60000;
		}
		if (hpChanged) {
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

	if (this->oDebug) {
		bool val = true;
		ImGui::ShowMetricsWindow(NULL);
	}
}

Menu *Menu::GetInstance() {
	if (!Instance)
		Instance = new Menu();

	return Instance;
}

