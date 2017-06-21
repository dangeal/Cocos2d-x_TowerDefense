#ifndef _CRYSTALMINE_H_
#define _CRYSTALMINE_H_
#include "cocos2d.h"
class CrystalMine : public cocos2d::Sprite
{
public:
	CrystalMine();
	~CrystalMine();
	virtual bool init();
	CREATE_FUNC(CrystalMine);

	CC_SYNTHESIZE(int, crystalMaxNum, CrystalMaxNum);//ˮ�����������
	CC_SYNTHESIZE(cocos2d::ProgressTimer*, crystalBar, CrystalBar);//ˮ��������������
	CC_SYNTHESIZE(int, crystalCurrentNum, crystalCurrentNum);//ˮ����ǰ����

	CC_SYNTHESIZE(std::vector<cocos2d::Point> , vacantMiningPos,VacantMiningPos); //ÿһ��ˮ������һ��vector������洢�˿��еĲɿ�λ��
	void updateVacantMiningPos(cocos2d::Point minePos);//���¿��еĲɿ�λ�ã������ũ��ɿ�ʱ�������˺��� ,������ʾ�Լ���map�е�λ��
	cocos2d::Vec2 convertTotileCoord(cocos2d::Vec2 position);  //��map����ת��Ϊtile����

	virtual void addTouch();//��Ӵ����¼�
	virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	//virtual void onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	//virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
private:
	cocos2d::Sprite * crystalSprite;//ˮ����ͼƬ
	cocos2d::Sprite * crystalBarSprite;//ˮ�������������ͼƬ
protected:
	cocos2d::EventListenerTouchOneByOne *  listener; //����������

};
#endif

