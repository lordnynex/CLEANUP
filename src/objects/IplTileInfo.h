#ifndef  IPLTILEINFO_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define IPLTILEINFO_NAME "IplTileInfo"

#define checkIplTileInfo(L,i) luacv_checkObject<IplTileInfo>(L,i,IPLTILEINFO_NAME)
#define pushIplTileInfo(L,data) luacv_pushObject<IplTileInfo>(L,data,IPLTILEINFO_NAME,true)

extern const struct luaL_Reg IplTileInfo_m[];
#endif
