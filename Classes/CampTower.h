#ifndef _CAMPTOWER_H_
#define _CAMPTOWER_H_
#include "BaseTower.h"
#include "cocos2d.h"
#include "GameManager.h"
#include "AStarPath.h"
#include "Miner.h"
#include <queue>
#define MAX_MINER_NUM 20
class CampTower :
	public BaseTower
{
public:
	CampTower();
	~CampTower();
	virtual bool init();
	CREATE_FUNC(CampTower);
	void shoot(float dt);//��ʵ���������������
	cocos2d::Sprite * getTrainingSprite();

	cocos2d::Vec2 convertTotileCoord(cocos2d::Vec2 position); //��map����ת��Ϊtile����
	cocos2d::Vec2 convertToScreenCoord(cocos2d::Vec2 position); //��tile����ת��Ϊmap��������
	cocos2d::Point getNextRallyPoint(cocos2d::Point rally);//��ȡ��һ�������

	CC_SYNTHESIZE(int, produceTime, ProduceTime);//�ɿ�������ʱ��
	CC_SYNTHESIZE(cocos2d::ProgressTimer*, timeBar, TimeBar);//�ɿ�������ʱ�������
	CC_SYNTHESIZE(cocos2d::Sprite*, produceTimeBarBg, ProduceTimeBarBg);//����ʱ�����ı���

	void produceMiner(float dt);//�����ɿ��ߵĺ���
	CC_SYNTHESIZE(std::vector<cocos2d::Point>, vacantUnloadPos, VacantUnloadPos); //����ж��λ��vector
	void updateVacantUnloadPos(cocos2d::Point Pos);//�ҵ������ж��λ�ù��ɿ���ж��

private:
	cocos2d::Sprite * trainingSprite;//�����ɿ��ߵ�button
	cocos2d::EventListenerTouchOneByOne * trainingButtonListener;
	GameManager * instance;
	Miner * miner;

	//�ɿ���վλ����:��������ʱĬ�ϵ�վλ
	std::queue<cocos2d::Point> posQueue;
	cocos2d::Point rallyPoint;//Ĭ��ʿ�������

	cocos2d::Vec2 src;//Դ�㣺ʿ�������ĵ�
	cocos2d::Label * minerNum;//������ʿ������Ŀlabel

	int minerToProduceCount;//������ʿ������Ŀ

	bool isTrainingSpriteSelected;
};
#endif

