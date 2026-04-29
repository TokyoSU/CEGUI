/***********************************************************************
    created:    Fri Jan 23 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#ifndef _CEGUIOpenGL_h_
#define _CEGUIOpenGL_h_

#include "CEGUI/Config.h"

#if defined CEGUI_USE_EPOXY
#include <epoxy/gl.h>
#elif defined CEGUI_USE_GLAD
 // Use glad loader
#include <glad/glad.h>
#if defined(_WIN32) || defined(__WIN32__)
#include <glad/glad_wgl.h>
#endif
#elif defined CEGUI_USE_GLEW
#include <GL/glew.h>
#include <GL/wglew.h>
 // When using GLEW, there's no need to "#include" the OpenGL headers.
#ifndef __APPLE__
# if (defined( __WIN32__ ) || defined( _WIN32 ))
# include <windows.h>
# endif
# include <GL/glu.h>
#else
# include <OpenGL/glu.h>
#endif
#pragma comment(lib, "glu32.lib")
#else
#error Either "CEGUI_USE_EPOXY", "CEGUI_USE_GLAD" or "CEGUI_USE_GLEW" must be defined. Defining multiple or none is invalid.
#endif

#if defined CEGUI_USE_GLEW
#define EXT_framebuffer_object GLEW_EXT_framebuffer_object
#define EXT_blend_func_separate GLEW_EXT_blend_func_separate
#define EXT_ARB_multitexture GLEW_ARB_multitexture
#define WARB_pbuffer WGLEW_ARB_pbuffer
#define GLLIB_VERSION_1_3 GLEW_VERSION_1_3
#define GLLIB_VERSION_1_4 GLEW_VERSION_1_4
#elif defined CEGUI_USE_GLAD
#define EXT_framebuffer_object GLAD_GL_EXT_framebuffer_object
#define EXT_blend_func_separate GLAD_GL_EXT_blend_func_separate
#define EXT_ARB_multitexture GLAD_GL_ARB_multitexture
#define WARB_pbuffer GLAD_WGL_ARB_pbuffer
#define GLLIB_VERSION_1_3 GLAD_GL_VERSION_1_3
#define GLLIB_VERSION_1_4 GLAD_GL_VERSION_1_4
#elif defined CEGUI_USE_EPOXY
#define EXT_framebuffer_object epoxy_gl_EXT_framebuffer_object
#define EXT_blend_func_separate epoxy_gl_EXT_blend_func_separate
#define EXT_ARB_multitexture epoxy_gl_ARB_multitexture
#define WARB_pbuffer epoxy_wgl_WGL_ARB_pbuffer
#define GLLIB_VERSION_1_3 epoxy_gl_VERSION_1_3
#define GLLIB_VERSION_1_4 epoxy_gl_VERSION_1_4
#endif

#ifndef GL_RGB565
#define GL_RGB565 0x8D62
#endif

#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(CEGUI_STATIC)
# if defined(CEGUIOPENGLRENDERER_EXPORTS) || defined(CEGUIOPENGLES2RENDERER_EXPORTS)
# define OPENGL_GUIRENDERER_API __declspec(dllexport)
# else
# define OPENGL_GUIRENDERER_API __declspec(dllimport)
# endif
#else
# define OPENGL_GUIRENDERER_API
#endif

namespace CEGUI {

    /*!
    \brief
     Provides information about the type of OpenGL used by an OpenGL context
     (desktop OpenGL or OpenGL ES), the OpenGL version, and the OpenGL
     extensions.
    */
    class OPENGL_GUIRENDERER_API OpenGLInfo
    {

    public:
        /*!
        \brief
        Type of the OpenGL (desktop or ES) context
        */
        enum Type
        {
            TYPE_NONE, /*!< Not initalized yet */
            TYPE_DESKTOP, /*!< Desktop OpenGL */
            TYPE_ES /*!< OpenGL ES */
        };

        static OpenGLInfo& getSingleton() { return s_instance; }

        /*!
        \brief
        Must be called before any other method.

        Note that the information returned by other methods is with respect to
        the OpenGL (desktop or ES) context that was current when this method was called.
        */
        void init();

        /*!
        \brief
        Type of the OpenGL (desktop or ES) context
        */
        Type type() const { return d_type; }

        /*!
        \brief
        Returns true if using Desktop OpenGL.
        */
        bool isUsingDesktopOpengl() const { return type() == TYPE_DESKTOP; }

        /*!
        \brief
        Returns true if using OpenGL ES.
        */
        bool isUsingOpenglEs() const { return type() == TYPE_ES; }

        /*!
        \brief
        Returns OpenGL (desktop or ES) major version. Only supports Epoxy!
        Otherwise returns -1;
        */
        GLint verMajor() const { return d_verMajor; }

        /*!
        \brief
        Returns OpenGL (desktop or ES) minor version. Only supports Epoxy!
        Otherwise returns -1;
        */
        GLint verMinor() const { return d_verMinor; }

        /*!
        \brief
        Returns true if the OpenGL (desktop or ES) version is at least "major.minor".
        Only supports Epoxy! Otherwise returns false.
        */
        bool verAtLeast(GLint major, GLint minor) const {
            return verMajor() > major || (verMajor() == major && verMinor() >= minor);
        }

        /*!
        \brief
        Returns true if "S3TC" texture compression is supported.

        Note: Works only with Epoxy OR with desktop OpenGL >=3.0. Otherwise
        returns false.
        */
        bool isS3tcSupported() const { return d_isS3tcSupported; }

        /*!
        \brief
        Returns true if NPOT (non-power-of-two) textures are supported.
        */
        bool isNpotTextureSupported() const { return d_isNpotTextureSupported; }

        /*!
        \brief
        Returns true if "glReadBuffer" is supported.
        */
        bool isReadBufferSupported() const
        {
            return d_isReadBufferSupported;
        }

        /*!
        \brief
        Returns true if "glPolygonMode" is supported.
        */
        bool isPolygonModeSupported() const
        {
            return d_isPolygonModeSupported;
        }

        /*!
        \brief
        Returns true if VAO-s (Vertex Array Objects) are supported.
        */
        bool isVaoSupported() const { return d_isVaoSupported; }

        /*!
        \brief
        Returns true if working with the read/draw framebuffers seperately is
        supported.
        */
        bool isSeperateReadAndDrawFramebufferSupported() const
        {
            return d_isSeperateReadAndDrawFramebufferSupported;
        }

        bool isSizedInternalFormatSupported() const
        {
            return d_isSizedInternalFormatSupported;
        }

        /* For internal use. Used to force the object to act is if we're using a
        context of the specificed "verMajor_.verMinor_". This is useful to
        check that an OpenGL (desktop/ES) version lower than the actual one
        works correctly. Of course, this should work only if the actual
        version is compatible with the forced version. For example this can be
        used to check OpenGL ES2.0 when the context is actually OpenGL ES3.0
        (which is compatible with OpenGL ES2.0). */
        void verForce(GLint verMajor_, GLint verMinor_);

    private:

        static OpenGLInfo s_instance;
        OpenGLInfo();
        void initTypeAndVer();
        void initSupportedFeatures();

        Type d_type;
        GLint d_verMajor;
        GLint d_verMinor;
        GLint d_verMajorForce;
        GLint d_verMinorForce;
        bool d_isS3tcSupported;
        bool d_isNpotTextureSupported;
        bool d_isReadBufferSupported;
        bool d_isPolygonModeSupported;
        bool d_isSeperateReadAndDrawFramebufferSupported;
        bool d_isVaoSupported;
        bool d_isSizedInternalFormatSupported;
    };

} // namespace CEGUI

#endif // end of guard _CEGUIOpenGL_h_
