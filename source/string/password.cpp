/*!
    \file password.cpp
    \brief Password string implementation
    \author Ivan Shynkarenka
    \date 04.06.2019
    \copyright MIT License
*/

#include "string/password.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
#include <string.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

namespace CppCommon {

void ZeroPasswordMemory(volatile void* buffer, size_t size)
{
#if defined(unix) || defined(__unix) || defined(__unix__)
    volatile char* ptr = (volatile char*)buffer;
    while (size--) *ptr++ = 0;
#elif defined(_WIN32) || defined(_WIN64)
    SecureZeroMemory(buffer, size);
#else
    #error Unsupported platform
#endif
}

} // namespace CppCommon
