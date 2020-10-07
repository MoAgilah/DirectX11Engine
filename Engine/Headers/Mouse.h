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

	void OnLeftPressed(const int& x, const int& y);
	void OnLeftReleased(const int& x, const int& y);
	void OnRightPressed(const int& x, const int& y);
	void OnRightReleased(const int& x, const int& y);
	void OnMiddlePressed(const int& x, const int& y);
	void OnMiddleReleased(const int& x, const int& y);
	void OnWheelUp(const int& x, const int& y);
	void OnWheelDown(const int& x, const int& y);
	void OnMouseMove(const int& x, const int& y);

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



