#include "CvHuMoments.h"

static int CvHuMoments_tostring(lua_State *L)
{
  CvHuMoments *m=checkCvHuMoments(L,1);
  lua_pushfstring(L,CVHUMOMENTS_NAME" object: %p\n\thu1=%d\n\thu2=%d\n\thu3=%d\n\thu4=%d\n\thu5=%d\n\thu6=%d\n\thu7=%d\n\t",m,m->hu1,m->hu2,m->hu3,m->hu4,m->hu5,m->hu6,m->hu7);
  return 1;
}

static int CvHuMoments_gc(lua_State *L)
{

  luacv_obj<CvHuMoments>*obj=(luacv_obj<CvHuMoments>*)luaL_checkudata(L,1,CVHUMOMENTS_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvHuMoments_nhu1(lua_State *L)
{
  const char f_msg[]=CVHUMOMENTS_NAME".hu1=num";
  checkCvHuMoments(L,1)->hu1=checknumber(L,3);
return 0;
}

static int CvHuMoments_hu1(lua_State *L)
{
  lua_pushnumber(L,checkCvHuMoments(L,1)->hu1);
return 1;
}

static int CvHuMoments_nhu2(lua_State *L)
{
  const char f_msg[]=CVHUMOMENTS_NAME".hu2=num";
  checkCvHuMoments(L,1)->hu2=checknumber(L,3);
return 0;
}

static int CvHuMoments_hu2(lua_State *L)
{
  lua_pushnumber(L,checkCvHuMoments(L,1)->hu2);
return 1;
}


static int CvHuMoments_nhu3(lua_State *L)
{
  const char f_msg[]=CVHUMOMENTS_NAME".hu3=num";
  checkCvHuMoments(L,1)->hu3=checknumber(L,3);
return 0;
}

static int CvHuMoments_hu3(lua_State *L)
{
  lua_pushnumber(L,checkCvHuMoments(L,1)->hu3);
return 1;
}


static int CvHuMoments_nhu4(lua_State *L)
{
  const char f_msg[]=CVHUMOMENTS_NAME".hu4=num";
  checkCvHuMoments(L,1)->hu4=checknumber(L,3);
return 0;
}

static int CvHuMoments_hu4(lua_State *L)
{
  lua_pushnumber(L,checkCvHuMoments(L,1)->hu4);
return 1;
}


static int CvHuMoments_nhu5(lua_State *L)
{
  const char f_msg[]=CVHUMOMENTS_NAME".hu5=num";
  checkCvHuMoments(L,1)->hu5=checknumber(L,3);
return 0;
}

static int CvHuMoments_hu5(lua_State *L)
{
  lua_pushnumber(L,checkCvHuMoments(L,1)->hu5);
return 1;
}


static int CvHuMoments_nhu6(lua_State *L)
{
  const char f_msg[]=CVHUMOMENTS_NAME".hu6=num";
  checkCvHuMoments(L,1)->hu6=checknumber(L,3);
return 0;
}

static int CvHuMoments_hu6(lua_State *L)
{
  lua_pushnumber(L,checkCvHuMoments(L,1)->hu6);
return 1;
}


static int CvHuMoments_nhu7(lua_State *L)
{
  const char f_msg[]=CVHUMOMENTS_NAME".hu7=num";
  checkCvHuMoments(L,1)->hu7=checknumber(L,3);
return 0;
}

static int CvHuMoments_hu7(lua_State *L)
{
  lua_pushnumber(L,checkCvHuMoments(L,1)->hu7);
return 1;
}


static const luacv_method CvHuMoments_v[]=
{
  {NULL,8,NULL,NULL},
  methodReg(hu1,CvHuMoments),
  methodReg(hu2,CvHuMoments),
  methodReg(hu3,CvHuMoments),
  methodReg(hu4,CvHuMoments),
  methodReg(hu5,CvHuMoments),
  methodReg(hu6,CvHuMoments),
  methodReg(hu7,CvHuMoments),
};

makeIndexFunctions(CvHuMoments)
makeObjectCallback(CvHuMoments);
