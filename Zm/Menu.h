#pragma once

class Menu {
public:
	void Toggle();
	void Draw();

	static Menu *GetInstance();

	// Cheat options
	bool oUnrealAimbot = false;
	bool oESP = false;
	bool oESPHealth = false;
	bool oSnap = false;
	bool oAimbot = false;
	bool oCrosshair = false;
	bool oHealth = false;
	bool oAmmo = false;
	bool oTrigger = false;
	bool oDebug = false;
	int oTriggerDelay = 50;
	int oWeaponDropdown = 0;
	bool oWeaponChange2 = false;
	int oSpeed = 190;
	int oZmHP = 1;
	char oName[32];

	bool oToggle = false;
private:
	Menu();
	~Menu();

	static Menu *Instance;
};
