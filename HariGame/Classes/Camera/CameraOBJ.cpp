#include "CameraOBJ.h"

USING_NS_CC;

bool CameraOBJ::operator()(cocos2d::CameraFlag flag, Scene* scene)
{
	auto defaultCamera = Camera::getDefaultCamera();
	auto screenSize = Director::getInstance()->getWinSize();
	auto camera = Camera::createOrthographic(screenSize.width, screenSize.height , defaultCamera->getNearPlane(), defaultCamera->getFarPlane());
	camera->setPosition3D(Vec3(0.0f, 0.0f, 10.0f));
	camera->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	camera->setDepth(0.0f);
	camera->setCameraFlag(flag);
	scene->addChild(camera);
	return true;
}
