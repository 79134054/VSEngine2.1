//这个示例for auto的使用
#include <VSMemManager.h>
#include <VSArray.h>
#include <iostream>
#include <vector>
using namespace VSEngine2;
void main()
{
	VSArray<int> Temp;

	Temp.AddElement(10);
	Temp.AddElement(2);
	Temp.AddElement(3);
	Temp.AddElement(5);
	Temp.AddElement(2);
	Temp.AddElement(7);


	for (VSArray<int>::VSArrayIterator i = Temp.Begin(); i != Temp.End() ; i++)
	{
		std::cout << *i << std::endl;
	}

	for (auto i = Temp.Begin(); i != Temp.End(); i++)
	{
		std::cout << *i << std::endl;
	}
	for (auto i : Temp)
	{
		std::cout << i << std::endl;
	}
	for (auto& i : Temp)
	{
		i++;
	}
	for (auto i : Temp)
	{
		std::cout << i << std::endl;
	}
}