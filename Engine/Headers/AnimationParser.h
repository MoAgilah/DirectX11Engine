////////////////////////////////////////////////////////////////////////////////
// Filename: AnimationParser.h
////////////////////////////////////////////////////////////////////////////////
#ifndef AnimationParser_H
#define AnimationParser_H

//////////////
// INCLUDES //
//////////////
#include <vector>
#include <string>

//////////////
// GLOBALS //
/////////////
struct anim
{
	std::string name;
	std::string transName;
	std::vector<float> kframe;
	std::vector<float> val;
	int stride;
};

////////////////////////////////////////////////////////////////////////////////
// Class name: AnimationParser
////////////////////////////////////////////////////////////////////////////////
__declspec(align(16)) class AnimationParser
{
public:
	AnimationParser();
	~AnimationParser();
	bool LoadFromFile(const std::string& folderName, const std::string& animName);
	std::vector<anim> PrepAnimationData(const float& scaler = 1.0f);
private:
	std::vector<std::string> m_vsAnimData;
	std::vector<anim> m_vAnimation;
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

