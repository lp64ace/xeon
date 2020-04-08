#include "wnd.h"

#include <glad/glad.h>

void WND::CreateContext ( void ) {
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof ( PIXELFORMATDESCRIPTOR ),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	if ( !hdc ) hdc = GetDC ( hwnd ) ;
	HDC ourWindowHandleToDeviceContext = hdc ;

	int  letWindowsChooseThisPixelFormat;
	letWindowsChooseThisPixelFormat = ChoosePixelFormat ( ourWindowHandleToDeviceContext , &pfd );
	SetPixelFormat ( ourWindowHandleToDeviceContext , letWindowsChooseThisPixelFormat , &pfd );

	HGLRC ourOpenGLRenderingContext = wglCreateContext ( ourWindowHandleToDeviceContext );
	wglMakeCurrent ( ourWindowHandleToDeviceContext , ourOpenGLRenderingContext );

	if ( !gladLoadGL ( ) ) {
		printf ( "WND::ERROR_CREATING_OPENGL_CONTEXT::GLAD_LOAD_GL FAILED!\n" ) ;
		exit ( 0 ) ;
	}
	else {
		printf ( "OPENGL::VERSIONS::%d.%d\n" , GLVersion.major , GLVersion.minor ) ;
	}

	glEnable ( GL_BLEND );
	glBlendFunc ( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
	glBlendFuncSeparate ( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA , GL_ONE , GL_ZERO );
}