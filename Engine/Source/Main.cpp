////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Framework.h"
#include "../Headers/Helpers.h"
#include "../Headers/Utils.h"

int main()
{
	Framework* p_System = new Framework;
	assert(p_System != nullptr);

	// Initialize and run the system object.
	if (fo_IfFailMsg(p_System->Initialise(), "Failed to initialise the Engine Framework"))
		p_System->Run();

	// Shutdown and release the system object.
	SAFE_RELEASE(p_System);
	return 0;
}