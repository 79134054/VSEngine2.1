#include "VSMath.h"
using namespace VSEngine2;
namespace VSEngine2
{

VSREAL VSMathInstance::GetFastSin(unsigned int i)
{
	return FastSin[i];
}
VSREAL VSMathInstance::GetFastCos(unsigned int i)
{
	return FastCos[i];
}


bool VSInitMath()
{
	VSMathInstance::GetMathInstance();	
	return 1;
}
void VSMathInstance::VSInitCRCTable()
{
#define POLYNOMIAL 0x04c11db7L
	int i, j;  
	unsigned int crc_accum;
	for ( i = 0;  i < 256;  i++ )
	{ 
		crc_accum = ( (unsigned int) i << 24 );
		for ( j = 0;  j < 8;  j++ )
		{
			if( crc_accum & 0x80000000L ) crc_accum = ( crc_accum << 1 ) ^ POLYNOMIAL;
			else crc_accum = ( crc_accum << 1 ); 
		}
		CRCTable[i] = crc_accum; 
	}
}
unsigned int VSMathInstance::CRC32Compute( const void *pData, unsigned int uiDataSize )
{
	unsigned int uiResult;

	if( uiDataSize == 0 ) return 0;

	const unsigned char *pDataTemp = (const unsigned char *)pData;
	uiResult  = *pDataTemp++ << 24;
	if( uiDataSize > 1 )
	{
		uiResult |= *pDataTemp++ << 16;
		if( uiDataSize > 2 )
		{
			uiResult |= *pDataTemp++ << 8;
			if( uiDataSize > 3 ) uiResult |= *pDataTemp++;
		}
	}
	uiResult = ~ uiResult;

	for( unsigned int i=4; i< uiDataSize; i++ )
	{
		uiResult = (uiResult << 8 | *pDataTemp++) ^ CRCTable[uiResult >> 24];
	}

	return ~uiResult;
}
VSMathInstance::VSMathInstance()
{
	VSInitCRCTable();
	for (unsigned int i = 0; i <= 360; i++)
	{
		VSREAL iRadian = AngleToRadian(VSREAL(i));
		FastSin[i] = SIN(iRadian);
		FastCos[i] = COS(iRadian);
	}
}
VSMathInstance::~VSMathInstance()
{

}
}
/*----------------------------------------------------------------*/
