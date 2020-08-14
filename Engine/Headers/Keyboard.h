////////////////////////////////////////////////////////////////////////////////
// Filename: Keyboard.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Keyboard_H
#define Keyboard_H

//////////////////
// MY INCLUDES //
/////////////////
#include <queue>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/KeyboardEvent.h"

////////////////////////////////////////////////////////////////////////////////
// Classname: Keyboard
////////////////////////////////////////////////////////////////////////////////
class Keyboard
{
public:
	Keyboard();
	Keyboard(const Keyboard&);
	~Keyboard();
	bool KeyIsPressed(const unsigned char keycode);
	bool KeyBufferIsEmpty();
	bool CharBufferIsEmpty();
	KeyboardEvent ReadKey();
	unsigned char ReadChar();
	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);
	void OnChar(const unsigned char key);
	void EnableAutoRepeatedKeys();
	void DisableAutoRepeatedKeys();
	void EnableAutoRepeatedChars();
	void DisableAutoRepeatedChars();
	bool IsKeysAutoRepeat();
	bool IsCharsAutoRepeat();

private:
	bool m_bAutoRepeatKeys;
	bool m_bAutoRepeatChars;
	bool m_bKeyStates[256];
	std::queue<KeyboardEvent> m_qKeyBuffer;
	std::queue<unsigned char> m_qCharBuffer;
};

#endif // !Keyboard_H

