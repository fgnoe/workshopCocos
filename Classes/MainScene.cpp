#include "MainScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

#define ROBOT_ANIMATION_TAG 2
#define ROBOT_JUMP_ACTION_TAG 3

Scene* MainScene::createScene()
{
    return MainScene::create();
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchEventListener, this);

    this->initRobot();
    this->schedule(schedule_selector(MainScene::checkCollisions));

    return true;
}

void MainScene::checkCollisions(float dt) {
    for (Sprite* obstacle : obstacles) {
        if (robot->getBoundingBox().intersectsRect(obstacle->getBoundingBox())) {
            exit(0);
        }
    }
}

void MainScene::initRobot() {
    robot = Sprite::create("res/sprites/robot_g_1.png");
    robot->setPosition(Director::getInstance()->getVisibleSize()/2);
    //robot->setScale(0.5);
    addChild(robot);
    initRobotAnimation();
}

void MainScene::initRobotAnimation() {
    auto robotAnimation = Animation::create();

    robotAnimation->addSpriteFrameWithFile("res/sprites/robot_g_1.png");
    robotAnimation->addSpriteFrameWithFile("res/sprites/robot_g_2.png");
    robotAnimation->setDelayPerUnit(0.1);

    auto robotAnimate = Animate::create(robotAnimation);
    auto repeatedAnimation = RepeatForever::create(robotAnimate);
    repeatedAnimation->setTag(ROBOT_ANIMATION_TAG);

    robot->runAction(repeatedAnimation);
}

bool MainScene::onTouchBegan(Touch *touch, Event *event) {
    if(robot->getNumberOfRunningActionsByTag(ROBOT_JUMP_ACTION_TAG) == 0) {
        jump();
    }
}

void MainScene::jump() {
    robot->stopActionByTag(ROBOT_ANIMATION_TAG);
    robot->setTexture("res/sprites/robot_g_jump.png");

    auto jumpAction = JumpBy::create(0.5, Vec2::ZERO, 100, 1);
    auto runningAnimationFunctionCaller = CallFunc::create(CC_CALLBACK_0(MainScene::initRobotAnimation, this));
    auto jumpSequence = Sequence::create(jumpAction, runningAnimationFunctionCaller, NULL);

    jumpSequence->setTag(ROBOT_JUMP_ACTION_TAG);
    robot->runAction(jumpSequence);
}