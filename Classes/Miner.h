#ifndef _MINER_H_
#define _MINER_H_
#include "cocos2d.h"
#include "AStarPath.h"
#include "GameManager.h"
class CampTower;
enum minerState //�ɿ���״̬
{
	stateIdle,
	stateWalkToMine,
	stateWalkToCamp,
	stateMining
};
class Miner : public cocos2d::Sprite  //�ɿ���
{
public:
	Miner();
	~Miner();
	virtual bool init();
	void setAnimation(int dir);//����Ӣ�۵����߶���,��ʾӢ�����߷����int����dir:0��ʾ���Ͻǣ�1��ʾ���Ϸ�..........7��ʾ����
	void stopMinerAction(int dir); //Ӣ��ֹͣʱ��ȷ��ʾ��վλ
	void runFollowShortestRoad();//�������·����
	void pathFinding(int srcX, int srcY, int destX, int destY);
	int getMinerDirection(cocos2d::Point now, cocos2d::Point next); //��ȡӢ�����߷���ĺ���
	cocos2d::Vec2 convertToTileCoord(cocos2d::Vec2 position); //����ͼ����ת��Ϊtile ���� 
	void openListClear();//���openlist�б�
	CC_SYNTHESIZE(int, time, Time);//����ʱ��
	CC_SYNTHESIZE(cocos2d::ProgressTimer*, timeBar, TimeBar);
	void decreaseSurviveTime(float dt);//����ʱ����ٺ���
	void setTimeBarVisibleOrNot(bool is);//�������������ɼ����
	CREATE_FUNC(Miner);

	//2��������¼��һ�η������һ�η���
	int prevDir;
	int nowDir;
	std::vector<cocos2d::Vec2> roadPoint;//������·��vector
	AStarPath * aStar; //AstarѰ·����ָ��

	cocos2d::Sprite * minerSprite;
	cocos2d::Sprite * minerDestPointer;//ʿ��Ŀ�ĵ�ָʾ��
	CC_SYNTHESIZE(cocos2d::Sprite*, stateSprite, StateSprite);//��״̬��ʶ
	CC_SYNTHESIZE(bool, isReachable, IsReachable);//Ŀ�ĵ��Ƿ�ɴ�
	CC_SYNTHESIZE(bool, isReached, IsReached);//�Ƿ񵽴�Ŀ�ĵ�
	CC_SYNTHESIZE(bool, isMiningFinished, IsMiningFinished);//�ɿ���ҵ�Ƿ����
	CC_SYNTHESIZE(int, miningTimer, MiningTimer);//�ɿ���ҵ��ʱ��
	void miningTimerIncrease(float dt);//�ɿ��ʱ������
	//��Ӵ�������;
	virtual void addTouch();
	virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);


	
	CC_SYNTHESIZE(cocos2d::Point, miningPoint, MiningPoint);//�ɿ��
	//����״̬����غ���
	void update(float dt);
	//���òɿ���״̬
	void setState(minerState nextState);
	//��Ѱ����Ĵ�Ӫ
	void searchNearestCamp();
	//�ҵ����Լ�����Ŀ���ж��λ��
	void searchNearestUnloadPos(CampTower* campTower);


private:

	minerState curState;//��ǰ״̬
	CampTower * nearestCamp;//����Ĵ�Ӫ
	cocos2d::Point unloadPoint;//ж��λ��

	
	

	cocos2d::Sprite * ring;//ѡ��minerʱ�����Ĺ�Ȧ
	cocos2d::Sprite * timeBarBg;//����ʱ�����ı���
	cocos2d::Label * timeLabel;//miner����ʱ���ı�

	cocos2d::Vector<cocos2d::SpriteFrame*> walkDown;//�����ߵĶ���vector
	cocos2d::Vector<cocos2d::SpriteFrame*> walkUp;//�����ߵĶ���vector
	cocos2d::Vector<cocos2d::SpriteFrame*> walkLeft;//�����ߵĶ���vector
	cocos2d::Vector<cocos2d::SpriteFrame*> walkRight;//�����ߵĶ���vector

	cocos2d::Vector<cocos2d::SpriteFrame*> walkLeftUp;
	cocos2d::Vector<cocos2d::SpriteFrame*> walkLeftDown;
	cocos2d::Vector<cocos2d::SpriteFrame*> walkRightUp;
	cocos2d::Vector<cocos2d::SpriteFrame*> walkRightDown;

	cocos2d::Animation * animationWalkDown;
	cocos2d::Animation * animationWalkUp;
	cocos2d::Animation * animationWalkLeft;
	cocos2d::Animation * animationWalkRight;

	cocos2d::Animation * animationWalkLeftUp;
	cocos2d::Animation * animationWalkRightUp;
	cocos2d::Animation * animationWalkLeftDown;
	cocos2d::Animation * animationWalkRightDown;


	int roadLength;//���·����
	GameManager * instance;

	cocos2d::EventListenerTouchOneByOne *  listener;
};
#endif

