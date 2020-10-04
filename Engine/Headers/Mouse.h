////////////////////////////////////////////////////////////////////////////////
// Filename: Mouse.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Mouse_H
#define Mouse_H

//////////////////
// MY INCLUDES //
/////////////////
#include <queue>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/MouseEvent.h"

////////////////////////////////////////////////////////////////////////////////
// Classname: Mouse
////////////////////////////////////////////////////////////////////////////////
class Mouse
{
public:
	Mouse();
	~Mouse();

	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnMiddlePressed(int x, int y);
	void OnMiddleReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void OnMouseMove(int x, int y);

	bool IsLeftDown() const;
	bool IsMiddleDown() const;
	bool IsRightDown() const;

	int GetPosX() const;
	int GetPosY() const;
	MouseEvent::MousePoint GetPos() const;

	bool EventBufferIsEmpty() const;
	MouseEvent ReadEvent();

private:
	std::queue<MouseEvent> m_qMEventBuffer;
	bool m_bLeftIsDown;
	bool m_bRightIsDown;
	bool m_bMiddleIsDown;
	int m_iXPos;
	int m_iYPos;
};

#endif // !Mouse_H



