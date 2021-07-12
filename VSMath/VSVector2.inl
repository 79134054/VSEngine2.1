/*----------------------------------------------------------------*/
inline void VSVector2::Set(VSREAL _x, VSREAL _y) 
{
	x=_x; y=_y; 
}
/*----------------------------------------------------------------*/
inline void VSVector2::Normalize(void) 
{
	VSREAL f = x * x + y * y;
	if (f > EPSILON_E4)
	{

#ifdef VS_SSE
		f = InvSqrt(f);
#else
		f = (VSREAL)1.0f / SQRT(f);
#endif
		x *= f; y *= f;
	}
	else
	{
		Set(0.0f, 0.0f);
	}
}