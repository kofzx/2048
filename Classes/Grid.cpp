#include "Grid.h"

std::string map[16] = {"0"};
std::string num[] = { "2", "4", "8", "16", "32", "64", "128", "256", "512", "1024", "2048", "4096", "8192", "16384", "32768", "65536",  "131072", "262144", "524288" };

bool Grid::init() {
	if (!Layer::init()) {
		return false;
	}

	loadMap();

	_value = 0;

	this->setContentSize(Size(115,115));
	// 设置背景、网格数字
	_bg = LayerColor::create(Color4B(213, 205, 194, 255), 115, 115);
	_label = Label::createWithSystemFont(map[_value],"Arial",30);
	_label->setPosition(this->getContentSize().width/2,this->getContentSize().height/2);
	this->addChild(_bg);
	this->addChild(_label);

	return true;
}
void Grid::loadMap() {
	for (int i = 0; i < 16; i++) {
		map[i] = num[i];
	}
}
void Grid::initValue(int value) {
	_value = value;
	updateBg();
}
void Grid::initValue(int value, int row, int column) {
	setLocalPosition(row, column);
	initValue(value);
}
// 更新网格的样式
void Grid::updateBg() {
	_label->setString(map[_value]);
	Color3B color, fcolor = Color3B(255, 255, 255);

	switch (_value)
	{
		case 0:
			color = Color3B(247, 213, 97);
			fcolor = Color3B(120, 120, 120);
			break;
		case 1:
			color = Color3B(166, 232, 103);
			fcolor = Color3B(90, 90, 90);
			break;
		case 2:
			color = Color3B(87, 212, 154);
			break;
		case 3:
			color = Color3B(19, 181, 177);
			break;
		case 4:
			color = Color3B(68, 138, 202);
			break;
		case 5:
			color = Color3B(200, 97, 234);
			break;
		case 6:
			color = Color3B(225, 115, 181);
			break;
		case 7:
			color = Color3B(238, 100, 141);
			break;
		case 8:
			color = Color3B(243, 157, 79);
			break;
		case 9:
			color = Color3B(245, 124, 78);
			break;
		case 10:
			color = Color3B(246, 76, 20);
			break;
		case 11:
			color = Color3B(210, 210, 10); //Color3B(105, 84, 187);
			break;
		case 12:
			color = Color3B(190, 194, 22); // Color3B(50, 86, 204);
			break;
		case 13:
			color = Color3B(160, 160, 10);
			break;
		case 14:
			color = Color3B(50, 86, 204);// Color3B(24, 66, 149);
			break;
		default:
			color = Color3B(24, 66, 149);
			break;
	}

	_label->setColor(fcolor);
	_bg->setColor(color);

	if (_value > 15) {
		_label->setSystemFontSize(20);
	}
	else if (_value > 12) {
		_label->setSystemFontSize(24);
	}
	else if (_value > 8) {
		_label->setSystemFontSize(26);
	}
}
void Grid::setLocalPosition(int row, int column) {
	this->setPosition(129 * column + 14, 129 * row + 14);
}
// 初始数字的动画
void Grid::initAction() {
	auto a1 = ScaleTo::create(0.3f, 1);
	auto a2 = FadeIn::create(1);
	auto a3 = Spawn::create(a1, a2, nullptr);

	setScale(0);
	runAction(a3);
}
void Grid::moveOnly(int targetRow, int targetColumn) {
	runAction(MoveTo::create(0.1f, Vec2(129 * targetColumn + 14, 129 * targetRow + 14)));
}
void Grid::moveAndClear(int targetRow, int targetColumn) {
	auto a1 = MoveTo::create(0.1f, Vec2(129 * targetColumn + 14, 129 * targetRow + 14));
	auto a2 = FadeOut::create(0.1f);
	auto a3 = Spawn::create(a1, a2, nullptr);
	auto a4 = CallFunc::create([&] {
		this->removeFromParent();
	});
	this->runAction(Sequence::create(a3, a4, nullptr));
}
void Grid::moveAndUpdate() {
	this->setVisible(false);
	auto a1 = ScaleTo::create(0.1f, 1.1f);
	auto a2 = ScaleTo::create(0.1f, 1);
	auto a3 = FadeIn::create(0.1f);
	auto a4 = Sequence::create(a3, CallFunc::create([&] {
		this->setVisible(true);
	}), a1, a2, nullptr);

	this->runAction(a4);
}
// 比较两个数字是否相同
bool Grid::compareTo(Grid* grid) {
	return grid != nullptr && _value == grid->getScoreValue();
}