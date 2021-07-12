#pragma once
#if _WIN32
#define WINDOWS_PLATFORM 1
#endif
enum EPLATRORM
{
	EP_Windows,
	EP_Android,
	EP_IOS,
	EP_MAX
};
#if WINDOWS_PLATFORM
#if _WIN64
#define SYSTEM_BIT_WIDTH 64
#define USIZE_TYPE unsigned long long
#define SIZE_TYPE long long
#else
#define SYSTEM_BIT_WIDTH 32
#define USIZE_TYPE unsigned int
#define SIZE_TYPE int
#endif
#endif