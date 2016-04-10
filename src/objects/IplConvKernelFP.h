#ifndef IPLCONVKERNELFP_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define IPLCONVKERNELFP_NAME "IplConvKernelFP"

#define checkIplConvKernelFP(L,i) luacv_checkObject<IplConvKernelFP>(L,i,IPLCONVKERNELFP_NAME)
#define pushIplConvKernelFP(L,data) luacv_pushObject<IplConvKernelFP>(L,data,IPLCONVKERNELFP_NAME,true)

extern const struct luaL_Reg IplConvKernelFP_m[];
#endif
