#include "Cube.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"

struct vertex
{
	Vector3D position;
	//Vector3D position1;
	Vector3D color;
	Vector3D color1;
};

Cube::Cube(string name, void* shader_byte_code, size_t size_shader):AGameObject(name)
{
	vertex vertex_list[] =
	{
		//X - Y - Z
		//Front Face
		{Vector3D(-0.5f,-0.5f,-0.5f),    Vector3D(1,0,0),  Vector3D(0.2f,0,0) },
		{Vector3D(-0.5f,0.5f,-0.5f),     Vector3D(1,1,0),  Vector3D(0.2f,0.2f,0) },
		{Vector3D(0.5f,0.5f,-0.5f),     Vector3D(1,1,0),  Vector3D(0.2f,0.2f,0) },
		{Vector3D(0.5f,-0.5f,-0.5f),      Vector3D(1,1,1),  Vector3D(0.2f,0,0) },

		//Back Face
		{Vector3D(0.5f,-0.5f,0.5f),    Vector3D(0,1,0),  Vector3D(0,0.2f,0) },
		{Vector3D(0.5f,0.5f,0.5f),     Vector3D(0,1,1),  Vector3D(0,0.2f,0.2f) },
		{Vector3D(-0.5f,0.5f,0.5f),     Vector3D(0,1,1),  Vector3D(0,0.2f,0.2f) },
		{Vector3D(-0.5f,-0.5f,0.5f),      Vector3D(0,1,0),  Vector3D(0,0.2f,0) }
	};

	vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(vertex_list);

	unsigned int index_list[] =
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
	};

	ib = GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);

	ib->load(index_list, size_index_list);
}

Cube::~Cube()
{
}

void Cube::update(float deltaTime)
{
}

void Cube::draw(int width, int height, VertexShader* vs, PixelShader* ps)
{

}

void Cube::setAnimSpeed(float speed)
{
	this->speed = speed;
}
