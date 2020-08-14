////////////////////////////////////////////////////////////////////////////////
// Filename: Animation.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Animation_H
#define Animation_H

//////////////
// INCLUDES //
//////////////
#include <string>
#include <vector>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Graphics.h"
#include "../Headers/AnimationParser.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Animation
////////////////////////////////////////////////////////////////////////////////
__declspec(align(16)) class Animation
{
public:
	Animation(const std::string& folderName, const std::string& animName, const bool& loop, const double& scalar = 1.0f);
	~Animation();
	void SetKeyFrames();
	std::vector<anim> GetFrameTrans() const;
	std::vector<float> GetKeyframes() const;
	bool GetShouldLoop() const;
	std::string GetName() const;
private:
	static int m_siAnimCounter;
	static AnimationParser* m_AnimParser;
	std::string m_sName;
	std::vector<anim> m_vAnim;
	std::vector<float> m_vKeyframes;
	bool m_bShouldLoop;
public:
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
};

#endif

