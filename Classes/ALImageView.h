//
//  ALImageView.h
//  AllenLRS
//
//  Created by Allen on 2017/6/30.
//
//

#ifndef ALImageView_h
#define ALImageView_h

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "network/HttpClient.h"

#define alImage_defult "images/definedHead.png"

USING_NS_CC;
class ALImageView : public cocos2d::ui::ImageView
{
public:
    static ALImageView* create(const std::string &imageFileName);
    
    ALImageView();
    ~ALImageView();
    
    
    void loadWebImageTextureWithLoad(const std::string url , const std::string loadImgaPath,const std::string defultImage = alImage_defult);
    
    void loadWebImageTexture(const std::string url , const std::string defultImage = alImage_defult);
    
//    void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
private:
    Sprite* loadSprite;
    
    bool _isaExist;
    
    
    
    std::string _defultImagePath;

};



#endif /* ALImageView_h */
