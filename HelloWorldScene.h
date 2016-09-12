#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#pragma once

#include <iostream>
#include <stdio.h>
//string是c++ 的头文件，其内包含了一个string类，string s1就是建立一个string类的对象 
#include <string>
//cstring.h 是对应于旧C 头文件的std 版本
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
	void selectFile(cocos2d::Ref* pSender);

	void createSpriteFormPath(std::string str);
	void getFiles(std::string path, vector<std::string>& files);
	std::string getApplicationExePath();
	char* HelloWorld::convertTCharToUtf8(const TCHAR* src);
	std::string HelloWorld::getApplicationPath();
	char g_szFileName[MAX_PATH];
	void HelloWorld::reckonNum();

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;

private:
	std::vector<cocos2d::Sprite3D*> _sprits;
	int _animateQuality;
	std::vector<cocos2d::Sprite3D*> sprite;

    
    // implement the "static create()" method manually
};

#endif // __HELLOWORLD_SCENE_H__
