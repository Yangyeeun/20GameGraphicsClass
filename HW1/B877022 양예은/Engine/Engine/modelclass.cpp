////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device)
{
	bool result;


	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}


bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	// Set the number of vertices in the vertex array.
	m_vertexCount = 18;

	// Set the number of indices in the index array.
	m_indexCount = 18;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array with data.
	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // Rectangle Bottom left.
	vertices[0].color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	vertices[3].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[3].color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	vertices[4].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[4].color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);


	vertices[5].position = D3DXVECTOR3(0.0f, -3.0f, 0.0f);  // Bottom right.
	vertices[5].color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	vertices[6].position = D3DXVECTOR3(-5.0f, -1.0f, 0.0f);  //Triangle Bottom left.
	vertices[6].color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[7].position = D3DXVECTOR3(-4.0f, 1.0f, 0.0f);  // Top middle.
	vertices[7].color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[8].position = D3DXVECTOR3(-3.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[8].color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[9].position = D3DXVECTOR3(3.0f, -1.0f, 0.0f);  // Pentagon Bottom left.
	vertices[9].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[10].position = D3DXVECTOR3(4.0f, 1.0f, 0.0f);  // Top middle.
	vertices[10].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[11].position = D3DXVECTOR3(5.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[11].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[12].position = D3DXVECTOR3(3.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[12].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[13].position = D3DXVECTOR3(5.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[13].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[14].position = D3DXVECTOR3(3.5f, -3.0f, 0.0f);  // Bottom right.
	vertices[14].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[15].position = D3DXVECTOR3(3.5f, -3.0f, 0.0f);  // Bottom right.
	vertices[15].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[16].position = D3DXVECTOR3(5.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[16].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[17].position = D3DXVECTOR3(4.5f, -3.0f, 0.0f);  // Bottom right.
	vertices[17].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	// Load the index array with data.
	for (int i = 0; i < 18; i++) {
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}