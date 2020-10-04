////////////////////////////////////////////////////////////////////////////////
// Filename: Input.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/Input.h"

Input* Input::s_pInstance = nullptr;

Input::Input()
	:m_pKeyboard(new Keyboard), m_pMouse(new Mouse)
{}

Input::~Input()
{
	Release();
}

void Input::Release()
{
	SAFE_DELETE(m_pMouse);
	SAFE_DELETE(m_pKeyboard);
}

Keyboard* Input::GetKeyboard()
{
	return m_pKeyboard;
}

Mouse* Input::GetMouse()
{
	return m_pMouse;
}

Input* Input::GetInput()
{
	if (!s_pInstance)
	{
		s_pInstance = new Input;
	}

	return s_pInstance;
}

