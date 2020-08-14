////////////////////////////////////////////////////////////////////////////////
// Filename: KeyboardEvent.h
////////////////////////////////////////////////////////////////////////////////
#ifndef MouseEvent_H
#define MouseEvent_H

//////////////
// GLOBALS //
/////////////
struct MousePoint
{
	int x;
	int y;
};

enum MEventType
{
	LPress,
	LRelease,
	RPress,
	RRelease,
	MPress,
	MRelease,
	WheelUp,
	WheelDown,
	Move,
	RawMove,
	Invalid
};

////////////////////////////////////////////////////////////////////////////////
// Classname: MouseEvent
////////////////////////////////////////////////////////////////////////////////
class MouseEvent
{
public:
	
	MouseEvent();
	MouseEvent(const MEventType type, const int x, const int y);
	~MouseEvent();
	
	bool IsValid() const;
	MEventType GetType() const;
	MousePoint GetPos() const;
	int GetPosX() const;
	int GetPosY() const;

private:
	MEventType m_MEventType;
	int m_XPos;
	int m_YPos;
};

#endif