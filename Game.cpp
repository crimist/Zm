#include "Game.h"

Game::tRegisterTag  Game::RegisterTag = (Game::tRegisterTag)0x4AE070;
Game::tGetTagPos Game::GetTagPos = (Game::tGetTagPos)(0x4C1230 + 0xAA4F0); // AA4F0 = difference from https://www.unknowncheats.me/forum/call-of-duty-black-ops-2-a/93117-gettagpos-crashed-game.html