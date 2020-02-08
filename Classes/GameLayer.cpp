#include "GameLayer.h"
#include "GameTool.h"
#include "DataConf.h"

GameLayer* GameLayer::_instance = nullptr;
int isMove = 0;
const int layerSize = 530;
const int cellSize = 115;
const int ERRORINDEX = -1;

GameLayer* GameLayer::getInstance() {
	if (_instance == nullptr) {
		_instance = GameLayer::create();
	}

	return _instance;
}
bool GameLayer::init() {
	do {
		CC_BREAK_IF(!Layer::init());

		this->setContentSize(Size(layerSize, layerSize));
		this->setPosition(15,100);

		initBg();
		initEvent();
	} while (0);

	return true;
}
void GameLayer::onExit() {
	Layer::onExit();

	DataConf::getInstance()->dumpData();
}
void GameLayer::initEvent() {
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameLayer::onKeyPressed,this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameLayer::onKeyReleased,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,this);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan,this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}
void GameLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		moveToTop();
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		moveToBottom();
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		moveToLeft();
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		moveToRight();
		break;
	}
}
void GameLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (isMove > 0) {
		randGenGrid();
	}

	_isOver = ifOver();
}
bool GameLayer::onTouchBegan(Touch* touch, Event* event) {
	// 只在GameLayer区域点击有效
	Vec2 _begin = touch->getLocation();
	auto rect = Rect(this->getPosition().x, this->getPosition().y, this->getContentSize().width, this->getContentSize().height);
	if (rect.containsPoint(_begin)) {
		if (_isOver) {
			auto fail = this->getChildByName("fail");
			fail->setVisible(false);
			restartGame();
			return false;
		}
	}

	return true;
}
void GameLayer::onTouchEnded(Touch* touch, Event* event) {

}
void GameLayer::initBg() {
	struct timeval now;
	gettimeofday(&now,NULL);
	srand((unsigned int)now.tv_sec * 1000 + now.tv_usec / 1000);
	
	// 绘制图层背景
	auto bg = LayerColor::create(Color4B(186, 172, 159, 255), layerSize, layerSize);
	this->addChild(bg);

	// 绘制背景网格
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			auto cellBg = LayerColor::create(Color4B(213,205,194,255), cellSize, cellSize);
			cellBg->setPosition(129 * j + 14, 129 * i + 14);
			this->addChild(cellBg);
		}
	}

	loadGrids();
}
void GameLayer::initGrids() {
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			_grids[row][column] = nullptr;
		}
	}

	clearLastGrids();
	// 随机生成两个数
	randGenGrid();
	randGenGrid();
}
void GameLayer::loadGrids() {
	_isOver = false;
	/*auto f = UserDefault::getInstance();
	if (!f->isXMLFileExist() || !f->getBoolForKey("exits", false)) {
		return initGrids();
	}*/

	// todo 游戏记录功能
	return initGrids();
}
#define EMPTY -1;
#define DOUBLE_EMPTY -2;
void GameLayer::clearLastGrids() {
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			_lastGrids[row][column] = DOUBLE_EMPTY;
		}
	}
}
void GameLayer::saveLastGrids() {
	if (++isMove != 1) {
		return;
	}

	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			if (_grids[row][column] != nullptr) {
				_lastGrids[row][column] = _grids[row][column]->getScoreValue();
			}
			else {
				_lastGrids[row][column] = EMPTY;
			}
		}
	}
}
void GameLayer::randGenGrid() {
	isMove = 0;
	int value, row, column;

	// 使用do-while循环防止生成重复数字
	do {
		value = CCRANDOM_0_1() * 16;
		row = value / 4;
		column = value % 4;
	} while (_grids[row][column] != nullptr);

	auto item = Grid::create();
	item->initValue(0, row, column);
	_grids[row][column] = item;
	item->initAction();
	this->addChild(item);
}
// 以下4个方法的target均为将要移动到的目标索引（行索引或列索引）
// 注意： 绘制网格是从下往上绘制的，所以最上面的行索引是3
bool GameLayer::moveToTop() {
	int target = 3;

	for (int column = 0; column < 4; column++) {
		target = 3;
		for (int row = 3; row >= 0; row--) {
			if (_grids[row][column] == nullptr) {
				continue;
			}
			row = moveGrid(Direction::UP, row, column, target, column);
			if (row == ERRORINDEX) {
				break;
			}
			target--;
		}
	}

	return isMove > 0;
}
bool GameLayer::moveToBottom() {
	int target = 0;

	for (int column = 0; column < 4; column++) {
		target = 0;
		for (int row = 0; row < 4; row++) {
			if (_grids[row][column] == nullptr) {
				continue;
			}
			row = moveGrid(Direction::DOWN, row, column, target, column);
			if (row == ERRORINDEX) {
				break;
			}
			target++;
		}
	}

	return isMove > 0;
}
bool GameLayer::moveToLeft() {
	int target = 0;

	for (int row = 0; row < 4; row++) {
		target = 0;
		for (int column = 0; column < 4; column++) {
			if (_grids[row][column] == nullptr) {
				continue;
			}
			column = moveGrid(Direction::LEFT, row, column, row, target);
			if (column == ERRORINDEX) {
				break;
			}
			target++;
		}
	}

	return isMove > 0;
}
bool GameLayer::moveToRight() {
	int target = 3;

	for (int row = 0; row < 4; row++) {
		target = 3;
		for (int column = 3; column >= 0; column--) {
			if (_grids[row][column] == nullptr) {
				continue;
			}
			column = moveGrid(Direction::RIGHT, row, column, row, target);
			if (column == ERRORINDEX) {
				break;
			}
			target--;
		}
	}

	return isMove > 0;
}
// targetRow: 将要移动到的目标行索引
// targetColumn: 将要移动到的目标列索引
int GameLayer::moveGrid(const int direction, int row, int column, int targetRow, int targetColumn) {
	int preColumnOrRow = getPreGridIndex(direction, row, column);
	if (preColumnOrRow == ERRORINDEX) {
		moveOnly(row, column, targetRow, targetColumn);
		return ERRORINDEX;
	}

	int preRow, preCol;
	int tag = -1;
	switch (direction) {
	case Direction::RIGHT:
		tag = 1;
	case Direction::LEFT:
		preRow = row;
		preCol = preColumnOrRow;
		break;
	case Direction::UP:
		tag = 1;
	case Direction::DOWN:
		preRow = preColumnOrRow;
		preCol = column;
		break;
	}

	if (_grids[row][column]->compareTo(_grids[preRow][preCol])) {
		auto value = _grids[row][column]->getScoreValue();
		moveAndClear(row, column, targetRow, targetColumn);
		moveAndClear(preRow, preCol, targetRow, targetColumn);
		moveAndUpdate(++value, targetRow, targetColumn);
		// 得分
		GameTool::getInstance()->updateScore(pow(2,++value));

		return preColumnOrRow;
	}

	moveOnly(row, column, targetRow, targetColumn);
	return preColumnOrRow + tag;
}
// 获取上一个网格的索引（指行索引或列索引）
int GameLayer::getPreGridIndex(const int direction, int row, int column) {
	switch (direction) {
	case Direction::RIGHT:
		while (--column >= 0) {
			if (_grids[row][column] != nullptr) {
				return column;
			}
		}
		break;
	case Direction::LEFT:
		while (++column <= 3) {
			if (_grids[row][column] != nullptr) {
				return column;
			}
		}
		break;
	case Direction::DOWN:
		while (++row <= 3) {
			if (_grids[row][column] != nullptr) {
				return row;
			}
		}
		break;
	case Direction::UP:
		while (--row >= 0) {
			if (_grids[row][column] != nullptr) {
				return row;
			}
		}
		break;
	}
	return ERRORINDEX;
}
void GameLayer::moveOnly(int row, int column, int targetRow, int targetColumn) {
	if (row == targetRow && column == targetColumn) {
		return;
	}

	saveLastGrids();

	// 视图层的移动
	_grids[row][column]->moveOnly(targetRow, targetColumn);
	// 数据层的移动
	_grids[targetRow][targetColumn] = _grids[row][column];
	_grids[row][column] = nullptr;
}
void GameLayer::moveAndClear(int row, int column, int targetRow, int targetColumn) {
	saveLastGrids();

	_grids[row][column]->moveAndClear(targetRow, targetColumn);
	_grids[row][column] = nullptr;
}
void GameLayer::moveAndUpdate(int value, int targetRow, int targetColumn) {
	auto temp = Grid::create();
	temp->initValue(value, targetRow, targetColumn);
	_grids[targetRow][targetColumn] = temp;
	this->addChild(_grids[targetRow][targetColumn]);
	temp->moveAndUpdate();
}
bool GameLayer::ifOver() {
	// 最大索引不超过3，防止数组越界
	for (int row = 0; row < 3; row++) {
		for (int column = 0; column < 3; column++) {
			if (_grids[row][column] == nullptr) {
				return false;
			}
			if (_grids[row][column]->compareTo(_grids[row][column + 1]) ||
				_grids[row][column]->compareTo(_grids[row + 1][column])) {
				return false;
			}
		}
	}

	for (int column = 0; column < 3; column++) {
		if (_grids[3][column] == nullptr || _grids[3][column]->compareTo(_grids[3][column + 1])) {
			return false;
		}
	}

	for (int row = 0; row < 3; row++) {
		if (_grids[row][3] == nullptr || _grids[row][3]->compareTo(_grids[row+1][3])) {
			return false;
		}
	}

	auto fail = this->getChildByName("fail");
	// 创建游戏结束文本
	if (fail == nullptr) {
		fail = LayerColor::create(Color4B(144,144,144,144), layerSize, layerSize);
		auto failLabel = Label::createWithSystemFont("Game Over!", "Arial", 50);
		failLabel->enableBold();
		failLabel->setPosition(layerSize/2, layerSize/2);
		fail->addChild(failLabel);
		fail->setName("fail");
		this->addChild(fail,2);
	}
	fail->setVisible(true);

	return true;
}
void GameLayer::restartGame() {
	_isOver = false;
	GameTool::getInstance()->resetScore();
	clearGrids();
	randGenGrid();
	randGenGrid();
}
void GameLayer::clearGrids() {
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			if (_grids[row][column] != nullptr) {
				_grids[row][column]->removeFromParent();
				_grids[row][column] = nullptr;
			}
		}
	}
}