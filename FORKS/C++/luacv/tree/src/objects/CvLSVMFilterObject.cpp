#include "CvLSVMFilterObject.h"
static int CvLSVMFilterObject_tostring(lua_State *L)
{
  CvLSVMFilterObject *e=checkCvLSVMFilterObject(L,1);
  lua_pushfstring(L,CVLSVMFILTEROBJECT_NAME" object %p\n\t"CVLSVMFILTERPOSITION_NAME" V=%p\n\tfineFunction={ [1]=>%d, [2]=>%d, [3]=>%d, [4]=>%d }\n\tint sizeX=%d\n\tint sizeY=%d\n\tnum H=%p",e,&e->V,e->fineFunction[0],e->fineFunction[1],e->fineFunction[2],e->fineFunction[3],e->sizeX,e->sizeY,e->H);
  return 1;
}

static int CvLSVMFilterObject_gc(lua_State *L)
{
  luacv_obj<CvLSVMFilterObject>*obj=(luacv_obj<CvLSVMFilterObject>*)luaL_checkudata(L,1,CVLSVMFILTEROBJECT_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvLSVMFilterObject_fineFunction(lua_State *L)
{
    CvLSVMFilterObject *s=checkCvLSVMFilterObject(L,1);
    lua_newtable(L);
    for(int i=0;i<4;i++)
    {
      lua_pushnumber(L,s->fineFunction[i]);
      lua_rawseti(L,3,i+1);
    }
    return 1;
}

static int CvLSVMFilterObject_nfineFunction(lua_State *L)
{
    const char f_msg[]=CVLSVMFILTEROBJECT_NAME".fineFunction={num, num, num}";
    CvLSVMFilterObject *s=checkCvLSVMFilterObject(L,1);
    if ((!lua_istable(L,3))||(lua_objlen(L,3)!=4)) luaL_error(L,f_msg);
    for(int i=0;i<4;i++) 
    {
      lua_rawgeti(L,3,i+1);
      s->fineFunction[i]=checknumber(L,4);
      lua_pop(L,1);
    }
    return 0;
}

static int CvLSVMFilterObject_sizeX(lua_State *L)
{
    lua_pushnumber(L,checkCvLSVMFilterObject(L,1)->sizeX);
    return 1;
}

static int CvLSVMFilterObject_nsizeX(lua_State *L)
{
    const char f_msg[]=CVLSVMFILTEROBJECT_NAME".sizeX=int";
    checkCvLSVMFilterObject(L,1)->sizeX=checkint(L,3);
    return 0;
}
static int CvLSVMFilterObject_sizeY(lua_State *L)
{
    lua_pushnumber(L,checkCvLSVMFilterObject(L,1)->sizeY);
    return 1;
}

static int CvLSVMFilterObject_nsizeY(lua_State *L)
{
    const char f_msg[]=CVLSVMFILTEROBJECT_NAME".sizeY=int";
    checkCvLSVMFilterObject(L,1)->sizeY=checkint(L,3);
    return 0;
}
static int CvLSVMFilterObject_V(lua_State *L)
{
    pushCvLSVMFilterPosition(L,&checkCvLSVMFilterObject(L,1)->V);
    return 1;
}

static int CvLSVMFilterObject_nV(lua_State *L)
{
    checkCvLSVMFilterObject(L,1)->V=*checkCvLSVMFilterPosition(L,3);
    return 0;
}

/*
static int CvLSVMFilterObject_numFeatures(lua_State *L)
{
    lua_pushnumber(L,checkCvLSVMFilterObject(L,1)->numFeatures);
    return 1;
}

static int CvLSVMFilterObject_nnumFeatures(lua_State *L)
{
    const char f_msg[]=CVLSVMFILTEROBJECT_NAME".numFeatures=int";
    checkCvLSVMFilterObject(L,1)->numFeatures=checkint(L,3);
    return 0;
}
*/


static int CvLSVMFilterObject_H(lua_State *L)
{
    UNIMPL(L);
    return 1;
}

static int CvLSVMFilterObject_nH(lua_State *L)
{
    UNIMPL(L);
    return 1;
}


static const luacv_method CvLSVMFilterObject_v[]=
{
  {NULL,6,NULL,NULL},
  methodReg(H,CvLSVMFilterObject),
  methodReg(fineFunction,CvLSVMFilterObject),
  //methodReg(numFeatures,CvLSVMFilterObject),
  methodReg(sizeX,CvLSVMFilterObject),
  methodReg(sizeY,CvLSVMFilterObject),
  methodReg(V,CvLSVMFilterObject),
};

makeIndexFunctions(CvLSVMFilterObject)
makeObjectCallback(CvLSVMFilterObject);
