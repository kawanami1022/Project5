#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <common\Vector2.h>

enum class STATE
{
	NORMAL,  // 通常
	EX,		 // 特殊
	DETH,	 // 爆発(死亡）
	MAX
};

enum class UNIT_ID
{
	NON,
	PLAYER,
	ENEMY,
	PL_BULLET,
	EM_BULLET
};

class Obj;  

using AnimVector = std::vector<std::pair<int, unsigned int>>;		// ID and frames
using sharedObj = std::shared_ptr<Obj>;

class Obj
{
public:
	Obj();
	virtual  void Update(sharedObj obj) = 0;


	virtual void Draw(void);
	void Draw(int id);
	virtual ~Obj();

	bool state(const STATE state);
	const STATE state(void) const;

	const UNIT_ID unitID(void) const;

	const Vector2Dbl pos(void) const;
	const Vector2Dbl size(void) const;

	bool SetAnim(const STATE Key,AnimVector& data);
	virtual bool SetAlive(bool alive);
	bool isAlive(void) { return _alive; }
	bool isDead(void) { return _dead; }
	bool isAnimEnd(void);	// ｱﾆﾒｰｼｮﾝが終了しているか
	
private:


protected:
	std::map<STATE, AnimVector> _animMap;
	STATE _state;
	//std::string _animKey;		// 現在どのｱﾆﾒｰｼｮﾝを表示するのかを表す鍵
	unsigned int _animFrame;	// ｱﾆﾒｰｼｮﾝﾌﾚｰﾑ　何ｺﾏ目か
	unsigned int _animCount;	// ｱﾆﾒｰｼｮﾝｶｳﾝﾄ　何回ﾙｰﾌﾟしたか

	bool DestroyPrpc(void);

	bool _alive;		// 生きているか
	bool _dead;			// 死んでいるか

	Vector2Dbl _pos;
	Vector2Dbl _size;
	double _rad;		// 角度

	int _zOrder;		// 描画優先度

	UNIT_ID _unitID;

	// std::vector<int> imageID;
    
};

