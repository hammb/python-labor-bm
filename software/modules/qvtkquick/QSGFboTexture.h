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

#ifndef Q_SG_FBO_TEXTURE_H
#define Q_SG_FBO_TEXTURE_H

#include <QtQuick>

class QSGFboTexture : public QSGTexture
{
    class PrivateData;
    QScopedPointer<PrivateData> d_data;

public:
    QSGFboTexture(QOpenGLFramebufferObject* fbo = nullptr);
    ~QSGFboTexture();

    void setFbo(QOpenGLFramebufferObject* fbo);

    void bind();

    bool hasAlphaChannel() const;
    bool hasMipmaps() const;

    int textureId() const;
    QSize textureSize() const;

};


#endif
