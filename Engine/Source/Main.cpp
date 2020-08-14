////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Framework.h"

int main()
{
	Framework* p_System;
	bool result;

	// Create the system object.
	p_System = new Framework;
	if (!p_System)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = p_System->Initialise();
	if (result)
	{
		p_System->Run();
	}

	// Shutdown and release the system object.
	SAFE_RELEASE(p_System);
	return 0;
}