#pragma once
enum class Teams
{
	RED = 0,
	GREEN = 1,
	BLUE = 2,
	PURPLE = 3,
	EMPTY
};

struct Team
{
	Teams colour = Teams::EMPTY;
	int id = 0;;
};