#include "stdafx.h"
#include "DerivedTest.h"
#include <iostream>

REGISTER_TYPE(DerivedTest)
REGISTER_TYPE(DerivedTest2)

DerivedTest::DerivedTest()
{
}

DerivedTest::~DerivedTest()
{
}

/*  Type factory example code, if decided to be used
std::vector<Base2*> b;
auto a = MAKE_INSTANCE("DerivedTest2", Base2);
if (a)
	b.push_back(a);*/
