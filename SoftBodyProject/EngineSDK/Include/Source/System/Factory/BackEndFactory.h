#pragma once


#include "BackEnd.h"

class BackEndFactory
{
public:
	static BackEnd CreateBackEnd();
};
