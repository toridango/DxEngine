

#include "VolumetricLaserShader.h"



VolumetricLaserShader::VolumetricLaserShader(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* immediateContext) :
	Shader(hwnd, device, immediateContext)
{
	m_cameraBuffer = 0;
	m_capsuleBuffer = 0;
	m_variableBuffer = 0;
}


VolumetricLaserShader::~VolumetricLaserShader()
{
	if (m_cameraBuffer)
	{
		m_cameraBuffer->Release();
		m_cameraBuffer = 0;
	}

	if (m_capsuleBuffer)
	{
		m_capsuleBuffer->Release();
		m_capsuleBuffer = 0;
	}

	if (m_variableBuffer)
	{
		m_variableBuffer->Release();
		m_variableBuffer = 0;
	}
}

bool VolumetricLaserShader::InitializeShader(ShaderFiles sf)
{
	return InitializeShader(sf.vs, sf.ps);
}

bool VolumetricLaserShader::InitializeShader(WCHAR* vsFilename, WCHAR* psFilename)
{

	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_BUFFER_DESC variableBufferDesc;
	D3D11_BUFFER_DESC capsuleBufferDesc;
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

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);


	m_psFunctionName = "VolumetricLaserPixelShader";
	m_vsFunctionName = "VolumetricLaserVertexShader";

	// Need to pass the number of elements, or it will wrongly calculate it inside the parent class
	if (!this->Shader::InitializeShader(vsFilename, psFilename, polygonLayout, numElements))
	{
		return false;
	}

	// Setup the description of the camera dynamic constant buffer that is in the vertex shader.
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	if (FAILED(m_device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer)))
	{
		return false;
	}



	// Setup the description of the variable dynamic constant buffer that is in the vertex shader.
	variableBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	variableBufferDesc.ByteWidth = sizeof(VariableBufferType);
	variableBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	variableBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	variableBufferDesc.MiscFlags = 0;
	variableBufferDesc.StructureByteStride = 0;

	// Create the variable constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	if (FAILED(m_device->CreateBuffer(&variableBufferDesc, NULL, &m_variableBuffer)))
	{
		return false;
	}


	// Setup the description of the variable dynamic constant buffer that is in the vertex shader.
	capsuleBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	capsuleBufferDesc.ByteWidth = sizeof(CapsuleBufferType);
	capsuleBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	capsuleBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	capsuleBufferDesc.MiscFlags = 0;
	capsuleBufferDesc.StructureByteStride = 0;

	// Create the variable constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	if (FAILED(m_device->CreateBuffer(&capsuleBufferDesc, NULL, &m_capsuleBuffer)))
	{
		return false;
	}


	return true;
}


// The SetShaderParameters function sets the shader parameters before rendering occurs.
bool VolumetricLaserShader::SetShaderParameters(XMMATRIX worldMatrix,
	XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT3 cameraPosition, 
	XMFLOAT3 cPos, XMFLOAT3 cDir, float capsuleLen, float boundingCubeSide, float deltavalue)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	CapsuleBufferType* dataPtr2;
	VariableBufferType* dataPtr3;
	CameraBufferType* dataPtr4;
	unsigned int bufferNumber;

	// texture or texture array have to be decided here, not in base class
	if (!this->Shader::SetMatrixBuffer(worldMatrix, viewMatrix, projectionMatrix))
	{
		return false;
	}

	// Lock the camera constant buffer so it can be written to.
	if (FAILED(m_context->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr4 = (CameraBufferType*)mappedResource.pData;

	// Copy the camera position into the constant buffer.
	dataPtr4->cameraPosition = cameraPosition;
	dataPtr4->cpadding = 0.0f;

	// Unlock the camera constant buffer.
	m_context->Unmap(m_cameraBuffer, 0);

	// Set the position of the camera constant buffer in the vertex shader.
	bufferNumber = 1;

	// Now set the camera constant buffer in the vertex shader with the updated values.
	m_context->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);


	//VARIABLE BUFFER
	result = m_context->Map(m_variableBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr3 = (VariableBufferType*)mappedResource.pData;

	// Copy the variablethe constant buffer.
	dataPtr3->delta = deltavalue;
	dataPtr3->padding = XMFLOAT3(0.0f, 0.0f, 0.0f); //this is just padding so this data isnt used.

	// Unlock the variable constant buffer.
	m_context->Unmap(m_variableBuffer, 0);

	// Set the position of the variable constant buffer in the vertex shader.
	bufferNumber = 2;

	// Now set the variable constant buffer in the vertex shader with the updated values.
	m_context->VSSetConstantBuffers(bufferNumber, 1, &m_variableBuffer);

	//END VARIABLE BUFFER




	// CAPSULE BUFFER
	result = m_context->Map(m_capsuleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr2 = (CapsuleBufferType*)mappedResource.pData;

	// Copy the variablethe constant buffer.
	dataPtr2->cPos = cPos;
	dataPtr2->cLen = capsuleLen;
	dataPtr2->cDir = cDir;
	dataPtr2->boundingCubeSide = boundingCubeSide;

	// Unlock the variable constant buffer.
	m_context->Unmap(m_capsuleBuffer, 0);

	// Set the position of the variable constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the variable constant buffer in the vertex shader with the updated values.
	m_context->PSSetConstantBuffers(bufferNumber, 1, &m_capsuleBuffer);

	// END CAPSULE BUFFER



	return true;
}



bool VolumetricLaserShader::Render(GameObject* go, CameraClass* camera, 
	XMFLOAT3 cDir, float capsuleLen, float boundingCubeSide, float deltavalue)
{
	XMFLOAT3 cPos;
	XMStoreFloat3(&cPos, go->GetWorldMatrix().r[3]);
	// Set the shader parameters that it will use for rendering.
	if (!SetShaderParameters(
		go->GetWorldMatrix(),
		camera->GetViewMatrix(),
		camera->GetProjectionMatrix(),
		camera->GetPosition(),
		cPos,
		cDir,
		capsuleLen,
		boundingCubeSide,
		deltavalue
	))
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(go->GetIndexCount());

	return true;
}
