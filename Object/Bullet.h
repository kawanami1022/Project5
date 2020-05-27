#pragma once
#include "Obj.h"

#define PI 3.14159265

class Bullet :
	public Obj
{
public:
	Bullet();
	Bullet(UNIT_ID unitID, Vector2Dbl pos, double speed);
	~Bullet();
	
private:
	bool init(void);
	void Update(sharedObj obj) override;
	bool DestroyPrpc(void);
	double _speed;
};

