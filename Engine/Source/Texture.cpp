////////////////////////////////////////////////////////////////////////////////
// Filename: Texture.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/Texture.h"


Texture::Texture()
{
	m_cpTargaData = nullptr;
	m_pTexture = nullptr;
	m_pTextureView = nullptr;

	
}


Texture::Texture(const Texture& other)
{
	
}


Texture::~Texture()
{
}


bool Texture::InitializeDDSTextureFromFile(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	//char* to a wstring 
	std::wstring ws(&filename[0], &filename[strlen(filename)]);

	if (FAILED(CreateDDSTextureFromFile(device, deviceContext, ws.c_str(), (ID3D11Resource * *)& m_pTexture, &m_pTextureView)))
	{
		return false;
	}

	// Generate mipmaps for this texture.
	deviceContext->GenerateMips(m_pTextureView);

	return true;
}

bool Texture::InitializeWICTextureFromFile(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	//char* to a wstring 
	std::wstring ws(&filename[0], &filename[strlen(filename)]);

	if (FAILED(CreateWICTextureFromFile(device, deviceContext, ws.c_str(), (ID3D11Resource * *)& m_pTexture, &m_pTextureView)))
	{
		return false;
	}

	// Generate mipmaps for this texture.
	deviceContext->GenerateMips(m_pTextureView);

	return true;
}

bool Texture::InitializeTextureFromTarga(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;
	int height, width;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;


	// Load the targa image data into memory.
	result = LoadTarga(filename, height, width);
	if(!result)
	{
		return false;
	}

	// Setup the description of the texture.
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty texture.
	hResult = device->CreateTexture2D(&textureDesc, NULL, &m_pTexture);
	if(FAILED(hResult))
	{
		return false;
	}

	// Set the row pitch of the targa image data.
	rowPitch = (width * 4) * sizeof(unsigned char);

	// Copy the targa image data into the texture.
	deviceContext->UpdateSubresource(m_pTexture, 0, NULL, m_cpTargaData, rowPitch, 0);

	// Setup the shader resource view description.
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture.
	hResult = device->CreateShaderResourceView(m_pTexture, &srvDesc, &m_pTextureView);
	if(FAILED(hResult))
	{
		return false;
	}

	// Generate mipmaps for this texture.
	deviceContext->GenerateMips(m_pTextureView);

	// Release the targa image data now that the image data has been loaded into the texture.
	SAFE_DELETEA(m_cpTargaData);

	return true;
}


void Texture::Release()
{
	SAFE_RELEASE(m_pTextureView);
	SAFE_RELEASE(m_pTexture);
	SAFE_DELETEA(m_cpTargaData);
}

ID3D11Texture2D* Texture::GetTexture2D()
{
	return m_pTexture;
}


ID3D11ShaderResourceView* Texture::GetTextureShaderResourceView()
{
	return m_pTextureView;
}

bool Texture::LoadTarga(char* filename, int& height, int& width)
{
	int error, bpp, imageSize, index, i, j, k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;


	// Open the targa file for reading in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if(error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if(count != 1)
	{
		return false;
	}

	// Get the important information from the header.
	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	// Check that it is 32 bit and not 24 bit.
	if(bpp != 32)
	{
		return false;
	}

	// Calculate the size of the 32 bit image data.
	imageSize = width * height * 4;

	// Allocate memory for the targa image data.
	targaImage = new unsigned char[imageSize];
	if(!targaImage)
	{
		return false;
	}

	// Read in the targa image data.
	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if(count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if(error != 0)
	{
		return false;
	}

	// Allocate memory for the targa destination data.
	m_cpTargaData = new unsigned char[imageSize];
	if(!m_cpTargaData)
	{
		return false;
	}

	// Initialize the index into the targa destination data array.
	index = 0;

	// Initialize the index into the targa image data.
	k = (width * height * 4) - (width * 4);

	// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down.
	for(j=0; j<height; j++)
	{
		for(i=0; i<width; i++)
		{
			m_cpTargaData[index + 0] = targaImage[k + 2];  // Red.
			m_cpTargaData[index + 1] = targaImage[k + 1];  // Green.
			m_cpTargaData[index + 2] = targaImage[k + 0];  // Blue
			m_cpTargaData[index + 3] = targaImage[k + 3];  // Alpha

			// Increment the indexes into the targa data.
			k += 4;
			index += 4;
		}

		// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
		k -= (width * 8);
	}

	// Release the targa image data now that it was copied into the destination array.
	SAFE_DELETEA(targaImage);

	return true;
}