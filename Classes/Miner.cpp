#include "Miner.h"
#include "CampTower.h"
USING_NS_CC;
#define FRAME_NUM 3
#define FRAME_WALK 8
#define MAP_SIZE 80
#define MINING_TIME 5
#define MINING_GOLD 10 //ÿ�βɿ�õ�10���

Miner::Miner()
{
	aStar = new AStarPath;
}


Miner::~Miner()
{
}

bool Miner::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	nearestCamp = nullptr;
	isReached = false;
	isMiningFinished = false;
	miningTimer = 0;//��ʱ����ʼ��Ϊ0
	instance = GameManager::getInstance();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	minerSprite = Sprite::create("miner/down_1.png");
	addChild(minerSprite);

	//ʿ��Ŀ��ص�ָʾ��
	minerDestPointer = Sprite::create("pointer.png");
	minerDestPointer->setScale(1.5);
	instance->map->addChild(minerDestPointer, 14);
	minerDestPointer->setPosition(-400, -400); //���䲻�ɼ�
	minerDestPointer->setVisible(false);

	isReachable = true;


	time = 100;//��ʼ������ʱ��Ϊ100
	timeBarBg = Sprite::create("manaBarBg.png");
	timeBarBg->setScale(0.45);
	timeBarBg->setPosition(minerSprite->getPosition() + Point(0, 70));
	addChild(timeBarBg, 11);


	//������ֵ��ʼ��
	timeBar = ProgressTimer::create(Sprite::create("time.png"));
	timeBar->setScaleX(0.9);
	timeBar->setScaleY(2.2);
	timeBar->setType(ProgressTimer::Type::BAR);
	timeBar->setMidpoint(Point(0, 0.5));
	timeBar->setBarChangeRate(Point(1, 0));
	timeBar->setPercentage(100);
	timeBar->setAnchorPoint(Point(0, 0.5));
	timeBar->setPosition(timeBarBg->getPosition() - Point(timeBarBg->getBoundingBox().size.width / 2 - 5, 0));
	addChild(timeBar, 10);

	//ʿ�����������ɼ�
	timeBar->setVisible(false);
	timeBarBg->setVisible(false);

	//��ʼ����״̬��ʶ
	stateSprite = Sprite::create("miner_idle.png");
	addChild(stateSprite, 12);
	stateSprite->setPosition(timeBar->getPosition()+Point(50,60));


	//��ʼ��Ӣ����·�Ķ���:������
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "miner/down_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //ֱ�Ӵ�res��ȡ��
		walkDown.pushBack(frame);
	}
	animationWalkDown = Animation::createWithSpriteFrames(walkDown, 0.1);

	//��ʼ��Ӣ����·�Ķ���:������
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "miner/up_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //ֱ�Ӵ�res��ȡ��
		walkUp.pushBack(frame);
	}
	animationWalkUp = Animation::createWithSpriteFrames(walkUp, 0.1);

	//��ʼ��Ӣ����·�Ķ���:����
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "miner/left_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //ֱ�Ӵ�res��ȡ��
		walkLeft.pushBack(frame);
	}
	animationWalkLeft = Animation::createWithSpriteFrames(walkLeft, 0.1);

	//��ʼ��Ӣ����·�Ķ���:����
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "miner/right_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //ֱ�Ӵ�res��ȡ��
		walkRight.pushBack(frame);
	}
	animationWalkRight = Animation::createWithSpriteFrames(walkRight, 0.1);


	//��ʼ��Ӣ����·�Ķ���:������
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "miner/leftup_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //ֱ�Ӵ�res��ȡ��
		walkLeftUp.pushBack(frame);
	}
	animationWalkLeftUp = Animation::createWithSpriteFrames(walkLeftUp, 0.1);

	//��ʼ��Ӣ����·�Ķ���:������
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "miner/leftdown_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //ֱ�Ӵ�res��ȡ��
		walkLeftDown.pushBack(frame);
	}
	animationWalkLeftDown = Animation::createWithSpriteFrames(walkLeftDown, 0.1);

	//��ʼ��Ӣ����·�Ķ���:������
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "miner/rightup_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //ֱ�Ӵ�res��ȡ��
		walkRightUp.pushBack(frame);
	}
	animationWalkRightUp = Animation::createWithSpriteFrames(walkRightUp, 0.1);

	//��ʼ��Ӣ����·�Ķ���:������
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "miner/rightdown_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //ֱ�Ӵ�res��ȡ��
		walkRightDown.pushBack(frame);
	}
	animationWalkRightDown = Animation::createWithSpriteFrames(walkRightDown, 0.1);


	//��animation���뵽cache��ȥ,���뽫animation���뵽cache��ȥ���õ�ʱ��ȡ����,ע������Ҫ�����֣���Ϊ�ǵ��������Զ������ֲ��ܳ�ͻ
	AnimationCache::getInstance()->addAnimation(animationWalkDown, "miner_down");
	AnimationCache::getInstance()->addAnimation(animationWalkUp, "miner_up");
	AnimationCache::getInstance()->addAnimation(animationWalkLeft, "miner_left");
	AnimationCache::getInstance()->addAnimation(animationWalkRight, "miner_right");

	AnimationCache::getInstance()->addAnimation(animationWalkLeftUp, "miner_leftup");
	AnimationCache::getInstance()->addAnimation(animationWalkLeftDown, "miner_leftdown");
	AnimationCache::getInstance()->addAnimation(animationWalkRightDown, "miner_rightdown");
	AnimationCache::getInstance()->addAnimation(animationWalkRightUp, "miner_rightup");

	//this->schedule(schedule_selector(Miner::decreaseSurviveTime), 2);
	curState = stateIdle;//��ʼΪ����״̬ 
	scheduleUpdate();//����״̬����ʼ��ת

	return true;
}

//���òɿ���״̬
void Miner::setState(minerState nextState)
{
	curState = nextState;
}

void Miner::update(float dt)//ÿһ֡��������״̬��
{
	switch (curState) 
	{
		case stateIdle: //����״̬
		{
							  //�����������
							  miningTimer = 0;
							  isReached = false;
							  this->aStar->roadPoint.clear(); 
							  this->openListClear();
							  nearestCamp = nullptr;
							  this->unschedule(schedule_selector(Miner::miningTimerIncrease));
							  this->getStateSprite()->setTexture("miner_idle.png");
		}
		break;

		case stateMining://�ɿ�״̬
		{
							if (miningTimer == MINING_TIME)//�ɿ����
							{
								miningTimer = 0;
								this->unschedule(schedule_selector(Miner::miningTimerIncrease));

								//������ߵ��òɿ�λ��,�������·
								this->aStar->roadPoint.clear(); //��յ���astar�����roadpoint
								this->openListClear();
								this->stopAllActions();
								//Ѱ�Ҳɿ���ж����֮�����·
								this->pathFinding(miningPoint.x, miningPoint.y, unloadPoint.x, unloadPoint.y);
								//log("pathfingd");
								//���Ӫ����
								this->runFollowShortestRoad();

								//�ı�״̬,��ʼ���ߵ���Ӫ 
								curState = stateWalkToCamp;
								this->getStateSprite()->setTexture("miner_unload.png");
							}
			 

		}
		break;

		case stateWalkToCamp://��������Ӫ
		{
			
								if (isReached) //���������·��
								{
									isReached = false;
									//������������������
									this->aStar->roadPoint.clear(); //��յ���astar�����roadpoint
									this->openListClear();
									this->stopAllActions();
									//Ѱ�Ҳɿ���ж����֮�����·
									this->pathFinding(unloadPoint.x, unloadPoint.y, miningPoint.x, miningPoint.y);
									//��ʼ����
									this->runFollowShortestRoad();
									//�ı�״̬
									curState = stateWalkToMine;
									this->getStateSprite()->setTexture("miner_readyToWork.png");

									//��ҽ�Ǯ����
									instance->setMoney(instance->getMoney() + MINING_GOLD);
									char temp[20];
									sprintf(temp,"%d", instance->getMoney());
									std::string str = temp;
									instance->moneyLabel->setString(str);
								}

		}
		break;

		case stateWalkToMine://��������ʯ�� 
		{

								if (isReached)
								{
									//Ѱ������Ĵ�Ӫ
									searchNearestCamp();
									if (nearestCamp)
									{
										//Ѱ�������ж���ص�
										searchNearestUnloadPos(nearestCamp);
									}
									//��ʼ�ɿ��ʱ
									this->schedule(schedule_selector(Miner::miningTimerIncrease), 1);
									//log("mining point %f %f", miningPoint.x, miningPoint.y);
											
									//�ı�״̬
									curState = stateMining;
									this->getStateSprite()->setTexture("miner_working.png");
									isReached = false;
								}

		}
		break;
	}
}


void Miner::setAnimation(int dir)//����Ӣ�۵����߶���,��ʾӢ�����߷����int����dir:0��ʾ���Ͻǣ�1��ʾ���Ϸ�..........7��ʾ����
{
	minerSprite->stopAllActions();
	switch (dir)
	{
	case 0:
		minerSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("miner_leftup"))));
		break;
	case 1:
		minerSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("miner_up"))));
		break;
	case 2:
		minerSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("miner_rightup"))));
		break;
	case 3:
		minerSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("miner_right"))));
		break;
	case 4:
		minerSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("miner_rightdown"))));
		break;
	case 5:
		minerSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("miner_down"))));
		break;
	case 6:
		minerSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("miner_leftdown"))));
		break;
	case 7:
		minerSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("miner_left"))));
		break;
	}
}
void Miner::stopMinerAction(int dir)//Ӣ��ֹͣʱ��ȷ��ʾ��վλ
{
	minerSprite->stopAllActions();
	switch (dir)
	{
	case 0:
		minerSprite->setTexture("miner/leftup_1.png");
		break;
	case 1:
		minerSprite->setTexture("miner/up_1.png");
		break;
	case 2:
		minerSprite->setTexture("miner/rightup_5.png");
		break;
	case 3:
		minerSprite->setTexture("miner/right_5.png");
		break;
	case 4:
		minerSprite->setTexture("miner/rightdown_5.png");
		break;
	case 5:
		minerSprite->setTexture("miner/down_1.png");
		break;
	case 6:
		minerSprite->setTexture("miner/leftdown_1.png");
		break;
	case 7:
		minerSprite->setTexture("miner/left_5.png");
		break;
	}
}

void Miner::runFollowShortestRoad()//�������·����
{
	//ȡ��ռ��
	auto tempPos = this->getPosition();
	auto tempTilePos = tempPos / 32.0;
	instance->walkMap[(int)tempTilePos.x][MAP_SIZE - (int)tempTilePos.y] = 0;//ȡ��ռ��

	//log("raod point %f %f", roadPoint[0].x, roadPoint[0].y);

	//�ݹ���øú����Դﵽ�������ߵ�Ŀ��
	if (roadLength != 0)
	{
		//��ȡ��ǰ�������һ�����꣺�����ж�������߷���
		Point nowPoint = roadPoint[roadLength];
		roadLength--;
		Point nextPoint = roadPoint[roadLength];

		nowDir = getMinerDirection(nowPoint, nextPoint);

		if (prevDir != nowDir)
		{
			this->setAnimation(nowDir);
		}
		prevDir = nowDir;

		Point temp = Point(roadPoint[roadLength].x * 32, 80 * 32 - roadPoint[roadLength].y * 32);
		this->runAction(Sequence::create(MoveTo::create(0.4, temp)
			, CallFuncN::create(CC_CALLBACK_0(Miner::runFollowShortestRoad, this))
			, NULL));
	}
	else // ����Ѿ�����Ŀ�ĵ�
	{
		//log("hereeeeeeeeeee");
		//heroDestPointer->setVisible(false);
		this->stopMinerAction(nowDir);
		//ȷ��ռ�����
		auto mapPos = this->getPosition();
		auto tilePos = mapPos / 32.0;
		//log("tile pos end %f %f", tilePos.x, tilePos.y);
		instance->walkMap[(int)tilePos.x][MAP_SIZE - (int)tilePos.y] = 1;//ռ����,ע��Ҫ��80������
		minerDestPointer->setVisible(false);
		
		isReached = true;//����Ŀ�ĵ�
		
		return;
	}
}

int Miner::getMinerDirection(cocos2d::Point now, cocos2d::Point next) //��ȡʿ�����߷���ĺ���
{
	int tempX = next.x - now.x;
	int tempY = next.y - now.y;

	if (tempX == 0 && tempY == 1) //�����·���
	{
		return 5;
	}
	else if (tempX == 0 && tempY == -1)//���Ϸ���
	{
		return 1;
	}
	else if (tempX == 1 && tempY == 0)//���ҷ���
	{
		return 3;
	}
	else if (tempX == -1 && tempY == 0)//������
	{
		return 7;
	}
	else if (tempX == 1 && tempY == -1)//���Ϸ���
	{
		return 2;
	}
	else if (tempX == -1 && tempY == 1)//���·���
	{
		return 6;
	}
	else if (tempX == -1 && tempY == -1)//���Ϸ���
	{
		return 0;
	}
	else if (tempX == 1 && tempY == 1)//���·���
	{
		return 4;
	}

}
void Miner::pathFinding(int srcX, int srcY, int destX, int destY)
{
	//log("ԴĿ�ĵ����꣺%d %d %d %d", srcX, srcY, destX, destY);
	aStar->AStar(srcX, srcY, destX, destY);
	isReachable = aStar->getIsReachable();//�ж��Ƿ�ɴ�Ŀ�ĵ�
	//����·������ �� ���·����
	roadPoint = aStar->roadPoint;
	roadLength = roadPoint.size() - 1;
	//log("soldier road lenght %d", roadLength);


}

void Miner::openListClear()//���openlist�б�
{
	while (!aStar->open.empty()) //�����٣���
	{
		aStar->open.pop();
	}
}

void Miner::setTimeBarVisibleOrNot(bool is)//�������������ɼ����
{

	timeBar->setVisible(is);
	timeBarBg->setVisible(is);
}

void Miner::addTouch()
{
	auto dispatcher = this->getEventDispatcher();
	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Miner::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Miner::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Miner::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this->minerSprite);
}

bool Miner::onTouchBegan(Touch* pTouch, Event* pEvent)
{

	return true;
}
void Miner::onTouchMoved(Touch* pTouch, Event* pEvent)
{

}
void Miner::onTouchEnded(Touch* pTouch, Event* pEvent)
{

}

void Miner::decreaseSurviveTime(float dt)//����ʱ����ٺ��� 1���Ӻ���ʧ
{

	time--;
	timeBar->setPercentage(time);
	if (time <= 0)
	{

		//��2��������ɾ��
		for (std::vector<Miner*>::iterator iter = instance->minerInRectVector.begin(); iter != instance->minerInRectVector.end();)
		{
			if (*iter == this)
				iter = instance->minerInRectVector.erase(iter);
			else
				iter++;
		}

		for (std::vector<Miner*>::iterator iter = instance->minerVector.begin(); iter != instance->minerVector.end();)
		{
			if (*iter == this)
				iter = instance->minerVector.erase(iter);
			else
				iter++;
		}

		minerDestPointer->removeFromParent();//������ȥ��ָʾ��

		instance->setTotalMinerNum(instance->getTotalMinerNum()-1);//�ɿ�����������1

		this->removeFromParent();
	}

}
//��Ѱ����Ĵ�Ӫ
void Miner::searchNearestCamp()
{
	float distance = 100000;
	nearestCamp = nullptr;
	for (int i = 0; i < instance->campTowerVector.size(); i++)
	{
		auto tempCamp = instance->campTowerVector.at(i);
		auto tempCampPos = tempCamp->getPosition();
		if ((tempCampPos.x - this->getPosition().x)*(tempCampPos.x - this->getPosition().x) + (tempCampPos.y - this->getPosition().y)*(tempCampPos.y - this->getPosition().y)<distance*distance)
		{
			distance = (tempCampPos.x - this->getPosition().x)*(tempCampPos.x - this->getPosition().x) + (tempCampPos.y - this->getPosition().y)*(tempCampPos.y - this->getPosition().y);
			nearestCamp = tempCamp;
		}
	}
	
}

//�ҵ����Լ�����Ŀ���ж��λ��
void Miner::searchNearestUnloadPos(CampTower* campTower)
{
	float distance = 10000;
	auto campTowerTilePos = convertToTileCoord(campTower->getPosition());
	campTower->updateVacantUnloadPos(campTowerTilePos);
	for (int i = 0; i < campTower->getVacantUnloadPos().size(); i++)
	{
		auto tempPos = campTower->getVacantUnloadPos().at(i);
		if ((tempPos.x - this->getPosition().x)*(tempPos.x - this->getPosition().x) + (tempPos.y - this->getPosition().y)*(tempPos.y - this->getPosition().y)< distance*distance)
		{
			distance = (tempPos.x - this->getPosition().x)*(tempPos.x - this->getPosition().x) + (tempPos.y - this->getPosition().y)*(tempPos.y - this->getPosition().y);
			unloadPoint = tempPos;
		}
	}

	//log("vacant unload size %d", campTower->getVacantUnloadPos().size());
}

Vec2 Miner::convertToTileCoord(Vec2 position)//��map����ת��Ϊtile����
{
	GameManager * instance = GameManager::getInstance();
	auto mapSize = instance->map->getMapSize();
	// ���㵱ǰ�����£�ÿ����Ƭ�ĳ���
	auto tileWidth = instance->map->getBoundingBox().size.width / instance->map->getMapSize().width;
	auto tileHeight = instance->map->getBoundingBox().size.height / instance->map->getMapSize().height;
	// ��positionת��Ϊ��Ƭ���꣬ȷ���õ���������
	int posX = position.x / tileWidth;
	int posY = instance->map->getMapSize().height - position.y / tileHeight;

	return Point(posX, posY);
}

void Miner::miningTimerIncrease(float dt)//�ɿ��ʱ������
{
	miningTimer++;
}