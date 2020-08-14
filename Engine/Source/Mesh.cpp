////////////////////////////////////////////////////////////////////////////////
// Filename: Mesh.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/Mesh.h"

Mesh::Mesh()
{
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	m_pTexture = nullptr;
}

Mesh::Mesh(const Mesh& other)
{
	m_pScene = other.m_pScene;
	m_pVertexBuffer = other.m_pVertexBuffer;
	m_pIndexBuffer = other.m_pIndexBuffer;
	m_pTexture = other.m_pTexture;
	m_vVertices = other.m_vVertices;
	m_vIndices = other.m_vIndices;
}


Mesh::~Mesh()
{
}

Mesh::Mesh(ID3D11Device* device, vector<VertexTypeTexNorm> vertices, vector<unsigned int> indices, Texture* texture)
{
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	m_pTexture = nullptr;

	m_vVertices = vertices;
	m_vIndices = indices;
	m_pTexture = texture;

	InitialiseMesh(device);
}

Mesh::Mesh(ID3D11Device* device, Texture* texture, char* MeshFilename, float size)
{
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	m_pTexture = nullptr;

	Initialise(device, texture, MeshFilename, size);
}

bool Mesh::Initialise(ID3D11Device* device, Texture* texture, char* MeshFilename, float size)
{
	bool result;
	Assimp::Importer imp;
	 m_pScene = imp.ReadFile(MeshFilename, aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_PreTransformVertices |
		aiProcess_CalcTangentSpace |
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
		aiProcess_FixInfacingNormals |
		aiProcess_FindInvalidData |
		aiProcess_ValidateDataStructure);

	if (m_pScene == nullptr)
	{
		return false;
	}

	//load in the Mesh data
	//initialize the vertex and index buffers
	result = InitialiseMesh(device, m_pScene->mRootNode, size);
	if (!result)
	{
		return false;
	}

	m_pTexture = texture;

	return true;
}

void Mesh::Release()
{
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
}

void Mesh::Render(ID3D11DeviceContext* deviceContext)
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

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); 

	
}

int Mesh::GetIndexCount()
{
	return m_vIndices.size();
}

ID3D11ShaderResourceView* Mesh::GetTexture()
{
	return m_pTexture->GetTextureShaderResourceView();
}

XMMATRIX Mesh::GetWorldMat()
{
	return m_WorldMatrix;
}

void Mesh::SetTexture(Texture* texture)
{
	m_pTexture = texture;
}

void Mesh::SetWorldMat(XMMATRIX world)
{
	m_WorldMatrix = world;
}

bool Mesh::InitialiseMesh(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexTypeTexNorm) * m_vVertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = &m_vVertices[0];
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
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * m_vIndices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = &m_vIndices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool Mesh::InitialiseMesh(ID3D11Device* device, aiNode* node, const float& size)
{
	//get the first mesh of the node
	aiMesh* mesh = m_pScene->mMeshes[node->mMeshes[0]];

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_vVertices.resize(mesh->mNumVertices);;

	// Read in the vertex data.
	for (int i = 0; i < m_vVertices.size(); i++)
	{
		//fin >> x >> y >> z;
		m_vVertices[i].position.x = mesh->mVertices[i].x;
		m_vVertices[i].position.y = mesh->mVertices[i].y;
		m_vVertices[i].position.z = mesh->mVertices[i].z;


		m_vVertices[i].position.x /= size;
		m_vVertices[i].position.y /= size;
		m_vVertices[i].position.z /= size;

		if (mesh->mTextureCoords[0])
		{
			m_vVertices[i].texture.x = mesh->mTextureCoords[0][i].x;
			m_vVertices[i].texture.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			m_vVertices[i].texture.x = 0;
			m_vVertices[i].texture.y = 0;
		}

		if (mesh->HasNormals())
		{
			m_vVertices[i].normal.x = mesh->mNormals[i].x;
			m_vVertices[i].normal.y = mesh->mNormals[i].y;
			m_vVertices[i].normal.z = mesh->mNormals[i].z;
		}
	}

	//Get indices
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			m_vIndices.push_back(face.mIndices[j]);
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexTypeTexNorm) * m_vVertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = &m_vVertices[0];
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
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * m_vIndices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = &m_vIndices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


