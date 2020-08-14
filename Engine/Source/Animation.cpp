#include "../Headers/Animation.h"


AnimationParser* Animation::m_AnimParser = nullptr;
int Animation::m_siAnimCounter = -1;

Animation::Animation(const std::string& folderName, const std::string& animName, const bool& loop, const double& scalar)
	:m_sName(animName), m_bShouldLoop(loop)
{
	m_siAnimCounter++;

	if (m_AnimParser == nullptr)
	{
		m_AnimParser = new AnimationParser();
	}
	
	//share one parser for all animations
	if (m_AnimParser->LoadFromFile(folderName, animName)){
		//extract data from parser
		m_vAnim = m_AnimParser->PrepAnimationData(scalar);
		//assing keyframes from beginning to end of animation
		SetKeyFrames();
	}
}


Animation::~Animation()
{
	m_siAnimCounter--;
	if (m_siAnimCounter == -1)
	{
		SAFE_DELETE(m_AnimParser);
	}
}

void Animation::SetKeyFrames(void)
{
	m_vKeyframes = m_vAnim[0].kframe;
	//if only one keyframe find the largest
	for (int i = 1; i < m_vAnim.size(); ++i)
	{
		if (m_vKeyframes.size() < m_vAnim[i].kframe.size())
		{
			m_vKeyframes = m_vAnim[i].kframe;
		}
	}
}

std::vector<anim> Animation::GetFrameTrans(void) const
{
	return m_vAnim;
}

std::vector<float> Animation::GetKeyframes(void) const
{
	return m_vKeyframes;
}

bool Animation::GetShouldLoop(void) const
{
	return m_bShouldLoop;
}

std::string Animation::GetName(void) const
{
	return m_sName;
}

	