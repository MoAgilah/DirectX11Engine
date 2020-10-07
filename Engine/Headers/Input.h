////////////////////////////////////////////////////////////////////////////////
// Filename: Input.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Input_H_
#define Input_H_

#include <memory>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Keyboard.h"
#include "../Headers/Mouse.h"
#include "../Headers/Utils.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Input
////////////////////////////////////////////////////////////////////////////////
class Input
{
public:
	~Input();
	Keyboard* GetKeyboard();
	Mouse* GetMouse();
	static Input* GetInput();
private:
	Input();
	
	static Input* s_pInstance;
	std::unique_ptr<Keyboard> m_pKeyboard;
	std::unique_ptr<Mouse> m_pMouse;
};

#endif