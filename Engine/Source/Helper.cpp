#include "../Headers/Helpers.h"

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN

//////////////
// INCLUDES //
//////////////
#include <windows.h>

bool IfFailMsg::operator()(bool res, std::string errMsg)
{
	if (!res) MessageBox(0, errMsg.c_str(), "Error Msg", MB_OK);

	return res;
}

bool IfFailMsg::operator()(HRESULT res, std::string errMsg)
{
	if (FAILED(res))
	{
		MessageBox(0, errMsg.c_str(), "Error Msg", MB_OK);
		return false;
	}
	
	return true;
}



