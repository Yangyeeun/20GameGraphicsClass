#define WIN32_LEAN_AND_MEAN

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <windows.h>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

bool g_keys[256];
LPCWSTR g_applicationName;
HINSTANCE g_hinstance;
HWND g_hwnd;

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

struct VertexType
{
	FLOAT X, Y, Z; D3DXCOLOR Color;
};

struct MatrixBufferType
{

};

VertexType g_vertices[];

DWORD g_indices[];

int g_vertexCount;
int g_indexCount;

// Do NOT modify the followiong codes
class GraphicsClass
{
public:
	GraphicsClass();
	void Shutdown();

	bool Initialize(int, int, HWND);

	bool InitializeD3D(int, int, HWND);
	bool InitializeBuffers();
	bool InitializeShader(HWND, WCHAR*, WCHAR*);
	bool RenderShader(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	bool Frame();

private:
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;

	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_viewMatrix;

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
};

// Do NOT modify the followiong codes
GraphicsClass::GraphicsClass()
{
	m_swapChain = 0, m_device = 0, m_deviceContext = 0, m_renderTargetView = 0;
	m_depthStencilBuffer = 0, m_depthStencilState = 0, m_depthStencilView = 0, m_rasterState = 0;
	m_vertexBuffer = 0, m_indexBuffer = 0;
	m_vertexShader = 0, m_pixelShader = 0, m_layout = 0, m_matrixBuffer = 0;
}

// Do NOT modify the followiong codes
void GraphicsClass::Shutdown()
{
	if (m_swapChain) { m_swapChain->SetFullscreenState(false, NULL); }
	if (m_rasterState) { m_rasterState->Release();			m_rasterState = 0; }
	if (m_depthStencilView) { m_depthStencilView->Release();		m_depthStencilView = 0; }
	if (m_depthStencilState) { m_depthStencilState->Release();		m_depthStencilState = 0; }
	if (m_depthStencilBuffer) { m_depthStencilBuffer->Release();	m_depthStencilBuffer = 0; }
	if (m_renderTargetView) { m_renderTargetView->Release();		m_renderTargetView = 0; }
	if (m_deviceContext) { m_deviceContext->Release();			m_deviceContext = 0; }
	if (m_device) { m_device->Release();				m_device = 0; }
	if (m_swapChain) { m_swapChain->Release();				m_swapChain = 0; }
	if (m_indexBuffer) { m_indexBuffer->Release();			m_indexBuffer = 0; }
	if (m_vertexBuffer) { m_vertexBuffer->Release();			m_vertexBuffer = 0; }
	if (m_matrixBuffer) { m_matrixBuffer->Release();			m_matrixBuffer = 0; }
	if (m_layout) { m_layout->Release();				m_layout = 0; }
	if (m_pixelShader) { m_pixelShader->Release();			m_pixelShader = 0; }
	if (m_vertexShader) { m_vertexShader->Release();			m_vertexShader = 0; }
}

// Do NOT modify the followiong codes
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{

	bool result;

	result = InitializeD3D(screenWidth, screenHeight, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	result = InitializeBuffers();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model buffers.", L"Error", MB_OK);
		return false;
	}

	result = InitializeShader(hwnd, L"./VertexColor.vs", L"./VertexColor.ps");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the shaders.", L"Error", MB_OK);
		return false;
	}

	return true;
}

bool GraphicsClass::InitializeD3D(int screenWidth, int screenHeight, HWND hwnd) {
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                   // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
	scd.BufferDesc.Width = screenWidth;                   // set the back buffer width
	scd.BufferDesc.Height = screenHeight;                 // set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
	scd.OutputWindow = hwnd;                               // the window to be used
	scd.SampleDesc.Count = 4;                              // how many multisamples
	scd.Windowed = TRUE;                                   // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&m_swapChain,
		&m_device,
		NULL,
		&m_deviceContext);


	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_renderTargetView);
	pBackBuffer->Release();

	// set the render target as the back buffer
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);


	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = screenWidth;
	viewport.Height = screenHeight;

	m_deviceContext->RSSetViewports(1, &viewport);

	

	return true;
}

bool GraphicsClass::InitializeBuffers()
{
	VertexType OurVertices[] =
	{
		{0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
		{0.45f, -0.5, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
		{-0.45f, -0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)}
	};


	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(VertexType) * 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	m_device->CreateBuffer(&bd, NULL, &m_vertexBuffer);       // create the buffer


	// copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	m_deviceContext->Map(m_vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
	m_deviceContext->Unmap(m_vertexBuffer, NULL);


	return true;
}

bool GraphicsClass::InitializeShader(HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	// load and compile the two shaders
	ID3D10Blob *VS, *PS;
	/*D3DX11CompileFromFile(L"VertexColor.vs", 0, 0, "VShader", "vs_5_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile(L"VertexColor.ps", 0, 0, "PShader", "ps_5_0", 0, 0, 0, &PS, 0, 0);*/

	// encapsulate both shaders into shader objects
	m_device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_vertexShader);
	m_device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pixelShader);

	// set the shader objects
	m_deviceContext->VSSetShader(m_vertexShader, 0, 0);
	m_deviceContext->PSSetShader(m_pixelShader, 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	m_device->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &m_layout);
	m_deviceContext->IASetInputLayout(m_layout);
	return true;
}

bool GraphicsClass::RenderShader(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	


	return true;
}

bool GraphicsClass::Frame()
{
	// clear the back buffer to a deep blue
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	// select which vertex buffer to display
	UINT stride = sizeof(VertexType);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// select which primtive type we are using
	m_deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw the vertex buffer to the back buffer
	m_deviceContext->Draw(3, 0);

	// switch the back buffer and the front buffer
	
	m_swapChain->Present(0, 0);
	///////
	// clear the back buffer to a deep blue
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	// select which vertex buffer to display
	/*UINT stride = sizeof(VertexType);
	UINT offset = 0;*/
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// select which primtive type we are using
	m_deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw the vertex buffer to the back buffer
	m_deviceContext->Draw(3, 0);

	// switch the back buffer and the front buffer
	m_swapChain->Present(0, 0);
	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	bool result;
	int screenWidth, screenHeight;

	screenWidth = 800;
	screenHeight = 600;

	// Initialize the windows api.
	WNDCLASSEX wc;
	int posX, posY;

	// Get the instance of this application.
	g_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	g_applicationName = L"Template";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = g_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Place the window in the middle of the screen.
	posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	// Create the window with the screen settings and get the handle to it.
	g_hwnd = CreateWindowEx(WS_EX_APPWINDOW, g_applicationName, g_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, g_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(g_hwnd, SW_SHOW);
	SetForegroundWindow(g_hwnd);
	SetFocus(g_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	for (int i = 0; i < 256; i++)
	{
		g_keys[i] = false;
	}

	GraphicsClass *pGraphics;
	pGraphics = new GraphicsClass;
	if (!pGraphics)
	{
		return 0;
	}

	result = pGraphics->Initialize(screenWidth, screenHeight, g_hwnd);
	if (!result)
	{
		return 0;
	}

	if (result)
	{
		MSG msg;
		bool done;

		// Initialize the message structure.
		ZeroMemory(&msg, sizeof(MSG));

		// Loop until there is a quit message from the window or the user.
		done = false;
		while (!done)
		{
			// Handle the windows messages.
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// If windows signals to end the application then exit out.
			if (msg.message == WM_QUIT)
			{
				done = true;
			}
			else
			{
				// Otherwise do the frame processing.

				// Check if the user pressed escape and wants to exit the application.
				if (g_keys[VK_ESCAPE])
				{
					done = true;
				}

				// Do the frame processing for the graphics object.
				if (!pGraphics->Frame())
				{
					done = true;
				}
			}
		}
	}

	// Shutdown and release the system object.
	// Release the graphics object.
	pGraphics->Shutdown();
	delete pGraphics;
	pGraphics = 0;

	// Shutdown the window.
	// Show the mouse cursor.
	ShowCursor(true);

	// Remove the window.
	DestroyWindow(g_hwnd);
	g_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(g_applicationName, g_hinstance);
	g_hinstance = NULL;

	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_KEYDOWN:
	{
		g_keys[(unsigned int)wparam] = true;
		return 0;
	}

	case WM_KEYUP:
	{
		g_keys[(unsigned int)wparam] = false;
		return 0;
	}

	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}

	}

}



