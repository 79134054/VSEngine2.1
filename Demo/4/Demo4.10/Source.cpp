//这个示例演示了哈希树的使用
#include "VSHash.h"
#include <iostream>
using namespace VSEngine2;
template <class T>
class VSPrintProcess
{
public:
	VSPrintProcess()
	{

	}
	~VSPrintProcess() {}
	void  operator()(VSBinaryTreeNode<T> * TreeNode)
	{
		std::cout << TreeNode->Element << std::endl;
	}
};
void main()
{
	VSHashTree<VSREAL> HashTree;
	HashTree.SetHashNum(10);
	VSHashList<VSREAL> HashList;
	HashList.SetHashNum(10);
	for (unsigned int i = 0 ; i < 1000 ; i++)
	{
		VSREAL f = VSFRand();
		HashTree.AddElement(f);
		HashList.AddElement(f);
	}
	std::cout <<TEXT("-------------------HashTree--------------------------")<<std::endl;
	HashTree.MiddleProcess(VSPrintProcess<VSREAL>());
	std::cout << TEXT("-------------------HashList--------------------------") << std::endl;
	for (VSHashList<VSREAL>::VSHashListIterator Temp(HashList.Begin()) ; Temp != HashList.End() ; ++Temp)
	{
		std::cout << *Temp << std::endl;
	}
}