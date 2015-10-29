#ifndef _UTILS_LAYOUTUTIL_
#define _UTILS_LAYOUTUTIL_
#include "cocos2d.h"

namespace LayoutUtil {
    void layoutLeft(cocos2d::Node* view, cocos2d::Node* target, float offsetX = 0.0, float offsetY = 0.0);
    void layoutRight(cocos2d::Node* view, cocos2d::Node* target, float offsetX = 0.0, float offsetY = 0.0);
    void layoutTop(cocos2d::Node* view, cocos2d::Node* target, float offsetX = 0.0, float offsetY = 0.0);
    void layoutBottom(cocos2d::Node* view, cocos2d::Node* target, float offsetX = 0.0, float offsetY = 0.0);
    void layoutCenter(cocos2d::Node* view, cocos2d::Node* target, float offsetX = 0.0, float offsetY = 0.0);
    void layoutLeftTop(cocos2d::Node* view, cocos2d::Node* target, float offsetX = 0.0, float offsetY = 0.0);
    void layoutRightTop(cocos2d::Node* view, cocos2d::Node* target, float offsetX = 0.0, float offsetY = 0.0);
    void layoutLeftBottom(cocos2d::Node* view, cocos2d::Node* target, float offsetX = 0.0, float offsetY = 0.0);
    void layoutRightBottom(cocos2d::Node* view, cocos2d::Node* target, float offsetX = 0.0, float offsetY = 0.0);

    void layoutParentLeft(cocos2d::Node* view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutParentRight(cocos2d::Node* view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutParentTop(cocos2d::Node* view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutParentBottom(cocos2d::Node* view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutParentCenter(cocos2d::Node* view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutParentLeftTop(cocos2d::Node* view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutParentRightTop(cocos2d::Node* view, float offsetX = 0.0, float offsetY = 0.0);
	void layoutParentLeftCenter(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */);
	void layoutParentRightCenter(cocos2d::Node* view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */);
    void layoutParentLeftBottom(cocos2d::Node* view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutParentRightBottom(cocos2d::Node* view, float offsetX = 0.0, float offsetY = 0.0);
    
    void layoutScreenLeft(cocos2d::Node* view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutScreenRight(cocos2d::Node* view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutScreenTop(cocos2d::Node* view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutScreenBottom(cocos2d::Node* view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutScreenLeftTop(cocos2d::Node* view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutScreenRightTop(cocos2d::Node* view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutScreenLeftBottom(cocos2d::Node* view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutScreenRightBottom(cocos2d::Node* view, float offsetX = 0.0, float offsetY = 0.0);
    
    void layout(cocos2d::Node* view, float viewAlignX, float viewAlignY, cocos2d::Node* target,
                float targetAlignX, float targetAlignY, bool targetIsParent = false, float offsetX = 0.0, float offsetY = 0.0);
    
    void scaleBackground(cocos2d::Node* background);
    
    float getScreenVisibleOffsetX();
    float getScreenVisibleOffsetY();

	void getScaleWidth(float &width);

	void getScaleHigth(float &higth);

	void getScaleLength(float &length);

}

#endif 
