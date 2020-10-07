////////////////////////////////////////////////////////////////////////////////
// Filename: Keyboard.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/Keyboard.h"

Keyboard::Keyboard()
	:m_AutoRepeatKeys(false), m_AutoRepeatChars(false)
{
	for(auto &i : m_KeyStates)
		m_KeyStates[i] = false; //Initialize all key states to off (false)
}

bool Keyboard::KeyIsPressed(const unsigned char& keycode)
{
	return m_KeyStates[keycode];
}

bool Keyboard::KeyBufferIsEmpty() const
{
	return mq_KeyBuffer.empty();
}

bool Keyboard::CharBufferIsEmpty() const
{
	return mq_CharBuffer.empty();
}

KeyboardEvent Keyboard::ReadKey()
{
	if (mq_KeyBuffer.empty()) //If no keys to be read?
	{
		return KeyboardEvent(); //return empty keyboard event
	}
	else
	{
		KeyboardEvent e = mq_KeyBuffer.front(); //Get first Keyboard Event from queue
		mq_KeyBuffer.pop(); //Remove first item from queue
		return e; //Returns keyboard event
	}
}

unsigned char Keyboard::ReadChar()
{
	if (mq_CharBuffer.empty()) //If no keys to be read?
	{
		return 0u; //return 0 (NULL char)
	}
	else
	{
		unsigned char e = mq_CharBuffer.front(); //Get first char from queue
		mq_CharBuffer.pop(); //Remove first char from queue
		return e; //Returns char
	}
}

void Keyboard::OnKeyPressed(const unsigned char& key)
{
	m_KeyStates[key] = true;
	mq_KeyBuffer.push(KeyboardEvent(KeyboardEvent::KEventType::Press, key));
}

void Keyboard::OnKeyReleased(const unsigned char& key)
{
	m_KeyStates[key] = false;
	mq_KeyBuffer.push(KeyboardEvent(KeyboardEvent::KEventType::Release, key));
}

void Keyboard::OnChar(const unsigned char& key)
{
	mq_CharBuffer.push(key);
}

void Keyboard::EnableAutoRepeatedKeys()
{
	m_AutoRepeatKeys = true;
}

void Keyboard::DisableAutoRepeatedKeys()
{
	m_AutoRepeatKeys = false;
}

void Keyboard::EnableAutoRepeatedChars()
{
	m_AutoRepeatChars = true;
}

void Keyboard::DisableAutoRepeatedChars()
{
	m_AutoRepeatChars = false;
}

bool Keyboard::IsKeysAutoRepeat() const
{
	return m_AutoRepeatKeys;
}

bool Keyboard::IsCharsAutoRepeat() const
{
	return m_AutoRepeatChars;
}
