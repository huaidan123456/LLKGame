//
//  ALWebImageUtil.cpp
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/4.
//

#include "ALWebImageUtil.h"

#include "network/HttpClient.h"
using namespace network;

void ALWebImageUtil::loadWebImageTexture(const char* url ,const ALCallBack& callback, const char* defultImage)
{
    std::string mUrl = url;
    if (mUrl.empty() || url == "nopic" || mUrl.length() < 7 || mUrl.compare(0, 4, "http") != 0)
    {
        callback(false);
    }
    else{
        Texture2D* texture = Director::getInstance()->getTextureCache()->getTextureForKey(url);
        if (texture) {
            callback(true);
        }
        
        HttpRequest *request = new HttpRequest();
        request->setUrl(mUrl.c_str());
        request->setRequestType(HttpRequest::Type::GET);
        request->setResponseCallback([=](HttpClient* client, HttpResponse* response)->void{
            if (response && response->isSucceed()) {
                std::vector<char> *buffer = response->getResponseData();
                //create image
                Image* image = new Image;
                image->initWithImageData((unsigned char*)buffer->data(), buffer->size());
                if (image->getData() != NULL){
                    CCLOG("图片有数据");
                    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(image, mUrl);
                    callback(true);
                }else{
                    callback(false);
                }
            }else{
                callback(false);
            }
        });
        
    }
}
