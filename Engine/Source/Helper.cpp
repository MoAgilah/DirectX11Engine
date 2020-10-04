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
	if (!res) MessageBox(0, errMsg.c_str(), 0, 0);

	return res;
}

template<typename T>
void SafeDelete<T>::operator()(T t)
{
	if (t)
		delete t;
	t = nullptr;
}

