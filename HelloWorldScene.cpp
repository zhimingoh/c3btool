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

    // create menu, it's an autorelease object
 //   auto menu = Menu::create(closeItem, NULL);
 //   menu->setPosition(Vec2::ZERO);
 //   this->addChild(menu, 1);


	//auto closeItem2 = MenuItemImage::create(
	//	"CloseNormal.png",
	//	"CloseSelected.png",
	//	CC_CALLBACK_1(HelloWorld::initCamera, this));

	//closeItem2->setPosition(Vec2(50, 50));

	//// create menu, it's an autorelease object
	//auto menu2= Menu::create(closeItem2, NULL);
	//menu2->setPosition(Vec2::ONE);
	//this->addChild(menu2, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
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
	if (!targetFiles.empty())
	{
		createSpriteFormPath(targetFiles[0]);
	}

    return true;
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

	createSpriteFormPath(targetFiles[cell->getIdx()].c_str());
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


