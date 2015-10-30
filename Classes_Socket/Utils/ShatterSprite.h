//
//  ShatterSprite.cpp
//
//  Created by SuperSuRaccoon on 12/8/12.
//
//

#ifndef ShatterSprite_h
#define ShatterSprite_h

#include "cocos2d.h"

NS_CC_BEGIN

//Can change this for what's really used...or really could alloc the memory too
//200 works for a 10x10 grid (with 2 triangles per square)
//128 = 8x8 *2
//98  = 7x7 *2
//64  = 6x6 *2
//50  = 5x5 *2
//32  = 4x4 *2
#define SHATTER_VERTEX_MAX    800

#ifndef DEFTriangleVertices
//Helper things, since it moves the triangles separately
typedef struct _TriangleVertices {
    cocos2d::Vec2        pt1;
    cocos2d::Vec2        pt2;
    cocos2d::Vec2        pt3;
} TriangleVertices;

static inline TriangleVertices tri(cocos2d::Vec2 pt1, cocos2d::Vec2 pt2, cocos2d::Vec2 pt3) {
    TriangleVertices t;
    t.pt1 = pt1; t.pt2 = pt2; t.pt3 = pt3;
    //= {pt1, pt2, pt3 };
    return t;
}

typedef struct _TriangleColors {
    cocos2d::Color4B        c1;
    cocos2d::Color4B        c2;
    cocos2d::Color4B        c3;
} TriangleColors;
#define DEFTriangleVertices
#endif


//Subclass of CCSprite, so all the color & opacity things work by just overriding updateColor, and can use the sprite's texture too.
class ShatterSprite : public cocos2d::Sprite
{
    
public:
    ShatterSprite();
    ~ShatterSprite();
    
    static ShatterSprite* shatterWithSprite(cocos2d::Sprite *sprite, int px, int py, float speedVar, float rotation, bool radial);
    bool initWithSprite(cocos2d::Sprite *sprite, int px, int py, float speedVar, float rotation, bool radial);
    void shatterSprite(cocos2d::Sprite *sprite, int px, int py, float speedVar, float rotation, bool radial);
    void subShatter();
    void shadowedPieces();
    
    // Overrides
	//virtual void draw(Renderer *renderer, const Mat4 &transform, bool transformUpdated) override;
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
    
	void onDraw(const Mat4 &transform, uint32_t transformUpdated);
    void updateColor(void);

	double roundf(double d,unsigned p);
    
    CC_SYNTHESIZE(int, subShatterPercent, cc_subShatterPercent)
    CC_SYNTHESIZE(cocos2d::Vec2, gravity, cc_gravity)
    
private:
    void update(float dt);
    
private:
    TriangleVertices    vertices[SHATTER_VERTEX_MAX];
    TriangleVertices    shadowVertices[SHATTER_VERTEX_MAX];
    TriangleVertices    texCoords[SHATTER_VERTEX_MAX];
    TriangleColors        colorArray[SHATTER_VERTEX_MAX];
    
    float                adelta[SHATTER_VERTEX_MAX];
    cocos2d::Vec2    vdelta[SHATTER_VERTEX_MAX];
    cocos2d::Vec2    centerPt[SHATTER_VERTEX_MAX];
    
    float                shatterSpeedVar, shatterRotVar;
    int                    numVertices;
    //    int                    subShatterPercent;
    bool                radial;
    bool                slowExplosion;
    int                    fallOdds, fallPerSec;
    //  cocos2d::CCPoint    gravity;
    bool                shadowed;
    cocos2d::Texture2D            *shadowTexture;
protected:
	CustomCommand _customCommand;
};

NS_CC_END

#endif



