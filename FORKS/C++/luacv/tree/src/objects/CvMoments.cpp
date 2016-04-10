#include "CvMoments.h"

static int CvMoments_tostring(lua_State *L)
{
  CvMoments *m=checkCvMoments(L,1);
  lua_pushfstring(L,CVMOMENTS_NAME" object: %p\n\tm00=%d\n\tm10=%d\n\tm01=%d\n\tm20=%d\n\tm11=%d\n\tm02=%d\n\tm30=%d\n\tm21=%d\n\tm12=%d\n\tm03=%d\n\tmu20=%d\n\tmu11=%d\n\tmu02=%d\n\tmu30=%d\n\tmu21=%d\n\tmu12=%d\n\tmu03\n\tinv_sqrt_m00=%d",m,m->m00,m->m10,m->m01,m->m20,m->m11,m->m02,m->m30,m->m21,m->m12,m->m03,m->mu20,m->mu11,m->mu02,m->mu30,m->mu21,m->mu12,m->mu03,m->inv_sqrt_m00);
  return 1;
}
static int CvMoments_gc(lua_State *L)
{

  luacv_obj<CvMoments>*obj=(luacv_obj<CvMoments>*)luaL_checkudata(L,1,CVMOMENTS_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvMoments_nm00(lua_State *L)
{
  const char f_msg[]=CVMOMENTS_NAME".m00=num";
  checkCvMoments(L,1)->m00=checknumber(L,3);
return 0;
}

static int CvMoments_m00(lua_State *L)
{
  lua_pushnumber(L,checkCvMoments(L,1)->m00);
return 1;
}

static int CvMoments_nm10(lua_State *L)
{
  const char f_msg[]=CVMOMENTS_NAME".m10=num";
  checkCvMoments(L,1)->m10=checknumber(L,3);
return 0;
}

static int CvMoments_m10(lua_State *L)
{
  lua_pushnumber(L,checkCvMoments(L,1)->m10);
return 1;
}

static int CvMoments_nm01(lua_State *L)
{
  const char f_msg[]=CVMOMENTS_NAME".m01=num";
  checkCvMoments(L,1)->m01=checknumber(L,3);
return 0;
}

static int CvMoments_m01(lua_State *L)
{
  lua_pushnumber(L,checkCvMoments(L,1)->m01);
return 1;
}

static int CvMoments_nm20(lua_State *L)
{
  const char f_msg[]=CVMOMENTS_NAME".m20=num";
  checkCvMoments(L,1)->m20=checknumber(L,3);
return 0;
}

static int CvMoments_m20(lua_State *L)
{
  lua_pushnumber(L,checkCvMoments(L,1)->m20);
return 1;
}

static int CvMoments_nm11(lua_State *L)
{
  const char f_msg[]=CVMOMENTS_NAME".m11=num";
  checkCvMoments(L,1)->m11=checknumber(L,3);
return 0;
}

static int CvMoments_m11(lua_State *L)
{
  lua_pushnumber(L,checkCvMoments(L,1)->m11);
return 1;
}

static int CvMoments_nm02(lua_State *L)
{
  const char f_msg[]=CVMOMENTS_NAME".m02=num";
  checkCvMoments(L,1)->m02=checknumber(L,3);
return 0;
}

static int CvMoments_m02(lua_State *L)
{
  lua_pushnumber(L,checkCvMoments(L,1)->m02);
return 1;
}

static int CvMoments_nm30(lua_State *L)
{
  const char f_msg[]=CVMOMENTS_NAME".m30=num";
  checkCvMoments(L,1)->m30=checknumber(L,3);
return 0;
}

static int CvMoments_m30(lua_State *L)
{
  lua_pushnumber(L,checkCvMoments(L,1)->m30);
return 1;
}

static int CvMoments_nm21(lua_State *L)
{
  const char f_msg[]=CVMOMENTS_NAME".m21=num";
  checkCvMoments(L,1)->m21=checknumber(L,3);
return 0;
}

static int CvMoments_m21(lua_State *L)
{
  lua_pushnumber(L,checkCvMoments(L,1)->m21);
return 1;
}

static int CvMoments_nm12(lua_State *L)
{
  const char f_msg[]=CVMOMENTS_NAME".m12=num";
  checkCvMoments(L,1)->m12=checknumber(L,3);
return 0;
}

static int CvMoments_m12(lua_State *L)
{
  lua_pushnumber(L,checkCvMoments(L,1)->m12);
return 1;
}

static int CvMoments_nm03(lua_State *L)
{
  const char f_msg[]=CVMOMENTS_NAME".m03=num";
  checkCvMoments(L,1)->m03=checknumber(L,3);
return 0;
}

static int CvMoments_m03(lua_State *L)
{
  lua_pushnumber(L,checkCvMoments(L,1)->m03);
return 1;
}


static int CvMoments_nmu20(lua_State *L)
{
  const char f_msg[]=CVMOMENTS_NAME".mu20=num";
  checkCvMoments(L,1)->mu20=checknumber(L,3);
return 0;
}

static int CvMoments_mu20(lua_State *L)
{
  lua_pushnumber(L,checkCvMoments(L,1)->mu20);
return 1;
}

static int CvMoments_nmu11(lua_State *L)
{
  const char f_msg[]=CVMOMENTS_NAME".mu11=num";
  checkCvMoments(L,1)->mu11=checknumber(L,3);
return 0;
}

static int CvMoments_mu11(lua_State *L)
{
  lua_pushnumber(L,checkCvMoments(L,1)->mu11);
return 1;
}

static int CvMoments_nmu02(lua_State *L)
{
  const char f_msg[]=CVMOMENTS_NAME".mu02=num";
  checkCvMoments(L,1)->mu02=checknumber(L,3);
return 0;
}

static int CvMoments_mu02(lua_State *L)
{
  lua_pushnumber(L,checkCvMoments(L,1)->mu02);
return 1;
}

static int CvMoments_nmu30(lua_State *L)
{
  const char f_msg[]=CVMOMENTS_NAME".mu30=num";
  checkCvMoments(L,1)->mu30=checknumber(L,3);
return 0;
}

static int CvMoments_mu30(lua_State *L)
{
  lua_pushnumber(L,checkCvMoments(L,1)->mu30);
return 1;
}

static int CvMoments_nmu21(lua_State *L)
{
  const char f_msg[]=CVMOMENTS_NAME".mu21=num";
  checkCvMoments(L,1)->mu21=checknumber(L,3);
return 0;
}

static int CvMoments_mu21(lua_State *L)
{
  lua_pushnumber(L,checkCvMoments(L,1)->mu21);
return 1;
}

static int CvMoments_nmu12(lua_State *L)
{
  const char f_msg[]=CVMOMENTS_NAME".mu12=num";
  checkCvMoments(L,1)->mu12=checknumber(L,3);
return 0;
}

static int CvMoments_mu12(lua_State *L)
{
  lua_pushnumber(L,checkCvMoments(L,1)->mu12);
return 1;
}

static int CvMoments_nmu03(lua_State *L)
{
  const char f_msg[]=CVMOMENTS_NAME".mu03=num";
  checkCvMoments(L,1)->mu03=checknumber(L,3);
return 0;
}

static int CvMoments_mu03(lua_State *L)
{
  lua_pushnumber(L,checkCvMoments(L,1)->mu03);
return 1;
}

static int CvMoments_ninv_sqrt_m00(lua_State *L)
{
  const char f_msg[]=CVMOMENTS_NAME".inv_sqrt_m00=num";
  checkCvMoments(L,1)->inv_sqrt_m00=checknumber(L,3);
return 0;
}

static int CvMoments_inv_sqrt_m00(lua_State *L)
{
  lua_pushnumber(L,checkCvMoments(L,1)->inv_sqrt_m00);
return 1;
}


static const luacv_method CvMoments_v[]=
{
  {NULL,19,NULL,NULL},
  methodReg(inv_sqrt_m00,CvMoments),
  methodReg(m00,CvMoments),
  methodReg(m10,CvMoments),
  methodReg(m01,CvMoments),
  methodReg(m20,CvMoments),
  methodReg(m11,CvMoments),
  methodReg(m02,CvMoments),
  methodReg(m30,CvMoments),
  methodReg(m21,CvMoments),
  methodReg(m12,CvMoments),
  methodReg(m03,CvMoments),
  methodReg(mu20,CvMoments),
  methodReg(mu11,CvMoments),
  methodReg(mu02,CvMoments),
  methodReg(mu30,CvMoments),
  methodReg(mu21,CvMoments),
  methodReg(mu12,CvMoments),
  methodReg(mu03,CvMoments),
};

makeIndexFunctions(CvMoments)
makeObjectCallback(CvMoments);
