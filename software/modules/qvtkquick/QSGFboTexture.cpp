///\file QSGFboTexture.h
///\brief QSGFboTexture - QSGTexture subclass that creates a scene graph texture from a fbo texture.
///  
///QSGTexture subclass that creates a scene graph texture from a fbo texture.
///
///\par Projekt : VTK/QML Interaction
///
///\par Company:   Mannheim University of Applied Sciences\n
///                emb-lab\n
///                Adress:     Paul-Wittsack-Strasse 10, D-68163 Mannheim, Germany\n
///                eMail:      \send zimmermann.emb@googlemail.com\n
///
///\author         Timo Zimmermann

#include "QSGFboTexture.h"

class QSGFboTexture::PrivateData
{
public:
    QOpenGLFramebufferObject* fbo;
    bool firstTimeBound;
};

QSGFboTexture::QSGFboTexture(QOpenGLFramebufferObject* fbo) : QSGTexture()
    ,d_data(new PrivateData())
{
    d_data->fbo = fbo;
    d_data->firstTimeBound = true;
}

QSGFboTexture::~QSGFboTexture()
{

}

void QSGFboTexture::setFbo(QOpenGLFramebufferObject* fbo)
{
    if(d_data->fbo == fbo)
        return;

    d_data->fbo = fbo;
    d_data->firstTimeBound = true;
}

void QSGFboTexture::bind()
{
    if(d_data->fbo == nullptr)
        return;

    auto fboTexture = textureId();
    glBindTexture(GL_TEXTURE_2D, fboTexture);

    if(d_data->firstTimeBound) {
        updateBindOptions(true);
        d_data->firstTimeBound = false;
    }
}

bool QSGFboTexture::hasAlphaChannel() const
{
    return true;
}

bool QSGFboTexture::hasMipmaps() const
{
    if(d_data->fbo == nullptr)
        return false;

    return d_data->fbo->format().mipmap();
}

int QSGFboTexture::textureId() const
{
    if(d_data->fbo == nullptr)
        return 0;

    return d_data->fbo->texture();
}

QSize QSGFboTexture::textureSize() const
{
    if(d_data->fbo == nullptr)
        return QSize();

    return d_data->fbo->size();
}
