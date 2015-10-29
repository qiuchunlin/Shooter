#include "ShatterSprite.h"

USING_NS_CC;

#ifndef RANDF
//A helper to do float random numbers in a range around a base value.
float randf(float base, float range) {
    if (range==0) return base;
    long        lRange = rand()%(int)((range*2)*10000);
    float        fRange = ((float)lRange/10000.0) - range;
    return    base + fRange;
}
#endif

ShatterSprite::ShatterSprite()
{
    shatterSpeedVar = 0;
    shatterRotVar = 0;
    numVertices = 0;
    radial = false;
    slowExplosion = false;
    fallOdds = 0;
    fallPerSec = 0;
    shadowed = false;
    shadowTexture = NULL;
    subShatterPercent = 0;
    gravity = Vec2(0,0);
    
    for(int i=0; i<SHATTER_VERTEX_MAX; i++)
    {
        vertices[i].pt1 = Vec2(0,0);
        vertices[i].pt2 = Vec2(0,0);
        vertices[i].pt3 = Vec2(0,0);
        
        shadowVertices[i].pt1 = Vec2(0,0);
        shadowVertices[i].pt2 = Vec2(0,0);
        shadowVertices[i].pt3 = Vec2(0,0);
        
        texCoords[i].pt1 = Vec2(0,0);
        texCoords[i].pt2 = Vec2(0,0);
        texCoords[i].pt3 = Vec2(0,0);
        
        colorArray[i].c1 = Color4B(0, 0, 0, 0);
        colorArray[i].c2 = Color4B(0, 0, 0, 0);
        colorArray[i].c3 = Color4B(0, 0, 0, 0);
        
        adelta[i] = 0;
        vdelta[i] = Vec2(0,0);
        centerPt[i] = Vec2(0,0);
    }
}

ShatterSprite::~ShatterSprite()
{
    CC_SAFE_RELEASE(shadowTexture);
}

ShatterSprite* ShatterSprite::shatterWithSprite(cocos2d::Sprite *sprite, int piecesX, int piecesY, float speedVar, float rotVar, bool radialIn)
{
    ShatterSprite *pRet = new ShatterSprite;
    pRet->initWithSprite(sprite, piecesX, piecesY, speedVar, rotVar, radialIn);
    pRet->autorelease();
    return pRet;
}

bool ShatterSprite::initWithSprite(cocos2d::Sprite *sprite, int piecesX, int piecesY, float speedVar, float rotVar, bool radialIn)
{
    if(Sprite::init())
    {
        // Initialization code here.
        slowExplosion = false;
        radial = radialIn;
        shatterSprite(sprite, piecesX, piecesY, speedVar, rotVar, radialIn);
        subShatterPercent = 0;
        scheduleUpdate();
        return true;
    }
    return false;
}

void ShatterSprite::update(float dt)
{
    //Note, does NOT adjust vdelta and adelta for slow frames;
    //To do that, need some d=(delta*60.0) that's multiplied by the vdelta and adelta
    for (int i = 0; i<numVertices; i++) {
        vertices[i].pt1 = vertices[i].pt1 + vdelta[i];
        vertices[i].pt2 = vertices[i].pt2 + vdelta[i];
        vertices[i].pt3 = vertices[i].pt3 + vdelta[i];
        centerPt[i] = centerPt[i] + vdelta[i];
        
		vertices[i].pt1 = vertices[i].pt1.rotateByAngle(centerPt[i], adelta[i]);
		vertices[i].pt2 = vertices[i].pt2.rotateByAngle(centerPt[i], adelta[i]);
		vertices[i].pt3 = vertices[i].pt3.rotateByAngle(centerPt[i], adelta[i]);
        
        if (slowExplosion) {
            if (adelta[i]==0 && vdelta[i].x==0 && vdelta[i].y==0 && rand()%fallOdds==0) {
                //Increases the odds each time
                if (fallOdds>fallPerSec) fallOdds -= fallPerSec;
                vdelta[i] = Vec2(randf(0.0, shatterSpeedVar), randf(0.0, shatterSpeedVar));
                adelta[i] = randf(0.0, shatterRotVar);
            }
            if (vdelta[i].x!=0 || vdelta[i].y!=0) {
                vdelta[i] = vdelta[i] + gravity;
            }
        }
    }
    if (rand()%100<subShatterPercent)
        subShatter();
}

void ShatterSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(ShatterSprite::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void ShatterSprite::onDraw(const Mat4 &transform, uint32_t transformUpdated){
	auto glProgram = getGLProgram();
    glProgram->use();
    glProgram->setUniformsForBuiltins(transform);

    GL::blendFunc(_blendFunc.src, _blendFunc.dst);

    CC_NODE_DRAW_SETUP();

    glBindTexture(GL_TEXTURE_2D, getTexture()->getName());
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, colorArray);
//	glVertexPointer(2, GL_FLOAT, 0, vertices);
//	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
//	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colorArray);
	glDrawArrays(GL_TRIANGLES, 0, numVertices*3);

}


void ShatterSprite::updateColor()
{
    //Update the color array...
    //Color4B color4 = Color4B(m_sColor.r, m_sColor.g, m_sColor.b, m_nOpacity);
	Color4B color4 = Color4B(getColor().r, getColor().g, getColor().b, getOpacity());
    TriangleColors    triColor4 = { color4, color4, color4 };
    for (int i=0; i<numVertices; i++) {
        colorArray[i] = triColor4;
    }
}

void ShatterSprite::shadowedPieces()
{
    RenderTexture *tex = RenderTexture::create(64, 64); //[CCRenderTexture renderTextureWithWidth:64 height:64];
    shadowed = true;
    
    tex->beginWithClear(0, 0, 0, 0.25);
    tex->end();
    shadowTexture = tex->getSprite()->getTexture();
    shadowTexture->retain();
}

void ShatterSprite::subShatter()
{
    int i = rand()%numVertices;
    if (numVertices>=SHATTER_VERTEX_MAX-1) return;
    TriangleVertices    v1 = vertices[i];
    TriangleVertices    t1 = texCoords[i];
    
    if (rand()%100<75) {
        //Split along the LONGEST edge most of the time
        float    d12 = ccpLengthSQ(ccpSub(v1.pt1, v1.pt2));
        float    d23 = ccpLengthSQ(ccpSub(v1.pt2, v1.pt3));
        float    d31 = ccpLengthSQ(ccpSub(v1.pt3, v1.pt1));
        if (d12>d23 && d12>d31) {
            v1 = tri(v1.pt2, v1.pt3, v1.pt1);
            t1 = tri(t1.pt2, t1.pt3, t1.pt1);
        } else if (d23>d12 && d23>d31) {
            v1 = tri(v1.pt3, v1.pt1, v1.pt2);
            t1 = tri(t1.pt3, t1.pt1, t1.pt2);
        }
    } else {
        //ROTATES the vertex and texture things, to do along a random axis sometimes
        while (rand()%3==0) {
            v1 = tri(v1.pt2, v1.pt3, v1.pt1);
            t1 = tri(t1.pt2, t1.pt3, t1.pt1);
        }
    }
    
    //Update the original one.
    vertices[i] = tri(ccpMidpoint(v1.pt1, v1.pt3), v1.pt2, v1.pt3);
    
    centerPt[i] = Vec2((vertices[i].pt1.x + vertices[i].pt2.x + vertices[i].pt3.x)/3.0,
                   (vertices[i].pt1.y + vertices[i].pt2.y + vertices[i].pt3.y)/3.0);
    
    texCoords[i] = tri(ccpMidpoint(t1.pt1, t1.pt3), t1.pt2, t1.pt3);
    
    //Shattering again changes it's rotation & direction
    Vec2    originalVDelta = vdelta[i];
    if (radial) {
        vdelta[i] = Vec2(randf(originalVDelta.x, shatterSpeedVar/4.0), randf(originalVDelta.y, shatterSpeedVar/4.0));
    } else {
        vdelta[i] = Vec2(randf(0.0, shatterSpeedVar), randf(0.0, shatterSpeedVar));
    }
    adelta[i] = randf(0.0, shatterRotVar);
    
    //Shift up to insert the new one in the next spot.
    //So overlapping things look right--ones behind break and don't jump forward.
    numVertices++;
    for (int j=numVertices-1; j>i+1; j--) {
        vdelta[j] = vdelta[j-1];
        adelta[j] = adelta[j-1];
        colorArray[j] = colorArray[j-1];
        
        vertices[j] = vertices[j-1];
        centerPt[j] = centerPt[j-1];
        texCoords[j] = texCoords[j-1];
    }
    
    //And add the new other half...
    vertices[i+1] = tri(v1.pt1, v1.pt2, ccpMidpoint(v1.pt1, v1.pt3));
    
    centerPt[i+1] = Vec2((vertices[i+1].pt1.x + vertices[i+1].pt2.x + vertices[i+1].pt3.x)/3.0,
                        (vertices[i+1].pt1.y + vertices[i+1].pt2.y + vertices[i+1].pt3.y)/3.0);
    
    texCoords[i+1] = tri(t1.pt1, t1.pt2, ccpMidpoint(t1.pt1, t1.pt3));
    
    if (radial) {
        vdelta[i+1] = Vec2(randf(originalVDelta.x, shatterSpeedVar/4.0), randf(originalVDelta.y, shatterSpeedVar/4.0));
        
    } else {
        vdelta[i+1] = Vec2(randf(0.0, shatterSpeedVar), randf(0.0, shatterSpeedVar));
    }
    
    adelta[i+1] = randf(0.0, shatterRotVar);
    
    colorArray[i+1] = colorArray[i];
    
    //Copy for Shadows
    if (shadowed) {
        for (int j=0; j<numVertices; j++) {
            shadowVertices[j] = tri(ccpAdd(vertices[j].pt1, Vec2(5, -5)), ccpAdd(vertices[j].pt2, Vec2(5, -5)), ccpAdd(vertices[j].pt3, Vec2(5, -5)));
        }
    }
}

double ShatterSprite::roundf(double d,unsigned p)
{
    if(d==0.0)
        return 0.0;
    double n=10;
    for(;p>0;p--)
        n*=10;
    if(d>0.0)
        return (long)(d*n+5) / 10 / (n/10);
    else
        return (long)(d*n-5) / 10 / (n/10);
}

void ShatterSprite::shatterSprite(cocos2d::Sprite *sprite, int piecesX, int piecesY, float speedVar, float rotVar, bool radialIn)
{
    //Do rendertexture to make a whole new texture, so not part of the textureCache
    RenderTexture *rt = RenderTexture::create(sprite->getContentSize().width, sprite->getContentSize().height);
    rt->begin();
    Sprite *s2 = Sprite::createWithTexture(sprite->getTexture(), sprite->getTextureRect());
    s2->setPosition(Vec2(sprite->getContentSize().width/2, sprite->getContentSize().height/2));
    s2->visit();
    rt->end();
    
    //Uses the Sprite's texture to reuse things.
    setTexture(rt->getSprite()->getTexture());
    setTextureRect(CCRectMake(0, 0, sprite->getContentSize().width, sprite->getContentSize().height));
    
    //Sizey thingys
    float wid = sprite->getTexture()->getContentSizeInPixels().width;
    float hgt = sprite->getTexture()->getContentSizeInPixels().height;
    float    pieceXsize = (wid/(float)piecesX);//*CC_CONTENT_SCALE_FACTOR();
    float    pieceYsize = (hgt/(float)piecesY);//*CC_CONTENT_SCALE_FACTOR();
    
    //Texture is padded out to a power of 2!!
    float texWid = (wid/getTexture()->getPixelsWide());
    float texHgt = (hgt/getTexture()->getPixelsHigh());
    Vec2 centerPoint = Vec2(wid/2, hgt/2);
    Color4B        color4 = Color4B(getColor().r, getColor().g, getColor().b, getOpacity());
    TriangleColors    triColor4 = { color4, color4, color4 };
    
    shatterSpeedVar = speedVar;
    shatterRotVar = rotVar;
    
    //Build the points first, so they can be wobbled a bit to look more random...
   // CCPoint    ptArray[piecesX+1][piecesY+1];
    Vec2    ptArray[20+1][20+1];
    for (int x=0; x<=piecesX; x++) {
        for (int y=0; y<=piecesY; y++) {
            Vec2            pt = CCPointMake((x*pieceXsize), (y*pieceYsize));
            //Edge pieces aren't wobbled, just interior.
            if (x>0 && x<piecesX && y>0 && y<piecesY) {
				pt = ccpAdd(pt, Vec2(CCRANDOM_MINUS1_1()*pieceXsize*0.25, CCRANDOM_MINUS1_1()*pieceYsize*0.25));
            }
            ptArray[x][y] = pt;
           // CCLog("px:%f, py:%f\n", pt.x, pt.y);
        }
    }





    numVertices = 0;
    for (int x=0; x<piecesX; x++) {
        for (int y=0; y<piecesY; y++) {
            if (numVertices>=SHATTER_VERTEX_MAX) {
               // CCLog("NeedABiggerArray!");
                return;
            }
            
            //Direction (v) and rotation (a) are done by triangle too.
            //CenterPoint is for rotating each triangle
            //vdelta is random, but could be done based on distance/direction from the center of the image to explode out...
            if (slowExplosion) {
                vdelta[numVertices] = Vec2::ZERO;
                adelta[numVertices] = 0.0;
            } else {
                vdelta[numVertices] = Vec2(randf(0.0, speedVar), randf(0.0, speedVar));
                adelta[numVertices] = randf(0.0, rotVar);
            }
            colorArray[numVertices] = triColor4;
            
            if (slowExplosion) {
				vdelta[numVertices + 1] = Vec2::ZERO;
                adelta[numVertices+1] = 0.0;
            } else {
                vdelta[numVertices+1] = Vec2(randf(0.0, speedVar), randf(0.0, speedVar));
                adelta[numVertices+1] = randf(0.0, rotVar);
            }
            colorArray[numVertices+1] = triColor4;
            
            //Randomly do the diagonal for the triangle
            if (rand()%2==0) {
                vertices[numVertices] = tri(ptArray[x][y],
                                            ptArray[x+1][y],
                                            ptArray[x][y+1]);
                centerPt[numVertices] = Vec2((vertices[numVertices].pt1.x + vertices[numVertices].pt2.x + vertices[numVertices].pt3.x)/3.0,
                                            (vertices[numVertices].pt1.y + vertices[numVertices].pt2.y + vertices[numVertices].pt3.y)/3.0);
                texCoords[numVertices] = tri(Vec2((ptArray[x][y].x/wid)*texWid, (ptArray[x][y].y/hgt)*texHgt),
                                             Vec2((ptArray[x+1][y].x/wid)*texWid, (ptArray[x+1][y].y/hgt)*texHgt),
                                             Vec2((ptArray[x][y+1].x/wid)*texWid, (ptArray[x][y+1].y/hgt)*texHgt));
                if (radialIn) {
                    vdelta[numVertices] = Vec2((centerPt[numVertices].x - centerPoint.x)/(wid/2.0) * speedVar, (centerPt[numVertices].y - centerPoint.y)/(hgt/2.0) * speedVar);
                }
                numVertices++;
                
                //Triangle #2
                vertices[numVertices] = tri(ptArray[x+1][y],
                                            ptArray[x+1][y+1],
                                            ptArray[x][y+1]);
                centerPt[numVertices] = Vec2((vertices[numVertices].pt1.x + vertices[numVertices].pt2.x + vertices[numVertices].pt3.x)/3.0,
                                            (vertices[numVertices].pt1.y + vertices[numVertices].pt2.y + vertices[numVertices].pt3.y)/3.0);
                texCoords[numVertices] = tri(Vec2((ptArray[x+1][y].x/wid)*texWid, (ptArray[x+1][y].y/hgt)*texHgt),
                                             Vec2((ptArray[x+1][y+1].x/wid)*texWid, (ptArray[x+1][y+1].y/hgt)*texHgt),
                                             Vec2((ptArray[x][y+1].x/wid)*texWid, (ptArray[x][y+1].y/hgt)*texHgt));
                if (radialIn) {
                    vdelta[numVertices] = Vec2((centerPt[numVertices].x - centerPoint.x)/(wid/2.0) * speedVar, (centerPt[numVertices].y - centerPoint.y)/(hgt/2.0) * speedVar);
                }
                numVertices++;
            } else {
                vertices[numVertices] = tri(ptArray[x][y],
                                            ptArray[x+1][y+1],
                                            ptArray[x][y+1]);
                centerPt[numVertices] = Vec2((vertices[numVertices].pt1.x + vertices[numVertices].pt2.x + vertices[numVertices].pt3.x)/3.0,
                                            (vertices[numVertices].pt1.y + vertices[numVertices].pt2.y + vertices[numVertices].pt3.y)/3.0);
                texCoords[numVertices] = tri(Vec2((ptArray[x][y].x/wid)*texWid, (ptArray[x][y].y/hgt)*texHgt),
                                             Vec2((ptArray[x+1][y+1].x/wid)*texWid, (ptArray[x+1][y+1].y/hgt)*texHgt),
                                             Vec2((ptArray[x][y+1].x/wid)*texWid, (ptArray[x][y+1].y/hgt)*texHgt));
                if (radialIn) {
                    vdelta[numVertices] = Vec2((centerPt[numVertices].x - centerPoint.x)/(wid/2.0) * speedVar, (centerPt[numVertices].y - centerPoint.y)/(hgt/2.0) * speedVar);
                }
                numVertices++;
                
                //Triangle #2
                vertices[numVertices] = tri(ptArray[x][y],
                                            ptArray[x+1][y],
                                            ptArray[x+1][y+1]);
                centerPt[numVertices] = Vec2((vertices[numVertices].pt1.x + vertices[numVertices].pt2.x + vertices[numVertices].pt3.x)/3.0,
                                            (vertices[numVertices].pt1.y + vertices[numVertices].pt2.y + vertices[numVertices].pt3.y)/3.0);
                texCoords[numVertices] = tri(Vec2((ptArray[x][y].x/wid)*texWid, (ptArray[x][y].y/hgt)*texHgt),
                                             Vec2((ptArray[x+1][y].x/wid)*texWid, (ptArray[x+1][y].y/hgt)*texHgt),
                                             Vec2((ptArray[x+1][y+1].x/wid)*texWid, (ptArray[x+1][y+1].y/hgt)*texHgt));
                if (radialIn) {
                    vdelta[numVertices] = Vec2((centerPt[numVertices].x - centerPoint.x)/(wid/2.0) * speedVar, (centerPt[numVertices].y - centerPoint.y)/(hgt/2.0) * speedVar);
                }
                numVertices++;
            }
        }
    }
    
    //Copy for Shadows
    for (int j=0; j<numVertices; j++) {
        shadowVertices[j] = tri(ccpAdd(vertices[j].pt1, Vec2(5, -5)), ccpAdd(vertices[j].pt2, Vec2(5, -5)), ccpAdd(vertices[j].pt3, Vec2(5, -5)));
    }
}