#pragma once

enum class INPUT_ID  // Ï¯Ìß‚Ì·°  Šù’è‚ÌŒ^‚ðŽw’è‚Å‚«‚é
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	BTN_1,
	BTN_2,
	BTN_3,
	BTN_4,
	MAX
};

INPUT_ID begin(INPUT_ID);
INPUT_ID end(INPUT_ID);
INPUT_ID operator*(INPUT_ID key);
INPUT_ID operator++(INPUT_ID& key);
