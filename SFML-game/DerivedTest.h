#pragma once

#include "TypeMapper.h"

class Base : public Object
{
public:
	Base() = default;
	~Base() = default;
};

class DerivedTest : public Base
{
public:
	DerivedTest();
	~DerivedTest();
};

class Base2 : public Object
{
public:
	Base2() = default;
	~Base2() = default;
};

class DerivedTest2 : public Base2
{
public:
	DerivedTest2() = default;
	~DerivedTest2() = default;
};