#include "LayoutUtil.h"
#include "../Common/Constants.h"
using namespace cocos2d;

void LayoutUtil::layoutLeft(cocos2d::Node* view, cocos2d::Node* target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 1, 0.5, target, 0, 0.5, false, offsetX, offsetY);
}

void LayoutUtil::layoutRight(cocos2d::Node* view, cocos2d::Node* target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0, 0.5, target, 1, 0.5, false, offsetX, offsetY);
}

void LayoutUtil::layoutTop(cocos2d::Node* view, cocos2d::Node* target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0.5, 0, target, 0.5, 1, false, offsetX, offsetY);
}

void LayoutUtil::layoutBottom(cocos2d::Node* view, cocos2d::Node* target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0.5, 1, target, 0.5, 0, false, offsetX, offsetY);
}

void LayoutUtil::layoutCenter(cocos2d::Node* view, cocos2d::Node* target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0.5, 0.5, target, 0.5, 0.5, false, offsetX, offsetY);
}

void LayoutUtil::layoutLeftTop(cocos2d::Node* view, cocos2d::Node* target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 1, 1, target, 0, 1, false, offsetX, offsetY);
}

void LayoutUtil::layoutRightTop(cocos2d::Node* view, cocos2d::Node* target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0, 1, target, 1, 1, false, offsetX, offsetY);
}

void LayoutUtil::layoutLeftBottom(cocos2d::Node* view, cocos2d::Node* target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 1, 0, target, 0, 0, false, offsetX, offsetY);
}

void LayoutUtil::layoutRightBottom(cocos2d::Node* view, cocos2d::Node* target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0, 0, target, 1, 0, false, offsetX, offsetY);
}

void LayoutUtil::layoutParentLeft(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0, 0.5, view->getParent(), 0, 0.5, true, offsetX, offsetY);
}

void LayoutUtil::layoutParentRight(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 1, 0.5, view->getParent(), 1, 0.5, true, offsetX, offsetY);
}

void LayoutUtil::layoutParentTop(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0.5, 1, view->getParent(), 0.5, 1, true, offsetX, offsetY);
}

void LayoutUtil::layoutParentBottom(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0.5, 0, view->getParent(), 0.5, 0, true, offsetX, offsetY);
}

void LayoutUtil::layoutParentCenter(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0.5, 0.5, view->getParent(), 0.5, 0.5, true, offsetX, offsetY);
}

void LayoutUtil::layoutParentLeftTop(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0, 1, view->getParent(), 0, 1, true, offsetX, offsetY);
}

void LayoutUtil::layoutParentRightTop(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 1, 1, view->getParent(), 1, 1, true, offsetX, offsetY);
}

void LayoutUtil::layoutParentLeftCenter(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
	layout(view, 0, 0.5, view->getParent(), 0, 0.5, true, offsetX, offsetY);
}

void LayoutUtil::layoutParentRightCenter(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
	layout(view, 1, 0.5, view->getParent(), 1, 0.5, true, offsetX, offsetY);
}


void LayoutUtil::layoutParentLeftBottom(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0, 0, view->getParent(), 0, 0, true, offsetX, offsetY);
}

void LayoutUtil::layoutParentRightBottom(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 1, 0, view->getParent(), 1, 0, true, offsetX, offsetY);
}

void LayoutUtil::layoutScreenLeft(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0, 0.5, view->getParent(), 0, 0.5, true, getScreenVisibleOffsetX() + offsetX, 0 + offsetY);
}

void LayoutUtil::layoutScreenRight(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 1, 0.5, view->getParent(), 1, 0.5, true, -getScreenVisibleOffsetX() + offsetX, 0 + offsetY);
}

void LayoutUtil::layoutScreenTop(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0.5, 1, view->getParent(), 0.5, 1, true, 0 + offsetX, -getScreenVisibleOffsetY() + offsetY);
}

void LayoutUtil::layoutScreenBottom(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0.5, 0, view->getParent(), 0.5, 0, true, 0 + offsetX, getScreenVisibleOffsetY() + offsetY);
}

void LayoutUtil::layoutScreenLeftTop(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0, 1, view->getParent(), 0, 1, true, getScreenVisibleOffsetX() + offsetX, -getScreenVisibleOffsetY() + offsetY);
}

void LayoutUtil::layoutScreenRightTop(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 1, 1, view->getParent(), 1, 1, true, -getScreenVisibleOffsetX() + offsetX, -getScreenVisibleOffsetY() + offsetY);
}

void LayoutUtil::layoutScreenLeftBottom(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0, 0, view->getParent(), 0, 0, true, getScreenVisibleOffsetX() + offsetX, getScreenVisibleOffsetY() + offsetY);
}

void LayoutUtil::layoutScreenRightBottom(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 1, 0, view->getParent(), 1, 0, true, -getScreenVisibleOffsetX() + offsetX, getScreenVisibleOffsetY() + offsetY);
}

void LayoutUtil::layout(cocos2d::Node* src, float srcAlignX, float srcAlignY, cocos2d::Node* target,
                        float targetAlignX, float targetAlignY, bool targetIsParent /* = false */, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    const Vec2& srcAnchorPoint = src->getAnchorPoint();
    const Vec2& anchorPointDiff = Vec2(srcAlignX - srcAnchorPoint.x, srcAlignY - srcAnchorPoint.y);
    const Vec2& targetAnchorPoint = target->getAnchorPoint();
    float targetAlignXPosition,targetAlignYPosition;
    if (targetIsParent) {
        targetAlignXPosition = target->getContentSize().width * targetAlignX;
        targetAlignYPosition = target->getContentSize().height * targetAlignY;
    } else {
        targetAlignXPosition = target->getPositionX() + target->getBoundingBox().size.width * (targetAlignX - targetAnchorPoint.x);
        targetAlignYPosition = target->getPositionY() + target->getBoundingBox().size.height * (targetAlignY - targetAnchorPoint.y);
    }
    src->setPosition(Vec2(targetAlignXPosition - anchorPointDiff.x * src->getBoundingBox().size.width + offsetX,
                         targetAlignYPosition - anchorPointDiff.y * src->getBoundingBox().size.height + offsetY));

}

// design UI size is 1136*768
// for memory saving, the background image is scaled to 1024*692
// this function is for restoring the designed background size
void LayoutUtil::scaleBackground(cocos2d::Node* background) {
	 background->setScaleX(Director::getInstance()->getOpenGLView()->getVisibleSize().width/Constants::DESIGN_WIDTH);
}

float LayoutUtil::getScreenVisibleOffsetX() {
    return (Constants::DESIGN_WIDTH - Director::getInstance()->getOpenGLView()->getVisibleSize().width) / 2.0f;
}

float LayoutUtil::getScreenVisibleOffsetY() {
    return (Constants::DESIGN_HEIGHT - Director::getInstance()->getOpenGLView()->getVisibleSize().height) / 2.0f;
}

void LayoutUtil::getScaleWidth(float &width)
{
	Size visibleSize = Director::getInstance()->getOpenGLView()->getVisibleSize();
	Size designSize = Size(Constants::DESIGN_WIDTH,Constants::DESIGN_HEIGHT);

	float fScale = (visibleSize.width/visibleSize.height)/(designSize.width/designSize.height);

	if (fScale < 1)
	{
		width *=fScale;
	}
	
}

void LayoutUtil::getScaleHigth(float &higth)
{

}

void LayoutUtil::getScaleLength(float &length)
{
	Size visibleSize = Director::getInstance()->getOpenGLView()->getVisibleSize();
	Size designSize = Size(Constants::DESIGN_WIDTH,Constants::DESIGN_HEIGHT);

	float fScale = (visibleSize.width/visibleSize.height)/(designSize.width/designSize.height);

	if (fScale < 1)
	{
		length *=fScale;
	}
	else if(fScale > 1)
	{
		length /=fScale;
	}

}
