#include "BumpShader.h"



BumpShader::BumpShader(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* immediateContext) :
	Shader(hwnd, device, immediateContext)
{
	m_lightBuffer = 0;
}


BumpShader::~BumpShader()
{
	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}
}

bool BumpShader::InitializeShader(ShaderFiles sf)
{
	return InitializeShader(sf.vs, sf.ps);
}

bool BumpShader::InitializeShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	// The polygon layout is now set to five elements to accommodate the tangent and binormal.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
	D3D11_BUFFER_DESC lightBufferDesc;
	unsigned int numElements;

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

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// The layout now includes a tangent and binormal element which are set up the 
	// same as the normal element with the exception of the semantic name.

	polygonLayout[3].SemanticName = "TANGENT";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	polygonLayout[4].SemanticName = "BINORMAL";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	m_psFunctionName = "BumpMapPixelShader";
	m_vsFunctionName = "BumpMapVertexShader";

	// Need to pass the number of tlements, or it will wrongly calculate it inside the parent class
	if (!this->Shader::InitializeShader(vsFilename, psFilename, polygonLayout, numElements))
	{
		return false;
	}
	   


	// The light constant buffer is set up here.
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	if (FAILED(m_device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer)))
	{
		return false;
	}



	return true;
}

// The Render function sets the shader parameters first and then renders the model using the bump map shader.
///bool BumpShader::Render(int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
///	XMMATRIX projectionMatrix, ID3D11ShaderResourceView** textureArray, XMFLOAT3 lightDirection)
bool BumpShader::Render(GameObject* go, CameraClass* camera, LightClass* light)
{
	bool result;

	// Set the shader parameters that it will use for rendering.
	if (!SetShaderParameters(
		go->GetWorldMatrix(),
		camera->GetViewMatrix(),
		camera->GetProjectionMatrix(),
		go->GetTextureArray(),
		light->GetDirection()
	))
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(go->GetIndexCount());

	return true;
}



bool BumpShader::SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView** textureArray, XMFLOAT3 lightDirection)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;
	LightBufferType* dataPtr2;


	// texture or texture array have to be decided here, not in base class
	if (!this->Shader::SetMatrixBuffer(worldMatrix, viewMatrix, projectionMatrix, lightDirection))
	{
		return false;
	}

	// The texture array is set here, it contains two textures:
	// The first texture is the color texture and 
	// the second texture is the normal map.

	// Set shader texture array resource in the pixel shader.
	m_context->PSSetShaderResources(0, 5, textureArray);

	// The light buffer in the pixel shader is then set with the diffuse light color and light direction.
	// Lock the light constant buffer so it can be written to.
	result = m_context->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr2 = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	//dataPtr2->diffuseColor = diffuseColor;
	dataPtr2->lightDirection = lightDirection;

	// Unlock the constant buffer.
	m_context->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	m_context->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	return true;
}