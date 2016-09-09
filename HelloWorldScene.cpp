#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <tchar.h>

USING_NS_CC;
#include<iostream>
#include <atlconv.h>
#include<string>
using namespace std;

CCLayer *layer;
CCLayer *labelLayer;
Label* label;
Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
										   CC_CALLBACK_1(HelloWorld::selectFile, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


	auto closeItem2 = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::selectFile, this));

	closeItem2->setPosition(Vec2(50, 50));

	// create menu, it's an autorelease object
	auto menu2= Menu::create(closeItem2, NULL);
	menu2->setPosition(Vec2::ONE);
	this->addChild(menu2, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

	CCSize s = CCDirector::sharedDirector()->getWinSize();
	//layer = CCLayerColor::create(ccc4(0xff, 0x00, 0x00, 0x80), 200, 200);
	layer = CCLayer::create();
	layer->setContentSize(Size(200,200));
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(s.width / 2, s.height / 2);
	this->addChild(layer);

	labelLayer = CCLayerColor::create(ccc4(0xff, 0x00, 0x00, 0x80), s.width, 25);
	//labelLayer->ignoreAnchorPointForPosition(false);
	labelLayer->setAnchorPoint(Vec2(0,0));
	labelLayer->setPosition(0,0);
	this->addChild(labelLayer);
    // add "HelloWorld" splash screen"
    //auto sprite = Sprite::create("HelloWorld.png");

    //// position the sprite on the center of the screen
    //sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    //// add the sprite as a child to this layer
    //this->addChild(sprite, 0);
    
    return true;
}
void HelloWorld::selectFile(Ref* pSender)
{
	CCLOG("SELECT");
	char szFileName[MAX_PATH] = { 0 };
	static TCHAR szFilter[] = TEXT("Text Files(*.c3b)\0*.c3b\0") \
		TEXT("All Files(*.*)\0*.*\0\0");

	OPENFILENAME openFileName = { 0 };
	openFileName.lStructSize = sizeof(OPENFILENAME);
	openFileName.hwndOwner = NULL;
	openFileName.hInstance = NULL;
	openFileName.nMaxFile = MAX_PATH;  //这个必须设置，不设置的话不会出现打开文件对话框  
	openFileName.lpstrFilter = szFilter;
	openFileName.lpstrFile = (LPWSTR)szFileName;
	//openFileName.nFilterIndex = 1;
	//openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	::GetOpenFileName(&openFileName);
	USES_CONVERSION;
	std::string  str(W2A(openFileName.lpstrFile));
	//MessageBox(str.c_str(), "File Name");
	createSpriteFormPath(str);
	
}

void HelloWorld::createSpriteFormPath(std::string str)
{
	layer->removeAllChildren();
	labelLayer->removeAllChildren();

	std::string fileName = str;
	auto sprite = Sprite3D::create(fileName);
	auto s = Director::getInstance()->getWinSize();
	sprite->setScale(10);
	sprite->setRotation3D(Vec3(0, 180, 0));
	sprite->setPosition(layer->getContentSize().width/2, layer->getContentSize().height);
	sprite->setAnchorPoint(Vec2(0, 1));
	layer->addChild(sprite);

	//显示当前选择文件路径
	label = Label::createWithSystemFont("File Path:" + str, "Arial", 24);
	label->setAnchorPoint(Vec2(0, 0));
	labelLayer->addChild(label);

	auto animation = Animation3D::create(fileName);
	if (animation)
	{
		auto animate = Animate3D::create(animation);
		bool inverse = (std::rand() % 3 == 0);

		int rand2 = std::rand();
		float speed = 1.0f;
		if (rand2 % 3 == 1)
		{
			speed = animate->getSpeed() + CCRANDOM_0_1();
		}
		else if (rand2 % 3 == 2)
		{
			speed = animate->getSpeed() - 0.5 * CCRANDOM_0_1();
		}
		animate->setSpeed(inverse ? -speed : speed);

		sprite->runAction(RepeatForever::create(animate));
	}
}
void HelloWorld::coutName(Ref* pSender)
{

}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
