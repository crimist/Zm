#pragma once
#include "Vectors.h"
#include "Helpers.h"
#include "Offsets.h"

namespace Math {
	float VecDistance(vec3 Self, vec3 Zombie);
	bool WorldToScreen(Vector3 pos, Vector3 &screen, float matrix[16]);
}