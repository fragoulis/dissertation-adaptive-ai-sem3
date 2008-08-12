#pragma once
#include <iostream>

#ifdef _DEBUG

#define _log_s(str)                     std::cout << str << std::endl
#define _log_n(str, num)                std::cout << str << num << std::endl
#define _log_2n(str1, num1, str2, num2) std::cout << str1 << num1 << str2 << num2 << std::endl
#else

#define _log_s(str)
#define _log_n(str, num)
#define _log_2n(str1, num1, str2, num2)

#endif