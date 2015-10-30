#ifndef _UTILS_PAUSEUTIL_H_
#define _UTILS_PAUSEUTIL_H_

#include "cocos2d.h"

class PauseUtil {
public:
    static void pauseNode(cocos2d::Node* node,bool bIsEveryNode = true);
	static void resumeNode(cocos2d::Node* node, bool bIsEveryNode = true);
};

#endif
