#include "AppWindow.h"
#include <Windows.h>
#include "EngineTime.h"

struct vec3
{
	float x, y, z;
};

struct vertex
{
	vec3 position;
	vec3 position1;
	vec3 color;
	vec3 color1;
};

__declspec(align(16))
struct constant
{
	float m_angle;
};

/*__declspec(align(16))
struct CBData
{
	float time;
};*/

AppWindow* AppWindow::sharedInstance = NULL;

AppWindow* AppWindow::getInstance()
{
	return sharedInstance;
}

void AppWindow::initialize()
{
	sharedInstance = new AppWindow();
}

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex list[] =
	{
		//X - Y - Z
		{-0.5f,-0.5f,0.0f,    -0.32f,-0.11f,0.0f,   0,0,0,  0,1,0 }, 
		{-0.5f,0.5f,0.0f,     -0.11f,0.78f,0.0f,    1,1,0,  0,1,1 }, 
		{ 0.5f,-0.5f,0.0f,     0.75f,-0.73f,0.0f,   0,0,1,  1,0,0 },
		{ 0.5f,0.5f,0.0f,      0.88f,0.77f,0.0f,    1,1,1,  0,0,1 }
	};

	/*vertex list[] =
	{
		//X - Y - Z
		{-0.15f,-0.5f,0.0f,    -0.15f,-0.5f,0.0f,   0,0,1,  0,1,0 },
		{-0.15f,0.5f,0.0f,     -0.15f,0.5f,0.0f,    0,0,1,  0,1,1 },
		{ 0.15f,-0.5f,0.0f,     0.15f,-0.5f,0.0f,   0,1,0,  1,0,0 },
		{ 0.15f,0.5f,0.0f,      0.15f,0.5f,0.0f,    0,1,0,  0,0,1 }
	};

	vertex list2[] =
	{
		//X - Y - Z
		{-0.75f,-0.75f,0.0f,    -0.75f,-0.75f,0.0f,   1,0,0,  0,1,0 },
		{-0.75f,0.75f,0.0f,     -0.75f,0.75f,0.0f,    1,1,0,  0,1,1 },
		{-0.2f,-0.25f,0.0f,     -0.2f,-0.25f,0.0f,    1,0,1,  1,0,0 },
		{-0.2f,0.25f,0.0f,      -0.2f,0.25f,0.0f,     1,0,1,  0,0,1 }
	};

	vertex list3[] =
	{
		//X - Y - Z
		{ 0.2f,-0.25f,0.0f,       0.2f,-0.25f,0.0f,     0,1,1,  0,1,0 },
		{ 0.2f,0.25f,0.0f,        0.2f,0.25f,0.0f,      1,0,1,  0,1,1 },
		{ 0.75f,-0.75f,0.0f,      0.75f,-0.75f,0.0f,    1,0,1,  1,0,0 },
		{ 0.75f,0.75f,0.0f,       0.75f,0.75f,0.0f,     1,1,0,  0,0,1 }
	};*/

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	/*m_vb2 = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list2 = ARRAYSIZE(list2);

	m_vb3 = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list3 = ARRAYSIZE(list3);*/

	//GraphicsEngine::get()->createShaders();

	void* shader_byte_code = nullptr;
	UINT size_shader = 0;
	//GraphicsEngine::get()->getShaderBufferAndSize(&shader_byte_code, &size_shader);
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);
	/*m_vs2 = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb2->load(list2, sizeof(vertex), size_list2, shader_byte_code, size_shader);
	m_vs3 = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb3->load(list3, sizeof(vertex), size_list3, shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();*/

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	/*m_ps2 = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	m_ps3 = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);*/
	GraphicsEngine::get()->releaseCompiledShader();

	constant cbData = {};
	cbData.m_angle = 0;
	this->m_cb = GraphicsEngine::getInstance()->createConstantBuffer();
	this->m_cb->load(&cbData, sizeof(constant));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	this->ticks += EngineTime::getDeltaTime() * 1.0f;

	if (this->speed > 10.0f) {
		this->increasing = false;
	}
	else if (this->speed < 0.0f) {
		this->increasing = true;
	}

	if (this->increasing) {
		this->speed += EngineTime::getDeltaTime();
	}
	else {
		this->speed -= EngineTime::getDeltaTime();
	}

	//std::cout << "Current speed " << this->speed << "\n";

	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	constant cbData = {};
	cbData.m_angle = this->ticks;

	std::cout << "Elapsed time since frame start is: " << cbData.m_angle << "\n";

	unsigned long new_time = 0;
	if (m_old_time)
		new_time = ::GetTickCount() - m_old_time;
	m_delta_time = new_time / 1000.0f;
	m_old_time = ::GetTickCount();

	m_angle += 1.57f * m_delta_time * speed;
	//m_angle += 1.57f * m_delta_time;
	constant cc;
	cc.m_angle = m_angle;

	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	//GraphicsEngine::get()->setShaders();
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);

	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	// FINALLY DRAW THE TRIANGLE
	//GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(m_vb->getSizeVertexList(), 0);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);
	//m_swap_chain->present(true);

	//Quad2
	/*m_cb2->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc2);

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs2, m_cb2);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps2, m_cb2);*/

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	//GraphicsEngine::get()->setShaders();
	//GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs2);
	//GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps2);

	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	//GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb2);

	// FINALLY DRAW THE TRIANGLE
	//GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(m_vb2->getSizeVertexList(), 0);
	//GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb2->getSizeVertexList(), 0);

	//Quad3
	/*m_cb2->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc2);

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs2, m_cb3);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps2, m_cb3);*/

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	//GraphicsEngine::get()->setShaders();
	//GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs3);
	//GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps3);

	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	//GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb3);

	// FINALLY DRAW THE TRIANGLE
	//GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(m_vb3->getSizeVertexList(), 0);
	//GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb3->getSizeVertexList(), 0);

	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	//m_vb2->release();
	//m_vb3->release();
	m_swap_chain->release();
	m_vs->release();
	//m_vs2->release();
	//m_vs3->release();
	m_ps->release();
	//m_ps2->release();
	//m_ps3->release();
	GraphicsEngine::get()->release();
	//GraphicsEngine::destroy();
}

/*void AppWindow::createGraphicsWindow()
{
	GraphicsEngine::initialize();
	GraphicsEngine* graphEngine = GraphicsEngine::getInstance();

	this->m_swap_chain = GraphicsEngine::getInstance()->createSwapChain();
	RECT rc = this->getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	this->m_swap_chain->init(this->getWindowHandle(), width, height);
	//this->m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	void* shaderByteCode = nullptr;
	size_t sizeShader = 0;

	graphEngine->compileVertexShader(L"VertexShader.hlsl", "main", &shaderByteCode, &sizeShader);
	this->m_vs = graphEngine->createVertexShader(shaderByteCode, sizeShader);

	vertex triangleList[] = {
		//X, Y, Z, R, G, B
		{-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f},
		{0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f}
	};

	vertex quadList[] = {
		//X, Y, Z
		{-0.5f,-0.5f,0.0f,    -0.32f,-0.11f,0.0f,   0,0,0,  0,1,0 }, 
		{-0.5f,0.5f,0.0f,     -0.11f,0.78f,0.0f,    1,1,0,  1,1,0 }, 
		{ 0.5f,-0.5f,0.0f,     0.0f,-0.73f,0.0f,    0,0,1,  1,0,0 },
		{ 0.0f,0.0f,0.0f,      0.88f,0.77f,0.0f,    1,1,1,  0,0,1 }
	};

	this->m_vb = GraphicsEngine::getInstance()->createVertexBuffer();
	this->m_vb->load(triangleList, sizeof(vertex), ARRAYSIZE(quadList), shaderByteCode, sizeShader);
	graphEngine->releaseCompiledShader(); 

	graphEngine->compilePixelShader(L"PixelShader.hlsl", "main", &shaderByteCode, &sizeShader);
	this->m_ps = graphEngine->createPixelShader(shaderByteCode, sizeShader);
	graphEngine->releaseCompiledShader();

	constant cbData = {};
	cbData.m_angle = 0;
	this->m_cb = GraphicsEngine::getInstance()->createConstantBuffer();
	this->m_cb->load(&cbData, sizeof(constant));
}*/
