#include "../Headers/AnimationParser.h"
#include "../TinyXml2/tinyxml2.h"
#include <sstream>

namespace tx2
{
	using namespace tinyxml2;
}

AnimationParser::AnimationParser()
{
}


AnimationParser::~AnimationParser()
{
}

bool AnimationParser::LoadFromFile(const std::string& folderName, const std::string& animName)
{
	tx2::XMLDocument doc;	//tinyXML document
	std::string path = "Resources/Data/" + folderName + animName + "AnimDAE.txt";
	doc.LoadFile(path.c_str());	//open and parse data from file
	
	//find root for operation
	tx2::XMLNode* pRoot = doc.FirstChildElement("library_animations");
	if (pRoot == nullptr){ return (bool)tx2::XML_ERROR_FILE_READ_ERROR; }
	
	//check to see if been used before if so clear values
	if (!m_vsAnimData.empty())m_vsAnimData.clear();

	//extract all data
	//find first child element animation and loop through all siblings
	for (tx2::XMLElement* pAnim = pRoot->FirstChildElement("animation"); pAnim != NULL; pAnim = pAnim->NextSiblingElement())
	{
		//extract animation id
		const char* aID = nullptr;
		aID = pAnim->Attribute("id", aID);
		if (aID == nullptr)return (bool)tx2::XML_ERROR_PARSING_ATTRIBUTE;

		//split animation id into name and type of transformation
		std::string Id = aID;
		size_t pos = Id.find('.');
		if ((Id.find("visibility") != std::string::npos))
		{
			continue;
		}

		//insert values into animData container
		m_vsAnimData.push_back(Id.substr(0, pos));
		m_vsAnimData.push_back(Id.substr(pos + 1));

		//get frist animation child source, and all subsequent siblings
		for (tx2::XMLElement* pSource = pAnim->FirstChildElement("source"); pSource != NULL; pSource = pSource->NextSiblingElement())
		{
			//get sources child float arrray
			tx2::XMLElement* pFArr = pSource->FirstChildElement("float_array");

			//extract values from float array
			const char* val = nullptr;
			val = pFArr->GetText();
			if (val == nullptr) return (bool)tx2::XML_ERROR_PARSING_ATTRIBUTE;

			//push values into animData container
			m_vsAnimData.push_back(val);

			//get the float array ID
			const char* faID = nullptr;
			Id = pFArr->Attribute("id", faID);

			//test to see if its the output array
			if ((Id.find("output") != std::string::npos))
			{
				//get the accessor source
				tx2::XMLElement* pS = pSource->FirstChildElement("technique_common")->FirstChildElement();

				//extract the stride from the accessor source
				const char* s = nullptr;
				s = pS->Attribute("stride", s);
				if (s == nullptr) return (bool)tx2::XML_ERROR_PARSING_ATTRIBUTE;

				//push stride into the array
				m_vsAnimData.push_back(s);
			}
		}
	}

	doc.Clear();//close file
	return true;
}


std::vector<anim> AnimationParser::PrepAnimationData(const float& scalar)
{
	//check to see if been used before if so clear values
	if (!m_vAnimation.empty())m_vAnimation.clear();
	
	m_vAnimation.reserve(m_vsAnimData.size() / 5);	//store enough space in advance to avoid memory reallocation

	float v,t,s;

	for (int i = 0; i < m_vsAnimData.size(); i += 5)
	{
		anim temp;
		temp.name = m_vsAnimData[i];//get component name
		temp.transName = m_vsAnimData[i + 1];//get transformation name

		//get keyframes i.e. input arrray
		std::stringstream time(m_vsAnimData[i + 2]);
		do
		{
			time >> t;
			
			temp.kframe.push_back(t);
		} while (time.rdbuf()->in_avail() != 0);//check if there are more values in stream



		//get values i.e. output array
		std::stringstream val(m_vsAnimData[i + 3]);
		do
		{
			val >> v;
			//if transformation name is translate scale if required
			if (temp.transName == "translate" && scalar != 1.0f)
			{
				v /= scalar;
			}

			temp.val.push_back(v);
		} while (val.rdbuf()->in_avail() != 0);//check if there are more values in stream

		std::stringstream stride(m_vsAnimData[i + 4]);
		stride >> s;
		temp.stride = (int)s;

		//push animation chunk into container
		m_vAnimation.push_back(temp);
	}
	
	return m_vAnimation;
}
