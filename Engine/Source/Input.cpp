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
}

Keyboard* Input::GetKeyboard()
{
	return m_pKeyboard.get();
}

Mouse* Input::GetMouse()
{
	return m_pMouse.get();
}

Input* Input::GetInput()
{
	if (!s_pInstance)
		s_pInstance = new Input;

	return s_pInstance;
}

