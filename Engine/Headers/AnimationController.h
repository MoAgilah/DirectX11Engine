////////////////////////////////////////////////////////////////////////////////
// Filename: AnimationController.h
////////////////////////////////////////////////////////////////////////////////
#ifndef AnimationController_H
#define AnimationController_H

//////////////
// INCLUDES //
//////////////
#include <map>
#include <string>
#include <vector>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Animation.h"
#include "../Headers/AnimationParser.h"
#include "../Headers/MeshComponent.h"

//////////////
// GLOBALS //
/////////////
struct CFrame
{
	std::string boneName;
	XMFLOAT4 pos;
	XMFLOAT4 rot;
};

struct Track
{
	int trackId;
	std::vector<anim> AnimData;
	std::vector<float> Keys;
	std::vector<CFrame> FrameData;
	double Timer;
	bool active;
	bool shouldLoop;
};

////////////////////////////////////////////////////////////////////////////////
// Class name: AnimationController
////////////////////////////////////////////////////////////////////////////////
__declspec(align(16)) class AnimationController
{
public:
	AnimationController(const XMFLOAT3& iPos, const XMFLOAT3& iRot, const double& scalar);
	~AnimationController();
	void LoadAnimation(MeshComponent* root, const std::string& fName, const std::string& aName, const double& sF = 1.0, const bool& sLoop = true);
	void AdvanceTime(const double& time, std::map<std::string, MeshComponent*>& bHierarchy);
	Track* GetClipByName(const std::string& name);
	void ChangeTrack(const std::string& name);
	bool GetIsActive(const std::string& name);
private:
	void SetInitialTrack(Animation* initial);
	void SetAdditionalTracks(Animation* other);
	void SetUpCFrameVec(const std::vector<anim>& aData, std::vector<CFrame>& FData);
	void SetUpCFrameVec(const std::vector<CFrame>& base, std::vector<CFrame>& FData);
	void Interpolate(Track* cTrack, const double& time);
	void InterpolateTracks(std::vector<Track*> activeTracks, const double& time);
	void StoreCurrentFrame(std::vector<CFrame> curr);
	void UpdateHiearachy(std::map<std::string, MeshComponent*>& bHierarchy);
	XMFLOAT3 m_f3InitPos, m_f3InitRot;
	std::vector<Track*> m_vpAnimTracks;
	int m_iActiveTracks;
	static int m_siTrackNum;
	int m_iCurAnimID;
	int m_iNextAnimID;
	double m_dScalar;
	double m_dTimer;
	bool m_bIsBlending;
	int m_iStrideNum;
	std::vector<CFrame> m_vCurrFrame;
	std::map<std::string, Track*> m_mAllTracks;
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

#endif;
