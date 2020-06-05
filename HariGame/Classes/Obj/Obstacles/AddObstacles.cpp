#include "AddObstacles.h"

#include "Obj/OBJ_COLLAR.h"
#include"BlackLadybug/BlackLadybug.h"
#include"Buds/Buds.h"


USING_NS_CC;

void AddObstacles::operator()(cocos2d::Layer* layer, cocos2d::TMXLayer* mapLayer)
{
    const Size visibleSize = Director::getInstance()->getVisibleSize();


    if (mapLayer == nullptr)
    {
        return;
    }

    int chipSize = 48;
    auto mapSize = mapLayer->getLayerSize();

    for (int y = 0; y < mapSize.height; y++)
    {
        for (int x = 0; x < mapSize.width; x++)
        {
            auto obstaclesPoint = Vec2{ (float)x,(float)y };
            auto obstaclesGid = mapLayer->getTileGIDAt(obstaclesPoint);

            auto putPos = Vec2(obstaclesPoint.x * chipSize + (chipSize / 2), (mapSize.height - obstaclesPoint.y) * chipSize - (chipSize / 2));

            // •‚¢‚Ä‚ñ‚Æ‚¤’Ž
            if (obstaclesGid == OBSTACLES::LADYBUG_R)
            {
                auto blackLadydug = BlackLadybug::createBlackLadybug(OBJ_COLOR::OBJ_RED);
                blackLadydug->setName("blackLadydug");
                layer->addChild(blackLadydug);
                blackLadydug->setPosition(putPos);
                blackLadydug->setScale(0.2f);
                blackLadydug->SetPoint(Vec2(20.0f, 20.0f));
            }
            if (obstaclesGid == OBSTACLES::LADYBUG_G)
            {
                auto blackLadydug = BlackLadybug::createBlackLadybug(OBJ_COLOR::OBJ_GREEN);
                blackLadydug->setName("blackLadydug");
                layer->addChild(blackLadydug);
                blackLadydug->setPosition(putPos);
                blackLadydug->setScale(0.2f);
                blackLadydug->SetPoint(Vec2(20.0f, 20.0f));
            }

            // ÂÎÞÐ
            if (obstaclesGid == OBSTACLES::BUDS_R)
            {
                auto buds = Buds::createBuds(OBJ_COLOR::OBJ_RED);
                buds->setName("buds");
                layer->addChild(buds);
                buds->setPosition(putPos);
                buds->setScale(0.5f);
                buds->SetPoint(Vec2(50.0f, 20.0f));
                buds->scheduleUpdate();
            }
            if (obstaclesGid == OBSTACLES::BUDS_G)
            {
                auto buds = Buds::createBuds(OBJ_COLOR::OBJ_GREEN);
                buds->setName("buds");
                layer->addChild(buds);
                buds->setPosition(putPos);
                buds->setScale(0.5f);
                buds->SetPoint(Vec2(50.0f, 20.0f));
                buds->scheduleUpdate();
            }

            // ×ÝÌß
            if (obstaclesGid == OBSTACLES::LAMP)
            {
                auto lamp = Sprite::create("Ornament/lamp.png");
                lamp->setName("lamp");
                layer->addChild(lamp);
                lamp->setScale(0.8f);
                lamp->setAnchorPoint(Point(0.5f, 0.0f));
                lamp->setPosition(Vec2(putPos.x, putPos.y - (chipSize / 2.0f)));
            }

            // –{“–‚Í‚±‚¤‚µ‚½‚¢
            //// •‚¢‚Ä‚ñ‚Æ‚¤’Ž
            //if (obstaclesName == "ladybug_r")
            //{
            //    auto blackLadydug = BlackLadybug::createBlackLadybug(OBJ_COLOR::OBJ_RED);
            //    blackLadydug->setName("blackLadydug");
            //    layer->addChild(blackLadydug);
            //    blackLadydug->setPosition(putPos);
            //    blackLadydug->setScale(0.2f);
            //    blackLadydug->SetPoint(Vec2(20.0f, 20.0f));
            //}
            //if (obstaclesName == "ladybug_g")
            //{
            //    auto blackLadydug = BlackLadybug::createBlackLadybug(OBJ_COLOR::OBJ_GREEN);
            //    blackLadydug->setName("blackLadydug");
            //    layer->addChild(blackLadydug);
            //    blackLadydug->setPosition(putPos);
            //    blackLadydug->setScale(0.2f);
            //    blackLadydug->SetPoint(Vec2(20.0f, 20.0f));
            //}
            //
            //// ÂÎÞÐ
            //if (obstaclesName == "buds_r")
            //{
            //    auto buds = Buds::createBuds(OBJ_COLOR::OBJ_RED);
            //    buds->setName("buds");
            //    layer->addChild(buds);
            //    buds->setPosition(putPos);
            //    buds->setScale(0.5f);
            //    buds->SetPoint(Vec2(50.0f, 20.0f));
            //    buds->scheduleUpdate();
            //}
            //if (obstaclesName == "buds_g")
            //{
            //    auto buds = Buds::createBuds(OBJ_COLOR::OBJ_GREEN);
            //    buds->setName("buds");
            //    layer->addChild(buds);
            //    buds->setPosition(putPos);
            //    buds->setScale(0.5f);
            //    buds->SetPoint(Vec2(50.0f, 20.0f));
            //    buds->scheduleUpdate();
            //}
            //
            //// ×ÝÌß
            //if (obstaclesName == "goalPoint")
            //{
            //    auto lamp = Sprite::create("Ornament/lamp.png");
            //    lamp->setName("lamp");
            //    layer->addChild(lamp);
            //    lamp->setScale(0.8f);
            //    lamp->setAnchorPoint(Point(0.5f, 0.0f));
            //    lamp->setPosition(Vec2(putPos.x, putPos.y - (chipSize / 2.0f)));
            //}
        }
    }
}
