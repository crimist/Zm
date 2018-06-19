#include "Math.h"
#include "Vectors.h"
#include "Offsets.h"

// Thanks ppl on fourms!
bool Math::WorldToScreen(Vector3 pos, Vector3 &screen, float matrix[16]) {
	// Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	vec4 clipCoords;
	clipCoords.x = pos.x*matrix[0] + pos.y*matrix[4] + pos.z*matrix[8] + matrix[12];
	clipCoords.y = pos.x*matrix[1] + pos.y*matrix[5] + pos.z*matrix[9] + matrix[13];
	clipCoords.z = pos.x*matrix[2] + pos.y*matrix[6] + pos.z*matrix[10] + matrix[14];
	clipCoords.w = pos.x*matrix[3] + pos.y*matrix[7] + pos.z*matrix[11] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	// perspective division, dividing by clip.W = Normalized Device Coordinates
	Vector3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	// Transform to window coordinates
	screen.x = (Offsets::Screen->Width / 2 * NDC.x) + (NDC.x + Offsets::Screen->Width / 2);
	screen.y = -(Offsets::Screen->Height / 2 * NDC.y) + (NDC.y + Offsets::Screen->Height / 2);

	return true;
}

float Math::VecDistance(vec3 Self, vec3 Zombie) {
	return (float)sqrt((Self[0] - Zombie[0]) * (Self[0] - Zombie[0]) + (Self[1] - Zombie[1]) * (Self[1] - Zombie[1]) + (Self[2] - Zombie[2]) * (Self[2] - Zombie[2]));
}