#include "wnd.h"

#include <glad/glad.h>

Context *activeOpenGLContext = NULL;

Context::Context() : windowContext(NULL), openglContext(NULL), window(NULL) {
}

Context::Context(HDC hdc, HGLRC hglrc, WND *wnd) : windowContext(hdc), openglContext(hglrc), window(wnd) {
}

void WND::CreateContext(void) {
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
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

	int  letWindowsChooseThisPixelFormat;
	letWindowsChooseThisPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, letWindowsChooseThisPixelFormat, &pfd);

	hglrc = wglCreateContext(hdc);

	MakeCurrentContext(new Context(hdc, hglrc, this));

	if(!gladLoadGL()) {
		printf("WND::ERROR_CREATING_OPENGL_CONTEXT::GLAD_LOAD_GL FAILED!\n");
		exit(0);
	}
	else {
		printf("OPENGL::VERSIONS::%d.%d\n", GLVersion.major, GLVersion.minor);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
}

void MakeCurrentContext(Context *context) {
	activeOpenGLContext = context;
	wglMakeCurrent(context->windowContext, context->openglContext);
}

Context *GetCurrentContext(void) {
	return activeOpenGLContext;
}

HDC WND::GetContext() {
	return hdc;
}