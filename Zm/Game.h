#pragma once

namespace Game {
	typedef unsigned short(__cdecl *tRegisterTag)(const char *szName);
	extern tRegisterTag RegisterTag;

	// int __cdecl sub_6013A0(int, char *, char)

	//typedef signed int ( _cdecl* tGetTagPos) (entity_t *pEntity,DWORD dwMeshResult,int iTag,float *flOut);
	typedef signed int(__cdecl *tGetTagPos)(void);
	extern tGetTagPos GetTagPos;
}
