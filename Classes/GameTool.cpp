#include "GameTool.h"

const int toolWidth = 530;
const int toolHeight = 106;
const int layerWidth = toolWidth / 3;

GameTool* GameTool::_instance = nullptr;
GameTool* GameTool::getInstance() {
	if (_instance == nullptr) {
		_instance = create();
	}
	return _instance;
}
bool GameTool::init() {
	do {
		CC_BREAK_IF(!Layer::init());

		this->setContentSize(Size(toolWidth, toolHeight));
		this->setPosition(15, 660);

		auto layerIcon = LayerColor::create(Color4B(230, 230, 0, 0), layerWidth, toolHeight);
		auto label = Label::createWithSystemFont("2048", "Arial", 54);
		label->enableBold();
		label->setTextColor(Color4B(134, 134, 134, 255));
		label->setPosition(layerWidth/2, toolHeight/2);
		layerIcon->addChild(label);
		this->addChild(layerIcon);

		initScore();
	} while (0);

	return true;
}
// 初始化样式
bool GameTool::initScore() {
	auto scoreIcon = LayerColor::create(Color4B(186, 172, 159, 150),141,88);
	auto scoreTitleLabel = Label::createWithSystemFont("SCORE","Arial",20);
	scoreTitleLabel->setPosition(70,55);
	scoreIcon->setPosition(230,7);
	scoreIcon->addChild(scoreTitleLabel);

	scoreLabel = Label::createWithSystemFont(Value(_score).asString(), "Arial", 24);
	scoreLabel->setPosition(70,25);
	scoreIcon->addChild(scoreLabel);
	this->addChild(scoreIcon);

	auto bestIcon = LayerColor::create(Color4B(186, 172, 159, 150), 141, 88);
	auto bestTitleLabel = Label::createWithSystemFont("BEST", "Arial", 20);
	bestTitleLabel->setPosition(70, 55);
	bestIcon->setPosition(389,7);
	bestIcon->addChild(bestTitleLabel);

	bestLabel = Label::createWithSystemFont(Value(_bestScore).asString(), "Arial", 24);
	bestLabel->setPosition(70, 25);
	bestIcon->addChild(bestLabel);
	this->addChild(bestIcon);

	loadScore();
	return true;
}
void GameTool::loadScore() {
	auto f = UserDefault::getInstance();
	setScore(f->getIntegerForKey("score",0));
	setScore(f->getIntegerForKey("best_score",0));
}
void GameTool::updateScore(int addScore) {
	setScore(_score + addScore);
	updateBestScore();
}
void GameTool::resetScore() {
	setScore(0);
}
void GameTool::setScore(int score) {
	_score = score;
	scoreLabel->setString(Value(_score).asString());
}
int GameTool::getScore() {
	return _score;
}
void GameTool::setBestScore(int bestScore) {
	_bestScore = bestScore;
	bestLabel->setString(Value(_bestScore).asString());
}
int GameTool::getBestScore() {
	return _bestScore;
}
void GameTool::updateBestScore() {
	if (_score < _bestScore) {
		return;
	}

	setBestScore(_score);
}