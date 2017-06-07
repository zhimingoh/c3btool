#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <tchar.h>
#include "iconv.h"

USING_NS_CC;
USING_NS_CC_EXT;

#include<iostream>
#include <atlconv.h>
#include<string>
#include<io.h>
#include <fstream>
using namespace std;
#include "CustomTableViewCell.h"
#define FONT_NAME     "fonts/Thonburi.ttf"
#define FONT_SIZE    20

CCLayer *layer;
CCLayer *buttonLayer;
CCLayer *listViewLayer;
PUParticleSystem3D *sprite;
Sprite *uiSprite;
Sprite* selectjpg1;
Sprite* selectjpg2;
Sprite* selectjpg3;

cocos2d::Node*  _trackNode;
cocos2d::Vec2  _beginPos;

Label* label;
char * filePath = "G:";
vector<string> files;
vector < string> targetFiles;
int filesNum;
bool initStatu = false;
bool rotaStatu = true;
bool moveStatu = false;
bool actionStatu = true;
bool cameraStatu = false;
Sprite3D* createx(Layer* roleLayer, int i);
std::string a2u(const char *inbuf);
EventListenerTouchAllAtOnce* listener;
EventListenerTouchOneByOne* listener2;



Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
	auto director = Director::getInstance();

	return scene;

}
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	_angleX = 0.0f;
	_angleY = 0.0f;
	sprite = nullptr;
	FileUtils::getInstance()->addSearchPath("Particle3D/materials");
	FileUtils::getInstance()->addSearchPath("Particle3D/scripts");
	FileUtils::getInstance()->addSearchPath("res");
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto  oneScaleBtn = MenuItemImage::create(
		"animationbuttonnormal.png",
		"animationbuttonpressed.png",
		CC_CALLBACK_1(HelloWorld::changeScale, this, 1));
	oneScaleBtn->setPosition(Vec2(0, 0));
	
	auto button_label01 = Label::createWithSystemFont(a2u("1x"), "Arial", 30);
	button_label01->setPosition(Point(oneScaleBtn->getContentSize().width / 2, 40));
	button_label01->setAnchorPoint(Vec2(0.5, 0.5));
	oneScaleBtn->addChild(button_label01, 2);

	auto  twoScaleBtn = MenuItemImage::create(
		"animationbuttonnormal.png",
		"animationbuttonpressed.png",
		CC_CALLBACK_1(HelloWorld::changeScale, this, 2));
	twoScaleBtn->setPosition(Vec2(0, -100));

	 button_label01 = Label::createWithSystemFont(a2u("放大2x"), "Arial", 30);
	 button_label01->setPosition(Point(twoScaleBtn->getContentSize().width / 2, 40));
	button_label01->setAnchorPoint(Vec2(0.5, 0.5));
	twoScaleBtn->addChild(button_label01, 2);

	auto  threeScaleBtn = MenuItemImage::create(
		"animationbuttonnormal.png",
		"animationbuttonpressed.png",
		CC_CALLBACK_1(HelloWorld::changeScale, this, 5));
	threeScaleBtn->setPosition(Vec2(0, -200));

	button_label01 = Label::createWithSystemFont(a2u("放大5x"), "Arial", 30);
	button_label01->setPosition(Point(threeScaleBtn->getContentSize().width / 2, 40));
	button_label01->setAnchorPoint(Vec2(0.5, 0.5));
	threeScaleBtn->addChild(button_label01, 2);

	auto rotaBtn1 = MenuItemImage::create(
		"animationbuttonnormal.png",
		"animationbuttonpressed.png",
		CC_CALLBACK_1(HelloWorld::changeRota, this, true));
	rotaBtn1->setPosition(Vec2(0, -300));

	button_label01 = Label::createWithSystemFont(a2u("任意旋转"), "Arial", 30);
	button_label01->setPosition(Point(rotaBtn1->getContentSize().width / 2, 40));
	button_label01->setAnchorPoint(Vec2(0.5, 0.5));
	rotaBtn1->addChild(button_label01, 2);
	selectjpg1 = Sprite::create("1.png");
	selectjpg1->setScale(0.5f);
	selectjpg1->setPosition(Vec2(200, 35));
	rotaBtn1->addChild(selectjpg1, 2);

	auto rotaBtn2 = MenuItemImage::create(
		"animationbuttonnormal.png",
		"animationbuttonpressed.png",
		CC_CALLBACK_1(HelloWorld::changeRota, this, false));
	rotaBtn2->setPosition(Vec2(0, -400));
	button_label01 = Label::createWithSystemFont(a2u("左右旋转"), "Arial", 30);
	button_label01->setPosition(Point(rotaBtn2->getContentSize().width / 2, 40));
	button_label01->setAnchorPoint(Vec2(0.5, 0.5));
	rotaBtn2->addChild(button_label01, 2);
	selectjpg2 = Sprite::create("1.png");
	selectjpg2->setScale(0.5f);
	selectjpg2->setPosition(Vec2(200, 35));
	selectjpg2->setVisible(false);
	rotaBtn2->addChild(selectjpg2, 2);

	auto moveBtn = MenuItemImage::create(
		"animationbuttonnormal.png",
		"animationbuttonpressed.png",
		CC_CALLBACK_0(HelloWorld::changeTouchEvent, this));
	moveBtn->setPosition(Vec2(0, -300));
	button_label01 = Label::createWithSystemFont(a2u("移动"), "Arial", 30);
	button_label01->setPosition(Point(moveBtn->getContentSize().width / 2, 40));
	button_label01->setAnchorPoint(Vec2(0.5, 0.5));
	moveBtn->addChild(button_label01, 2);
	selectjpg3 = Sprite::create("1.png");
	selectjpg3->setScale(0.5f);
	selectjpg3->setPosition(Vec2(200, 35));
	selectjpg3->setVisible(false);
	moveBtn->addChild(selectjpg3, 2);


	auto pauseBtn = MenuItemImage::create(
		"animationbuttonnormal.png",
		"animationbuttonpressed.png",
		CC_CALLBACK_0(HelloWorld::pauseAction, this));
	pauseBtn->setPosition(Vec2(0, -600));

	button_label01 = Label::createWithSystemFont(a2u("暂停/恢复"), "Arial", 30);
	button_label01->setPosition(Point(pauseBtn->getContentSize().width / 2, 40));
	button_label01->setAnchorPoint(Vec2(0.5, 0.5));
	pauseBtn->addChild(button_label01, 2);

	auto selectBtn = MenuItemImage::create(
		"animationbuttonnormal.png",
		"animationbuttonpressed.png",
		CC_CALLBACK_0(HelloWorld::selectFile, this));
	selectBtn->setPosition(Vec2(0, -400));

	button_label01 = Label::createWithSystemFont(a2u("选择背景图"), "Arial", 30);
	button_label01->setPosition(Point(selectBtn->getContentSize().width / 2, 40));
	button_label01->setAnchorPoint(Vec2(0.5, 0.5));
	selectBtn->addChild(button_label01, 2);


	auto changeCameraBtn = MenuItemImage::create(
		"animationbuttonnormal.png",
		"animationbuttonpressed.png",
		CC_CALLBACK_0(HelloWorld::changeCamera, this));
	changeCameraBtn->setPosition(Vec2(0, -500));

	button_label01 = Label::createWithSystemFont(a2u("切换摄像机"), "Arial", 30);
	button_label01->setPosition(Point(changeCameraBtn->getContentSize().width / 2, 40));
	button_label01->setAnchorPoint(Vec2(0.5, 0.5));
	changeCameraBtn->addChild(button_label01, 2);

	label = Label::create();
	label->setColor(Color3B::MAGENTA);
	label->setPosition(250, 35);
	changeCameraBtn->addChild(label, 2);
	label->setString(a2u("正交"));
	label->setSystemFontSize(35.0f);
	auto menu = Menu::create(oneScaleBtn, twoScaleBtn, threeScaleBtn, moveBtn, selectBtn, changeCameraBtn, NULL);
	menu->setScale(0.5);
	menu->setPosition(Vec2(-221,440));
	this->addChild(menu, 1);

	//获取该路径下的所有文件  
	getFiles(getApplicationPath(), files);
	reckonNum();

	CCSize s = CCDirector::sharedDirector()->getWinSize();
	//layer = CCLayerColor::create(ccc4(0xff, 0x00, 0x00, 0x80), 200, 200);
	layer = CCLayer::create();
	layer->setContentSize(Size(200,200));
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(s.width / 2, s.height / 2);
	this->addChild(layer);

	
	listViewLayer = CCLayerColor::create(ccc4(0xff, 0x00, 0x00, 0x80), 200, 200);
	listViewLayer->setContentSize(Size(150, s.height));
	listViewLayer->ignoreAnchorPointForPosition(false);
	listViewLayer->setAnchorPoint(Vec2(1,0));
	listViewLayer->setPosition(s.width, 0);
	this->addChild(listViewLayer);
	
	TableView* tableView = TableView::create(this, listViewLayer->getContentSize());
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setPosition(Vec2(0, 0));
	tableView->setDelegate(this);
	tableView->setAnchorPoint(Vec2(0, 0));
	listViewLayer->addChild(tableView);
	tableView->reloadData();

	uiSprite = Sprite::create("BattleScene.png");
	uiSprite->setPosition3D(Vec3(s.width / 2, s.height / 2, 0));
	uiSprite->setAnchorPoint(Vec2(0, 0));
	addChild(uiSprite);
	uiSprite->setGlobalZOrder(-1);
	uiSprite->setCameraMask(2);

	 listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	auto pTextField = TextFieldTTF::textFieldWithPlaceHolder("Scale",
		FONT_NAME,
		FONT_SIZE);
	this->addChild(pTextField);
	pTextField->setPosition(Vec2(67,300));
	pTextField->setColor(Color3B::WHITE);
	pTextField->setAnchorPoint(Vec2(1, 0));
	_trackNode = pTextField;

	auto lister3 = EventListenerTouchOneByOne::create();
	lister3->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	lister3->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lister3, pTextField);


    return true;
}

Sprite3D* createx(Layer* roleLayer, int i)
{
	Sprite3D* _hero1 = Sprite3D::create("1001_0_01.c3b");
	_hero1->setRotation3D(Vec3(0, 0, 0));
	roleLayer->addChild(_hero1);

	Sprite3D* weapon = Sprite3D::create("weapon_01_01b.c3b");
	//weapon->setCameraMask(2);
	_hero1->getAttachNode("rhandnode")->addChild(weapon);

	weapon->setTag(2);
	auto mrole2 = weapon->getNodeToParentTransform();
	mrole2.rotateX(CC_DEGREES_TO_RADIANS(-180));
	weapon->setNodeToParentTransform(mrole2);

	Sprite3D* hair = Sprite3D::create("1001_1_01.c3b");
	//hair->setRotation3D(Vec3(-90, 0, 0));	
	_hero1->getAttachNode("headnode")->addChild(hair);
	hair->setTag(1);
	auto mrole = hair->getNodeToParentTransform();
	mrole.rotateX(CC_DEGREES_TO_RADIANS(-180));
	hair->setNodeToParentTransform(mrole);
	auto label = Label::createWithTTF(to_string(i), "fonts/Marker Felt.ttf", 24);
	label->setColor(Color3B::MAGENTA);
	label->setPosition(0, -20);

	auto animation = Animation3D::create("1001_0_01.c3b");
	Animate3D* heroAnimate = Animate3D::createWithFrames(animation, 180, 204);
	_hero1->runAction(RepeatForever::create(heroAnimate));

	auto bill1 = BillBoard::create(BillBoard::Mode::VIEW_PLANE_ORIENTED);
	bill1->setColor(Color3B::YELLOW);
	bill1->setScale(0.6f);
	bill1->addChild(label);
	_hero1->addChild(bill1);
	return _hero1;
}

//如果你重写了onEnter方法，你应该调用它的父类
void HelloWorld::onEnter()
{
	Node::onEnter();
	initCamera();
}
void HelloWorld::onExit()
{
	Node::onExit();

}

void HelloWorld::initCamera(){
	if (initStatu)
	{
		return;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto size = Director::getInstance()->getWinSize();

	//人物摄像机
	_camera = Camera::createOrthographic(size.width, size.height, -1024, 1024);
	_camera->setCameraFlag(CameraFlag::USER1);
	_camera->setPosition3D(Vec3(size.width / 2, size.height/2, Director::getInstance()->getZEye()));
	this->addChild(_camera);

	_camera2 = Camera::createPerspective(60, (GLfloat)size.width / size.height, 1, 1024);
	_camera2->setCameraFlag(CameraFlag::USER2);
	_camera2->setPosition3D(Vec3(size.width / 2, size.height / 2, Director::getInstance()->getZEye()));
	_camera2->retain();
	this->addChild(_camera2);

	schedule(schedule_selector(HelloWorld::update));
	initStatu = true;//已经初始化改变状态

}
void HelloWorld::update(float dt)
{
}
//获取exe路径
std::string HelloWorld::getApplicationExePath()
{
	TCHAR szFileName[MAX_PATH];
	GetModuleFileName(NULL, szFileName, MAX_PATH);
	std::u16string u16ApplicationName;
	char *applicationExePath = convertTCharToUtf8(szFileName);
	std::string path(applicationExePath);
	CC_SAFE_FREE(applicationExePath);
	CCLOG("EXE PATH%s", path.c_str());
	return path;
}
std::string HelloWorld::getApplicationPath()
{
	std::string path = getApplicationExePath();
	size_t pos;
	while ((pos = path.find_first_of("\\")) != std::string::npos)
	{
		path.replace(pos, 1, "/");
	}
	size_t p = path.find_last_of("/");
	string workdir;
	if (p != path.npos)
	{
		workdir = path.substr(0, p);
	}
	//CCLOG("getApplicationPath %s", workdir.c_str());
	workdir += "\\res";
	//CCLOG("workdir %s", workdir.c_str());
	return workdir;
}

char* HelloWorld::convertTCharToUtf8(const TCHAR* src)
{
#ifdef UNICODE
	WCHAR* tmp = (WCHAR*)src;
	size_t size = wcslen(src) * 3 + 1;
	char* dest = new char[size];
	memset(dest, 0, size);
	WideCharToMultiByte(CP_UTF8, 0, tmp, -1, dest, size, NULL, NULL);
	return dest;
#else
	char* tmp = (char*)src;
	uint32 size = strlen(tmp) + 1;
	WCHAR* dest = new WCHAR[size];
	memset(dest, 0, sizeof(WCHAR)*size);
	MultiByteToWideChar(CP_ACP, 0, src, -1, dest, (int)size); // convert local code to unicode.

	size = wcslen(dest) * 3 + 1;
	char* dest2 = new char[size];

	memset(dest2, 0, size);
	WideCharToMultiByte(CP_UTF8, 0, dest, -1, dest2, size, NULL, NULL); // convert unicode to utf8.
	delete[] dest;
	return dest2;
#endif
}

void HelloWorld::selectFile()
{

	char szFileName[MAX_PATH] = { 0 };
	static TCHAR szFilter[] = TEXT("Text Files(*.png)\0*.png\0") \
		TEXT("Text Files(*.jpg)\0*.jpg\0") \
		TEXT("All Files(*.*)\0*.*\0\0");

	OPENFILENAME openFileName = { 0 };
	openFileName.lStructSize = sizeof(OPENFILENAME);	
	openFileName.hwndOwner = NULL;
	openFileName.hInstance = NULL;
	openFileName.nMaxFile = MAX_PATH;  //这个必须设置，不设置的话不会出现打开文件对话框  
	openFileName.lpstrFilter = szFilter;
	openFileName.lpstrFile = (LPWSTR)szFileName;
	::GetOpenFileName(&openFileName);
	USES_CONVERSION;
	std::string  str(W2A(openFileName.lpstrFile));
	//CCLOG("PATH ==%s", str.c_str());
	changeBG(str);
}

void HelloWorld::changeBG(std::string str)
{
	if (uiSprite)
	{
		uiSprite->removeFromParent();
		uiSprite = nullptr;
	}
	if (str.length() == 0)
	{
		return;
	}
	uiSprite = Sprite::create(str);
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	uiSprite->setPosition3D(Vec3(s.width / 2, s.height / 2, 0));
	uiSprite->setAnchorPoint(Vec2(0, 0));
	this->addChild(uiSprite);
	uiSprite->setGlobalZOrder(-1);
	uiSprite->setCameraMask(2);

}
void HelloWorld::changeCamera()
{
	if (sprite)
	{
		if (cameraStatu)
		{
			sprite->setCameraMask(2);
			auto s = Director::getInstance()->getWinSize();
			sprite->setPosition3D(Vec3(s.width / 2, s.height / 2, 0));
			cameraStatu = false;
			label->setString(a2u("正交"));
		}
		else
		{
			sprite->setCameraMask(4);
			sprite->setPosition3D(Vec3(100, 50, 0));
			cameraStatu = true;
			label->setString(a2u("透视"));
		}
	}
}
//根据传入的路径创建C3Dsprite
void HelloWorld::createSpriteFormPath(std::string str)
{
	if (str.length() == 0)
	{
		return;
	}
	layer->removeAllChildren();

	size_t pos;
	while ((pos = str.find_first_of("/")) != std::string::npos)
	{
		str.replace(pos, 1, "\\");
	}

	std::string fileName = str;

	CCLOG("fileName: %s", fileName.c_str());
	//sprite = Sprite3D::create(fileName);

	sprite = PUParticleSystem3D::create(fileName);
	sprite->setPosition(500, 500);
	sprite->startParticleSystem();
	auto pusize = sprite->getContentSize();
	CCLOG("============%f%f", pusize.width, pusize.height);

	if (!sprite)
	{
		return;
	}
	auto s = Director::getInstance()->getWinSize();
	sprite->setScale(1);
	//sprite->setRotation3D(Vec3(0, 180, 0));
	layer->addChild(sprite);
	//sprite->setPosition3D(Vec3(s.width / 2, s.height / 2, 0));
	sprite->setCameraMask((unsigned short)CameraFlag::USER1);
	//sprite->setForceDepthWrite(true);

	_camera2->lookAt(sprite->getPosition3D());
	label->setString(a2u("正交"));
	cameraStatu = false;

	/*auto animation = Animation3D::create(fileName);
	if (animation)
	{
		auto ani = Animate3D::create(animation);
		sprite->runAction(RepeatForever::create(ani));
	}*/
}

void HelloWorld::getFiles(string path, vector<string>& files)
{
	//std::string ext = FileUtils::getInstance()->getFileExtension(path); //获取文件后缀
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}


//table
void HelloWorld::tableCellTouched(TableView* table, TableViewCell* cell)
{
	createSpriteFormPath(targetFiles[cell->getIdx()].c_str());
}

void HelloWorld::changeScale(Ref* pSender, int i)
{
	if (sprite)
	{
		sprite->setScale(i);

	}
}
void HelloWorld::changeRota(Ref* pSender, bool i)
{
	_eventDispatcher->removeEventListener(listener);
	_eventDispatcher->removeEventListener(listener2);
	selectjpg3->setVisible(false);

	moveStatu = false;
	listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	rotaStatu = i;
	selectjpg1->setVisible(rotaStatu);
	CCLOG("selectjpg2->isVisible()  %d", selectjpg2->isVisible());
	selectjpg2->setVisible(!rotaStatu);

}
void HelloWorld::changeTouchEvent()
{
	if (sprite==nullptr)
	{
		return;
	}
	_eventDispatcher->removeEventListener(listener);
	_eventDispatcher->removeEventListener(listener2);
	selectjpg3->setVisible(!moveStatu);

	if (moveStatu)
	{
		/*selectjpg1->setVisible(rotaStatu);
		selectjpg2->setVisible(!rotaStatu);*/

		moveStatu = false;
		listener = EventListenerTouchAllAtOnce::create();
		listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
		listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	}
	else
	{
	/*	selectjpg1->setVisible(false);
		selectjpg2->setVisible(false);*/
		moveStatu = true;
		listener2 = EventListenerTouchOneByOne::create();
		listener2->setSwallowTouches(true);
		listener2->onTouchBegan = CC_CALLBACK_2(HelloWorld::OnTouchBeganRole, this);
		listener2->onTouchMoved = CC_CALLBACK_2(HelloWorld::OnTouchMovedRole, this);
		listener2->onTouchEnded = CC_CALLBACK_2(HelloWorld::OnTouchEndedRole, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, sprite);

	}

}
void HelloWorld::pauseAction()
{
	if (actionStatu)
	{
		auto director = Director::getInstance();
		director->getActionManager()->pauseTarget(sprite);
		actionStatu = false;
	} 
	else
	{
		auto director = Director::getInstance();
		director->getActionManager()->resumeTarget(sprite);
		actionStatu = true;
	}

}

Size HelloWorld::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	//CCLOG("tableCellSizeForIndex IDX %s", idx);
	//if (idx == 2) {
	//	return Size(100, 100);
	//}
	return Size(60, 30);
}

//创建每一项tablecell
TableViewCell* HelloWorld::tableCellAtIndex(TableView *table, ssize_t idx)
{

	char* save_name, *pos;
	int name_len;
	auto fullpathname = targetFiles[idx].c_str();
	name_len = strlen(fullpathname);
	pos = (char*)fullpathname + name_len;
	while (*pos != '\\' && pos != fullpathname)
		pos--;
	if (pos == fullpathname)
	{
		save_name = (char*)fullpathname + 1;
	}
	name_len = name_len - (pos - fullpathname);
	save_name = (char*)malloc(name_len + 1);
	memcpy(save_name, pos + 1, name_len);
	auto string = StringUtils::format("%s", save_name);

	TableViewCell *cell = table->dequeueCell();
	if (!cell) {
		cell = new (std::nothrow) CustomTableViewCell();
		cell->autorelease();
		auto label = Label::createWithSystemFont(string, "Helvetica", 20.0);
		label->setPosition(Vec2::ZERO);
		label->setAnchorPoint(Vec2::ZERO);
		label->setTag(123);
		cell->addChild(label);
	}
	else
	{
		auto label = (Label*)cell->getChildByTag(123);
		label->setString(string);
	}


	return cell;
}

void HelloWorld::reckonNum()
{
	char str[30];
	int j = 0;
	int size = files.size();
	for (int i = 0; i < size; i++)
	{
		cout << files[i].c_str() << endl;
		std::string ext = FileUtils::getInstance()->getFileExtension(files[i].c_str()); //获取文件后缀
		std::string targetExt = ".pu";
		int temp = ext.compare(targetExt);
		if (temp == 0)
		{
			j += 1;
			targetFiles.push_back(files[i]);
		}

	}
	filesNum = j;
}
void HelloWorld::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	if (!sprite)
	{
		return;
	}
	if (touches.size())
	{
		auto touch = touches[0];
		auto delta = touch->getDelta();
		
		if (rotaStatu)
		{
			if (fabs(delta.x) > 1.5) {
				_angleX -= CC_DEGREES_TO_RADIANS(delta.x);
			}
			if (fabs(delta.y) > 1.5) {
				_angleY -= CC_DEGREES_TO_RADIANS(delta.y);
			}
			sprite->setRotation3D(Vec3(-180 * _angleY, -180 * _angleX, sprite->getRotation3D().z));
		} 
		else
		{
			_angleX -= CC_DEGREES_TO_RADIANS(delta.x);
			sprite->setRotation3D(Vec3(sprite->getRotation3D().x, -180 * _angleX, sprite->getRotation3D().z));
		}
		return;
	}
}

void HelloWorld::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{

}
ssize_t HelloWorld::numberOfCellsInTableView(TableView *table)
{	
	return filesNum;
}

void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

//void HelloWorld::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
//{
//
//	auto target = static_cast<Sprite3D*>(event->getCurrentTarget());
//	target->setPosition(target->getPosition() + touches[0]->getDelta());
//}
//
//
//void HelloWorld::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
//{
//
//}

void HelloWorld::onClickTrackNode(bool bClicked)
{
	auto pTextField = (TextFieldTTF*)_trackNode;
	if (bClicked)
	{
		pTextField->attachWithIME();
	}
	else
	{
		pTextField->detachWithIME();
		auto text = pTextField->getString();
		if (sprite && text.length() != 0)
		{
			sprite->setScale(atof(text.c_str()));
		}
	}
}

bool HelloWorld::onTouchBegan(Touch  *touch, Event  *event)
{
	//CCLOG("++++++++++++++++++++++++++++++++++++++++++++");
	_beginPos = touch->getLocation();
	return true;
}

void HelloWorld::onTouchEnded(Touch  *touch, Event  *event)
{
	if (!_trackNode)
	{
		return;
	}

	auto endPos = touch->getLocation();

	float delta = 5.0f;
	if (std::abs(endPos.x - _beginPos.x) > delta
		|| std::abs(endPos.y - _beginPos.y) > delta)
	{
		// not click
		_beginPos.x = _beginPos.y = -1;
		return;
	}

	// decide the trackNode is clicked.
	Rect rect;
	rect.size = _trackNode->getContentSize();
	auto clicked = isScreenPointInRect(endPos, Camera::getVisitingCamera(), _trackNode->getWorldToNodeTransform(), rect, nullptr);
	this->onClickTrackNode(clicked);
}
bool HelloWorld::OnTouchBeganRole(Touch* touch, Event* event)
{
	
	auto target = static_cast<Sprite3D*>(event->getCurrentTarget());

	Rect rect = target->getBoundingBox();
	CCLOG("rect============%d", rect);
	//if (rect.containsPoint(touch->getLocation()))
	{
		//log("sprite3d began... x = %f, y = %f", touch->getLocation().x, touch->getLocation().y);
		target->setOpacity(100);
		return true;
	}
	return false;
}

void HelloWorld::OnTouchMovedRole(Touch* touch, Event* event)
{
	auto target = static_cast<Sprite3D*>(event->getCurrentTarget());
	target->setPosition(target->getPosition() + touch->getDelta());
}

void HelloWorld::OnTouchEndedRole(Touch* touch, Event* event)
{
	auto target = static_cast<Sprite3D*>(event->getCurrentTarget());
	Vec3 aa = target->getPosition3D();
	log("sprite3d onTouchesEnded.. %f %f %f", aa.x, aa.y, aa.z);
	target->setOpacity(255);
}







//中文转码
int code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	iconv_t cd;
	const char *temp = inbuf;
	const char **pin = &temp;
	char **pout = &outbuf;
	memset(outbuf, 0, outlen);
	cd = iconv_open(to_charset, from_charset);
	if (cd == 0) return -1;
	if (iconv(cd, pin, &inlen, pout, &outlen) == -1) return -1;
	iconv_close(cd);
	return 0;
}

std::string u2a(const char *inbuf)
{
	size_t inlen = strlen(inbuf);
	char * outbuf = new char[inlen * 2 + 2];
	std::string strRet;
	if (code_convert("utf-8", "gb2312", inbuf, inlen, outbuf, inlen * 2 + 2) == 0)
	{
		strRet = outbuf;
	}
	delete[] outbuf;
	return strRet;
}

std::string a2u(const char *inbuf)
{
	size_t inlen = strlen(inbuf);
	char * outbuf = new char[inlen * 2 + 2];
	std::string strRet;
	if (code_convert("gb2312", "utf-8", inbuf, inlen, outbuf, inlen * 2 + 2) == 0)
	{
		strRet = outbuf;
	}
	delete[] outbuf;
	return strRet;
}
