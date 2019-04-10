////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Terrain.h"


Terrain::Terrain() :
	m_perlinImpNoise()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_heightMap = 0;
	m_terrainGeneratedToggle = false;

	for (TextureClass* t : m_Textures)
	{
		t = 0;
	}
}


Terrain::Terrain(const Terrain& other)
{
}


Terrain::~Terrain()
{
	ModelShutdown();
}




bool Terrain::Initialize(ID3D11Device* device, int terrainWidth, int terrainHeight,
	WCHAR* lowestTexFile, WCHAR* lowTexFile, WCHAR* highTexFile, WCHAR* highestTexFile)
{
	int index;
	float height = 0.0;
	bool result;
	m_device = device;
	m_Textures.reserve(4);

	// Commented randomisation for testing purposes
	m_perlinImpNoise.InitialisePerlin(/*m_perlinImpNoise.RollUInt()*/);


	// Save the dimensions of the terrain.
	m_terrainWidth = terrainWidth;
	m_terrainHeight = terrainHeight;

	// Create the structure to hold the terrain data.
	m_heightMap = new HeightMapType[m_terrainWidth * m_terrainHeight];
	if (!m_heightMap) { return false; }

	m_pastMap = new HeightMapType[m_terrainWidth * m_terrainHeight];
	if (!m_pastMap) { return false; }

	float invWidth = 1.0f / m_terrainWidth;
	float invHeight = 1.0f / m_terrainHeight;

	// Initialise the data in the height map (flat).
	for (int j = 0; j < m_terrainHeight; j++)
	{
		for (int i = 0; i < m_terrainWidth; i++)
		{
			index = (m_terrainHeight * j) + i;

			m_heightMap[index].x = (float)i;
			m_heightMap[index].y = (float)height;
			m_heightMap[index].z = (float)j;

			m_heightMap[index].tu = (float)i * invWidth; //(float)(i % 1024) / 1024.0f;
			m_heightMap[index].tv = (float)j * invHeight; //(float)(j % 1024) / 1024.0f;

			m_pastMap[index].x = m_heightMap[index].x;
			m_pastMap[index].y = m_heightMap[index].y;
			m_pastMap[index].z = m_heightMap[index].z;

		}
	}

	// Normalize the height of the height map.
	NormalizeHeightMap();

	//even though we are generating a flat terrain, we still need to normalise it. 
	// Calculate the normals for the terrain data.
	result = CalculateNormals();
	if (!result) { return false; }


	result = LoadTexture(device, lowestTexFile, LOWEST);
	if (!result) { return false; }
	result = LoadTexture(device, lowTexFile, LOW);
	if (!result) { return false; }
	result = LoadTexture(device, highTexFile, HIGH);
	if (!result) { return false; }
	result = LoadTexture(device, highestTexFile, HIGHEST);
	if (!result) { return false; }




	// Initialize the vertex and index buffer that hold the geometry for the terrain.
	result = InitializeBuffers(device);
	if (!result) { return false; }

	return true;
}





void Terrain::ModelShutdown()
{
	// Release the vertex and index buffer.
	ShutdownBuffers();

	// Release the height map data.
	ShutdownHeightMap();

	ReleaseTextures();
}



ID3D11ShaderResourceView* Terrain::GetTexture(Terrain::TEXID id)
{
	return m_Textures[id]->GetTexture();
}


bool Terrain::LoadTexture(ID3D11Device* device, WCHAR* filename, Terrain::TEXID id)
{
	bool result;


	// Create the texture object.
	//std::vector<TextureClass*>::allocator_type texAlloc = m_Textures.get_allocator();
	//TextureClass* tex = new TextureClass;
	m_Textures.emplace_back(new TextureClass);
	//m_Textures[id] = new TextureClass;
	if (!m_Textures[id])
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Textures[id]->Initialize(device, filename);
	if (!result) { return false; }

	return true;
}




void Terrain::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int Terrain::GetIndexCount()
{
	return m_indexCount;
}


int Terrain::RandBetween(int min, int max)
{
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(generator);
}

float Terrain::Rand01()
{
	return ((float)rand() / (RAND_MAX)) + 1;
}

bool Terrain::GenerateHeightMap(double scaling, double zoom)
{

	bool result;
	//the toggle is just a bool that I use to make sure this is only called ONCE when you press a key
	//until you release the key and start again. We dont want to be generating the terrain 500
	//times per second. 
	//if (keydown && (!m_terrainGeneratedToggle))
	//{
	int index;
	float height = 0.0;

	double maxHeight = -DBL_MAX;
	double minHeight = DBL_MAX;

	//loop through the terrain and set the heights how we want. This is where we generate the terrain
	//in this case I will run a sin-wave through the terrain in one axis.

	for (int j = 0; j < m_terrainHeight; j++)
	{
		for (int i = 0; i < m_terrainWidth; i++)
		{
			index = (m_terrainHeight * j) + i;
			double z = (double)m_heightMap[index].z * zoom * (1.0 / (double)m_terrainHeight);
			double y = (double)m_heightMap[index].y * zoom;
			double x = (double)m_heightMap[index].x * zoom * (1.0 / (double)m_terrainWidth);

			double k[] = { 1.0, 2.0, 4.0 };
			double n = 0.0;
			for (double d : k)
			{
				n += (1.0 / d) * m_perlinImpNoise.noise(d*x, d*y, d*z);
			}

			double e = (n * scaling);

			if (e > maxHeight) maxHeight = e;
			if (e < minHeight) minHeight = e;

			m_heightMap[index].x = (float)i;
			m_heightMap[index].y = (float)e;
			m_heightMap[index].z = (float)j;


		}
	}

	m_maxHeight = maxHeight;
	m_minHeight = minHeight;

	CalculateBlendings();

	result = CalculateNormals();
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffer that hold the geometry for the terrain.
	result = InitializeBuffers(m_device);
	if (!result) { return false; }

	//	m_terrainGeneratedToggle = true;
	//}
	//else
	//{
	//	m_terrainGeneratedToggle = false;
	//}




	return true;
}


bool Terrain::Smooth()
{

	int i, j;
	bool result;
	//if (keyDown)
	//{
	for (j = 0; j < m_terrainHeight; j++)
	{
		for (i = 0; i < m_terrainWidth; i++)
		{
			m_pastMap[(m_terrainHeight * j) + i].x = m_heightMap[(m_terrainHeight * j) + i].x;
			m_pastMap[(m_terrainHeight * j) + i].y = m_heightMap[(m_terrainHeight * j) + i].y;
			m_pastMap[(m_terrainHeight * j) + i].z = m_heightMap[(m_terrainHeight * j) + i].z;
		}
	}

	for (j = 0; j < m_terrainHeight; j++)
	{
		for (i = 0; i < m_terrainWidth; i++)
		{
			//m_heightMap[(m_terrainHeight * j) + i].y = m_pastMap[(m_terrainHeight * j) + i].y;
			m_heightMap[(m_terrainHeight * j) + i].y += m_pastMap[(m_terrainHeight * j) + i - 1].y;
			m_heightMap[(m_terrainHeight * j) + i].y += m_pastMap[(m_terrainHeight * j) + i + 1].y;

			m_heightMap[(m_terrainHeight * j) + i].y += m_pastMap[(m_terrainHeight * j + 1) + i].y;
			m_heightMap[(m_terrainHeight * j) + i].y += m_pastMap[(m_terrainHeight * j + 1) + i - 1].y;
			m_heightMap[(m_terrainHeight * j) + i].y += m_pastMap[(m_terrainHeight * j + 1) + i + 1].y;

			m_heightMap[(m_terrainHeight * j) + i].y += m_pastMap[(m_terrainHeight * j - 1) + i].y;
			m_heightMap[(m_terrainHeight * j) + i].y += m_pastMap[(m_terrainHeight * j - 1) + i - 1].y;
			m_heightMap[(m_terrainHeight * j) + i].y += m_pastMap[(m_terrainHeight * j - 1) + i + 1].y;

			m_heightMap[(m_terrainHeight * j) + i].y /= 9.0f;
		}
	}

	result = CalculateNormals();
	if (!result) { return false; }

	// Initialize the vertex and index buffer that hold the geometry for the terrain.
	result = InitializeBuffers(m_device);
	if (!result) { return false; }

	m_terrainGeneratedToggle = true;
	//}

	return true;

}

XMFLOAT4 Terrain::TextureBlendingByHeight(HeightMapType h)
{
	return TextureBlendingByHeight(h.y);
}


XMFLOAT4 Terrain::TextureBlendingByHeight(float h)
{
	XMFLOAT4 blending;
	float e = (h - m_minHeight) / (m_maxHeight - m_minHeight);

	// Thresholds: for 4 textures, 6 thresholds are needed for the 7 zones:
	// only colour1, colour1 & colour2 blend,
	// only colour2, colour2 & colour3 blend,
	// only colour3, colour3 & colour4 blend,
	// only colour4
	// From Highest to lowest:
	//			c1 | c1&2 | c2 | c2&3 | c3 | c3&4 | c4 
	float th[] = { 0.9f, 0.7f, 0.6f, 0.4f, 0.3f, 0.1f };
	

	if (e >= th[0])
	{
		blending = XMFLOAT4{ 1.0f, 0.0f, 0.0f, 0.0f };
	}
	else if (e < th[0] && e >= th[1])
	{
		float a = (e - th[1]) / (th[0] - th[1]);
		blending = XMFLOAT4{ a, 1.0f - a, 0.0f, 0.0f };
	}
	else if (e < th[1] && e >= th[2])
	{
		blending = XMFLOAT4{ 0.0f, 1.0f, 0.0f, 0.0f };
	}
	else if (e < th[2] && e >= th[3])
	{
		float a = (e - th[3]) / (th[2] - th[3]);
		blending = XMFLOAT4{ 0.0f, a, 1.0f - a, 0.0f };
	}
	else if (e < th[3] && e >= th[4])
	{
		blending = XMFLOAT4{ 0.0f, 0.0f, 1.0f, 0.0f };
	}
	else if (e < th[4] && e >= th[5])
	{
		float a = (e - th[5]) / (th[4] - th[5]);
		blending = XMFLOAT4{ 0.0f, 0.0f, a, 1.0f - a };
	}
	else if (e < th[5])
	{
		blending = XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f };
	}

	
	return blending;
}

void Terrain::CalculateBlendings()
{
	
	int index;
	for (int j = 0; j < m_terrainHeight; j++)
	{
		for (int i = 0; i < m_terrainWidth; i++)
		{
			index = (m_terrainHeight * j) + i;
			m_heightMap[index].blending = TextureBlendingByHeight(m_heightMap[index]);
		}
	}
}


void Terrain::NormalizeHeightMap()
{
	int i, j;


	for (j = 0; j < m_terrainHeight; j++)
	{
		for (i = 0; i < m_terrainWidth; i++)
		{
			m_heightMap[(m_terrainHeight * j) + i].y /= 15.0f;
		}
	}

	return;
}


bool Terrain::CalculateNormals()
{
	int i, j, index1, index2, index3, index, count;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	VectorType* normals;


	// Create a temporary array to hold the un-normalized normal vectors.
	normals = new VectorType[(m_terrainHeight - 1) * (m_terrainWidth - 1)];
	if (!normals)
	{
		return false;
	}

	// Go through all the faces in the mesh and calculate their normals.
	for (j = 0; j < (m_terrainHeight - 1); j++)
	{
		for (i = 0; i < (m_terrainWidth - 1); i++)
		{
			index1 = (j * m_terrainHeight) + i;
			index2 = (j * m_terrainHeight) + (i + 1);
			index3 = ((j + 1) * m_terrainHeight) + i;

			// Get three vertices from the face.
			vertex1[0] = m_heightMap[index1].x;
			vertex1[1] = m_heightMap[index1].y;
			vertex1[2] = m_heightMap[index1].z;

			vertex2[0] = m_heightMap[index2].x;
			vertex2[1] = m_heightMap[index2].y;
			vertex2[2] = m_heightMap[index2].z;

			vertex3[0] = m_heightMap[index3].x;
			vertex3[1] = m_heightMap[index3].y;
			vertex3[2] = m_heightMap[index3].z;

			// Calculate the two vectors for this face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (m_terrainHeight - 1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
		}
	}

	// Now go through all the vertices and take an average of each face normal 	
	// that the vertex touches to get the averaged normal for that vertex.
	for (j = 0; j < m_terrainHeight; j++)
	{
		for (i = 0; i < m_terrainWidth; i++)
		{
			// Initialize the sum.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Initialize the count.
			count = 0;

			// Bottom left face.
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_terrainHeight - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Bottom right face.
			if ((i < (m_terrainWidth - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_terrainHeight - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper left face.
			if (((i - 1) >= 0) && (j < (m_terrainHeight - 1)))
			{
				index = (j * (m_terrainHeight - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper right face.
			if ((i < (m_terrainWidth - 1)) && (j < (m_terrainHeight - 1)))
			{
				index = (j * (m_terrainHeight - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Take the average of the faces touching this vertex.
			sum[0] = (sum[0] / (float)count);
			sum[1] = (sum[1] / (float)count);
			sum[2] = (sum[2] / (float)count);

			// Calculate the length of this normal.
			length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

			// Get an index to the vertex location in the height map array.
			index = (j * m_terrainHeight) + i;

			// Normalize the final shared normal for this vertex and store it in the height map array.
			m_heightMap[index].nx = (sum[0] / length);
			m_heightMap[index].ny = (sum[1] / length);
			m_heightMap[index].nz = (sum[2] / length);
		}
	}

	// Release the temporary normals.
	delete[] normals;
	normals = 0;

	return true;
}

void Terrain::CalculateTextureCoordinates()
{
	int incrementCount, i, j, tuCount, tvCount;
	float incrementValue, tuCoordinate, tvCoordinate;


	// Calculate how much to increment the texture coordinates by.
	incrementValue = (float)TEXTURE_REPEAT / (float)m_terrainWidth;

	// Calculate how many times to repeat the texture.
	incrementCount = m_terrainWidth / TEXTURE_REPEAT;

	// Initialize the tu and tv coordinate values.
	tuCoordinate = 0.0f;
	tvCoordinate = 1.0f;

	// Initialize the tu and tv coordinate indexes.
	tuCount = 0;
	tvCount = 0;

	// Loop through the entire height map and calculate the tu and tv texture coordinates for each vertex.
	for (j = 0; j < m_terrainHeight; j++)
	{
		for (i = 0; i < m_terrainWidth; i++)
		{
			// Store the texture coordinate in the height map.
			m_heightMap[(m_terrainHeight * j) + i].tu = tuCoordinate;
			m_heightMap[(m_terrainHeight * j) + i].tv = tvCoordinate;

			// Increment the tu texture coordinate by the increment value and increment the index by one.
			tuCoordinate += incrementValue;
			tuCount++;

			// Check if at the far right end of the texture and if so then start at the beginning again.
			if (tuCount == incrementCount)
			{
				tuCoordinate = 0.0f;
				tuCount = 0;
			}
		}

		// Increment the tv texture coordinate by the increment value and increment the index by one.
		tvCoordinate -= incrementValue;
		tvCount++;

		// Check if at the top of the texture and if so then start at the bottom again.
		if (tvCount == incrementCount)
		{
			tvCoordinate = 1.0f;
			tvCount = 0;
		}
	}

	return;
}





void Terrain::ShutdownHeightMap()
{
	if (m_heightMap)
	{
		delete[] m_heightMap;
		m_heightMap = 0;
	}

	if (m_pastMap)
	{
		delete[] m_pastMap;
		m_pastMap = 0;
	}

	return;
}

void Terrain::ReleaseTextures()
{
	for (TextureClass* tex : m_Textures)
	{
		// Release the texture object.
		if (tex)
		{
			tex->Shutdown();
			delete tex;
			tex = nullptr;
		}
	}
}


bool Terrain::InitializeBuffers(ID3D11Device* device)
{
	ShutdownBuffers();

	VertexType* vertices;
	unsigned long* indices;
	int index, i, j;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int index1, index2, index3, index4;


	// Calculate the number of vertices in the terrain mesh.
	m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 6;

	// Set the index count to the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices) { return false; }

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices) { return false; }

	// Initialize the index to the vertex buffer.
	index = 0;

	// Load the vertex and index array with the terrain data.
	for (j = 0; j < (m_terrainHeight - 1); j++)
	{
		for (i = 0; i < (m_terrainWidth - 1); i++)
		{
			index1 = (m_terrainHeight * j) + i;          // Bottom left.
			index2 = (m_terrainHeight * j) + (i + 1);      // Bottom right.
			index3 = (m_terrainHeight * (j + 1)) + i;      // Upper left.
			index4 = (m_terrainHeight * (j + 1)) + (i + 1);  // Upper right.

			// Upper left.
			vertices[index].position = XMFLOAT3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
			vertices[index].normal = XMFLOAT3(m_heightMap[index3].nx, m_heightMap[index3].ny, m_heightMap[index3].nz);
			vertices[index].uv = XMFLOAT2(m_heightMap[index3].tu, m_heightMap[index3].tv);
			vertices[index].blending = m_heightMap[index3].blending;
			indices[index] = index;
			index++;

			// Upper right.
			vertices[index].position = XMFLOAT3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
			vertices[index].normal = XMFLOAT3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);
			vertices[index].uv = XMFLOAT2(m_heightMap[index4].tu, m_heightMap[index4].tv);
			vertices[index].blending = m_heightMap[index4].blending;
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = XMFLOAT3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
			vertices[index].normal = XMFLOAT3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);
			vertices[index].uv = XMFLOAT2(m_heightMap[index1].tu, m_heightMap[index1].tv);
			vertices[index].blending = m_heightMap[index1].blending;
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = XMFLOAT3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
			vertices[index].normal = XMFLOAT3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);
			vertices[index].uv = XMFLOAT2(m_heightMap[index1].tu, m_heightMap[index1].tv);
			vertices[index].blending = m_heightMap[index1].blending;
			indices[index] = index;
			index++;

			// Upper right.
			vertices[index].position = XMFLOAT3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
			vertices[index].normal = XMFLOAT3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);
			vertices[index].uv = XMFLOAT2(m_heightMap[index4].tu, m_heightMap[index4].tv);
			vertices[index].blending = m_heightMap[index4].blending;
			indices[index] = index;
			index++;

			// Bottom right.
			vertices[index].position = XMFLOAT3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
			vertices[index].normal = XMFLOAT3(m_heightMap[index2].nx, m_heightMap[index2].ny, m_heightMap[index2].nz);
			vertices[index].uv = XMFLOAT2(m_heightMap[index2].tu, m_heightMap[index2].tv);
			vertices[index].blending = m_heightMap[index2].blending;
			indices[index] = index;
			index++;
		}
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
	if (FAILED(result))
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
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void Terrain::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void Terrain::RenderBuffers(ID3D11DeviceContext* deviceContext)
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