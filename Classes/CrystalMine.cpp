#include "CrystalMine.h"
#include "GameManager.h"
USING_NS_CC;


CrystalMine::CrystalMine()
{
}


CrystalMine::~CrystalMine()
{
}

bool CrystalMine::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	//��ʼ��
	crystalSprite = Sprite::create("mine.png"); //ˮ����ͼƬ
	//crystalSprite->setAnchorPoint(Point(0, 0));
	addChild(crystalSprite, 3);

	setCrystalMaxNum(100);
	setcrystalCurrentNum(100);
	//ˮ�������������ͼƬ
	crystalBarSprite = Sprite::create("manaBarBg.png");
	crystalBarSprite->setPosition(crystalSprite->getPosition() + Point(0, 50));
	crystalBarSprite->setScale(0.4);
	addChild(crystalBarSprite, 10);
	//ˮ����洢����������ʼ��
	crystalBar = ProgressTimer::create(Sprite::create("soldierProduceTimeBar.png")); //������һ��ͼƬsprite
	crystalBar->setScaleX(2);
	crystalBar->setScaleY(5.2);
	crystalBar->setType(ProgressTimer::Type::BAR);
	crystalBar->setMidpoint(Point(0, 0.5f));
	crystalBar->setBarChangeRate(Point(1, 0));
	crystalBar->setPercentage(100);
	crystalBar->setPosition(Point(crystalBarSprite->getContentSize().width / 2, crystalBarSprite->getContentSize().height / 3 * 2 - 10));
	crystalBarSprite->addChild(crystalBar, 5);


	addTouch();

	//Point tempPos = convertTotileCoord(this->getPosition());
	//updateVacantMiningPos(tempPos);


	return true;
}

bool CrystalMine::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	auto target = static_cast<Sprite*>(pEvent->getCurrentTarget());
	// ��ȡ��ǰ�����������԰�ť��λ������   
	Point locationInNode = target->convertToNodeSpace(pTouch->getLocation());          //һ��Ҫ������
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);

	// �����Χ�жϼ��
	if (rect.containsPoint(locationInNode))
	{
		crystalBarSprite->setVisible(true);
		crystalBar->setVisible(true);
		return true;
	}
	else
	{
		crystalBarSprite->setVisible(false);
		crystalBar->setVisible(false);
	}

	return true;
}

void CrystalMine::addTouch()//��Ӵ����¼�
{
	auto dispatcher = this->getEventDispatcher();
	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(CrystalMine::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this->crystalSprite); //���ˮ��������������
}

void CrystalMine::updateVacantMiningPos(Point minePos)//���¿��еĲɿ�λ�ã������ũ��ɿ�ʱ�������˺���,�õ����µĿ���λ��vector
{
	int centerX = minePos.x;
	int centerY = minePos.y;
	//���vector
	vacantMiningPos.clear();
	//��ȡ����
	auto instance = GameManager::getInstance();
	//�ж���Χ6��λ���Ƿ�Ϸ��ҿ��У�����ǵĻ��ͼ���vector
	//ˮ��������һ��
	if (centerX - 1 >= 0 && centerY - 2 >= 0 && instance->walkMap[centerX - 1][centerY - 2] == 0)
	{
		auto tempPoint = Point(centerX - 1, centerY - 2);
		vacantMiningPos.push_back(tempPoint);
	}
	if (centerX  >= 0 && centerY - 2 >= 0 && instance->walkMap[centerX ][centerY - 2] == 0)
	{
		auto tempPoint = Point(centerX , centerY - 2);
		vacantMiningPos.push_back(tempPoint);
	}
	//ˮ�����Ҳ�
	if (centerX +1>= 0 && centerY  >= 0 && instance->walkMap[centerX+1][centerY] == 0)
	{
		auto tempPoint = Point(centerX+1, centerY);
		vacantMiningPos.push_back(tempPoint);
	}
	//ˮ�����²�
	if (centerX - 1 >= 0 && centerY + 2>= 0 && instance->walkMap[centerX -1][centerY+2] == 0)
	{
		auto tempPoint = Point(centerX - 1, centerY+2);
		vacantMiningPos.push_back(tempPoint);
	}
	if (centerX  >= 0 && centerY + 2 >= 0 && instance->walkMap[centerX ][centerY + 2] == 0)
	{
		auto tempPoint = Point(centerX , centerY + 2);
		vacantMiningPos.push_back(tempPoint);
	}
	//ˮ�������
	if (centerX-2 >= 0 && centerY  >= 0 && instance->walkMap[centerX-2][centerY ] == 0)
	{
		auto tempPoint = Point(centerX-2, centerY);
		vacantMiningPos.push_back(tempPoint);
	}

	//log("vacant size %d", vacantMiningPos.size());
	/*for (int i = 0; i < vacantMiningPos.size(); i++)
	{
		auto temp = vacantMiningPos.at(i);
		log("pos %f %f", temp.x, temp.y);
	}*/
}

Vec2 CrystalMine::convertTotileCoord(Vec2 position)  //��map����ת��Ϊtile����
{
	auto instance = GameManager::getInstance();

	auto mapSize = instance->map->getMapSize();
	// ���㵱ǰ�����£�ÿ����Ƭ�ĳ���
	auto tileWidth = instance->map->getBoundingBox().size.width / instance->map->getMapSize().width;
	auto tileHeight = instance->map->getBoundingBox().size.height / instance->map->getMapSize().height;
	// ��positionת��Ϊ��Ƭ���꣬ȷ���õ���������
	int posX = position.x / tileWidth;
	int posY = instance->map->getMapSize().height - position.y / tileHeight;

	return Point(posX, posY);
}