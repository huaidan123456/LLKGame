//
//  ALHelpTools.cpp
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/8.
//

#include "ALHelpTools.h"
#include <time.h>


USING_NS_CC;

long ALHelpTools::getCurrentTime()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    struct tm *tm;
    time_t timep;
    time(&timep);
    tm = localtime(&timep);
    return tm->tm_sec;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec;
#endif
    
}


/**
 *  设置Node的灰度
 */
void ALHelpTools::setupNodeGray(cocos2d::Node* node)
{
    if (node)
    {
        GLProgram* p = new GLProgram();
        p->initWithFilenames("file/gray.vsh", "file/gray.fsh");
        p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
        p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
        p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
        p->link();
        p->updateUniforms();
        node->setGLProgram(p);
    }
}

/**
 *  移除node的灰色
 */
void ALHelpTools::removeNodeGray(cocos2d::Node* node)
{
    if (node)
    {
        GLProgram * pProgram = ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP);
        node->setGLProgram(pProgram);
        CHECK_GL_ERROR_DEBUG();
    }
}
