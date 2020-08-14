////////////////////////////////////////////////////////////////////////////////
// Filename: Terrain.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/Terrain.h"


Terrain::Terrain()
{
	m_pVertexBuffer = 0;
	m_pIndexBuffer = 0;
	m_cpTerrainFilename = 0;
	m_pHeightMap = 0;
	m_pTerrainModel = 0;
}


Terrain::Terrain(const Terrain& other)
{
}


Terrain::~Terrain()
{
}


bool Terrain::Initialize(ID3D11Device* device, Texture* texture, char* setupFilename)
{
	bool result;


	// Get the terrain filename, dimensions, and so forth from the setup file.
	result = LoadSetupFile(setupFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the terrain height map with the data from the bitmap file.
	result = LoadBitmapHeightMap();
	if (!result)
	{
		return false;
	}

	// Setup the X and Z coordinates for the height map as well as scale the terrain height by the height scale value.
	SetTerrainCoordinates();

	// Calculate the normals for the terrain data.
	result = CalculateNormals();
	if (!result)
	{
		return false;
	}

	// Load in the color map for the terrain.
	result = LoadColourMap();
	if (!result)
	{
		return false;
	}

	// Now build the 3D model of the terrain.
	result = BuildTerrainModel();
	if (!result)
	{
		return false;
	}

	// We can now release the height map since it is no longer needed in memory once the 3D terrain model has been built.
	SAFE_DELETEA(m_pHeightMap);

	// Load the rendering buffers with the terrain data.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Release the terrain model now that the rendering buffers have been loaded.
	SAFE_DELETEA(m_pTerrainModel);

	m_pTexture = texture;

	return true;
}


void Terrain::Release()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_DELETEA(m_pTerrainModel);
	SAFE_DELETEA(m_pHeightMap);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexBuffer);

	return;
}


bool Terrain::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return true;
}


int Terrain::GetIndexCount()
{
	return m_iIndexCount;
}

void Terrain::AssignTexture(Texture* texture)
{
	m_pTexture = texture;
}

ID3D11ShaderResourceView* Terrain::GetTexture()
{
	return m_pTexture->GetTextureShaderResourceView();
}

bool Terrain::LoadSetupFile(char* filename)
{
	int stringLength;
	ifstream fin;
	char input;

	// Initialize the string that will hold the terrain file name.
	stringLength = 256;
	m_cpTerrainFilename = new char[stringLength];
	if (!m_cpTerrainFilename)
	{
		return false;
	}

	m_cpColourMapFilename = new char[stringLength];
	if (!m_cpColourMapFilename)
	{
		return false;
	}

	// Open the setup file.  If it could not open the file then exit.
	fin.open(filename);
	
	if (fin.fail())
	{
		return false;
	}

	// Read up to the terrain file name.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the terrain file name.
	fin >> m_cpTerrainFilename;

	// Read up to the value of terrain height.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the terrain height.
	fin >> m_iTerrainHeight;

	// Read up to the value of terrain width.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the terrain width.
	fin >> m_iTerrainWidth;

	// Read up to the value of terrain height scaling.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the terrain height scaling.
	fin >> m_fHeightScale;

	// Read up to the value of terrain height scaling.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the color map file name.
	fin >> m_cpColourMapFilename;

	// Close the setup file.
	fin.close();

	return true;
}

bool Terrain::LoadBitmapHeightMap()
{
	int error, imageSize, i, j, k, index;
	FILE* filePtr;
	unsigned long long count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	unsigned char* bitmapImage;
	unsigned char height;


	// Start by creating the array structure to hold the height map data.
	m_pHeightMap = new HeightMapType[m_iTerrainWidth * m_iTerrainHeight];
	if (!m_pHeightMap)
	{
		return false;
	}

	// Open the bitmap map file in binary.
	error = fopen_s(&filePtr, m_cpTerrainFilename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the bitmap file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Make sure the height map dimensions are the same as the terrain dimensions for easy 1 to 1 mapping.
	if ((bitmapInfoHeader.biHeight != m_iTerrainHeight) || (bitmapInfoHeader.biWidth != m_iTerrainWidth))
	{
		return false;
	}

	// Calculate the size of the bitmap image data.  
	// Since we use non-divide by 2 dimensions (eg. 257x257) we need to add an extra byte to each line.
	imageSize = m_iTerrainHeight * ((m_iTerrainWidth * 3) + 1);

	// Allocate memory for the bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Initialize the position in the image data buffer.
	k = 0;

	// Read the image data into the height map array.
	for (j = 0; j < m_iTerrainHeight; j++)
	{
		for (i = 0; i < m_iTerrainWidth; i++)
		{
			// Bitmaps are upside down so load bottom to top into the height map array.
			index = (m_iTerrainWidth * (m_iTerrainHeight - 1 - j)) + i;

			// Get the grey scale pixel value from the bitmap image data at this location.
			height = bitmapImage[k];

			// Store the pixel value as the height at this point in the height map array.
			m_pHeightMap[index].y = (float)height;

			// Increment the bitmap image data index.
			k += 3;
		}

		// Compensate for the extra byte at end of each line in non-divide by 2 bitmaps (eg. 257x257).
		k++;
	}

	// Release the bitmap image data now that the height map array has been loaded.
	delete[] bitmapImage;
	bitmapImage = 0;

	// Release the terrain filename now that is has been read in.
	delete[] m_cpTerrainFilename;
	m_cpTerrainFilename = 0;

	return true;
}

void Terrain::SetTerrainCoordinates()
{
	int i, j, index;


	// Loop through all the elements in the height map array and adjust their coordinates correctly.
	for (j = 0; j < m_iTerrainHeight; j++)
	{
		for (i = 0; i < m_iTerrainWidth; i++)
		{
			index = (m_iTerrainWidth * j) + i;

			// Set the X and Z coordinates.
			m_pHeightMap[index].x = (float)i;
			m_pHeightMap[index].z = -(float)j;

			// Move the terrain depth into the positive range.  For example from (0, -256) to (256, 0).
			m_pHeightMap[index].z += (float)(m_iTerrainHeight - 1);

			// Scale the height.
			m_pHeightMap[index].y /= m_fHeightScale;
		}
	}
}

bool Terrain::CalculateNormals()
{
	int i, j, UpperLeft, UpperRight, BottomLeft, index;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	VectorType* normals;


	// Create a temporary array to hold the face normal vectors.
	normals = new VectorType[(m_iTerrainHeight - 1) * (m_iTerrainWidth - 1)];
	if (!normals)
	{
		return false;
	}

	// Go through all the faces in the mesh and calculate their normals.
	for (j = 0; j < (m_iTerrainHeight - 1); j++)
	{
		for (i = 0; i < (m_iTerrainWidth - 1); i++)
		{
			UpperLeft = ((j + 1) * m_iTerrainWidth) + i;      // Bottom left vertex.
			UpperRight = ((j + 1) * m_iTerrainWidth) + (i + 1);  // Bottom right vertex.
			BottomLeft = (j * m_iTerrainWidth) + i;          // Upper left vertex.

			// Get three vertices from the face.
			vertex1[0] = m_pHeightMap[UpperLeft].x;
			vertex1[1] = m_pHeightMap[UpperLeft].y;
			vertex1[2] = m_pHeightMap[UpperLeft].z;

			vertex2[0] = m_pHeightMap[UpperRight].x;
			vertex2[1] = m_pHeightMap[UpperRight].y;
			vertex2[2] = m_pHeightMap[UpperRight].z;

			vertex3[0] = m_pHeightMap[BottomLeft].x;
			vertex3[1] = m_pHeightMap[BottomLeft].y;
			vertex3[2] = m_pHeightMap[BottomLeft].z;

			// Calculate the two vectors for this face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (m_iTerrainWidth - 1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);

			// Calculate the length.
			length = (float)sqrt((normals[index].x * normals[index].x) + (normals[index].y * normals[index].y) +
				(normals[index].z * normals[index].z));

			// Normalize the final value for this face using the length.
			normals[index].x = (normals[index].x / length);
			normals[index].y = (normals[index].y / length);
			normals[index].z = (normals[index].z / length);
		}
	}

	// Now go through all the vertices and take a sum of the face normals that touch this vertex.
	for (j = 0; j < m_iTerrainHeight; j++)
	{
		for (i = 0; i < m_iTerrainWidth; i++)
		{
			// Initialize the sum.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Bottom left face.
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_iTerrainWidth - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
			}

			// Bottom right face.
			if ((i < (m_iTerrainWidth - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_iTerrainWidth - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
			}

			// Upper left face.
			if (((i - 1) >= 0) && (j < (m_iTerrainHeight - 1)))
			{
				index = (j * (m_iTerrainWidth - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
			}

			// Upper right face.
			if ((i < (m_iTerrainWidth - 1)) && (j < (m_iTerrainHeight - 1)))
			{
				index = (j * (m_iTerrainWidth - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
			}

			// Calculate the length of this normal.
			length = (float)sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

			// Get an index to the vertex location in the height map array.
			index = (j * m_iTerrainWidth) + i;

			// Normalize the final shared normal for this vertex and store it in the height map array.
			m_pHeightMap[index].nx = (sum[0] / length);
			m_pHeightMap[index].ny = (sum[1] / length);
			m_pHeightMap[index].nz = (sum[2] / length);
		}
	}

	// Release the temporary normals.
	delete[] normals;
	normals = 0;

	return true;
}

bool Terrain::LoadColourMap()
{
	int error, imageSize, i, j, k, index;
	FILE* filePtr;
	unsigned long long count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	unsigned char* bitmapImage;


	// Open the color map file in binary.
	error = fopen_s(&filePtr, m_cpColourMapFilename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Make sure the color map dimensions are the same as the terrain dimensions for easy 1 to 1 mapping.
	if ((bitmapInfoHeader.biWidth != m_iTerrainWidth) || (bitmapInfoHeader.biHeight != m_iTerrainHeight))
	{
		return false;
	}

	// Calculate the size of the bitmap image data.  Since this is non-divide by 2 dimensions (eg. 257x257) need to add extra byte to each line.
	imageSize = m_iTerrainHeight * ((m_iTerrainWidth * 3) + 1);

	// Allocate memory for the bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Initialize the position in the image data buffer.
	k = 0;

	// Read the image data into the color map portion of the height map structure.
	for (j = 0; j < m_iTerrainHeight; j++)
	{
		for (i = 0; i < m_iTerrainWidth; i++)
		{
			// Bitmaps are upside down so load bottom to top into the array.
			index = (m_iTerrainWidth * (m_iTerrainHeight - 1 - j)) + i;

			//m_pHeightMap[index].b = (float)bitmapImage[k] / 255.0f;
			//m_pHeightMap[index].g = (float)bitmapImage[k + 1] / 255.0f;
			//m_pHeightMap[index].r = (float)bitmapImage[k + 2] / 255.0f;

			k += 3;
		}

		// Compensate for extra byte at end of each line in non-divide by 2 bitmaps (eg. 257x257).
		k++;
	}

	// Release the bitmap image data.
	SAFE_DELETEA(bitmapImage);

	// Release the color map filename now that is has been read in.
	SAFE_DELETEA(m_cpColourMapFilename);

	return true;
}

bool Terrain::BuildTerrainModel()
{
	int i, j, index, UpperLeft, UpperRight, BottomLeft, BottomRight;


	// Calculate the number of vertices in the 3D terrain model.
	m_iVertexCount = (m_iTerrainHeight - 1) * (m_iTerrainWidth - 1) * 6;

	// Create the 3D terrain model array.
	m_pTerrainModel = new ModelType[m_iVertexCount];
	if (!m_pTerrainModel)
	{
		return false;
	}

	// Initialize the index into the height map array.
	index = 0;

	// Load the 3D terrain model with the height map terrain data.
	// We will be creating 2 triangles for each of the four points in a quad.
	for (j = 0; j < (m_iTerrainHeight - 1); j++)
	{
		for (i = 0; i < (m_iTerrainWidth - 1); i++)
		{
			// Get the indexes to the four points of the quad.
			UpperLeft = (m_iTerrainWidth * j) + i;          // Upper left.
			UpperRight = (m_iTerrainWidth * j) + (i + 1);      // Upper right.
			BottomLeft = (m_iTerrainWidth * (j + 1)) + i;      // Bottom left.
			BottomRight = (m_iTerrainWidth * (j + 1)) + (i + 1);  // Bottom right.

			// Now create two triangles for that quad.
			// Triangle 1 - Upper left.
			m_pTerrainModel[index].x = m_pHeightMap[UpperLeft].x;
			m_pTerrainModel[index].y = m_pHeightMap[UpperLeft].y;
			m_pTerrainModel[index].z = m_pHeightMap[UpperLeft].z;
			/*m_pTerrainModel[index].r = m_pHeightMap[UpperLeft].r;
			m_pTerrainModel[index].g = m_pHeightMap[UpperLeft].g;
			m_pTerrainModel[index].b = m_pHeightMap[UpperLeft].b;*/
			m_pTerrainModel[index].tu = 0.0f;
			m_pTerrainModel[index].tv = 0.0f;
			m_pTerrainModel[index].nx = m_pHeightMap[UpperLeft].nx;
			m_pTerrainModel[index].ny = m_pHeightMap[UpperLeft].ny;
			m_pTerrainModel[index].nz = m_pHeightMap[UpperLeft].nz;
			index++;

			// Triangle 1 - Upper right.
			m_pTerrainModel[index].x = m_pHeightMap[UpperRight].x;
			m_pTerrainModel[index].y = m_pHeightMap[UpperRight].y;
			m_pTerrainModel[index].z = m_pHeightMap[UpperRight].z;
			/*m_pTerrainModel[index].r = m_pHeightMap[UpperRight].r;
			m_pTerrainModel[index].g = m_pHeightMap[UpperRight].g;
			m_pTerrainModel[index].b = m_pHeightMap[UpperRight].b;*/
			m_pTerrainModel[index].tu = 1.0f;
			m_pTerrainModel[index].tv = 0.0f;
			m_pTerrainModel[index].nx = m_pHeightMap[UpperRight].nx;
			m_pTerrainModel[index].ny = m_pHeightMap[UpperRight].ny;
			m_pTerrainModel[index].nz = m_pHeightMap[UpperRight].nz;
			index++;

			// Triangle 1 - Bottom left.
			m_pTerrainModel[index].x = m_pHeightMap[BottomLeft].x;
			m_pTerrainModel[index].y = m_pHeightMap[BottomLeft].y;
			m_pTerrainModel[index].z = m_pHeightMap[BottomLeft].z;
			/*m_pTerrainModel[index].r = m_pHeightMap[BottomLeft].r;
			m_pTerrainModel[index].g = m_pHeightMap[BottomLeft].g;
			m_pTerrainModel[index].b = m_pHeightMap[BottomLeft].b;*/
			m_pTerrainModel[index].tu = 0.0f;
			m_pTerrainModel[index].tv = 1.0f;
			m_pTerrainModel[index].nx = m_pHeightMap[BottomLeft].nx;
			m_pTerrainModel[index].ny = m_pHeightMap[BottomLeft].ny;
			m_pTerrainModel[index].nz = m_pHeightMap[BottomLeft].nz;
			index++;

			// Triangle 2 - Bottom left.
			m_pTerrainModel[index].x = m_pHeightMap[BottomLeft].x;
			m_pTerrainModel[index].y = m_pHeightMap[BottomLeft].y;
			m_pTerrainModel[index].z = m_pHeightMap[BottomLeft].z;
			/*m_pTerrainModel[index].r = m_pHeightMap[BottomLeft].r;
			m_pTerrainModel[index].g = m_pHeightMap[BottomLeft].g;
			m_pTerrainModel[index].b = m_pHeightMap[BottomLeft].b;*/
			m_pTerrainModel[index].tu = 0.0f;
			m_pTerrainModel[index].tv = 1.0f;
			m_pTerrainModel[index].nx = m_pHeightMap[BottomLeft].nx;
			m_pTerrainModel[index].ny = m_pHeightMap[BottomLeft].ny;
			m_pTerrainModel[index].nz = m_pHeightMap[BottomLeft].nz;
			index++;

			// Triangle 2 - Upper right.
			m_pTerrainModel[index].x = m_pHeightMap[UpperRight].x;
			m_pTerrainModel[index].y = m_pHeightMap[UpperRight].y;
			m_pTerrainModel[index].z = m_pHeightMap[UpperRight].z;
			/*m_pTerrainModel[index].r = m_pHeightMap[UpperRight].r;
			m_pTerrainModel[index].g = m_pHeightMap[UpperRight].g;
			m_pTerrainModel[index].b = m_pHeightMap[UpperRight].b;*/
			m_pTerrainModel[index].tu = 1.0f;
			m_pTerrainModel[index].tv = 0.0f;
			m_pTerrainModel[index].nx = m_pHeightMap[UpperRight].nx;
			m_pTerrainModel[index].ny = m_pHeightMap[UpperRight].ny;
			m_pTerrainModel[index].nz = m_pHeightMap[UpperRight].nz;
			index++;

			// Triangle 2 - Bottom right.
			m_pTerrainModel[index].x = m_pHeightMap[BottomRight].x;
			m_pTerrainModel[index].y = m_pHeightMap[BottomRight].y;
			m_pTerrainModel[index].z = m_pHeightMap[BottomRight].z;
			/*m_pTerrainModel[index].r = m_pHeightMap[BottomRight].r;
			m_pTerrainModel[index].g = m_pHeightMap[BottomRight].g;
			m_pTerrainModel[index].b = m_pHeightMap[BottomRight].b;*/
			m_pTerrainModel[index].tu = 1.0f;
			m_pTerrainModel[index].tv = 1.0f;
			m_pTerrainModel[index].nx = m_pHeightMap[BottomRight].nx;
			m_pTerrainModel[index].ny = m_pHeightMap[BottomRight].ny;
			m_pTerrainModel[index].nz = m_pHeightMap[BottomRight].nz;
			index++;
		}
	}

	return true;
}

bool Terrain::InitializeBuffers(ID3D11Device* device)
{
	VertexTypeTexNorm* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Calculate the number of vertices in the terrain.
	m_iVertexCount = (m_iTerrainWidth - 1) * (m_iTerrainHeight - 1) * 6;

	// Set the index count to the same as the vertex count.
	m_iIndexCount = m_iVertexCount;

	// Create the vertex array.
	vertices = new VertexTypeTexNorm[m_iVertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_iIndexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with 3D terrain model data.
	for (i = 0; i < m_iVertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_pTerrainModel[i].x, m_pTerrainModel[i].y, m_pTerrainModel[i].z);
		vertices[i].texture = XMFLOAT2(m_pTerrainModel[i].tu, m_pTerrainModel[i].tv);
		vertices[i].normal = XMFLOAT3(m_pTerrainModel[i].nx, m_pTerrainModel[i].ny, m_pTerrainModel[i].nz);
		//vertices[i].colour = XMFLOAT3(m_pTerrainModel[i].r, m_pTerrainModel[i].g, m_pTerrainModel[i].b);
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexTypeTexNorm) * m_iVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_iIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the buffers have been created and loaded.
	SAFE_DELETEA(vertices);
	SAFE_DELETEA(indices);

	return true;
}

void Terrain::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexTypeTexNorm);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case lines.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}