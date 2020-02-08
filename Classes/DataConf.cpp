#include "DataConf.h"
#include "GameTool.h"
#include "GameLayer.h"

static DataConf* instance;
DataConf* DataConf::getInstance()
{
	if (instance == nullptr)
		instance = create();

	return instance;
}

bool DataConf::init()
{
	log("DataConf init");
	return true;
}
void DataConf::dumpData() {
	auto f = UserDefault::getInstance();
	f->setIntegerForKey("score",GameTool::getInstance()->getScore());
	f->setIntegerForKey("best_score",GameTool::getInstance()->getBestScore());
	f->setBoolForKey("exits",true);
	f->flush();
}