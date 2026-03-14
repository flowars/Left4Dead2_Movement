#pragma once
#include "../../Utils/Memory.h"
#include "../Classes/VCollide.h"

class IPhysicsCollision
{
public:
	//virtual void sub_644147C0() = 0;
	//virtual void sub_64413230() = 0;
	//virtual void sub_64411AF0() = 0;
	//virtual void sub_644123E0() = 0;
	//virtual void sub_644125E0() = 0;
	//virtual void sub_644113D0() = 0;
	//virtual void sub_644112E0() = 0;
	//virtual void sub_64413830() = 0;
	//virtual void sub_64411BE0() = 0;
	//virtual void sub_64412260() = 0;
	//virtual void sub_64413A00() = 0;
	//virtual void sub_644143D0() = 0;
	//virtual void sub_644127C0() = 0;
	//virtual void sub_64413A20() = 0;
	//virtual void sub_64412780() = 0;
	//virtual void sub_644132F0() = 0;
	//virtual void sub_64413DF0() = 0;
	//virtual void sub_64411300() = 0;
	//virtual void sub_64411320() = 0;
	//virtual void Call_UnserializeFromBuffer() = 0;
	//virtual void sub_64413E30() = 0;
	//virtual void sub_64413EC0() = 0;
	//virtual void sub_64411550() = 0;
	//virtual void sub_64413D10() = 0;
	//virtual void sub_644117D0() = 0;
	//virtual void sub_64411800() = 0;
	//virtual void sub_64411840() = 0;
	//virtual void sub_64411880() = 0;
	//virtual void sub_64411820() = 0;
	//virtual void sub_64414A70() = 0;
	//virtual void sub_64413280() = 0;
	//virtual void sub_64411480() = 0;
	//virtual void sub_644114F0() = 0;
	//virtual void sub_644114E0() = 0;
	//virtual void IsBoxIntersectingConeMayBe() = 0;
	//virtual void sub_64411440() = 0;
	//virtual void sub_64411430() = 0;
	//virtual void sub_64411400() = 0;
	//virtual void sub_644113F0() = 0;
	//virtual void sub_64411520() = 0;
	//virtual void sub_644115B0() = 0;
	//virtual void VCollideLoad(vcollide_t* pOutput, int solidCount, const char* pBuffer, int size, bool swap = false) = 0;
	//virtual void VCollideUnload(vcollide_t* pVCollide) = 0;

	void VCollideLoad(vcollide_t* pOutput, int solidCount, const char* pBuffer, int size, bool swap = false)
	{
		memory::Call<void>(this, 41, pOutput, solidCount, pBuffer, size, swap);
	}

	void VCollideUnload(vcollide_t* pVCollide)
	{
		memory::Call<void>(this, 42, pVCollide);
	}
};