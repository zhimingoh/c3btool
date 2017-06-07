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

using namespace std;
class HelloWorld : public cocos2d::Layer, public cocos2d::extension::TableViewDelegate, cocos2d::extension::TableViewDataSource
{
public:
	CREATE_FUNC(HelloWorld);

    static cocos2d::Scene* createScene();
    virtual bool init();


    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void selectFile();

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

	void HelloWorld::changeScale(Ref* pSender, int i);
	void HelloWorld::changeRota(Ref* pSender, bool i);
	void HelloWorld::pauseAction();
	void HelloWorld::changeBG(std::string str);
	void HelloWorld::changeCamera();
	void HelloWorld::changeTouchEvent();

	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

	bool OnTouchBeganRole(cocos2d::Touch* touch, cocos2d::Event* event);
	void OnTouchMovedRole(cocos2d::Touch* touch, cocos2d::Event* event);
	void OnTouchEndedRole(cocos2d::Touch* touch, cocos2d::Event* event);

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void onClickTrackNode(bool bClicked);

	float _angleX, _angleY;

private:
	std::vector<cocos2d::Sprite3D*> _sprits;
	int _animateQuality;
	//std::vector<cocos2d::Sprite3D*> sprite;

protected:
	cocos2d::Node* _camControlNode;
	cocos2d::Node* _camNode;
	cocos2d::EventListenerTouchOneByOne* _lis;
	cocos2d::BillBoard* bill1;
	cocos2d::Camera *_camera;
	cocos2d::Camera *_camera2;
     //implement the "static create()" method manually
};


#endif // __HELLOWORLD_SCENE_H__
