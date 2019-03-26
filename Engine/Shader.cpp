#include "Shader.h"



Shader::Shader(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context) :
	m_hwnd(hwnd),
	m_device(device), 
	m_context(context)
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_sampleState = 0;
	m_matrixBuffer = 0;
}


Shader::~Shader()
{

	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	
	}
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}
	
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}
}


bool Shader::CompileVertexShaderFromFile(WCHAR* filename, LPCSTR pFunctionName, ID3D10Blob **ppShader)
{
	return CompileShaderFromFile(filename, pFunctionName, "vs_5_0", ppShader);
}

bool Shader::CompilePixelShaderFromFile(WCHAR* filename, LPCSTR pFunctionName, ID3D10Blob **ppShader)
{
	return CompileShaderFromFile(filename, pFunctionName, "ps_5_0", ppShader);
}


bool Shader::CompileShaderFromFile(WCHAR* filename, LPCSTR pFunctionName, LPCSTR pProfile, ID3D10Blob **ppShader)
{
	ID3D10Blob* errorMessage;
	HRESULT result = D3DX11CompileFromFile(filename, NULL, NULL, pFunctionName, pProfile, D3D10_SHADER_ENABLE_STRICTNESS,
		0, NULL, ppShader, &errorMessage, NULL);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, m_hwnd, filename);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(m_hwnd, filename, L"Missing Shader File", MB_OK);
		}

		return false;
	}
	return true;
}

bool Shader::InitializeShader(WCHAR* vsFilename, WCHAR* psFilename, 
	const D3D11_INPUT_ELEMENT_DESC* polygonLayout, unsigned int numElementsPolyLayout)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	
	// NEED TO COMPILE SHADER IN CHILD CLASS ?
	// Load bump map vertex shader
	if (!CompileVertexShaderFromFile(vsFilename, m_vsFunctionName, &vertexShaderBuffer))
	{
		return false;
	}

	// Load bump map pixel shader
	if (!CompilePixelShaderFromFile(psFilename, m_psFunctionName, &pixelShaderBuffer))
	{
		return false;
	}
	


	// Create the vertex shader from the buffer.
	if (FAILED(m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL,
		&m_vertexShader)))
	{
		return false;
	}

	// Create the vertex shader from the buffer.
	if (FAILED(m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL,
		&m_pixelShader)))
	{
		return false;
	}

	/*
		Polygon layout setup went here
	*/


	// Create the vertex input layout.
	if (FAILED(m_device->CreateInputLayout(polygonLayout, numElementsPolyLayout, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout)))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Setup the description of the matrix dynamic constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the matrix constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = m_device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = m_device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}
	

	return true;
}


// OutputShaderErrorMessage writes out errors to a text file if the HLSL shader file won't compile properly.
void Shader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}


// The SetShaderParameters function sets the shader parameters before rendering occurs.
bool Shader::SetMatrixBuffer(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT3 lightDirection)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;
	//LightBufferType* dataPtr2;


	// Transpose the matrices to prepare them for the shader.
	//D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	//D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	//D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// Lock the matrix constant buffer so it can be written to.
	result = m_context->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the matrix constant buffer.
	m_context->Unmap(m_matrixBuffer, 0);

	// Set the position of the matrix constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the matrix constant buffer in the vertex shader with the updated values.
	m_context->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// The texture array is set here, it contains two textures:
	// The first texture is the color texture and 
	// the second texture is the normal map.

	// Set shader texture array resource in the pixel shader.
	//deviceContext->PSSetShaderResources(0, 5, textureArray);

	// The light buffer in the pixel shader is then set with the diffuse light color and light direction.
	// Lock the light constant buffer so it can be written to.
	/*result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
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
	deviceContext->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);*/

	return true;
}


// RenderShader draws the model using the bump map shader.
void Shader::RenderShader(int indexCount)
{
	// Set the vertex input layout.
	m_context->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	m_context->VSSetShader(m_vertexShader, NULL, 0);
	m_context->PSSetShader(m_pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	m_context->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangles.
	m_context->DrawIndexed(indexCount, 0, 0);

	return;
}