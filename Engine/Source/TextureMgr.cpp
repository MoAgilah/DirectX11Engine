////////////////////////////////////////////////////////////////////////////////
// Filename: Terrain.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/TextureMgr.h"

TextureMgr::TextureMgr()
{
}

TextureMgr::~TextureMgr()
{
}

Texture* TextureMgr::Load(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename, TexType texType)
{
	string str = filename;
	
	//remove filepath
	str = str.substr(str.find_last_of('/')+1);
	
	//remove file extension
	str = str.substr(0, str.find('.'));

	Texture* tex = new Texture;

	auto it = m_mTextures.find(str);
	if (it == m_mTextures.end())
	{
		LoadTexture(tex, device, deviceContext, filename, texType);
		m_mTextures.emplace(str, tex);
	}
	else
	{
		tex = it->second;
	}

	

	return tex;
}

void TextureMgr::Unload(Texture* texture)
{
	string str;
	Texture* tex = nullptr;
	auto it = m_mTextures.find(str);

	if (it == m_mTextures.end())
	{
		return;
	}
	else
	{
		m_mTextures.erase(it->first);
	}
}

void TextureMgr::Release()
{
	m_mTextures.erase(m_mTextures.begin(), m_mTextures.end());
}

Texture* TextureMgr::GetTexture(string id)
{
	Texture* tex = nullptr;
	auto it = m_mTextures.find(id);

	if (it == m_mTextures.end())
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

inline bool TextureMgr::LoadTexture(Texture* texture, ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename, TexType texType)
{
	bool result;


	//// Create the texture object.
	//texture = new Texture;
	//if (!texture)
	//{
	//	return false;
	//}

	// Initialize the texture object.
	switch (texType)
	{
	case WIC:
		result = texture->InitializeWICTextureFromFile(device, deviceContext, filename);
		break;
	case DDS:
		result = texture->InitializeDDSTextureFromFile(device, deviceContext, filename);
		break;
	case Targa:
		result = texture->InitializeTextureFromTarga(device, deviceContext, filename);
		break;
	default:
		return false;
		break;
	}

	if (!result)
	{
		return false;
	}

	return true;
}

