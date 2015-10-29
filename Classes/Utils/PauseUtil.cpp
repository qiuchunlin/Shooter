#include <iostream>
#include "PauseUtil.h"
using namespace cocos2d;

void PauseUtil::pauseNode(cocos2d::Node *node, bool bIsEveryNode)
{
   CCAssert(node, "null node to pause");
   node->pause();
   Vector<Node*> vChildren = node->getChildren();
   for(auto &pNode:vChildren)
   {
	   if (bIsEveryNode)
	   {
		   PauseUtil::pauseNode(pNode);
	   }
	   else
	   {
		   pNode->pause();
	   }
	   
   }
}

void PauseUtil::resumeNode(cocos2d::Node *node, bool bIsEveryNode)
{
	CCAssert(node, "null node to pause");
	node->resume();
	Vector<Node*> vChildren = node->getChildren();
	for(auto& pNode:vChildren)
	{
		if (bIsEveryNode)
		{
			PauseUtil::resumeNode(pNode);
		}
		else
		{
			pNode->resume();
		}
	}
}