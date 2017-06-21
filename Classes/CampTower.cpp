#include "CampTower.h"

USING_NS_CC;

//��ʾ���������
int direction[3][2] = { { -1, -1 }, { -1, 0 }, { 0, -1 } }; //ֻ������,��,���¿�����

CampTower::CampTower()
{
}


CampTower::~CampTower()
{
}

bool CampTower::init()
{
	if (!Sprite::init()) //�����������仰�����runApplication����
	{
		return false;
	}

	instance = GameManager::getInstance();

	minerToProduceCount = 0;
	isTrainingSpriteSelected = false;
	instance->setTotalMinerNum(0);

	setTowerType(8);
	setIsSelected(false);
	setGrade(0);//��ʼû�еȼ�
	setPower(40);//�����ĵ���
	setMove(false);//��ʼ�������ƶ�״̬
	setIsPowerEnough(true);//��ʼ״̬�����㹻
	setIsPowerConsumption(false);//�������ĵ�

	towerSprite = Sprite::create("towerItem/Item9.png");
	towerSprite->setScale(0.4);
	addChild(towerSprite, 5);

	trainingSprite = Sprite::create("traingButton.png"); //ѵ��button����ʼ����ʾ������ý�������ʾ
	trainingSprite->setScale(0.4);
	auto sb = ScaleBy::create(1, 1.2);
	auto seq = Sequence::create(sb, sb->reverse(), NULL);
	auto rp = RepeatForever::create(seq);
	trainingSprite->runAction(rp);
	trainingSprite->setPosition(towerSprite->getBoundingBox().size.width - 10, 0);
	addChild(trainingSprite, 5);
	trainingSprite->setVisible(false);

	produceTime = 0;//��ʼ������ʱ��Ϊ0
	produceTimeBarBg = Sprite::create("manaBarBg.png");
	produceTimeBarBg->setScale(0.3); //0.45
	produceTimeBarBg->setPosition(towerSprite->getPosition() + Point(0, 105));
	addChild(produceTimeBarBg, 11);

	//������ʿ������Ŀ
	minerNum = Label::createWithTTF("", "b.ttf", 10);
	minerNum->setPosition(produceTimeBarBg->getPosition());
	addChild(minerNum, 12);


	//����ʱ������ʼ��
	timeBar = ProgressTimer::create(Sprite::create("time.png"));
	timeBar->setScaleX(0.6);
	timeBar->setScaleY(2.15);
	timeBar->setType(ProgressTimer::Type::BAR);
	timeBar->setMidpoint(Point(0, 0.5));
	timeBar->setBarChangeRate(Point(1, 0));
	timeBar->setPercentage(produceTime);
	timeBar->setAnchorPoint(Point(0, 0.5));
	timeBar->setPosition(produceTimeBarBg->getPosition() - Point(produceTimeBarBg->getBoundingBox().size.width / 2 - 5, 0));
	addChild(timeBar, 10);

	//����ʱ�����������ɼ�
	timeBar->setVisible(false);
	produceTimeBarBg->setVisible(false);
	minerNum->setVisible(false);

	//Ѫ��������ͼƬ
	towerHpSprite = Sprite::create("manaBarBg.png");
	towerHpSprite->setPosition(towerSprite->getPosition() + Point(0, 90));
	towerHpSprite->setScale(0.3);
	addChild(towerHpSprite, 10);
	//����Ѫ��
	hp = 4;
	//����Ѫ��������
	towerHp = ProgressTimer::create(Sprite::create("soldierProduceTimeBar.png")); //������һ��ͼƬsprite
	towerHp->setScaleX(2);
	towerHp->setScaleY(5.2);
	towerHp->setType(ProgressTimer::Type::BAR);
	towerHp->setMidpoint(Point(0, 0.5f));
	towerHp->setBarChangeRate(Point(1, 0));
	towerHp->setPercentage(100);
	towerHp->setPosition(Point(towerHpSprite->getContentSize().width / 2, towerHpSprite->getContentSize().height / 3 * 2 - 10));
	towerHpSprite->addChild(towerHp, 5);
	//��ʼ�����ɼ�
	towerHp->setVisible(false);
	towerHpSprite->setVisible(false);

	addTouch();//��Ӵ����¼�

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
			minerNum->setVisible(true);

			towerHp->setVisible(true);
			towerHpSprite->setVisible(true);
			return true;
		}
		else if (!isTrainingSpriteSelected)//�����Ϳ���ʵ�ֵ�������ط������������Ĺ�����
		{
			timeBar->setVisible(false);
			produceTimeBarBg->setVisible(false);
			trainingSprite->setVisible(false);
			minerNum->setVisible(false);
			towerHp->setVisible(false);
			towerHpSprite->setVisible(false);
		}

		return false;
	};



	trainingButtonListener = EventListenerTouchOneByOne::create();
	trainingButtonListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent){ //������������� �������ѵ��ʿ����ť�ĵ��,�����÷�������Ҫ�����ͼƬ�ϼ�listener

		auto target = static_cast<Sprite*>(pEvent->getCurrentTarget());

		Point locationInNode = target->convertTouchToNodeSpace(pTouch);

		Size size = target->getContentSize();
		Rect rect = Rect(0, 0, size.width, size.height);

		//log("rect size %f %f", size.width, size.height);
		if (rect.containsPoint(locationInNode) && trainingSprite->isVisible() && instance->totalSoldierNum<MAX_MINER_NUM)
		{
			if (instance->getMoney() >= 100)
			{
				isTrainingSpriteSelected = true;
				minerToProduceCount++;
				instance->setTotalMinerNum(instance->getTotalMinerNum() + 1);//�ɿ����������20��
				char c[10];
				sprintf(c, "%d", minerToProduceCount);
				std::string str = c;
				minerNum->setString(str);

				//��Ǯ����
				instance->setMoney(instance->getMoney() - 100);
				char c1[10];
				sprintf(c1, "%d", instance->getMoney());
				std::string str1 = c1;
				instance->moneyLabel->setString(str1);
			}
			else //��Ǯ����
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

	this->schedule(schedule_selector(CampTower::produceMiner), 0.1);

	return true;
}

Sprite * CampTower::getTrainingSprite()
{
	return trainingSprite;
}

Vec2 CampTower::convertTotileCoord(Vec2 position)//��map����ת��Ϊtile����
{
	auto mapSize = instance->map->getMapSize();
	// ���㵱ǰ�����£�ÿ����Ƭ�ĳ���
	auto tileWidth = instance->map->getBoundingBox().size.width / instance->map->getMapSize().width;
	auto tileHeight = instance->map->getBoundingBox().size.height / instance->map->getMapSize().height;
	// ��positionת��Ϊ��Ƭ���꣬ȷ���õ���������
	int posX = position.x / tileWidth;
	int posY = instance->map->getMapSize().height - position.y / tileHeight;

	return Point(posX, posY);
}
Vec2 CampTower::convertToScreenCoord(Vec2 position) //��tile����ת��Ϊmap��������
{
	Vec2 mapCoord;

	auto mapSize = instance->map->getMapSize();
	// ���㵱ǰ�����£�ÿ����Ƭ�ĳ���
	auto tileWidth = instance->map->getBoundingBox().size.width / instance->map->getMapSize().width;
	auto tileHeight = instance->map->getBoundingBox().size.height / instance->map->getMapSize().height;

	mapCoord.x = position.x * tileWidth;
	mapCoord.y = instance->map->getBoundingBox().size.height - position.y * tileHeight;

	return mapCoord;
}

Point CampTower::getNextRallyPoint(Point rally)//��ȡ��һ�������
{
	if (instance->walkMap[(int)rally.x][(int)rally.y] == 0) //δռ��
	{
		return rally;
	}
	else
	{
		while (true)
		{
			for (int i = 0; i < 3; i++)
			{
				int tempX = (int)rally.x + direction[i][0];
				int tempY = (int)rally.y + direction[i][1];

				posQueue.push(Point(tempX, tempY));

				if (instance->walkMap[tempX][tempY] == 0)
				{

					return Point(tempX, tempY);
				}
			}
			//���һ���ڵ���Χ3�����򶼱�ռ����,��rallypoint����Ϊ�����е���һ��
			auto tempPoint = posQueue.front();
			posQueue.pop();
			rally = tempPoint;
			rallyPoint = tempPoint;
		}

	}


	return Point(0, 0);
}

void CampTower::shoot(float dt)
{
	//do nothing
}

void CampTower::produceMiner(float dt)//�����ɿ��ߵĺ���
{
	if (minerToProduceCount > 0)
	{
		produceTime += 1;
		timeBar->setPercentage(produceTime);
		if (timeBar->getPercentage() == 100)
		{
			timeBar->setPercentage(0);
			produceTime = 0;
			minerToProduceCount--;
			//ʵʱ�ı��ı���ʾ
			char t1[10];
			sprintf(t1, "%d", minerToProduceCount);
			std::string str1 = t1;
			minerNum->setString(str1);
			if (minerToProduceCount == 0)//����ʾ0
			{
				minerNum->setString("");
			}

			//����miner
			auto mapPos = this->getPosition() - Point(30, 30);
			src = convertTotileCoord(mapPos);
			//log("src %f %f",src.x, src.y);
			auto mapGridPos = convertToScreenCoord(src); //�õ�map��������

			miner = Miner::create();
			miner->setScale(0.7);

			miner->setPosition(mapGridPos);
			instance->map->addChild(miner, 30);
			instance->minerVector.push_back(miner);//���뵽vector��ͳһ����

			rallyPoint = src - Point(3, -3);

			auto nextRallyPoint = getNextRallyPoint(rallyPoint);

			miner->pathFinding(src.x, src.y, nextRallyPoint.x, nextRallyPoint.y);
			miner->runFollowShortestRoad();
		}
	}
}

void CampTower::updateVacantUnloadPos(Point pos)//�ҵ������ж��λ�ù��ɿ���ж��
{
	int centerX = pos.x;
	int centerY = pos.y;
	//���vector
	vacantUnloadPos.clear();
	//��ȡ����
	auto instance = GameManager::getInstance();
	
	//һ��8��λ��
	//���
	if (centerX -2>= 0 && centerY  >= 0 && instance->walkMap[centerX-2][centerY] == 0)
	{
		auto tempPoint = Point(centerX-2, centerY);
		vacantUnloadPos.push_back(tempPoint);
	}
	
	if (centerX - 2 >= 0 && centerY + 2 >= 0 && instance->walkMap[centerX - 2][centerY + 2] == 0)
	{
		auto tempPoint = Point(centerX - 2, centerY + 2);
		vacantUnloadPos.push_back(tempPoint);
	}

	//�ϲ�
	if (centerX >= 0 && centerY - 2 >= 0 && instance->walkMap[centerX][centerY - 2] == 0)
	{
		auto tempPoint = Point(centerX, centerY - 2);
		vacantUnloadPos.push_back(tempPoint);
	}
	if (centerX + 1 >= 0 && centerY-2 >= 0 && instance->walkMap[centerX+1][centerY-2] == 0)
	{
		auto tempPoint = Point(centerX +1, centerY-2);
		vacantUnloadPos.push_back(tempPoint);
	}

	//�Ҳ�
	if (centerX +3>= 0 && centerY  >= 0 && instance->walkMap[centerX+3][centerY] == 0)
	{
		auto tempPoint = Point(centerX+3, centerY);
		vacantUnloadPos.push_back(tempPoint);
	}
	if (centerX + 3 >= 0 && centerY + 2 >= 0 && instance->walkMap[centerX + 3][centerY + 2] == 0)
	{
		auto tempPoint = Point(centerX + 3, centerY + 2);
		vacantUnloadPos.push_back(tempPoint);
	}

	//�²�
	if (centerX  >= 0 && centerY+3 >= 0 && instance->walkMap[centerX ][centerY+3] == 0)
	{
		auto tempPoint = Point(centerX , centerY+3);
		vacantUnloadPos.push_back(tempPoint);
	}
	if (centerX +1 >= 0 && centerY + 3 >= 0 && instance->walkMap[centerX + 1][centerY + 3] == 0)
	{
		auto tempPoint = Point(centerX + 1, centerY + 3);
		vacantUnloadPos.push_back(tempPoint);
	}
}