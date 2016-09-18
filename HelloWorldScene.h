#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#pragma once

#include <iostream>
#include <stdio.h>
#include <string>
#include <cstringt.h>
#include <locale>
#include "DrawNode3D.h"


using namespace std;

enum class CameraType
{
	Free = 0,
	FirstPerson = 1,
	ThirdPerson = 2,
};
enum State
{
	State_None = 0,
	State_Idle = 0x01,
	State_Move = 0x02,
	State_Rotate = 0x04,
	State_Speak = 0x08,
	State_MeleeAttack = 0x10,
	State_RemoteAttack = 0x20,
	State_Attack = 0x40,
};

class HelloWorld : public cocos2d::Layer, public cocos2d::extension::TableViewDelegate, cocos2d::extension::TableViewDataSource
{
public:
	CREATE_FUNC(HelloWorld);

    static cocos2d::Scene* createScene();
    virtual bool init();


    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void selectFile(cocos2d::Ref* pSender);

	void createSpriteFormPath(std::string str);
	void getFiles(std::string path, vector<std::string>& files);
	std::string getApplicationExePath();
	char* HelloWorld::convertTCharToUtf8(const TCHAR* src);
	std::string HelloWorld::getApplicationPath();
	char g_szFileName[MAX_PATH];
	void HelloWorld::reckonNum();
	 void initCamera();
	 void onEnter();
	 void onExit();
	 //void onEnterTransitionDidFinish();
	 //void onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event);

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;

	virtual void update(float dt) override;

	void reachEndCallBack();
	void switchViewCallback(cocos2d::Ref* sender);
	void addSpriteCallback(cocos2d::Ref* sender);
	void delSpriteCallback(cocos2d::Ref* sender);
	void scaleCameraCallback(cocos2d::Ref* sender, float value);
	void rotateCameraCallback(cocos2d::Ref* sender, float value);
	void SwitchViewCallback(cocos2d::Ref* sender, CameraType cameraType);
	void updateCamera(float fDelta);
	void move3D(float elapsedTime);
	void updateState(float elapsedTime);
	bool isState(unsigned int state, unsigned int bit) const;

	bool onTouchesCommon(cocos2d::Touch* touch, cocos2d::Event* event, bool* touchProperty);
	bool onTouchesZoomOut(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchesZoomOutEnd(cocos2d::Touch* touch, cocos2d::Event* event);
	bool onTouchesZoomIn(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchesZoomInEnd(cocos2d::Touch* touch, cocos2d::Event* event);

	bool onTouchesRotateLeft(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchesRotateLeftEnd(cocos2d::Touch* touch, cocos2d::Event* event);
	bool onTouchesRotateRight(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchesRotateRightEnd(cocos2d::Touch* touch, cocos2d::Event* event);
	void addNewSpriteWithCoords(cocos2d::Vec3 p, std::string fileName, bool playAnimation = false, float scale = 1.0f, bool bindCamera = false);

	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

	void initSprite();

private:
	std::vector<cocos2d::Sprite3D*> _sprits;
	int _animateQuality;
	std::vector<cocos2d::Sprite3D*> sprite;

protected:
	cocos2d::Node* _camControlNode;
	cocos2d::Node* _camNode;
	cocos2d::EventListenerTouchOneByOne* _lis;
	cocos2d::BillBoard* bill1;
	cocos2d::Camera *_camera;

	std::string    _title;
	cocos2d::Layer*         _layer3D;
	cocos2d::Sprite3D*      _sprite3D;
	cocos2d::Vec3           _targetPos;
	CameraType     _cameraType;
	cocos2d::MenuItem*      _incRot;
	cocos2d::MenuItem*      _decRot;
	unsigned int   _curState;
	cocos2d::MoveTo* _moveAction;
	bool _bZoomOut;
	bool _bZoomIn;
	bool _bRotateLeft;
	bool _bRotateRight;
	cocos2d::Label* _RotateRightlabel;
	cocos2d::Label* _RotateLeftlabel;
	cocos2d::Label* _ZoomInlabel;
	cocos2d::Label* _ZoomOutlabel;


     //implement the "static create()" method manually
};

#endif // __HELLOWORLD_SCENE_H__
