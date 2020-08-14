////////////////////////////////////////////////////////////////////////////////
// Filename: KeyboardEvent.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/KeyboardEvent.h"

KeyboardEvent::KeyboardEvent()
	:m_KEventType(KEventType::Invalid),m_cKey(0u)
{
}

KeyboardEvent::KeyboardEvent(const KEventType type, const unsigned char key)
	: m_KEventType(type), m_cKey(key)
{
}

KeyboardEvent::~KeyboardEvent()
{
}

bool KeyboardEvent::IsPressed() const
{
	return m_KEventType == KEventType::Press;
}

bool KeyboardEvent::IsRelease() const
{
	return m_KEventType == KEventType::Release;
}

bool KeyboardEvent::IsValid() const
{
	return m_KEventType == KEventType::Invalid;
}

unsigned char KeyboardEvent::GetKeyCode() const
{
	return m_cKey;
}
