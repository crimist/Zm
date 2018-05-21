#pragma once
#include "Vectors.h"

// Offsets and structs
// I'm using zm version 41.627.1	
// Some credits (all the offsets and some of their structs were wrong but they gave me a foundation)
// stevemk14ebr weapon list
// CypherPresents enums
// CypherPresents structs
// Whoever made "Zombies Hack - BO2" structs 

// shitty Tyepdef i need to delet
typedef char pad;
typedef float vec2[2];
typedef float vec3[3];

// https://www.unknowncheats.me/forum/call-duty-black-ops-2/82221-black-ops-2-update-14-december-2012-a.html
// + 0xF1C00

//#define OFFSET_RegisterTag 0x004AE070 // I think?

// AimTarget_GetTagPos: Cannot find tag [%s] on entity

// GetTagPos()
// 0055C320 prob
// 006013A0 #2

/* https://www.unknowncheats.me/forum/call-of-duty-black-ops-2-a/106397-offsets-41-11540-7-a.html
Some CE stuff

RegisterFont		(t6zm.exe+332640)	8B 44 24 04 6A FF 6A 01 50 6A 14
NullPointer			(t6zm.exe+1C1900)	A1 ?? ?? ?? ?? 83 EC 24 55 8B 2D ?? ?? ?? ??
GetTagPos			(Failed):			56 8B 74 24 08 57 33 FF 66 83 BE ?? ?? ?? ?? ??
RegisterTag			(t6zm.exe+20F360)	8B 54 24 04 8B C2 56 8D 70 01 8D 9B ?? ?? ?? ?? 8A 08 40 84 C9 75 F9 2B C6 6A 06 40 50 6A 01 52 E8 ?? ?? ?? ?? 83 C4 10 5E C3
World2Screen		(t6zm.exe+39B190)	56 8B 35 ?? ?? ?? ?? 50 E8 ?? ?? ?? ?? F3 0F 10 17
C_GS				(t6zm.exe+1E8B9)	A1 ?? ?? ?? ?? 33 ED 89 44 24 3C 39 68 2C
RegisterShader		(t6zm.exe+3333C0)	8B 44 24 04 80 38 00 75 06 A1 ?? ?? ?? ?? C3
GetWeapon			(t6zm.exe+22CAC0)	8B 44 24 04 25 ?? ?? ?? ?? 8B 04 85 ?? ?? ?? ?? C3
Camera:(t6zm.exe+86847)(t6zm.exe+271367)23 05 ?? ?? ?? ?? 83 78 ?? ?? 75 03 33 C0
Trace:				(t6zm.exe+1FCBA0)	83 EC 28 56 8B 74 24 40 57 56 8D 4C 24 0C
Console				(t6zm.exe+2B9D20)	6A 33 E8 ?? ?? ?? ?? 8B 4C 24 0C 83 C4 04
NullPointerCamera	(t6zm.exe+52B409)	23 2D ?? ?? ?? ?? 8B 8D ?? ?? ?? ?? 8B C1
NameTags			(Failed)			83 EC 68 F3 0F 10 05 ?? ?? ?? ?? 0F 2F 44 24 74 C6 44 24 ?? ??

*/

#define OFFSET_VIEWMATRIX 0x33F0400
#define OFFSET_ZCOUNT 0x2330388
#define OFFSET_AMMO 0x2342BC8
#define OFFSET_PLAYERINFO 0x23427C8
#define OFFSET_PLAYERINFO_T 0x23427A0
#define OFFSET_GENTITY_BASE 0x21C13C0 // Same as player but for clairity
#define OFFSET_GENTITY_PLAYER 0x21C13C0 // Player0
#define OFFSET_GENTITY_ZOMBIE 0x21C5828 // Zombie0
#define OFFSET_GENTITY_SIZE 0x31C
#define OFFSET_ENTITY 0x2E4EDCC0
#define OFFSET_ENTITY_SIZE 0x380
#define OFFSET_SPEED 0x29F1820
#define OFFSET_ADS 0x0103ACA0
#define OFFSET_ZOOM 0x103AD20

#define OFFSET_ROUND 0x233B710
#define OFFSET_HEALTH 0x21C1568
#define OFFSET_MAXHEALTH 0x2347C6C
#define OFFSET_SCORE 0x2347D68
#define OFFSET_POSITION 0x23427C8

#pragma region Enums
enum WeaponID : __int32 {
	defaultweapon_mp = 1,
	ak74u_zm = 2,
	mp5k_zm = 3,
	qcw05_zm = 4,
	ak74u_upgraded_zm = 5,
	mp5k_upgraded_zm = 6,
	qcw05_upgraded_zm = 7,
	m14_zm = 9,
	m16_zm = 10,
	saritch_zm = 11,
	xm8_zm = 12,
	type95_zm = 13,
	tar21_zm = 14,
	m14_upgraded_zm = 15,
	m16_gl_upgraded_zm = 16,
	saritch_upgraded_zm = 18,
	xm8_upgraded_zm = 20,
	type95_upgraded_zm = 22,
	tar21_upgraded_zm = 24,
	gun870mcs_zm = 26,
	rottweil72_zm = 27,
	saiga12_zm = 28,
	srm1216_zm = 29,
	gun870mcs_upgraded_zm = 30,
	rottweil72_upgraded_zm = 31,
	saiga12_upgraded_zm = 32,
	srm1216_upgraded_zm = 33,
	galil_zm = 34,
	fnfal_zm = 35,
	rpd_zm = 36,
	hamr_zm = 37,
	galil_upgraded_zm = 38,
	fnfal_upgraded_zm = 39,
	rpd_upgraded_zm = 40,
	hamr_upgraded_zm = 41,
	dsr50_zm = 42,
	barretm82_zm = 43,
	dsr50_upgraded_zm = 44,
	barretm82_upgraded_zm = 45,
	knife_zm = 46,
	m1911_zm = 47,
	python_zm = 48,
	judge_zm = 49,
	kard_zm = 50,
	fiveseven_zm = 51,
	beretta93r_zm = 52,
	fivesevendw_zm = 53,
	m1911_upgraded_zm = 55,
	python_upgraded_zm = 57,
	judge_upgraded_zm = 58,
	kard_upgraded_zm = 59,
	fiveseven_upgraded_zm = 60,
	beretta93r_upgraded_zm = 61,
	fivesevendw_upgraded_zm = 62,
	usrpg_zm = 64,
	m32_zm = 65,
	usrpg_upgraded_zm = 66,
	m32_upgraded_zm = 67,
	ray_gun_zm = 68,
	jetgun_zm = 69,
	riotshield_zm = 70,
	tazer_knuckles_zm = 71,
	knife_ballistic_zm = 72,
	knife_ballistic_upgraded_zm = 73,
	knife_ballistic_bowie_zm = 74,
	knife_ballistic_bowie_upgraded_zm = 75,
	raygun_mark2_zm = 76,
	raygun_mark2_upgraded_zm = 77,
	ray_gun_upgraded_zm = 78,
	frag_grenade_zm = 79,
	sticky_grenade_zm = 80,
	cymbal_monkey_zm = 81,
	emp_grenade_zm = 82,
	claymore_zm = 83,
	knife_ballistic_no_melee_zm = 84,
	knife_ballistic_no_melee_upgraded_zm = 85,
	equip_turbine_zm = 86,
	equip_turret_zm = 87,
	equip_electrictrap_zm = 88,
	zombie_builder_zm = 89,
	buildable_piece_zm = 90,
	syrette_zm = 91,
	zombie_knuckle_crack = 92,
	zombie_perk_bottle_doubletap = 93,
	zombie_perk_bottle_jugg = 94,
	zombie_perk_bottle_marathon = 95,
	zombie_perk_bottle_revive = 96,
	zombie_perk_bottle_sleight = 97,
	zombie_perk_bottle_tombstone = 98,
	zombie_fists_zm = 99,
	zombie_tazer_flourish = 100,
	bowie_knife_zm = 101,
	zombie_bowie_flourish = 102,
	death_throe_zm = 103,
	death_self_zm = 104,
};

enum EntityType : unsigned short {
	GENERAL,
	PLAYER,
	PLAYER_CORPSE,
	ITEM,
	MISSILE,
	INVISIBLE,
	SCRIPTMOVIER,
	SOUND_BLEND,
	FX,
	LOOP_FX,
	PRIMARY_LIGHT,
	TURRET,
	HELICOPTER,
	PLANE,
	VEHICLE,
	VEHICLE_CORPSE,
	ZOMBIE,
	ZOMBIE_SPAWNER,
	ZOMBIE_CORPSE,
	STREAMER_HINT,
	ZBARRIER,
};

enum EntityFlags : unsigned __int32 {
	STANDING = 2,
	CROUCH = 6,
	PRONE = 10,
	BARRIER = 237,
	ZM = 67110914,
};

enum TeamType : __int32 {
	FRIEND = 1,
};

enum PerkFlags : __int32 {
	//0x10 = Reduced Spread, 0x80000 = Faster Knife
	A = 1 << 0, // binary 0001
	B = 1 << 1, // binary 0010
	C = 1 << 2, // binary 0100
	D = 1 << 3, // binary 1000
};

#pragma endregion

namespace Offsets {
#pragma pack(push,1)
	struct Ammo { // This is not in order FIX IT!
		__int32 Wep3;			// 0x00 -> 2342BC8
		__int32 Wep1; 			// 0x04 -> 2342BCC
		__int32 WepGrenadeLthl; // 0x08 -> 2342BD0
		__int32 Wep2; 			// 0x0C -> 2342BD4
		__int32 Wep1Akimbo;		// 0x10 -> 2342BD8
		__int32 WepGrenadeTtcl; // 0x14 -> 2342BDC
		__int32 Wep2Akimbo; 	// 0x18 -> 2342BE0
	};

	// this is PlayerActor_t!!!
	struct PlayerInfo {
		vec3 Position;			// 0x00 -> 23427C8 // Position (x,y,z)
		vec3 Velocity;			// 0x0C -> 23427D4 // Velocity (x,y,z)
		char pad_0x84[0x84];	// 23427E0;
		float LastJump;			// 2342864 // Height where standing on last jump
		char pad_0x118[0x118];	
		float ADS;				// 2342980 // % (0.00 to 1.00) of how much ADS
		char pad_0x14[0x14];
		vec2 Angle;				// 2342998 // x(-85up 85down) y(-180 180)
		char pad_0x53D0[0x53C8];
		__int32 Money;			// 2347D68
		__int32 Kills2;			// 2347D6C
		char pad_0x10[0x10];
		__int32 Kills;			// 2347D80
		__int32 Downs;			// 2347D84
		__int32 Revives;		// 2347D88
		__int32 Headshots;		// 2347D8C
		char pad_0x4[0x4];
		__int32 __8;			// Revives???
	};
	
	struct PlayerInfo_t {
		__int32 ServerTime;		// 0x0000 -> 23427A0
		pad _0x0004[0x24];		// 0x0004
		vec3 Position;			// 0x0028 -> 23427C8 // Position (x,y,z)
		vec3 Velocity;			// 0x0034 -> 23427D4 // Velocity (x,y,z)
		pad _0x0040[0x4C];		// 0x0040
		__int32 Gravity;		// 0x008C -> 234282C
		pad _0x0090[0x4];		// 0x0090
		__int32 Speed;			// 0x0094 -> 2342834
		pad _0x0098[0x2C];		// 0x0098
		float LastJump;			// 0x00C4 -> 2342864 // Height where standing on last jump
		pad _0x00C8[0x118];		// 0x00C8
		float ADS;				// 0x01E4 -> 2342980 // % (0.00 to 1.00) of how much ADS
		pad _0x01E8[0x14];		// 0x01E8
		vec2 Angle;				// 0x01FC -> 2342998 // x(-85up 85down) y(-180 180)
		pad _0x0200[0x04];		// 0x0200
		__int32 HeightInt;		// 0x0204 -> 23429A4
		float HeightFloat;		// 0x0208 
		pad _0x020C[0x2C];		// 0x020C
		__int32 Health;			// 0x0238 -> 23429D8 // Read only
		pad _0x023C[0x4];		// 0x023C
		__int32 MaxHealth;		// 0x0240 // xd doesn't work
		pad _0x0244[0x04];		// 0x0244
		__int32 Weapon2ID;		// 0x0248 -> 23429E8
		pad _0x024C[0x18];		// 0x024C -> 23429EC
		__int32 Weapon1ID;		// 0x0264 -> 2342A04
		pad _0x0268[0x18];		// 0x0268 -> 2342A08
		__int32 LethalID;		// 0x0280 -> 2342A20
		char _0x0284[0x18];		// 0x0284 -> 2342A24
		__int32 TacticalID;	// 0x02B8 -> 2342A3C
		char _0x02BC[0x14C];	// 0x02BC -> 2342A40
		__int32 Weapon1Stock;	// 0x03F0 -> 2342B8C
		char _0x03F4[0x4];		// 0x03F4
		__int32 Weapon2Stock;	// 0x03F8 
		char _0x03FC[0x30];		// 0x03FC
		__int32 Weapon1Clip;	// 0x042C -> 2342BC8
		pad fuk[0x4];			// 0x042C
		__int32 LethalAmmo;		// 0x0430 -> 2342BD0
		__int32 Weapon2Clip;	// 0x0434 
		__int32 TacticalAmmo;	// 0x0438 
		char _0x043C[0x110];	// 0x043C
		__int32 PerkFlags;		// 0x054C // 0x10 = Reduced Spread, 0x80000 = Faster Knife
		char _0x0550[0x4F5C];	// 0x0550
		char NameReadOnly[32];	// 0x54AC -> 2347C4C // Readonly 
		char _0x54BC[0x20];		// 0x54BC
		pad fuckinghell[0x48];
		char Name[32];			// 0x5534 -> 2347CD4 // Writable (Null terminate plz)
		char _0x5544[0x74];		// 0x5544
		__int32 Score;			// 0x55C8 -> 2347D68
		__int32 Kills1;			// 0x55CC -> 2347D6C
		pad _0x55D0[0x10];		// 0x55D0 -> 2347D70
		__int32 Kills;			// 0x55E0 -> 2347D80
		__int32 Downs;			// 0x55E4 -> 2347D84
		__int32 Revives;		// 0x55E8 -> 2347D88
		__int32 Headshots;		// 0x55EC -> 2347D8C
		pad _0x55F0[0x08];		// 0x55F0 -> 2347D90
		__int32 Headshots1;		// 0x55F8 -> 2347D98
		pad _0x55FC[0x28];		// 0x55FC -> 2347D9C
		__int32 Kills2;			// 0x5620 -> 2347DC0
		pad _0x5624[0x38];		// 0x5624 -> 2347DC4
		__int32 Headshots2;		// 0x565C -> 2347DFC
	}; // Size 0x045C ???

	struct TeamInfo_t {
		char _0x0000[4];	// 0x0000
		TeamType Team;	// 0x0004 // TeamType
		char _0x0008[56];	// 0x0008

	}; // Size 0x0040

	struct gentity_t {
		__int32 ClientNum;			// 0x0000
		EntityFlags Flags;			// 0x0004 // EntityFlags
		char _0x0008[16];			// 0x0008
		vec3 Position;				// 0x0018 // Position
		char _0x0024[24];			// 0x0024
		vec2 Angles;				// 0x003C // Angles x(-85up 85down) y(-180 180)
		char _0x0044[28];			// 0x0044
		__int32 HeldWeapon;			// 0x0060 // Weapon they're currently using
		char _0x0064[116];			// 0x0064
		EntityType Type;			// 0x00D8
		char _0x00DA[66];			// 0x00DA
		vec3 Position2;				// 0x011C // Other position ??
		char _0x0128[44];			// 0x0128
		PlayerInfo_t *PlayerInfo;	// 0x0154 
		char _0x0158[4];			// 0x0158
		TeamInfo_t *TeamInfo;		// 0x015C 
		char _0x0160[12];			// 0x0160
		__int32 ModelIndex;			// 0x016C // Model ID (44 == Bus)
		char _0x0170[56];			// 0x0170
		__int32 Health;				// 0x01A8 // Health
		char _0x01AC[29320];		// 0x01AC
	};

	struct entity_t { // 0x2E4EDCC4 wow
		char _0x0000[0x4];                            //0x0
		int iPlayerMesh;                              //0x4 (0x2E4EDCC4)
		char _0x0008[0x24];                           //0x8
		Vector3 vOrigin;                              //0x2C (0x2E4EDCEC)
		char _0x0038[0x13C];                          //0x38
		Vector3 vOldOrigin;                           //0x174 (0x2E4EDE34)
		char _0x0180[0x5C];                           //0x180
		int iClientNum;                               //0x1DC (0x2E4EDE9C)
		int iFlags;                                   //0x1E0 (0x2E4EDEA0)
		char _0x01E4[0x10];                           //0x1E4
		Vector3 vNewOrigin;                           //0x1F4 (0x2E4EDEB4)
		char _0x0200[0xB4];                           //0x200
		short shType;                                 //0x2B4 (0x2E4EDF74)
		char _0x02B6[0xA];                            //0x2B6
		int iWeaponID;                                //0x2C0 (0x2E4EDF80)
		char _0x02C4[0xB4];                           //0x2C4
		unsigned char bAlive;                         //0x378 (0x2E4EE038)
		char _0x0379[0x7];                            //0x379
	}; // size = 0x380

	struct ScreenSize { // ????
		__int32 Width;	// 0x00 -> 011A554C
		__int32 Height;	// 0x04 -> 011A5550
		__int32 WidthWrite;	// 0x08 -> 011A5554
		__int32 HeightWrite;// 0x0C -> 011A5558
	};

	struct AnglePos { // ????
		vec3 Angle;		// 0x00 -> 011A58C4 // angle in percent??
		vec3 Position;	// 0x10 -> 011A58D0
	};
#pragma pack(pop)

	// Globals I guess
	extern HWND SelfWnd;
	extern Vector3 *Pos;
	extern ScreenSize *Screen;
	extern entity_t *Ent;
	extern gentity_t *GEnt;
	extern const char *WeaponList[95];
	extern int WepLookup[];
}
