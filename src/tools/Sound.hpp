#pragma once
#include <string>

struct Sound
{
	std::string soundPath;
	float  pitch  { 1.f };
	float  volume { 100.f };
	bool   repeat { false };
};