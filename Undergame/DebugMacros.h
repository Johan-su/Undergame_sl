#pragma once
#include <iostream>

//#define ECS_DEBUG
//#define PRINT_DEBUG











#ifdef PRINT_DEBUG

#define DP(x) std::cout << x << std::endl

#else

#define DP(x) while(0){}

#endif

