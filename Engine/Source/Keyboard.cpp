////////////////////////////////////////////////////////////////////////////////
// Filename: Keyboard.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/Keyboard.h"

Keyboard::Keyboard()
{
	m_bAutoRepeatKeys = false;
	m_bAutoRepeatChars = false;
	for (int i = 0; i < 256; i++)
		m_bKeyStates[i] = false; //Initialize all key states to off (false)
}

Keyboard::Keyboard(const Keyboard& other)
{
	for (int i = 0; i < 256; i++)
		m_bKeyStates[i] = other.m_bKeyStates[i]; //Initialize all key states to off (false)
}

Keyboard::~Keyboard()
{
}

bool Keyboard::KeyIsPressed(const unsigned char keycode)
{
	return m_bKeyStates[keycode];
}

bool Keyboard::KeyBufferIsEmpty()
{
	return m_qKeyBuffer.empty();
}

bool Keyboard::CharBufferIsEmpty()
{
	return m_qCharBuffer.empty();
}

KeyboardEvent Keyboard::ReadKey()
{
	if (m_qKeyBuffer.empty()) //If no keys to be read?
	{
		return KeyboardEvent(); //return empty keyboard event
	}
	else
	{
		KeyboardEvent e = m_qKeyBuffer.front(); //Get first Keyboard Event from queue
		m_qKeyBuffer.pop(); //Remove first item from queue
		return e; //Returns keyboard event
	}
}

unsigned char Keyboard::ReadChar()
{
	if (m_qCharBuffer.empty()) //If no keys to be read?
	{
		return 0u; //return 0 (NULL char)
	}
	else
	{
		unsigned char e = m_qCharBuffer.front(); //Get first char from queue
		m_qCharBuffer.pop(); //Remove first char from queue
		return e; //Returns char
	}
}

void Keyboard::OnKeyPressed(const unsigned char key)
{
	m_bKeyStates[key] = true;
	m_qKeyBuffer.push(KeyboardEvent(KeyboardEvent::KEventType::Press, key));
}

void Keyboard::OnKeyReleased(const unsigned char key)
{
	m_bKeyStates[key] = false;
	m_qKeyBuffer.push(KeyboardEvent(KeyboardEvent::KEventType::Release, key));
}

void Keyboard::OnChar(const unsigned char key)
{
	m_qCharBuffer.push(key);
}

void Keyboard::EnableAutoRepeatedKeys()
{
	m_bAutoRepeatKeys = true;
}

void Keyboard::DisableAutoRepeatedKeys()
{
	m_bAutoRepeatKeys = false;
}

void Keyboard::EnableAutoRepeatedChars()
{
	m_bAutoRepeatChars = true;
}

void Keyboard::DisableAutoRepeatedChars()
{
	m_bAutoRepeatChars = false;
}

bool Keyboard::IsKeysAutoRepeat()
{
	return m_bAutoRepeatKeys;
}

bool Keyboard::IsCharsAutoRepeat()
{
	return m_bAutoRepeatChars;
}
