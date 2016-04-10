#ifndef IPLCONVKERNEL_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define IPLCONVKERNEL_NAME "IplConvKernel"
#define checkIplConvKernel(L,i) luacv_checkObject<IplConvKernel>(L,i,IPLCONVKERNEL_NAME)
#define pushIplConvKernel(L,data) luacv_pushObject<IplConvKernel>(L,data,IPLCONVKERNEL_NAME,true)

extern const struct luaL_Reg IplConvKernel_m[];
#endif
