/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#include "AppWindow.h"
#include "Resource.h"
#include <gl/GL.h>

#include "AssignmentTests.h"

using namespace AssignmentTests;

AppWindow::AppWindow()
{
	mCurrentTest = TEST1;
}

AppWindow::~AppWindow()
{
	delete mRasterizer;
}

AppWindow::AppWindow(HINSTANCE hInstance, int width, int height)
{
	InitWindow(hInstance, width, height);
	mCurrentTest = TEST1;
}

void AppWindow::SetCurrentTestCase(ETEST test)
{
	char newtitle[256];
	
	sprintf_s(newtitle, "TinyRaster - Graphics I -- TEST %d", test+1);
	
	SetWindowTextA(m_hwnd, newtitle);

	mCurrentTest = test;
}

HGLRC AppWindow::CreateOGLContext(HDC hdc)
{
	unsigned int pixelformat;
	HGLRC hglrc;

	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Support Window
		PFD_SUPPORT_OPENGL |						// Format Support OpenGL
		PFD_DOUBLEBUFFER,							// OK let's use single buffering
		PFD_TYPE_RGBA,								// Request RGBA Format
		32,											// Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits mask
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// Z-Buffer depth
		0,											// Stencil Buffer
		0,											// Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(pixelformat=ChoosePixelFormat(hdc,&pfd)))
	{
		return 0;
	}

	if(!SetPixelFormat(hdc,pixelformat,&pfd))
	{
		return 0;
	}

	if (!(hglrc=wglCreateContext(hdc)))
	{
		return 0;
	}

	if(!wglMakeCurrent(hdc,hglrc))
	{
		return 0;
	}

	return hglrc;
}

void AppWindow::DestroyOGLWindow()
{
	DestroyOGLContext();
	
	DestroyWindow( m_hwnd );

	m_hwnd = NULL;
	m_hdc = NULL;
}

BOOL AppWindow::DestroyOGLContext()
{
	if ( m_hglrc )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( m_hglrc );
		m_hglrc = NULL;
	}
	
	if ( !ReleaseDC ( m_hwnd, m_hdc ) )
		return FALSE;

	return TRUE;
}

BOOL AppWindow::InitWindow(HINSTANCE hInstance, int width, int height)
{
	m_hwnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		L"AppWindow", L"TinyRaster - Graphics I", WS_OVERLAPPED|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_SYSMENU,
		0, 0, width, height, NULL, NULL, hInstance, NULL);

	if ( ! m_hwnd )
		return FALSE;

	RECT clientrect;

	GetClientRect(m_hwnd, &clientrect);

	m_hdc = GetDC( m_hwnd );

	if ( !(m_hglrc = CreateOGLContext( m_hdc )) )
		return FALSE;
	
	SetWindowPos(m_hwnd, NULL, 10, 10, (width << 1) - clientrect.right, (height << 1) - clientrect.bottom, SWP_HIDEWINDOW|SWP_NOREDRAW);

	m_width = width;
	m_height = height;

	mRasterizer = new Rasterizer(m_width, m_height);

	SetCurrentTestCase(TEST1);

	return TRUE;
}

void AppWindow::SetVisible ( BOOL visible )
{
	ShowWindow ( m_hwnd, visible? SW_SHOW : SW_HIDE );
}

void AppWindow::Render()
{
	PixelRGBA *buffer = mRasterizer->GetFrameBuffer()->GetBuffer();

	mRasterizer->Clear(Colour4(0.1, 0.1, 0.1, 1.0));

	switch (mCurrentTest) {
		case TEST1:
			AssignmentTest01(mRasterizer);
			break;
		case TEST2:
			AssignmentTest02(mRasterizer);
			break;
		case TEST3:
			AssignmentTest03(mRasterizer);
			break;
		case TEST4:
			AssignmentTest04(mRasterizer);
			break;
		case TEST5:
			AssignmentTest05(mRasterizer);
			break;
		case TEST6:
			AssignmentTest06(mRasterizer);
			break;
		case TEST7:
			AssignmentTest07(mRasterizer);
			break;
		case TEST8:
			AssignmentTest08(mRasterizer);
			break;

	}

	Vector2 centre(m_width >> 1, m_height >> 1);
	Vector2 pt(mMousePt[0], mMousePt[1]);

	Vertex2d c = { Colour4(1, 1, 1, 1), centre };
	Vertex2d p = { Colour4(0, 1, 0, 1), pt };

	mRasterizer->SetGeometryMode(Rasterizer::LINE);
	mRasterizer->SetFillMode(Rasterizer::INTERPOLATED_FILLED);
	mRasterizer->DrawLine2D(c, p);
	mRasterizer->SetFillMode(Rasterizer::SOLID_FILLED);

	glDrawPixels(m_width, m_height, GL_RGBA, GL_FLOAT, buffer);

	SwapBuffers(m_hdc);
	return;
}

void AppWindow::Resize( int width, int height )
{
	RECT clientrect;
	GetClientRect(m_hwnd, &clientrect);
	
	glViewport( 0, 0, width, height );
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0.0, width, 0.0, height, -1.0, 1.0);
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	return;
}

void AppWindow::InitOGLState()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}


BOOL AppWindow::MouseLBDown ( int x, int y )
{
	return TRUE;
}

BOOL AppWindow::MouseLBUp ( int x, int y )
{
	return TRUE;
}

BOOL AppWindow::MouseMove ( int x, int y )
{
	mMousePt.SetVector(x, m_height - y);

	return TRUE;
}

BOOL AppWindow::KeyUp(WPARAM key)
{
	switch (key)
	{
	case VK_F1:
		SetCurrentTestCase(TEST1);
		break;
	case VK_F2:
		SetCurrentTestCase(TEST2);
		break;
	case VK_F3:
		SetCurrentTestCase(TEST3);
		break;
	case VK_F4:
		SetCurrentTestCase(TEST4);
		break;
	case VK_F5:
		SetCurrentTestCase(TEST5);
		break;
	case VK_F6:
		SetCurrentTestCase(TEST6);
		break;
	case VK_F7:
		SetCurrentTestCase(TEST7);
		break;
	case VK_F8:
		SetCurrentTestCase(TEST8);
		break;
	}

	mRasterizer->Clear(Colour4(0.1, 0.1, 0.1, 1.0));


	return TRUE;
}