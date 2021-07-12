//这个示例演示了BitArray的使用
#include "VSBitArray.h"
#include <iostream>
using namespace VSEngine2;
void main()
{
	VSBit<unsigned int> Bit;
	Bit.Set(3, true);
	Bit.Set(4, true);
	Bit.Set(21, true);

	for (unsigned int i = 0 ; i < Bit.GetNum() ; i++)
	{
		std::cout << i <<" "<<Bit[i] <<std::endl;
	}

	VSBitArray BitArray(63);
	BitArray.Set(3, true);
	BitArray.Set(4, true);
	BitArray.Set(21, true);
	BitArray.Set(55, true);
}