//这个示例演示了二叉树的使用
#include "VSBinaryTree.h"
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
	VSBinaryTree<int> BinaryTree;
	BinaryTree.AddElement(89);
	BinaryTree.AddElement(19);
	BinaryTree.AddElement(4);
	BinaryTree.AddElement(-11);
	BinaryTree.AddElement(-44);
	BinaryTree.AddElement(0);
	BinaryTree.AddElement(132);
	BinaryTree.AddElement(9);
	BinaryTree.AddElement(11);
	BinaryTree.MiddleProcess(VSPrintProcess<int>());
}