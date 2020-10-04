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
	~Keyboard();

	bool KeyIsPressed(const unsigned char keycode);
	bool KeyBufferIsEmpty() const;
	bool CharBufferIsEmpty() const;
	KeyboardEvent ReadKey();
	unsigned char ReadChar();
	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);
	void OnChar(const unsigned char key);

	void EnableAutoRepeatedKeys();
	void DisableAutoRepeatedKeys();
	void EnableAutoRepeatedChars();
	void DisableAutoRepeatedChars();
	bool IsKeysAutoRepeat() const;
	bool IsCharsAutoRepeat() const;
private:
	bool m_AutoRepeatKeys;
	bool m_AutoRepeatChars;
	bool m_KeyStates[256];
	std::queue<KeyboardEvent> mq_KeyBuffer;
	std::queue<unsigned char> mq_CharBuffer;
};

#endif // !Keyboard_H

