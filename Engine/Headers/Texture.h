////////////////////////////////////////////////////////////////////////////////
// Filename: Texture.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Texture_H
#define Texture_H

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <stdio.h>
#include <string>

/////////////////////////
// MY LIBRARY INCLUDES //
/////////////////////////
#include "../DirectXTemplate/WICTextureLoader.h"
#include "../DirectXTemplate/DDSTextureLoader.h"

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Utils.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Texture
////////////////////////////////////////////////////////////////////////////////
class Texture
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool InitializeDDSTextureFromFile(ID3D11Device*, ID3D11DeviceContext*, char*);
	bool InitializeWICTextureFromFile(ID3D11Device*, ID3D11DeviceContext*, char*);
	bool InitializeTextureFromTarga(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Release();

	ID3D11Texture2D* GetTexture2D();
	ID3D11ShaderResourceView* GetTextureShaderResourceView();
private:
	bool LoadTarga(char*, int&, int&);

private:
	unsigned char* m_cpTargaData;

	ID3D11Texture2D* m_pTexture;
	ID3D11ShaderResourceView* m_pTextureView;

};

#endif