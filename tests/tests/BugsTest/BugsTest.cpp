#include "BugsTest.h"
#include "Bug-350.h"
#include "Bug-422.h"
#include "Bug-458/Bug-458.h"
#include "Bug-624.h"
#include "Bug-886.h"
#include "Bug-899.h"
#include "Bug-914.h"
#include "Bug-1159.h"
#include "Bug-1174.h"
#include "ParticleBufOverflow.h"

#define TEST_BUG(bugNO)                                 \
{                                                       \
CCScene* pScene = CCScene::node();                      \
Bug##bugNO##Layer* pLayer = new Bug##bugNO##Layer();    \
pLayer->init();                                         \
pScene->addChild(pLayer);                               \
CCDirector::sharedDirector()->replaceScene(pScene);     \
pLayer->autorelease();                                  \
}

enum
{
    MAX_COUNT = 10,
    LINE_SPACE = 40,
    kItemTagBasic = 5432,
};

static CCPoint s_tCurPos = CCPointZero;

const std::string testsName[MAX_COUNT] = 
{
    "Bug-350",
    "Bug-422",
    "Bug-458",
    "Bug-624",
    "Bug-886",
    "Bug-899",
    "Bug-914",
    "Bug-1159",
    "Bug-1174",
    "Particle Buffer Overflow"
};

////////////////////////////////////////////////////////
//
// BugsTestMainLayer
//
////////////////////////////////////////////////////////
void BugsTestMainLayer::onEnter()
{
    CCLayer::onEnter();

    CCSize s = CCDirector::sharedDirector()->getWinSize();
    m_pItmeMenu = CCMenu::menuWithItems(NULL);
    CCMenuItemFont::setFontName("Arial");
    CCMenuItemFont::setFontSize(24);
    for (int i = 0; i < MAX_COUNT; ++i)
    {
        CCMenuItemFont* pItem = CCMenuItemFont::itemFromString(testsName[i].c_str(), this,
                                                    menu_selector(BugsTestMainLayer::menuCallback));
        pItem->setPosition(ccp(s.width / 2, s.height - (i + 1) * LINE_SPACE));
        m_pItmeMenu->addChild(pItem, kItemTagBasic + i);
    }

    m_pItmeMenu->setPosition(s_tCurPos);
    addChild(m_pItmeMenu);
    setIsTouchEnabled(true);
}

void BugsTestMainLayer::menuCallback(CCObject* pSender)
{
    CCMenuItemFont* pItem = (CCMenuItemFont*)pSender;
    int nIndex = pItem->getZOrder() - kItemTagBasic;

    switch (nIndex)
    {
    case 0:
        TEST_BUG(350);
        break;
    case 1:
        TEST_BUG(422);
        break;
    case 2:
        TEST_BUG(458);
        break;
    case 3:
        TEST_BUG(624);
        break;
    case 4:
        TEST_BUG(886);
        break;
    case 5:
        TEST_BUG(899);
        break;
    case 6:
        TEST_BUG(914);
        break;
    case 7:
        TEST_BUG(1159);
        break;
    case 8:
        TEST_BUG(1174);
        break;
    case 9:
        TEST_BUG(ParticleBufOverflow);
        break;
    default:
        break;
    }
}

void BugsTestMainLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);

    m_tBeginPos = touch->locationInView();	
    m_tBeginPos = CCDirector::sharedDirector()->convertToGL( m_tBeginPos );
}

void BugsTestMainLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);

    CCPoint touchLocation = touch->locationInView();	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    float nMoveY = touchLocation.y - m_tBeginPos.y;

    CCPoint curPos  = m_pItmeMenu->getPosition();
    CCPoint nextPos = ccp(curPos.x, curPos.y + nMoveY);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if (nextPos.y < 0.0f)
    {
        m_pItmeMenu->setPosition(CCPointZero);
        return;
    }

    if (nextPos.y > ((MAX_COUNT + 1)* LINE_SPACE - winSize.height))
    {
        m_pItmeMenu->setPosition(ccp(0, ((MAX_COUNT + 1)* LINE_SPACE - winSize.height)));
        return;
    }

    m_pItmeMenu->setPosition(nextPos);
    m_tBeginPos = touchLocation;
    s_tCurPos   = nextPos;
}

////////////////////////////////////////////////////////
//
// BugsTestBaseLayer
//
////////////////////////////////////////////////////////
void BugsTestBaseLayer::onEnter()
{
    CCLayer::onEnter();

    CCSize s = CCDirector::sharedDirector()->getWinSize();

    CCMenuItemFont::setFontName("Arial");
    CCMenuItemFont::setFontSize(24);
    CCMenuItemFont* pMainItem = CCMenuItemFont::itemFromString("Back", this,
        menu_selector(BugsTestBaseLayer::backCallback));
    pMainItem->setPosition(ccp(s.width - 50, 25));
    CCMenu* pMenu = CCMenu::menuWithItems(pMainItem, NULL);
    pMenu->setPosition( CCPointZero );
    addChild(pMenu);
}

void BugsTestBaseLayer::backCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->enableRetinaDisplay(false);
    BugsTestScene* pScene = new BugsTestScene();
    pScene->runThisTest();
    pScene->autorelease();
}

////////////////////////////////////////////////////////
//
// BugsTestScene
//
////////////////////////////////////////////////////////
void BugsTestScene::runThisTest()
{
    CCLayer* pLayer = new BugsTestMainLayer();
    addChild(pLayer);
    pLayer->release();

    CCDirector::sharedDirector()->replaceScene(this);
}
