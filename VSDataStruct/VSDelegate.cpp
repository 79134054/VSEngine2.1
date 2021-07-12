#include "VSDelegate.h"
using namespace VSEngine2;
typedef VSDelegateEvent<int(int, int)> MouseMoveEvent;
int sss(int k, int m)
{
	return 1;
}
class A
{
public:
	int LL(int a, int b)const
	{
		return 1;
	}
	int MM(int a, int b)const
	{
		return 1;
	}
};
void m()
{
	A a;
	MouseMoveEvent MouseMove;
	int f = 0;
	MouseMove += MouseMoveEvent::Handler::FromFun<&sss>();
	MouseMove += MouseMoveEvent::Handler::FromMethod<A,&A::LL>(&a);
	MouseMove += MouseMoveEvent::Handler::FromMethod<A,&A::MM>(&a);
	MouseMove += MouseMoveEvent::Handler::FromLambda(([f](int a, int b)->int {return a + b + f; }));
	MouseMove(1, 2);
}