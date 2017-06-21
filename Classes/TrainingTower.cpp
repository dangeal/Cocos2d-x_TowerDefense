﻿#include "TrainingTower.h"
USING_NS_CC;

//表示方向的数组
int direc[3][2] = { { -1, -1 }, { -1, 0 }, { 0, -1 }}; //只有左下,左,右下可以走
TrainingTower::TrainingTower()
{
}


TrainingTower::~TrainingTower()
{
}

bool TrainingTower::init()
{
	if (!Sprite::init()) //如果忘记了这句话则会在runApplication报错
	{
		return false;
	}

	instance = GameManager::getInstance();

	soldierToProduceCount = 0;
	isTrainingSpriteSelected = false;
	instance->totalSoldierNum = 0;

	setTowerType(7);
	setIsSelected(false);
	setGrade(0);//开始没有等级
	setPower(40);//塔消耗电力
	setMove(false);//开始不处于移动状态
	setIsPowerEnough(true);//开始状态电力足够
	setIsPowerConsumption(false);//该塔不耗电

	towerSprite = Sprite::create("towerItem/Item8.png");
	towerSprite->setScale(0.4);
	addChild(towerSprite, 5);

	trainingSprite = Sprite::create("traingButton.png"); //训练button，开始不显示，点击该建筑才显示
	trainingSprite->setScale(0.25);
	auto sb = ScaleBy::create(1, 1.2);
	auto seq = Sequence::create(sb, sb->reverse(), NULL);
	auto rp = RepeatForever::create(seq);
	trainingSprite->runAction(rp);
	trainingSprite->setPosition(towerSprite->getBoundingBox().size.width-10,0);
	addChild(trainingSprite, 5);
	trainingSprite->setVisible(false);

	produceTime = 0;//初始化生产时间为0
	produceTimeBarBg = Sprite::create("manaBarBg.png");
	produceTimeBarBg->setScale(0.3); //0.45
	produceTimeBarBg->setPosition(towerSprite->getPosition() + Point(0, 65));
	addChild(produceTimeBarBg, 11);

	//待生产士兵的数目
	soldierNum = Label::createWithTTF("", "b.ttf", 10);
	soldierNum->setPosition(produceTimeBarBg->getPosition());
	addChild(soldierNum, 12);


	//生产时间条初始化
	timeBar = ProgressTimer::create(Sprite::create("time.png"));
	timeBar->setScaleX(0.6);
	timeBar->setScaleY(2.2);
	timeBar->setType(ProgressTimer::Type::BAR);
	timeBar->setMidpoint(Point(0, 0.5));
	timeBar->setBarChangeRate(Point(1, 0));
	timeBar->setPercentage(produceTime);
	timeBar->setAnchorPoint(Point(0, 0.5));
	timeBar->setPosition(produceTimeBarBg->getPosition() - Point(produceTimeBarBg->getBoundingBox().size.width / 2 - 5, 0));
	addChild(timeBar, 10);

	//生产时间属性条不可见
	timeBar->setVisible(false);
	produceTimeBarBg->setVisible(false);
	soldierNum->setVisible(false);

	//血量条背景图片
	towerHpSprite = Sprite::create("manaBarBg.png");
	towerHpSprite->setPosition(towerSprite->getPosition() + Point(0, 50));
	towerHpSprite->setScale(0.3);
	addChild(towerHpSprite, 10);
	//炮塔血量
	hp = 4;
	//炮塔血量进度条
	towerHp = ProgressTimer::create(Sprite::create("soldierProduceTimeBar.png")); //参数是一个图片sprite
	towerHp->setScaleX(2);
	towerHp->setScaleY(5.2);
	towerHp->setType(ProgressTimer::Type::BAR);
	towerHp->setMidpoint(Point(0, 0.5f));
	towerHp->setBarChangeRate(Point(1, 0));
	towerHp->setPercentage(100);
	towerHp->setPosition(Point(towerHpSprite->getContentSize().width / 2, towerHpSprite->getContentSize().height / 3 * 2 - 10));
	towerHpSprite->addChild(towerHp, 5);
	//初始化不可见
	towerHp->setVisible(false);
	towerHpSprite->setVisible(false);

	addTouch();//添加触摸事件

	listener->onTouchBegan = [=](Touch* pTouch, Event* pEvent){
		
		
		auto target = static_cast<Sprite*>(pEvent->getCurrentTarget());
		
		Point locationInNode = target->convertTouchToNodeSpace(pTouch);
		
		Size size = target->getContentSize();
		Rect rect = Rect(0, 0, size.width, size.height);
	
		//log("rect size %f %f", size.width, size.height);
		if (rect.containsPoint(locationInNode)) 
		{
			timeBar->setVisible(true);
			produceTimeBarBg->setVisible(true);
			trainingSprite->setVisible(true);
			soldierNum->setVisible(true);

			towerHp->setVisible(true);
			towerHpSprite->setVisible(true);
			return true;
		}
		else if (!isTrainingSpriteSelected)//这样就可以实现点击其他地方隐藏属性条的功能了
		{
			timeBar->setVisible(false);
			produceTimeBarBg->setVisible(false);
			trainingSprite->setVisible(false);
			soldierNum->setVisible(false);
			towerHp->setVisible(false);
			towerHpSprite->setVisible(false);
		}
		
		return false;
	};

	

	trainingButtonListener = EventListenerTouchOneByOne::create();
	trainingButtonListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent){ //这个是特殊声明 用来检测训练士兵按钮的点击,这种用法就是在要点击的图片上加listener

		auto target = static_cast<Sprite*>(pEvent->getCurrentTarget());

		Point locationInNode = target->convertTouchToNodeSpace(pTouch);

		Size size = target->getContentSize();
		Rect rect = Rect(0, 0, size.width, size.height);

		//log("rect size %f %f", size.width, size.height);
		if (rect.containsPoint(locationInNode) && trainingSprite->isVisible() && instance->totalSoldierNum<MAX_SOLDIER_NUM )
		{
			if (instance->getMoney() >= 100)
			{
				isTrainingSpriteSelected = true;
				soldierToProduceCount++;
				instance->totalSoldierNum++;//士兵总数最多10个
				char c[10];
				sprintf(c, "%d", soldierToProduceCount);
				std::string str = c;
				soldierNum->setString(str);

				//金钱减少
				instance->setMoney(instance->getMoney() - 100);
				char c1[10];
				sprintf(c1, "%d", instance->getMoney());
				std::string str1 = c1;
				instance->moneyLabel->setString(str1);
			}
			else //金钱不足
			{
				auto sb = ScaleBy::create(0.3, 1.2);
				auto seq = Sequence::create(sb, sb->reverse(), NULL);
				instance->moneyLabel->runAction(seq);
			}

			return true;
		}
		else
		{
			isTrainingSpriteSelected = false;
			trainingSprite->setVisible(false);
		}
		
		return false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(trainingButtonListener, this->trainingSprite);

	this->schedule(schedule_selector(TrainingTower::produceSoldier), 0.1);

	return true;
}

void TrainingTower::shoot(float dt)
{
	//do nothing
}
cocos2d::Sprite * TrainingTower::getTrainingSprite()
{
	return trainingSprite;
}

Vec2 TrainingTower::convertTotileCoord(Vec2 position)//将map坐标转化为tile坐标
{
	auto mapSize = instance->map->getMapSize();
	// 计算当前缩放下，每块瓦片的长宽
	auto tileWidth = instance->map->getBoundingBox().size.width / instance->map->getMapSize().width;
	auto tileHeight = instance->map->getBoundingBox().size.height / instance->map->getMapSize().height;
	// 把position转换为瓦片坐标，确保得到的是整数
	int posX = position.x / tileWidth;
	int posY = instance->map->getMapSize().height - position.y / tileHeight;

	return Point(posX, posY);
}
Vec2 TrainingTower::convertToScreenCoord(Vec2 position) //将tile坐标转化为map整数坐标
{
	Vec2 mapCoord;

	auto mapSize = instance->map->getMapSize();
	// 计算当前缩放下，每块瓦片的长宽
	auto tileWidth = instance->map->getBoundingBox().size.width / instance->map->getMapSize().width;
	auto tileHeight = instance->map->getBoundingBox().size.height / instance->map->getMapSize().height;

	mapCoord.x = position.x * tileWidth;
	mapCoord.y = instance->map->getBoundingBox().size.height - position.y * tileHeight;

	return mapCoord;
}

Point TrainingTower::getNextRallyPoint(Point rally)//获取下一个集结点
{
	if (instance->walkMap[(int)rally.x][(int)rally.y] == 0) //未占地
	{		
		return rally;
	}
	else
	{
		while (true)
		{
			for (int i = 0; i < 3; i++)
			{
				int tempX = (int)rally.x + direc[i][0];
				int tempY = (int)rally.y + direc[i][1];

				posQueue.push(Point(tempX, tempY));

				if (instance->walkMap[tempX][tempY] == 0)
				{

					return Point(tempX, tempY);
				}
			}
			//如果一个节点周围3个方向都被占用了,则rallypoint更新为队列中的下一个
			auto tempPoint = posQueue.front();
			posQueue.pop();
			rally = tempPoint;
			rallyPoint = tempPoint;
		}

	}
	
	
	return Point(0, 0);
}

void TrainingTower::produceSoldier(float dt)//生产士兵的函数
{
	if (soldierToProduceCount > 0)
	{
		produceTime += 1;
		timeBar->setPercentage(produceTime);
		if (timeBar->getPercentage() == 100)
		{
			timeBar->setPercentage(0);
			produceTime = 0;
			soldierToProduceCount--;
			//实时改变文本显示
			char t1[10];
			sprintf(t1, "%d", soldierToProduceCount);
			std::string str1 = t1;
			soldierNum->setString(str1);
			if (soldierToProduceCount == 0)//不显示0
			{
				soldierNum->setString("");
			}

			//生产士兵
			auto mapPos = this->getPosition() - Point(30, 30);
			src = convertTotileCoord(mapPos);
			//log("src %f %f",src.x, src.y);
			auto mapGridPos = convertToScreenCoord(src); //得到map整数坐标

			soldier = Soldier::create();
			soldier->setScale(0.7);

			soldier->setPosition(mapGridPos);
			instance->map->addChild(soldier, 30);
			instance->soldierVector.push_back(soldier);//加入到vector中统一管理

			rallyPoint = src - Point(2, -2);

			auto nextRallyPoint = getNextRallyPoint(rallyPoint);

			soldier->pathFinding(src.x, src.y, nextRallyPoint.x, nextRallyPoint.y);
			soldier->runFollowShortestRoad();
		}
	}
}