////////////////////////////////////////////////////////////////////////////////
// Filename: Mouse.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/Mouse.h"

Mouse::Mouse()
	:m_bLeftIsDown(false), m_bRightIsDown(false), m_bMiddleIsDown(false),
	m_iXPos(0),m_iYPos(0)
{}

Mouse::~Mouse()
{}

void Mouse::OnLeftPressed(int x, int y)
{
	m_bLeftIsDown = true;
	MouseEvent me(MouseEvent::MEventType::LPress, x, y);
	m_qMEventBuffer.push(me);
}

void Mouse::OnLeftReleased(int x, int y)
{
	m_bLeftIsDown = false;
	m_qMEventBuffer.push(MouseEvent(MouseEvent::MEventType::LRelease, x, y));
}

void Mouse::OnRightPressed(int x, int y)
{
	m_bRightIsDown = true;
	m_qMEventBuffer.push(MouseEvent(MouseEvent::MEventType::RPress, x, y));
}

void Mouse::OnRightReleased(int x, int y)
{
	m_bRightIsDown = false;
	m_qMEventBuffer.push(MouseEvent(MouseEvent::MEventType::RRelease, x, y));
}

void Mouse::OnMiddlePressed(int x, int y)
{
	m_bMiddleIsDown = true;
	m_qMEventBuffer.push(MouseEvent(MouseEvent::MEventType::MPress, x, y));
}

void Mouse::OnMiddleReleased(int x, int y)
{
	m_bMiddleIsDown = false;
	m_qMEventBuffer.push(MouseEvent(MouseEvent::MEventType::MRelease, x, y));
}

void Mouse::OnWheelUp(int x, int y)
{
	m_qMEventBuffer.push(MouseEvent(MouseEvent::MEventType::WheelUp, x, y));
}

void Mouse::OnWheelDown(int x, int y)
{
	m_qMEventBuffer.push(MouseEvent(MouseEvent::MEventType::WheelDown, x, y));
}

void Mouse::OnMouseMove(int x, int y)
{
	m_iXPos = x;
	m_iYPos = y;
	m_qMEventBuffer.push(MouseEvent(MouseEvent::MEventType::Move, x, y));
}

bool Mouse::IsLeftDown() const
{
	return m_bLeftIsDown;
}

bool Mouse::IsMiddleDown() const
{
	return m_bMiddleIsDown;
}

bool Mouse::IsRightDown() const
{
	return m_bRightIsDown;
}

int Mouse::GetPosX() const
{
	return m_iXPos;
}

int Mouse::GetPosY() const
{
	return m_iYPos;
}

MouseEvent::MousePoint Mouse::GetPos() const
{
	return { m_iXPos, m_iYPos };
}

bool Mouse::EventBufferIsEmpty() const
{
	return m_qMEventBuffer.empty();
}

MouseEvent Mouse::ReadEvent()
{
	if (m_qMEventBuffer.empty())
	{
		return MouseEvent();
	}
	else
	{
		MouseEvent e = m_qMEventBuffer.front(); //Get first event from buffer
		m_qMEventBuffer.pop(); //Remove first event from buffer
		return e;
	}
}