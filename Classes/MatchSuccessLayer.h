//
//  MatchSuccessLayer.h
//  LLKGame
//
//  Created by Allen on 2017/12/15.
//
//

#ifndef MatchSuccessLayer_h
#define MatchSuccessLayer_h

#include "ALBaseLayer.h"
#include "ui/CocosGUI.h"


class ALImageView;
class ALUserInfoModel;
class MatchSuccessLayer:public ALBaseLayer
{
public:
    typedef std::function<void ()> ALMatchSuccessCallBack;
    

    static MatchSuccessLayer* create();
    
    MatchSuccessLayer();
    ~MatchSuccessLayer();
    
    void initUI();
    
    virtual void layerWillAppear() override;
    
    virtual void layerDidDisappear() override;
    
    
    void setupMatchUserInfos(ALUserInfoModel* model1,ALUserInfoModel* model2);
    
    void showLayerWithMatchSuccess(const ALMatchSuccessCallBack& callback);
    
    
    
private:
    Node* baseNode;
    
    Node* headNode_1;
    Node* headNode_2;
    
    ALImageView* headView_1;
    ALImageView* headView_2;
    
    
    cocos2d::ui::Text* nameText_1;
    cocos2d::ui::Text* nameText_2;
    
    
};
#endif /* MatchSuccessLayer_h */
