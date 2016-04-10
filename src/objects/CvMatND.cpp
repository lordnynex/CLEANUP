#include "CvMatND.h"

static int CvMatND_tostring(lua_State *L)
{
  CvMatND *mat=checkCvMatND(L,1);
  lua_pushfstring(L,CVMATND_NAME" object %p\n\ttype=%d\n\tdims=%d\n\thdr_refcount=%d",mat,mat->type,mat->dims,mat->hdr_refcount);
  return 1;
}

static int CvMatND_gc(lua_State *L)
{

  luacv_obj<CvMatND>*obj=(luacv_obj<CvMatND>*)luaL_checkudata(L,1,CVMATND_NAME);
  if (obj->dealloc_data)
    cvReleaseMatND(&obj->data);
  return 0;
}


static int CvMatND_ntype(lua_State *L)
{
  const char f_msg[]=CVMATND_NAME".type=int";
  checkCvMatND(L,1)->type=checkint(L,3);
return 0;
}

static int CvMatND_type(lua_State *L)
{
  lua_pushnumber(L,checkCvMatND(L,1)->type);
return 1;
}

static int CvMatND_ndims(lua_State *L)
{
  const char f_msg[]=CVMATND_NAME".dims=int";
  checkCvMatND(L,1)->dims=checkint(L,3);
return 0;
}

static int CvMatND_dims(lua_State *L)
{
  lua_pushnumber(L,checkCvMatND(L,1)->dims);
return 1;
}

static int CvMatND_nhdr_refcount(lua_State *L)
{
  const char f_msg[]=CVMATND_NAME".hdr_refcount=int";
  checkCvMatND(L,1)->hdr_refcount=checkint(L,3);
return 0;
}

static int CvMatND_hdr_refcount(lua_State *L)
{
  lua_pushnumber(L,checkCvMatND(L,1)->hdr_refcount);
return 1;
}

static int CvMatND_nrefcount(lua_State *L)
{
  UNIMPL(L);
return 0;
}

static int CvMatND_refcount(lua_State *L)
{
  UNIMPL(L);
return 1;
}

static int CvMatND_ndata(lua_State *L)
{
  UNIMPL(L);
return 0;
}

static int CvMatND_data(lua_State *L)
{
  UNIMPL(L);
return 1;
}
static int CvMatND_ndim(lua_State *L)
{
  if ((!lua_istable(L,3))|| (lua_objlen(L,3)>CV_MAX_DIM))luaL_error(L,CVMATND_NAME"->dim must be table of  elements indexed from 0. Maximum number of elements must be CV_MAX_DIM");
  const char f_msg[]=CVMATND_NAME".dim[CV_MAX_DIM] elements must be integer";

  CvMatND *mat=checkCvMatND(L,1);
  unsigned int len=lua_objlen(L,3);
  for(unsigned int i=0;i<len;i++)
  {
    lua_rawgeti(L,3,i);
    lua_getfield(L,4,"size");
    mat->dim[i].size=checkint(L,5);
    lua_pop(L,1);
    lua_getfield(L,4,"step");
    mat->dim[i].step=checkint(L,5);
    lua_pop(L,2);
  }
return 0;
}

static int CvMatND_dim(lua_State *L)
{
  CvMatND *mat=checkCvMatND(L,1);
  lua_newtable(L);
  for(unsigned int i=0;i<CV_MAX_DIM;i++)
  {
    lua_newtable(L);
    lua_pushnumber(L,mat->dim[i].size);
    lua_setfield(L,-2,"size");
    lua_pushnumber(L,mat->dim[i].step);
    lua_setfield(L,-2,"step");
  }
return 1;
}


static const luacv_method CvMatND_v[]=
{
  {NULL,7,NULL,NULL},
  methodReg(data,CvMatND),
  methodReg(dim,CvMatND),
  methodReg(dims,CvMatND),
  methodReg(hdr_refcount,CvMatND),
  methodReg(refcount,CvMatND),
  methodReg(type,CvMatND),
};

makeIndexFunctions(CvMatND)
makeObjectCallback(CvMatND);
