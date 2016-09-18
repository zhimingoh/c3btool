#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <tchar.h>

USING_NS_CC;
USING_NS_CC_EXT;

#include<iostream>
#include <atlconv.h>
#include<string>
#include<io.h>
#include "CustomTableViewCell.h"
#include "VisibleRect.h"

using namespace std;

CCLayer *layer;
CCLayer *labelLayer;
CCLayer *listViewLayer;

Label* label;
char * filePath = "G:";
vector<string> files;
vector < string> targetFiles;
int filesNum;
bool initStatu = false;


//Billboard
//Camera3D
//Sprite3D(getAttachNode、getMeshByName)
//Mesh

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


    
    auto label = Label::createWithTTF("Select a file on the right", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

	////获取该路径下的所有文件  
	getFiles(getApplicationPath(), files);
	reckonNum();

	CCSize s = CCDirector::sharedDirector()->getWinSize();
	//layer = CCLayerColor::create(ccc4(0xff, 0x00, 0x00, 0x80), 200, 200);
	layer = CCLayer::create();
	layer->setContentSize(Size(200,200));
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(s.width / 2, s.height / 2);
	this->addChild(layer);

	
	listViewLayer = CCLayerColor::create(ccc4(0xff, 0x00, 0x00, 0x80), s.width, 25);
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

	_incRot = nullptr;
	_decRot = nullptr;
	_camera = nullptr;
	_bZoomOut = false;
	_bZoomIn = false;
	_bRotateLeft = false;
	_bRotateRight = false;

	//auto sp3d = Sprite3D::create();
	//sp3d->setPosition(s.width,0);
	//addChild(sp3d);
	////Billboards
	////Yellow is at the back
	//bill1 = BillBoard::create("Images/Icon.png");
	//bill1->setPosition3D(Vec3(50, 10, -10));
	//bill1->setColor(Color3B::YELLOW);
	//bill1->setScale(0.6f);
	//sp3d->addChild(bill1);
	//如果目录不为空则默认显示第一个C3B文件
	
	//if (!targetFiles.empty())
	//{
	//	createSpriteFormPath(targetFiles[0]);
	//}

    return true;
}


//如果你重写了onEnter方法，你应该调用它的父类
//void HelloWorld::onEnter()
//{
//	Node::onEnter();
//	initCamera();
//
//}
//void HelloWorld::onExit()
//{
//	Node::onExit();
//
//}
//void HelloWorld::onEnterTransitionDidFinish()
//{
//
//}
// 摄像机事件
void HelloWorld::initCamera(){
	CCLOG("getDefaultCamera %d", Camera::getDefaultCamera()->getPosition3D().z);
	if (initStatu)
	{
		CCLOG("-=-=-=");
		return;
	}
	_camControlNode = Node::create();
	_camControlNode->setNormalizedPosition(Vec2(.5, .5));
	this->addChild(_camControlNode);

	_camNode = Node::create();
	_camNode->setPositionZ(Camera::getDefaultCamera()->getPosition3D().z);
	_camControlNode->addChild(_camNode);


	//Listener
	_lis = EventListenerTouchOneByOne::create();
	_lis->onTouchBegan = [this](Touch* t, Event* e) {
		return true;
	};

	_lis->onTouchMoved = [this](Touch* t, Event* e) {
		float dx = t->getDelta().x;
		Vec3 rot = _camControlNode->getRotation3D();
		rot.y += dx;
		_camControlNode->setRotation3D(rot);

		Vec3 worldPos;
		_camNode->getNodeToWorldTransform().getTranslation(&worldPos);

		Camera::getDefaultCamera()->setPosition3D(worldPos);
		Camera::getDefaultCamera()->lookAt(_camControlNode->getPosition3D());
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_lis, layer);

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
	CCLOG("getApplicationPath %s", workdir.c_str());
	workdir += "\\res";
	CCLOG("workdir %s", workdir.c_str());
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

void HelloWorld::selectFile(Ref* pSender)
{

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
	CCLOG("PATH ==%s", str.c_str());
	createSpriteFormPath(str);
	
}

//根据传入的路径创建C3Dsprite
void HelloWorld::createSpriteFormPath(std::string str)
{
	if (str.length() == 0)
	{
		return;
	}
	layer->removeAllChildren();
	//labelLayer->removeAllChildren();
	size_t pos;
	while ((pos = str.find_first_of("/")) != std::string::npos)
	{
		str.replace(pos, 1, "\\");
	}
	CCLOG("=END PATH", str.c_str());

	std::string fileName = str;
	auto sprite = Sprite3D::create(fileName);
	if (!sprite)
	{
		return;
	}
	auto s = Director::getInstance()->getWinSize();
	sprite->setScale(7);
	sprite->setRotation3D(Vec3(0, 180, 0));
	sprite->setPosition(layer->getContentSize().width/2, layer->getContentSize().height);
	sprite->setAnchorPoint(Vec2(0, 1));
	layer->addChild(sprite);

	//显示当前选择文件路径
	//label = Label::createWithSystemFont("File Path:" + str, "Arial", 24);
	//label->setAnchorPoint(Vec2(0, 0));
	//labelLayer->addChild(label);

	auto animation = Animation3D::create(fileName);
	if (animation)
	{
		CCLOG("=============animation========");
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

	//灯光
	auto _directionalLight = DirectionLight::create(Vec3(1.0f, -1.0f, 0.0f), Color3B(255, 255, 0));

	_directionalLight->retain();

	_directionalLight->setEnabled(true);

	addChild(_directionalLight);

	//_directionalLight->setCameraMask(2);
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
	//CCLOG("cell touched at index: %ld", static_cast<long>(cell->getIdx()));
	CCLOG("tableCellTouched == %s", targetFiles[cell->getIdx()].c_str());

	//createSpriteFormPath(targetFiles[cell->getIdx()].c_str());
	addNewSpriteWithCoords(Vec3(0, 0, 0), targetFiles[cell->getIdx()].c_str(), true, 1, true);

	//initCamera();
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
//计算c3b总数
void HelloWorld::reckonNum()
{
	char str[30];
	int j = 0;
	int size = files.size();
	for (int i = 0; i < size; i++)
	{
		cout << files[i].c_str() << endl;
		std::string ext = FileUtils::getInstance()->getFileExtension(files[i].c_str()); //获取文件后缀
		std::string targetExt = ".c3b";
		int temp = ext.compare(targetExt);
		if (temp == 0)
		{
			j += 1;
			
			targetFiles.push_back(files[i]);
		}

	}
	filesNum = j;
	CCLOG("has %d .c3b files", filesNum);
}
ssize_t HelloWorld::numberOfCellsInTableView(TableView *table)
{	
	return filesNum;
}

//------------------------------------------------------------------
//
// Camera3DTestDemo
//
//------------------------------------------------------------------
//HelloWorld::Camera3DTestDemo(void)
//: _incRot(nullptr)
//, _decRot(nullptr)
//, _camera(nullptr)
//, _bZoomOut(false)
//, _bZoomIn(false)
//, _bRotateLeft(false)
//, _bRotateRight(false)
//{
//}
//HelloWorld::~Camera3DTestDemo(void)
//{
//}
void HelloWorld::reachEndCallBack()
{
}
//std::string HelloWorld::title() const
//{
//	return "Testing Camera";
//}

void HelloWorld::scaleCameraCallback(Ref* sender, float value)
{
	if (_camera&& _cameraType != CameraType::FirstPerson)
	{
		Vec3 cameraPos = _camera->getPosition3D();
		cameraPos += cameraPos.getNormalized()*value;
		_camera->setPosition3D(cameraPos);
	}
}
void HelloWorld::rotateCameraCallback(Ref* sender, float value)
{
	if (_cameraType == CameraType::Free || _cameraType == CameraType::FirstPerson)
	{
		Vec3  rotation3D = _camera->getRotation3D();
		rotation3D.y += value;
		_camera->setRotation3D(rotation3D);
	}
}
void HelloWorld::SwitchViewCallback(Ref* sender, CameraType cameraType)
{
	if (_cameraType == cameraType)
	{
		return;
	}
	_cameraType = cameraType;
	if (_cameraType == CameraType::Free)
	{
		_camera->setPosition3D(Vec3(0, 130, 130) + _sprite3D->getPosition3D());
		_camera->lookAt(_sprite3D->getPosition3D());

		_RotateRightlabel->setColor(Color3B::WHITE);
		_RotateLeftlabel->setColor(Color3B::WHITE);
		_ZoomInlabel->setColor(Color3B::WHITE);
		_ZoomOutlabel->setColor(Color3B::WHITE);
	}
	else if (_cameraType == CameraType::FirstPerson)
	{
		Vec3 newFaceDir;
		_sprite3D->getWorldToNodeTransform().getForwardVector(&newFaceDir);
		newFaceDir.normalize();
		_camera->setPosition3D(Vec3(0, 35, 0) + _sprite3D->getPosition3D());
		_camera->lookAt(_sprite3D->getPosition3D() + newFaceDir * 50);

		_RotateRightlabel->setColor(Color3B::WHITE);
		_RotateLeftlabel->setColor(Color3B::WHITE);
		_ZoomInlabel->setColor(Color3B::GRAY);
		_ZoomOutlabel->setColor(Color3B::GRAY);
	}
	else if (_cameraType == CameraType::ThirdPerson)
	{
		_camera->setPosition3D(Vec3(0, 130, 130) + _sprite3D->getPosition3D());
		_camera->lookAt(_sprite3D->getPosition3D());

		_RotateRightlabel->setColor(Color3B::GRAY);
		_RotateLeftlabel->setColor(Color3B::GRAY);
		_ZoomInlabel->setColor(Color3B::WHITE);
		_ZoomOutlabel->setColor(Color3B::WHITE);
	}
}

void HelloWorld::initSprite()
{
	_sprite3D = nullptr;
	auto s = Director::getInstance()->getWinSize();
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	auto layer3D = Layer::create();
	addChild(layer3D, 0);
	_layer3D = layer3D;
	_curState = State_None;
	if (targetFiles.empty())
	{
		return;
	}

	//转换路径
	size_t pos;
	while ((pos = targetFiles[1].find_first_of("/")) != std::string::npos)
	{
		targetFiles[1].replace(pos, 1, "\\");
	}
	addNewSpriteWithCoords(Vec3(0, 0, 0), targetFiles[1], true, 1, true);
	TTFConfig ttfConfig("fonts/arial.ttf", 20);

	auto containerForLabel1 = Node::create();
	_ZoomOutlabel = Label::createWithTTF(ttfConfig, "zoom out");
	_ZoomOutlabel->setPosition(s.width - listViewLayer->getContentSize().width, VisibleRect::top().y - 30);
	_ZoomOutlabel->setAnchorPoint(Vec2(1, 0.5));
	containerForLabel1->addChild(_ZoomOutlabel);
	addChild(containerForLabel1, 10);

	auto listener1 = EventListenerTouchOneByOne::create();
	listener1->setSwallowTouches(true);

	listener1->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchesZoomOut, this);
	listener1->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchesZoomOutEnd, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, _ZoomOutlabel);

	auto containerForLabel2 = Node::create();
	_ZoomInlabel = Label::createWithTTF(ttfConfig, "zoom in");
	_ZoomInlabel->setPosition(s.width - listViewLayer->getContentSize().width, VisibleRect::top().y - 100);
	_ZoomInlabel->setAnchorPoint(Vec2(1, 0.5));
	containerForLabel2->addChild(_ZoomInlabel);
	addChild(containerForLabel2, 10);

	auto listener2 = EventListenerTouchOneByOne::create();
	listener2->setSwallowTouches(true);

	listener2->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchesZoomIn, this);
	listener2->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchesZoomInEnd, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, _ZoomInlabel);

	auto containerForLabel3 = Node::create();
	_RotateLeftlabel = Label::createWithTTF(ttfConfig, "rotate left");
	_RotateLeftlabel->setPosition(s.width - listViewLayer->getContentSize().width, VisibleRect::top().y - 170);
	containerForLabel3->addChild(_RotateLeftlabel);
	_RotateLeftlabel->setAnchorPoint(Vec2(1, 0.5));

	addChild(containerForLabel3, 10);

	auto listener3 = EventListenerTouchOneByOne::create();
	listener3->setSwallowTouches(true);

	listener3->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchesRotateLeft, this);
	listener3->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchesRotateLeftEnd, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener3, _RotateLeftlabel);

	auto containerForLabel4 = Node::create();
	_RotateRightlabel = Label::createWithTTF(ttfConfig, "rotate right");
	_RotateRightlabel->setPosition(s.width - listViewLayer->getContentSize().width, VisibleRect::top().y - 240);
	_RotateRightlabel->setAnchorPoint(Vec2(1, 0.5));
	containerForLabel4->addChild(_RotateRightlabel);
	addChild(containerForLabel4, 10);

	auto listener4 = EventListenerTouchOneByOne::create();
	listener4->setSwallowTouches(true);

	listener4->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchesRotateRight, this);
	listener4->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchesRotateRightEnd, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener4, _RotateRightlabel);

	//几个按钮
	auto label1 = Label::createWithTTF(ttfConfig, "free ");
	auto menuItem1 = MenuItemLabel::create(label1, CC_CALLBACK_1(HelloWorld::SwitchViewCallback, this, CameraType::Free));
	auto label2 = Label::createWithTTF(ttfConfig, "third person");
	auto menuItem2 = MenuItemLabel::create(label2, CC_CALLBACK_1(HelloWorld::SwitchViewCallback, this, CameraType::ThirdPerson));
	auto label3 = Label::createWithTTF(ttfConfig, "first person");
	auto menuItem3 = MenuItemLabel::create(label3, CC_CALLBACK_1(HelloWorld::SwitchViewCallback, this, CameraType::FirstPerson));
	auto menu = Menu::create(menuItem1, menuItem2, menuItem3, nullptr);

	menu->setPosition(Vec2::ZERO);

	menuItem1->setPosition(VisibleRect::left().x + 100, VisibleRect::top().y - 50);
	menuItem2->setPosition(VisibleRect::left().x + 100, VisibleRect::top().y - 100);
	menuItem3->setPosition(VisibleRect::left().x + 100, VisibleRect::top().y - 150);
	addChild(menu, 0);
	schedule(CC_SCHEDULE_SELECTOR(HelloWorld::updateCamera), 0.0f);
	if (_camera == nullptr)
	{
		_camera = Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
		_camera->setCameraFlag(CameraFlag::USER1);
		_layer3D->addChild(_camera);
	}
	SwitchViewCallback(this, CameraType::ThirdPerson);
	DrawNode3D* line = DrawNode3D::create();
	//draw x
	for (int j = -20; j <= 20; j++)
	{
		line->drawLine(Vec3(-100, 0, 5 * j), Vec3(100, 0, 5 * j), Color4F(1, 0, 0, 1));
	}
	//draw z
	for (int j = -20; j <= 20; j++)
	{
		line->drawLine(Vec3(5 * j, 0, -100), Vec3(5 * j, 0, 100), Color4F(0, 0, 1, 1));
	}
	//draw y
	line->drawLine(Vec3(0, -50, 0), Vec3(0, 0, 0), Color4F(0, 0.5, 0, 1));
	line->drawLine(Vec3(0, 0, 0), Vec3(0, 50, 0), Color4F(0, 1, 0, 1));
	_layer3D->addChild(line);

	_layer3D->setCameraMask(2);
}
void HelloWorld::onEnter()
{
	Node::onEnter();
	initSprite();
		
}
void HelloWorld::onExit()
{
	Node::onExit();
	if (_camera)
	{
		_camera = nullptr;
	}
}

void HelloWorld::addNewSpriteWithCoords(Vec3 p, std::string fileName, bool playAnimation, float scale, bool bindCamera)
{
	auto sprite = Sprite3D::create(fileName);
	_layer3D->addChild(sprite);
	float globalZOrder = sprite->getGlobalZOrder();
	sprite->setPosition3D(Vec3(p.x, p.y, p.z));
	sprite->setGlobalZOrder(globalZOrder);
	if (playAnimation)
	{
		auto animation = Animation3D::create(fileName, "Take 001");
		if (animation)
		{
			auto animate = Animate3D::create(animation);
			sprite->runAction(RepeatForever::create(animate));
		}
	}
	if (bindCamera)
	{
		_sprite3D = sprite;
	}
	sprite->setScale(scale);
}
void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	for (auto &item : touches)
	{
		auto touch = item;
		auto location = touch->getLocation();
	}
}
void HelloWorld::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	if (touches.size() == 1)
	{
		auto touch = touches[0];
		auto location = touch->getLocation();
		Point newPos = touch->getPreviousLocation() - location;
		if (_cameraType == CameraType::Free || _cameraType == CameraType::FirstPerson)
		{
			Vec3 cameraDir;
			Vec3 cameraRightDir;
			_camera->getNodeToWorldTransform().getForwardVector(&cameraDir);
			cameraDir.normalize();
			cameraDir.y = 0;
			_camera->getNodeToWorldTransform().getRightVector(&cameraRightDir);
			cameraRightDir.normalize();
			cameraRightDir.y = 0;
			Vec3 cameraPos = _camera->getPosition3D();
			cameraPos += cameraDir*newPos.y*0.1f;
			cameraPos += cameraRightDir*newPos.x*0.1f;
			_camera->setPosition3D(cameraPos);
			if (_sprite3D &&  _cameraType == CameraType::FirstPerson)
			{
				_sprite3D->setPosition3D(Vec3(_camera->getPositionX(), 0, _camera->getPositionZ()));
				_targetPos = _sprite3D->getPosition3D();
			}
		}
	}
}
void HelloWorld::move3D(float elapsedTime)
{
	if (_sprite3D)
	{
		Vec3 curPos = _sprite3D->getPosition3D();
		Vec3 newFaceDir = _targetPos - curPos;
		newFaceDir.y = 0.0f;
		newFaceDir.normalize();
		Vec3 offset = newFaceDir * 25.0f * elapsedTime;
		curPos += offset;
		_sprite3D->setPosition3D(curPos);
		if (_cameraType == CameraType::ThirdPerson)
		{
			Vec3 cameraPos = _camera->getPosition3D();
			cameraPos.x += offset.x;
			cameraPos.z += offset.z;
			_camera->setPosition3D(cameraPos);
		}
	}
}
void HelloWorld::updateState(float elapsedTime)
{
	if (_sprite3D)
	{
		Vec3 curPos = _sprite3D->getPosition3D();
		Vec3 curFaceDir;
		_sprite3D->getNodeToWorldTransform().getForwardVector(&curFaceDir);
		//curFaceDir = -curFaceDir;
		curFaceDir.normalize();
		Vec3 newFaceDir = _targetPos - curPos;
		newFaceDir.y = 0.0f;
		newFaceDir.normalize();
		float cosAngle = std::fabs(Vec3::dot(curFaceDir, newFaceDir) - 1.0f);
		float dist = curPos.distanceSquared(_targetPos);
		if (dist <= 4.0f)
		{
			if (cosAngle <= 0.01f)
				_curState = State_Idle;
			else
				_curState = State_Rotate;
		}
		else
		{
			if (cosAngle > 0.01f)
				_curState = State_Rotate | State_Move;
			else
				_curState = State_Move;
		}
	}
}
void HelloWorld::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	for (auto &item : touches)
	{
		auto touch = item;
		auto location = touch->getLocationInView();
		if (_camera)
		{
			if (_sprite3D && _cameraType == CameraType::ThirdPerson && _bZoomOut == false && _bZoomIn == false && _bRotateLeft == false && _bRotateRight == false)
			{
				Vec3 nearP(location.x, location.y, -1.0f), farP(location.x, location.y, 1.0f);

				auto size = Director::getInstance()->getWinSize();
				nearP = _camera->unproject(nearP);
				farP = _camera->unproject(farP);
				Vec3 dir(farP - nearP);
				float dist = 0.0f;
				float ndd = Vec3::dot(Vec3(0, 1, 0), dir);
				if (ndd == 0)
					dist = 0.0f;
				float ndo = Vec3::dot(Vec3(0, 1, 0), nearP);
				dist = (0 - ndo) / ndd;
				Vec3 p = nearP + dist *  dir;

				if (p.x > 100)
					p.x = 100;
				if (p.x < -100)
					p.x = -100;
				if (p.z > 100)
					p.z = 100;
				if (p.z < -100)
					p.z = -100;

				_targetPos = p;
			}
		}
	}
}
void onTouchesCancelled(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
}
void HelloWorld::updateCamera(float fDelta)
{
	if (_sprite3D)
	{
		if (_cameraType == CameraType::ThirdPerson)
		{
			updateState(fDelta);
			if (isState(_curState, State_Move))
			{
				move3D(fDelta);
				if (isState(_curState, State_Rotate))
				{
					Vec3 curPos = _sprite3D->getPosition3D();

					Vec3 newFaceDir = _targetPos - curPos;
					newFaceDir.y = 0;
					newFaceDir.normalize();
					Vec3 up;
					_sprite3D->getNodeToWorldTransform().getUpVector(&up);
					up.normalize();
					Vec3 right;
					Vec3::cross(-newFaceDir, up, &right);
					right.normalize();
					Vec3 pos = Vec3(0, 0, 0);
					Mat4 mat;
					mat.m[0] = right.x;
					mat.m[1] = right.y;
					mat.m[2] = right.z;
					mat.m[3] = 0.0f;

					mat.m[4] = up.x;
					mat.m[5] = up.y;
					mat.m[6] = up.z;
					mat.m[7] = 0.0f;

					mat.m[8] = newFaceDir.x;
					mat.m[9] = newFaceDir.y;
					mat.m[10] = newFaceDir.z;
					mat.m[11] = 0.0f;

					mat.m[12] = pos.x;
					mat.m[13] = pos.y;
					mat.m[14] = pos.z;
					mat.m[15] = 1.0f;
					_sprite3D->setAdditionalTransform(&mat);
				}
			}
		}
		if (_bZoomOut == true)
		{
			if (_camera)
			{
				if (_cameraType == CameraType::ThirdPerson)
				{
					Vec3 lookDir = _camera->getPosition3D() - _sprite3D->getPosition3D();
					Vec3 cameraPos = _camera->getPosition3D();
					if (lookDir.length() <= 300)
					{
						cameraPos += lookDir.getNormalized();
						_camera->setPosition3D(cameraPos);
					}
				}
				else if (_cameraType == CameraType::Free)
				{
					Vec3 cameraPos = _camera->getPosition3D();
					if (cameraPos.length() <= 300)
					{
						cameraPos += cameraPos.getNormalized();
						_camera->setPosition3D(cameraPos);
					}
				}
			}
		}
		if (_bZoomIn == true)
		{
			if (_camera)
			{
				if (_cameraType == CameraType::ThirdPerson)
				{
					Vec3 lookDir = _camera->getPosition3D() - _sprite3D->getPosition3D();
					Vec3 cameraPos = _camera->getPosition3D();
					if (lookDir.length() >= 50)
					{
						cameraPos -= lookDir.getNormalized();
						_camera->setPosition3D(cameraPos);
					}
				}
				else if (_cameraType == CameraType::Free)
				{
					Vec3 cameraPos = _camera->getPosition3D();
					if (cameraPos.length() >= 50)
					{
						cameraPos -= cameraPos.getNormalized();
						_camera->setPosition3D(cameraPos);
					}
				}
			}
		}
		if (_bRotateLeft == true)
		{
			if (_cameraType == CameraType::Free || _cameraType == CameraType::FirstPerson)
			{
				Vec3  rotation3D = _camera->getRotation3D();
				rotation3D.y += 1;
				_camera->setRotation3D(rotation3D);
			}
		}
		if (_bRotateRight == true)
		{
			if (_cameraType == CameraType::Free || _cameraType == CameraType::FirstPerson)
			{
				Vec3  rotation3D = _camera->getRotation3D();
				rotation3D.y -= 1;
				_camera->setRotation3D(rotation3D);
			}
		}
	}
}
bool HelloWorld::onTouchesCommon(Touch* touch, Event* event, bool* touchProperty)
{
	auto target = static_cast<Label*>(event->getCurrentTarget());

	Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);

	if (rect.containsPoint(locationInNode))
	{
		*touchProperty = true;
		return true;
	}
	return false;
}
bool HelloWorld::isState(unsigned int state, unsigned int bit) const
{
	return (state & bit) == bit;
}
bool HelloWorld::onTouchesZoomOut(Touch* touch, Event* event)
{
	return HelloWorld::onTouchesCommon(touch, event, &_bZoomOut);
}
void HelloWorld::onTouchesZoomOutEnd(Touch* touch, Event* event)
{
	_bZoomOut = false;
}
bool HelloWorld::onTouchesZoomIn(Touch* touch, Event* event)
{
	return HelloWorld::onTouchesCommon(touch, event, &_bZoomIn);
}
void HelloWorld::onTouchesZoomInEnd(Touch* touch, Event* event)
{
	_bZoomIn = false;
}
bool HelloWorld::onTouchesRotateLeft(Touch* touch, Event* event)
{
	return HelloWorld::onTouchesCommon(touch, event, &_bRotateLeft);
}
void HelloWorld::onTouchesRotateLeftEnd(Touch* touch, Event* event)
{
	_bRotateLeft = false;
}
bool HelloWorld::onTouchesRotateRight(Touch* touch, Event* event)
{
	return HelloWorld::onTouchesCommon(touch, event, &_bRotateRight);
}
void HelloWorld::onTouchesRotateRightEnd(Touch* touch, Event* event)
{
	_bRotateRight = false;
}

