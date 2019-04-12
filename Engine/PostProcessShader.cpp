#include "PostProcessShader.h"


PostProcessShader::PostProcessShader(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* immediateContext) :
	Shader(hwnd, device, immediateContext)
{
	//m_vertexShader = 0;
	//m_pixelShader = 0;
	//m_layout = 0;
	//m_sampleState = 0;
	//m_matrixBuffer = 0;

	m_screenSizeBuffer = 0;
}


PostProcessShader::~PostProcessShader()
{
	Shutdown();
}


void PostProcessShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();
}


bool PostProcessShader::InitializeShader(ShaderFiles sf)
{
	return InitializeShader(sf.vs, sf.ps);
}




bool PostProcessShader::InitializeShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC screenSizeBufferDesc;




	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	m_psFunctionName = "PostProcessPixelShader";
	m_vsFunctionName = "PostProcessVertexShader";



	// Need to pass the number of elements, or it will wrongly calculate it inside the parent class
	if (!this->Shader::InitializeShader(vsFilename, psFilename, polygonLayout, numElements))
	{
		return false;
	}


	// Setup the description of the dynamic screen size constant buffer that is in the vertex shader.
	screenSizeBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	screenSizeBufferDesc.ByteWidth = sizeof(ScreenSizeBufferType);
	screenSizeBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	screenSizeBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	screenSizeBufferDesc.MiscFlags = 0;
	screenSizeBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = m_device->CreateBuffer(&screenSizeBufferDesc, NULL, &m_screenSizeBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}



bool PostProcessShader::Render(int indexCount, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, float screenWidth)
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(
		XMMatrixIdentity(), 
		viewMatrix, 
		projectionMatrix, 
		texture, 
		screenWidth);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(indexCount);

	return true;
}



void PostProcessShader::ShutdownShader()
{
	// Release the screen size constant buffer.
	if (m_screenSizeBuffer)
	{
		m_screenSizeBuffer->Release();
		m_screenSizeBuffer = 0;
	}

}


bool PostProcessShader::SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, float screenWidth)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;
	ScreenSizeBufferType* dataPtr2;

	// texture or texture array have to be decided here, not in base class
	if (!this->Shader::SetMatrixBuffer(worldMatrix, viewMatrix, projectionMatrix))
	{
		return false;
	}



	// Lock the screen size constant buffer so it can be written to.
	/*result = m_context->Map(m_screenSizeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr2 = (ScreenSizeBufferType*)mappedResource.pData;

	// Copy the data into the constant buffer.
	dataPtr2->screenWidth = screenWidth;
	dataPtr2->padding = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// Unlock the constant buffer.
	m_context->Unmap(m_screenSizeBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 1;

	// Now set the constant buffer in the vertex shader with the updated values.
	m_context->VSSetConstantBuffers(bufferNumber, 1, &m_screenSizeBuffer);*/

	// Set shader texture resource in the pixel shader.
	m_context->PSSetShaderResources(0, 1, &texture);

	return true;
}
