#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void selectFile(cocos2d::Ref* pSender);
	void coutName(cocos2d::Ref* pSender);
	void createSpriteFormPath(std::string str);

	char g_szFileName[MAX_PATH];
private:
	std::vector<cocos2d::Sprite3D*> _sprits;
	int _animateQuality;
	std::vector<cocos2d::Sprite3D*> sprite;

    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
