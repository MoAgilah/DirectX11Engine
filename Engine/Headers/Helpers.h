#ifndef Helper_H
#define Helper_H

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN

//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <string>

struct IfFailMsg
{
	bool operator()(bool res, std::string errMsg);
	bool operator()(HRESULT res, std::string errMsg);
};
static IfFailMsg fo_IfFailMsg;


#endif


