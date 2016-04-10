#include "CvHistogram.h"

static int CvHistogram_tostring(lua_State *L)
{
  CvHistogram *hist=checkCvHistogram(L,1);
  lua_pushfstring(L,CVHISTOGRAM_NAME" object %p\n\ttype=%d\n\tbins="CVARR_NAME"\n\tmat="CVMATND_NAME,hist->type);

  return 1;
}

static int CvHistogram_gc(lua_State *L)
{

  luacv_obj<CvHistogram>*obj=(luacv_obj<CvHistogram>*)luaL_checkudata(L,1,CVHISTOGRAM_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvHistogram_ntype(lua_State *L)
{
  const char f_msg[]=CVHISTOGRAM_NAME".type=int";
  checkCvHistogram(L,1)->type=checkint(L,3);
return 0;
}

static int CvHistogram_type(lua_State *L)
{
  lua_pushnumber(L,checkCvHistogram(L,1)->type);
return 1;
}

static int CvHistogram_nbins(lua_State *L)
{
  const char f_msg[]=CVHISTOGRAM_NAME".bins="CVARR_NAME;
  checkCvHistogram(L,1)->bins=checkldata(L,3);
return 0;
}

static int CvHistogram_bins(lua_State *L)
{
  pushUserData(L,checkCvHistogram(L,1)->bins);
return 1;
}

static int CvHistogram_nmat(lua_State *L)
{
  checkCvHistogram(L,1)->mat=*checkCvMatND(L,3);
return 0;
}

static int CvHistogram_mat(lua_State *L)
{
  pushCvMatND(L,&checkCvHistogram(L,1)->mat);
return 1;
}

static int CvHistogram_nthresh(lua_State *L)
{
  UNIMPL(L);
return 0;
}

static int CvHistogram_thresh(lua_State *L)
{
  UNIMPL(L);
return 1;
}

static int CvHistogram_nthresh2(lua_State *L)
{
  UNIMPL(L);
return 0;
}

static int CvHistogram_thresh2(lua_State *L)
{
  UNIMPL(L);
return 1;
}

static const luacv_method CvHistogram_v[]=
{
  {NULL,6,NULL,NULL},
  methodReg(bins,CvHistogram),
  methodReg(mat,CvHistogram),
  methodReg(type,CvHistogram),
  methodReg(thresh,CvHistogram),
  methodReg(thresh2,CvHistogram),
};

makeIndexFunctions(CvHistogram)
makeObjectCallback(CvHistogram);


