#pragma once
#include "cocos2d.h"
#include "Grid.h"

USING_NS_CC;

class GameLayer : public cocos2d::Layer
{
public:
	static GameLayer* getInstance();

	virtual bool init();
	virtual void onExit();
	virtual void initBg();
	virtual void initEvent();
	void initGrids();
	void loadGrids();
	void clearGrids();

	void randGenGrid();
	void restartGame();

	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

	Grid* _grids[4][4];
	int _lastGrids[4][4];

	CREATE_FUNC(GameLayer);
private:
	static GameLayer* _instance;
	bool _isOver;

	void clearLastGrids();
	void saveLastGrids();

	bool moveToTop();
	bool moveToBottom();
	bool moveToLeft();
	bool moveToRight();

	int getPreGridIndex(const int direction, int row, int column);
	int moveGrid(const int direction, int row, int column, int targetRow, int targetColumn);
	void moveOnly(int row, int column, int targetRow, int targetColumn);
	void moveAndClear(int row, int column, int targetRow, int targetColumn);
	void moveAndUpdate(int value, int targetRow, int targetColumn);

	bool ifOver();
};
enum Direction {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

