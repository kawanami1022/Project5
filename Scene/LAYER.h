#pragma once

enum class LAYER
{
	BG,
	CHAR,
	UI,
	EX,
	MAX
};

LAYER begin(LAYER);
LAYER end(LAYER);
LAYER operator*(LAYER layer);
LAYER operator++(LAYER& layer);

