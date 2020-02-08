#pragma once
#include "cocos2d.h"

USING_NS_CC;

class Grid : public cocos2d::Layer
{
public:
	virtual bool init();
	CC_SYNTHESIZE(int, _value, ScoreValue);
	void initValue(int value);
	void initValue(int value, int row, int column);
	bool compareTo(Grid* grid);

	void setLocalPosition(int row, int column);

	void initAction();
	void moveOnly(int targetRow, int targetColumn);
	void moveAndClear(int targetRow, int targetColumn);
	void moveAndUpdate();

	CREATE_FUNC(Grid);
private:
	LayerColor* _bg;
	Label* _label;

	void loadMap();
	void updateBg();
};

