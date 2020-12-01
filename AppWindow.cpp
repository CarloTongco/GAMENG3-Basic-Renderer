#include "AppWindow.h"
#include <Windows.h>
#include "EngineTime.h"
#include "Vector3D.h"
#include "Matrix4x4.h"

struct vec3
{
	float x, y, z;
};

struct vec4
{
	float r, g, b, a;
};

/*struct vertex
{
	vec3 position;
	vec3 position1;
	vec3 color;
	vec3 color1;
};*/

struct vertex
{
	Vector3D position;
	Vector3D position1;
	Vector3D color;
	Vector3D color1;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
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

void AppWindow::updateQuadPosition()
{
	constant cc;
	cc.m_angle = ::GetTickCount();

	m_delta_pos += m_delta_time / 10.0f;
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;

	Matrix4x4 temp;

	m_delta_scale += m_delta_time / 0.15f;
	cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5f, 0.5f, 0), Vector3D(1.0f, 1.0f, 0), (sin(m_delta_scale) + 1.0f) / 2.0f));
	//temp.setScale(Vector3D::lerp(Vector3D(0.5f, 0.5f, 0), Vector3D(1.0f, 1.0f, 0), (sin(m_delta_scale) + 1.0f) / 2.0f));

	//cc.m_world.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f, 1.5f, 0), m_delta_pos));
	temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f, 1.5f, 0), m_delta_pos));

	cc.m_world *= temp;

	//cc.m_world.setScale(Vector3D(1, 1, 1));

	/*temp.setIdentity();
	temp.setRotationZ(m_delta_scale);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(m_delta_scale);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(m_delta_scale);
	cc.m_world *= temp;*/

	cc.m_view.setIdentity();
	cc.m_proj.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 400.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 400.0f,
		-4.0f,
		4.0f
	);

	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
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

	/*vertex list[] =
	{
		//X - Y - Z
		{-0.5f,-0.5f,0.0f,    -0.32f,-0.11f,0.0f,   0,0,0,  0,1,0 },
		{-0.5f,0.5f,0.0f,     -0.11f,0.78f,0.0f,    1,1,0,  0,1,1 },
		{0.5f,-0.5f,0.0f,     0.75f,-0.73f,0.0f,    0,0,1,  1,0,0 },
		{0.5f,0.5f,0.0f,      0.88f,0.77f,0.0f,     1,1,1,  0,0,1 }
	};*/

	//SLIDE 13
	vertex list[] =
	{
		//X - Y - Z
		{Vector3D(-0.5f,-0.5f,0.0f),    Vector3D(-0.32f,-0.11f,0.0f),   Vector3D(0,0,0),  Vector3D(0,1,0) },
		{Vector3D(-0.5f,0.5f,0.0f),     Vector3D(-0.11f,0.78f,0.0f),    Vector3D(1,1,0),  Vector3D(0,1,1) },
		{Vector3D(0.5f,-0.5f,0.0f),     Vector3D(0.75f,-0.73f,0.0f),    Vector3D(0,0,1),  Vector3D(1,0,0) },
		{Vector3D(0.5f,0.5f,0.0f),      Vector3D(0.88f,0.77f,0.0f),     Vector3D(1,1,1),  Vector3D(0,0,1) }
	};

	/*vertex vertex_list[] =
	{
		//X - Y - Z
		//Front Face
		{Vector3D(-0.5f,-0.5f,-0.5f),    Vector3D(0,0,0),  Vector3D(0,1,0) },
		{Vector3D(-0.5f,0.5f,-0.5f),     Vector3D(1,1,0),  Vector3D(0,1,1) },
		{Vector3D(0.5f,0.5f,-0.5f),     Vector3D(0,0,1),  Vector3D(1,0,0) },
		{Vector3D(0.5f,-0.5f,-0.5f),      Vector3D(1,1,1),  Vector3D(0,0,1) },

		//Back Face
		{Vector3D(0.5f,-0.5f,0.5f),    Vector3D(0,0,0),  Vector3D(0,1,0) },
		{Vector3D(0.5f,0.5f,0.5f),     Vector3D(1,1,0),  Vector3D(0,1,1) },
		{Vector3D(-0.5f,0.5f,0.5f),     Vector3D(0,0,1),  Vector3D(1,0,0) },
		{Vector3D(-0.5f,-0.5f,0.5f),      Vector3D(1,1,1),  Vector3D(0,0,1) }
	};*/

	//SLIDE 14
	/*vertex list[] =
	{
		//X - Y - Z
		{Vector3D(-0.75f,-0.75f,0.0f),    Vector3D(-0.02f,0.21f,0.0f),   Vector3D(0,0,0),  Vector3D(0,1,0) },
		{Vector3D(-0.95f, 0.5f, 0.0f),    Vector3D(0.11f,0.87f,0.0f),    Vector3D(1,1,0),  Vector3D(0,1,1) },
		{Vector3D(0.75f,-0.5f, 0.0f),     Vector3D(0.45f,-0.63f,0.0f),   Vector3D(0,0,1),  Vector3D(1,0,0) },
		{Vector3D(-0.75f,-0.75f,0.0f),    Vector3D(0.88f,0.87f,0.0f),    Vector3D(1,1,1),  Vector3D(0,0,1) }
	};*/

	/*vertex list[] =
	{
		//X - Y - Z
		{-0.15f,-0.5f,0.0f,    -0.15f,-0.5f,0.0f,   0,0,1,  0,1,0 },
		{-0.15f,0.5f,0.0f,     -0.15f,0.5f,0.0f,    0,0,1,  0,1,1 },
		{ 0.15f,-0.5f,0.0f,     0.15f,-0.5f,0.0f,   0,1,0,  1,0,0 },
		{ 0.15f,0.5f,0.0f,      0.15f,0.5f,0.0f,    0,1,0,  0,0,1 }
	};*/

	/*vertex list2[] =
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

	/*unsigned int index_list[] =
	{
		//Front Side
		0, 1, 2, //First Triangle
		2, 3, 0, //Second Triangle

		//Back Side
		4, 5, 6,
		6, 7, 4,

		//Top Side
		1, 6, 5,
		5, 2, 1,

		//Bottom Side
		7, 0, 3,
		3, 4, 7,

		//Right Side
		3, 2, 5,
		5, 4, 3,

		//Left Side
		7, 6, 1,
		1, 0, 7,
	};*/

	/*m_vb2 = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list2 = ARRAYSIZE(list2);

	m_vb3 = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list3 = ARRAYSIZE(list3);*/

	/*m_ib = GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);

	m_ib->load(index_list, size_index_list);*/

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
	m_vb3->load(list3, sizeof(vertex), size_list3, shader_byte_code, size_shader);*/
	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	//m_ps2 = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	//m_ps3 = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	constant cbData = {};
	cbData.m_angle = 0;
	this->m_cb = GraphicsEngine::getInstance()->createConstantBuffer();
	this->m_cb->load(&cbData, sizeof(constant));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	//this->ticks += EngineTime::getDeltaTime() * 1.0f;

	//animSpeed = modifySpeed();

	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	updateQuadPosition();

	/*constant cbData = {};
	cbData.m_angle = this->ticks;*/

	//std::cout << "Elapsed time since frame start is: " << cbData.m_angle << "\n";

	/*unsigned long new_time = 0;
	if (m_old_time)
		new_time = ::GetTickCount() - m_old_time;
	m_delta_time = new_time / 1000.0f;
	m_old_time = ::GetTickCount();

	//m_angle += 1.57f * m_delta_time * animSpeed;
	m_angle += 1.57f * m_delta_time;
	constant cc;
	cc.m_angle = m_angle;

	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);*/

	//drawIndexedTriangle(m_vb, m_ib, m_cb, m_vs, m_ps);
	drawQuad(m_vb, m_cb, m_vs, m_ps);
	//drawQuad(m_vb, m_vs, m_ps);
	//drawQuad(m_vb2, m_vs2, m_ps2);
	//drawQuad(m_vb3, m_vs3, m_ps3);

	m_swap_chain->present(true);

	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();

	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	//m_ib->release();
	m_cb->release();
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

/*void AppWindow::drawIndexedTriangle(VertexBuffer* vb, IndexBuffer* ib, ConstantBuffer* cb, VertexShader* vs, PixelShader* ps)
{
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(vs, cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(ps, cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	//GraphicsEngine::get()->setShaders();
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(ps);

	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(vb);
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(ib);

	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(ib->getSizeIndexList(), 0, 0);
}*/

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
