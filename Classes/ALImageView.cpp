//
//  ALImageView.cpp
//  AllenLRS
//
//  Created by Allen on 2017/6/30.
//
//

#include "ALImageView.h"
using namespace network;

using namespace cocos2d::ui;


ALImageView* ALImageView::create(const std::string &imageFileName)
{
    ALImageView *widget = new (std::nothrow) ALImageView;
    if (widget && widget->init(imageFileName, TextureResType::LOCAL))
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}




ALImageView::ALImageView()
{
    loadSprite = nullptr;
    _isaExist = true;
}


ALImageView::~ALImageView()
{
    _isaExist = false;
    CCLOG("~ALImageView()");
}

void ALImageView::loadWebImageTextureWithLoad(const std::string url , const std::string loadImgaPath,const std::string defultImage)
{
    if (url.empty() || url == "nopic" || url.length() < 7 || url.compare(0, 4, "http") != 0)
    {
        if (!defultImage.empty()) {
            this->loadTexture(defultImage);
        }
        return;
    }
    _imageUrl = url;
    Texture2D* texture = Director::getInstance()->getTextureCache()->getTextureForKey(url);
    if (texture) {
//        CCLOG("texture2D 存在");
        SpriteFrame * frame = SpriteFrame::createWithTexture(texture,Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
        this->loadTexture(frame);
    }else{
        if (loadSprite) {
            loadSprite->stopAllActions();
            loadSprite->setVisible(false);
            loadSprite = nullptr;
        }

        loadSprite = Sprite::create(loadImgaPath);
        loadSprite->setVisible(true);
        loadSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
        this->addChild(loadSprite);
        
        loadSprite->runAction(RepeatForever::create(RotateBy::create(0.5, 180)));
        
        this->retain();
        HttpRequest *request = new HttpRequest();
        request->setUrl(url.c_str());
        request->setRequestType(HttpRequest::Type::GET);
        //request->setResponseCallback(CC_CALLBACK_2(ALImageView::onHttpRequestCompleted,this));
        request->setResponseCallback([=](HttpClient* client, HttpResponse* response)->void{
            
            CCLOG("请求图片回调回来---  %s",response->getHttpRequest()->getUrl());
            
            if (response && response->isSucceed()) {
                std::vector<char> *buffer = response->getResponseData();
                //create image
                Image* image = new Image;
                image->initWithImageData((unsigned char*)buffer->data(), buffer->size());
                if (image->getData() != NULL) {
                    CCLOG("图片有数据");
                    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(image, response->getHttpRequest()->getUrl());
                    if (texture) {
                        if (this->getReferenceCount() > 0 && strcmp(response->getHttpRequest()->getUrl(), _imageUrl.c_str()) == 0) {
                            this->loadTexture(SpriteFrame::createWithTexture(texture,Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height)));
                        }
                    }
                }else{
                    CCLOG("图片没数据");
                    this->loadTexture(defultImage);
                }
                image->release();
            }else{
                CCLOG("加载图片失败");
                if (!defultImage.empty()) {
                    this->loadTexture(defultImage);
                }
            }
            if (loadSprite) {
                loadSprite->stopAllActions();
                loadSprite->setVisible(false);
                loadSprite = nullptr;
            }
            
            this->release();
        });
        
        HttpClient::getInstance()->sendImmediate(request);
        request->release();
        
    }
    
}



void ALImageView::loadWebImageTexture(const std::string url , const std::string defultImage)
{
    if (url.empty() || url == "nopic" || url.length() < 7 || url.compare(0, 4, "http") != 0)
    {
        if (!defultImage.empty()) {
            this->loadTexture(defultImage);
        }
        return;
    }
    _imageUrl = url;
    Texture2D* texture = Director::getInstance()->getTextureCache()->getTextureForKey(url);
    if (texture) {
//        CCLOG("texture2D 存在");
        SpriteFrame * frame = SpriteFrame::createWithTexture(texture,Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
        this->loadTexture(frame);
    }else{

        this->retain();
        HttpRequest *request = new HttpRequest();
        request->setUrl(url.c_str());
        request->setRequestType(HttpRequest::Type::GET);
//        request->setResponseCallback(CC_CALLBACK_2(ALImageView::onHttpRequestCompleted,this));
        request->setResponseCallback([=](HttpClient* client, HttpResponse* response)->void{
            if (response && response->isSucceed()) {
                std::vector<char> *buffer = response->getResponseData();
                //create image
                Image* image = new Image;
                image->initWithImageData((unsigned char*)buffer->data(), buffer->size());

                if (image->getData() != NULL) {
                    CCLOG("图片有数据");
                    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(image, response->getHttpRequest()->getUrl());
                    if (texture) {
                        if (this->getReferenceCount() > 0 && strcmp(response->getHttpRequest()->getUrl(), _imageUrl.c_str()) == 0) {
                            this->loadTexture(SpriteFrame::createWithTexture(texture,Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height)));
                        }
                    }
                }else{
                    CCLOG("图片没数据");

                    this->loadTexture(defultImage);
                }
                image->release();
            }else{
                CCLOG("加载图片失败");
                if (!defultImage.empty()) {
                    this->loadTexture(defultImage);
                }
            }
            this->release();
        });
        
        HttpClient::getInstance()->sendImmediate(request);
        request->release();
        
    }

}



//void ALImageView::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
//{
//    if (this && this->getReferenceCount() > 0) {
//        if (response && response->isSucceed()) {
//            std::vector<char> *buffer = response->getResponseData();
//            //create image
//            Image* image = new Image;
//            image->initWithImageData((unsigned char*)buffer->data(), buffer->size());
//
//            if (image->getData() != NULL) {
//                CCLOG("图片有数据");
//                Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(image, response->getHttpRequest()->getUrl());
//                if (texture) {
//                    if (this->getReferenceCount() > 0) {
//
//                        this->loadTexture(SpriteFrame::createWithTexture(texture,Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height)));
//                    }
//                }
//            }else{
//                CCLOG("图片没数据");
//
//            }
//            image->release();
//        }else{
//            CCLOG("加载图片失败");
//        }
//    }
//
//    this->release();
//}

