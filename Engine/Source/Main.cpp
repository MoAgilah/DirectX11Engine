////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////

/////////////////
// MY INCLUDES //
/////////////////
#include <memory>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Framework.h"
#include "../Headers/Helpers.h"

int main()
{
	std::unique_ptr<Framework> p_System(new Framework);
	assert(p_System != nullptr);

	// Initialize and run the system object.
	if (fo_IfFailMsg(p_System->Initialise(), "Failed to initialise the Engine Framework"))
		p_System->Run();

	return 0;
}