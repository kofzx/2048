#include "GameScene.h"
#include "GameLayer.h"
#include "GameTool.h"

USING_NS_CC;

static GameScene* _gameScene;

Scene* GameScene::createScene() {
	auto scene = Scene::create();
	auto layer = GameScene::getInstance();
	scene->addChild(layer);

	return scene;
}
GameScene* GameScene::getInstance() {
	if (!_gameScene) {
		_gameScene = GameScene::create();
	}

	return _gameScene;
}
bool GameScene::init() {
	if (!Layer::init()) {
		return false;
	}
	// add background
	this->addChild(LayerColor::create(Color4B(242,242,242,255)));

	auto tool = GameTool::getInstance();
	this->addChild(tool);

	auto gameLayer = GameLayer::getInstance();
	this->addChild(gameLayer);

	return true;
}