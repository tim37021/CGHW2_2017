#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <GL/glew.h>
#include <string>

enum class WrapMode {
    eRepeat, eMirrorRepeat, eClampToEdge, eClampToBorder
};

enum class FilterMode {
    eNearest, eLinear, eNearestMipmapLinear, eLinearMipmapLinear, eNearestMipmapNearest, eLinearMipmapNearest
};

class ITexture {
public:
    virtual void bindToChannel(GLuint channel)=0;
    virtual void setWrap(WrapMode s, WrapMode t, WrapMode r)=0;
    virtual void setFilter(FilterMode min, FilterMode mag)=0;
    virtual void release()=0;
    virtual bool hasMipmap()=0;
};

class Texture2D : public ITexture {
public:
    static Texture2D LoadFromFile(const std::string &pngfile);

    virtual void bindToChannel(GLuint channel) override;
    virtual void setWrap(WrapMode s, WrapMode t, WrapMode r = WrapMode::eRepeat) override;
    virtual void setFilter(FilterMode min, FilterMode mag) override;
    virtual void release() override;
    virtual bool hasMipmap() override;

private:
    Texture2D(GLuint id);
    GLuint m_id;
    bool m_mipmap;
};

#endif