#ifndef Helper_H
#define Helper_H

#include <string>

struct IfFailMsg
{
	bool operator()(bool res, std::string errMsg);
};
static IfFailMsg fo_IfFailMsg;

template<typename T>
struct SafeDelete
{
	void operator()(T t);
};
template<typename T>
static SafeDelete<T> fo_SafeDelete;


#endif

