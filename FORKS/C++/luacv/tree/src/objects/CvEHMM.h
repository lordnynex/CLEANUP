#ifndef CVEHMM_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"

#define CVEHMM_NAME "CvEHMM"
#define checkCvEHMM(L,i) luacv_checkObject<CvEHMM>(L,i,CVEHMM_NAME)
#define pushCvEHMM(L,data) luacv_pushObject<CvEHMM>(L,data,CVEHMM_NAME,false)

extern const struct luaL_Reg CvEHMM_m[];
#endif
