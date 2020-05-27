#pragma once
#include "Object\Obj.h"
#include <memory>
#include <Input\InputState.h>
#include <Object\Bullet.h>



class Player :
	public Obj
{
public:
	Player();
	Player(Vector2Dbl pos, Vector2Dbl size);
	void Update(sharedObj obj) override;
	~Player();
private:
	void Draw(void) override;
    void Init(void);
	std::shared_ptr<InputState> _input;
	std::vector<Bullet> _bulletList;
	int _glowID[2];
	double _zoom;
};

