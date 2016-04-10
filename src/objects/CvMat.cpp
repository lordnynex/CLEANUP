#include "CvMat.h"

static int CvMat_tostring(lua_State *L)
{
  CvMat *mat=checkCvMat(L,1);
  lua_pushfstring(L,CVMAT_NAME" object %p\n\ttype=%d\n\tstep=%d\n\trows,height=%d\n\tcols,width=%d",mat,CV_MAT_TYPE(mat->type),mat->step,mat->rows,mat->cols);
  return 1;
}
static int CvMat_gc(lua_State *L)
{
  luacv_obj<CvMat>*obj=(luacv_obj<CvMat>*)luaL_checkudata(L,1,CVMAT_NAME);
  if (obj->dealloc_data)
    cvReleaseMat(&obj->data);
  return 0;
}
static int CvMat_ntype(lua_State *L)
{
    const char f_msg[]=CVMAT_NAME".type=int";
    checkCvMat(L,1)->type=checkint(L,3);
    return 0;
}

static int CvMat_type(lua_State *L)
{
    lua_pushnumber(L,checkCvMat(L,1)->type);
    return 1;
}


static int CvMat_nstep(lua_State *L)
{
    const char f_msg[]=CVMAT_NAME".step=int";
    checkCvMat(L,1)->step=checkint(L,3);
    return 0;
}
static int CvMat_step(lua_State *L)
{
    lua_pushnumber(L,checkCvMat(L,1)->step);
    return 1;
}
static int CvMat_nrows(lua_State *L)
{
    const char f_msg[]=CVMAT_NAME".rows=int";
    checkCvMat(L,1)->rows=checkint(L,3);
    return 0;
}

static int CvMat_rows(lua_State *L)
{
    lua_pushnumber(L,checkCvMat(L,1)->rows);
    return 1;
}

static int CvMat_nheight(lua_State *L)
{
    const char f_msg[]=CVMAT_NAME".height=int";
    checkCvMat(L,1)->height=checkint(L,3);
    return 0;
}

static int CvMat_height(lua_State *L)
{
    lua_pushnumber(L,checkCvMat(L,1)->height);
    return 1;
}

static int CvMat_ncols(lua_State *L)
{
    const char f_msg[]=CVMAT_NAME".cols=int";
    checkCvMat(L,1)->cols=checkint(L,3);
    return 0;
}
static int CvMat_cols(lua_State *L)
{
    lua_pushnumber(L,checkCvMat(L,1)->cols);
    return 1;
}

static int CvMat_nwidth(lua_State *L)
{
    const char f_msg[]=CVMAT_NAME".width=int";
    checkCvMat(L,1)->width=checkint(L,3);
    return 0;
}
static int CvMat_width(lua_State *L)
{
    lua_pushnumber(L,checkCvMat(L,1)->width);
    return 1;
}

static int CvMat_ndata(lua_State *L)
{
    UNIMPL(L);
    return 0;
}

static int CvMat_data(lua_State *L)
{
    UNIMPL(L);
    return 1;
}

static const luacv_method CvMat_v[]=
{
  {NULL,8,NULL,NULL},
  methodReg(cols,CvMat),
  methodReg(data,CvMat),
  methodReg(height,CvMat),
  methodReg(rows,CvMat),
  methodReg(step,CvMat),
  methodReg(type,CvMat),
  methodReg(width,CvMat),
};

makeIndexFunctions(CvMat)
makeObjectCallback(CvMat);
