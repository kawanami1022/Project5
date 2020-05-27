#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <common\Vector2.h>

enum class STATE
{
	NORMAL,  // �ʏ�
	EX,		 // ����
	DETH,	 // ����(���S�j
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
	bool isAnimEnd(void);	// ��Ұ��݂��I�����Ă��邩
	
private:


protected:
	std::map<STATE, AnimVector> _animMap;
	STATE _state;
	//std::string _animKey;		// ���݂ǂ̱�Ұ��݂�\������̂���\����
	unsigned int _animFrame;	// ��Ұ����ڰс@���ϖڂ�
	unsigned int _animCount;	// ��Ұ��ݶ��ā@����ٰ�߂�����

	bool DestroyPrpc(void);

	bool _alive;		// �����Ă��邩
	bool _dead;			// ����ł��邩

	Vector2Dbl _pos;
	Vector2Dbl _size;
	double _rad;		// �p�x

	int _zOrder;		// �`��D��x

	UNIT_ID _unitID;

	// std::vector<int> imageID;
    
};

