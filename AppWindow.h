#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
//#include "Cube.h"

class AppWindow: public Window
{
public:
	static AppWindow* getInstance();
	static void initialize();

	//AppWindow() = default;
	AppWindow();
	void updateQuadPosition();
	~AppWindow();

	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

	void drawQuad(VertexBuffer* vb, VertexShader* vs, PixelShader* ps);
	void drawTriangle(VertexBuffer* vb, VertexShader* vs, PixelShader* ps);
	void drawQuad(VertexBuffer* vb, ConstantBuffer* cb, VertexShader* vs, PixelShader* ps);
	void drawTriangle(VertexBuffer* vb, ConstantBuffer* cb, VertexShader* vs, PixelShader* ps);
	void drawIndexedTriangle(VertexBuffer* vb, IndexBuffer* ib, ConstantBuffer* cb, VertexShader* vs, PixelShader* ps);
	void drawIndexedTriangle2(VertexBuffer* vb, IndexBuffer* ib, ConstantBuffer* cb, VertexShader* vs, PixelShader* ps);
	void drawIndexedTriangle(VertexBuffer* vb, IndexBuffer* ib, VertexShader* vs, PixelShader* ps);
	float modifySpeed();

private:

	static AppWindow* sharedInstance;
	SwapChain* m_swap_chain;
	VertexBuffer* m_vb;
	VertexBuffer* m_vb2;
	VertexBuffer* m_vb3;
	ConstantBuffer* m_cb;
	ConstantBuffer* m_cb2;
	ConstantBuffer* m_cb3;
	IndexBuffer* m_ib;
	IndexBuffer* m_ib2;
	VertexShader* m_vs;
	VertexShader* m_vs2;
	VertexShader* m_vs3;
	PixelShader* m_ps;
	PixelShader* m_ps2;
	PixelShader* m_ps3;

	unsigned long m_old_time = 0;
	float ticks = 0.0f;
	float speed = 0.0f;
	float animSpeed = 0.5f;
	float timer = 0.0f;
	
	float m_angle = 0;
	bool speedup = false;

	float m_old_delta = 0.0f;
	float m_new_delta = 0.0f;
	float m_delta_time = 0.0f;
	float m_delta_pos = 0.0f;
	float m_delta_scale = 0.0f;

};

