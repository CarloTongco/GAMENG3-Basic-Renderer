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

	//SLIDE 13
	/*vertex list[] =
	{
		//X - Y - Z
		{-0.5f,-0.5f,0.0f,    -0.32f,-0.11f,0.0f,   0,0,0,  0,1,0 }, 
		{-0.5f,0.5f,0.0f,     -0.11f,0.78f,0.0f,    1,1,0,  0,1,1 }, 
		{ 0.5f,-0.5f,0.0f,     0.75f,-0.73f,0.0f,   0,0,1,  1,0,0 },
		{ 0.5f,0.5f,0.0f,      0.88f,0.77f,0.0f,    1,1,1,  0,0,1 }
	};*/

	//SLIDE 14
	vertex list[] =
	{
		//X - Y - Z
		{-0.75f,-0.75f,0.0f,    -0.02f,0.21f,0.0f,   0,0,0,  0,1,0 },
		{-0.95f, 0.5f, 0.0f,     0.11f,0.87f,0.0f,    1,1,0,  0,1,1 },
		{ 0.75f,-0.5f, 0.0f,     0.45f,-0.63f,0.0f,   0,0,1,  1,0,0 },
		{-0.75f,-0.75f,0.0f,     0.88f,0.87f,0.0f,    1,1,1,  0,0,1 }
	};

	/*vertex list[] =
	{
		//X - Y - Z
		{-0.15f,-0.5f,0.0f,    -0.15f,-0.5f,0.0f,   0,0,1,  0,1,0 },
		{-0.15f,0.5f,0.0f,     -0.15f,0.5f,0.0f,    0,0,1,  0,1,1 },
		{ 0.15f,-0.5f,0.0f,     0.15f,-0.5f,0.0f,   0,1,0,  1,0,0 },
		{ 0.15f,0.5f,0.0f,      0.15f,0.5f,0.0f,    0,1,0,  0,0,1 }
	};*/

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
	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	m_vb2 = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list2 = ARRAYSIZE(list2);

	m_vb3 = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list3 = ARRAYSIZE(list3);

	//GraphicsEngine::get()->createShaders();

	void* shader_byte_code = nullptr;
	UINT size_shader = 0;
	//GraphicsEngine::get()->getShaderBufferAndSize(&shader_byte_code, &size_shader);
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);
	m_vs2 = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb2->load(list2, sizeof(vertex), size_list2, shader_byte_code, size_shader);
	m_vs3 = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb3->load(list3, sizeof(vertex), size_list3, shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	m_ps2 = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	m_ps3 = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
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

	//animSpeed = modifySpeed();

	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	constant cbData = {};
	cbData.m_angle = this->ticks;

	//std::cout << "Elapsed time since frame start is: " << cbData.m_angle << "\n";

	unsigned long new_time = 0;
	if (m_old_time)
		new_time = ::GetTickCount() - m_old_time;
	m_delta_time = new_time / 1000.0f;
	m_old_time = ::GetTickCount();

	m_angle += 1.57f * m_delta_time * animSpeed;
	//m_angle += 1.57f * m_delta_time;
	constant cc;
	cc.m_angle = m_angle;

	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);

	drawQuad(m_vb, m_cb, m_vs, m_ps);
	//drawQuad(m_vb, m_vs, m_ps);
	//drawQuad(m_vb2, m_vs2, m_ps2);
	//drawQuad(m_vb3, m_vs3, m_ps3);

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

void AppWindow::drawQuad(VertexBuffer* vb, VertexShader* vs, PixelShader* ps)
{
	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	//GraphicsEngine::get()->setShaders();
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(ps);

	//SET THE VERTICES OF THE QUAD TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(vb);

	// FINALLY DRAW THE QUAD
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(vb->getSizeVertexList(), 0);
}

void AppWindow::drawTriangle(VertexBuffer* vb, VertexShader* vs, PixelShader* ps)
{
	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	//GraphicsEngine::get()->setShaders();
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(ps);

	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(vb);

	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(vb->getSizeVertexList(), 0);
}

void AppWindow::drawQuad(VertexBuffer* vb, ConstantBuffer* cb, VertexShader* vs, PixelShader* ps)
{
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(vs, cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(ps, cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	//GraphicsEngine::get()->setShaders();
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(ps);

	//SET THE VERTICES OF THE QUAD TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(vb);

	// FINALLY DRAW THE QUAD
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(vb->getSizeVertexList(), 0);
}

void AppWindow::drawTriangle(VertexBuffer* vb, ConstantBuffer* cb, VertexShader* vs, PixelShader* ps)
{
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(vs, cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(ps, cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	//GraphicsEngine::get()->setShaders();
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(ps);

	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(vb);

	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(vb->getSizeVertexList(), 0);
}

float AppWindow::modifySpeed()
{
	if (timer <= 0.0f) {
		speedup = true;
	}
	else if (timer >= 10.0f) {
		speedup = false;
	}
	if (speedup == true) {
		std::cout << "Increasing speed! \n";
		timer += EngineTime::getDeltaTime() * 1.0f;
	}
	else if (speedup == false) {
		std::cout << "Decreasing speed! \n";
		timer -= EngineTime::getDeltaTime() * 1.0f;
	}
	this->speed = timer;
	//std::cout << "Elapsed time: " << timer << "\n";
	std::cout << "Current speed: " << this->speed << "\n";

	return speed;
}
