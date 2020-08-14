////////////////////////////////////////////////////////////////////////////////
// Filename: TextureMgr.h
////////////////////////////////////////////////////////////////////////////////
#ifndef TextureMgr_H
#define TextureMgr_H

//////////////
// INCLUDES //
//////////////
#include <string>
#include <unordered_map>
using namespace std;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/D3DMgr.h"
#include "../Headers/Texture.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: TextureMgr
////////////////////////////////////////////////////////////////////////////////
class TextureMgr
{
public:
	TextureMgr();
	~TextureMgr();
	Texture* Load(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename, TexType texType);
	void Unload(Texture* texture);
	void Release();
	Texture* GetTexture(string id);
private:
	bool LoadTexture(Texture* texture, ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename, TexType texType);

	unordered_map<string , Texture*> m_mTextures;
};

#endif // !TextureMgr_H



