#include "CvStereoGCState.h"

static int CvStereoGCState_tostring(lua_State *L)
{
  CvStereoGCState *s=checkCvStereoGCState(L,1);
  lua_pushfstring(L,CVSTEREOGCSTATE_NAME" object: %p\n\tint Ithreshold=%d\n\tint interactionRadius=%d\n\tnum K=%f\n\tnum lambda=%f\n\tnum lambda1=%f\n\tnum lambda2=%f\n\tint occlusionCost=%d\n\tint minDisparity=%d\n\tint numberOfDisparities=%d\n\tint maxIters=%d\n\t"CVMAT_NAME" left=%p\n\t"CVMAT_NAME" right=%p\n\t"CVMAT_NAME" dispLeft=%p\n\t"CVMAT_NAME" dispRight=%p\n\t"CVMAT_NAME" ptrLeft=%p"CVMAT_NAME" ptrRight=%p"CVMAT_NAME" vtxBuf=%p"CVMAT_NAME" edgeBuf=%p",s,s->Ithreshold,s->interactionRadius,s->K,s->lambda,s->lambda1,s->lambda2,s->occlusionCost,s->minDisparity,s->numberOfDisparities,s->maxIters,s->left,s->right,s->dispLeft,s->dispRight,s->ptrLeft,s->ptrRight,s->vtxBuf,s->edgeBuf);
  return 1;
}

static int CvStereoGCState_gc(lua_State *L)
{
  luacv_obj<CvStereoGCState>*obj=(luacv_obj<CvStereoGCState>*)luaL_checkudata(L,1,CVSTEREOGCSTATE_NAME);
  if (obj->dealloc_data)
    cvReleaseStereoGCState(&(obj->data)); 

  return 0;
}



static int CvStereoGCState_nnumberOfDisparities(lua_State *L)
{
  const char f_msg[]=CVSTEREOGCSTATE_NAME".numberOfDisparities=int";
  checkCvStereoGCState(L,1)->numberOfDisparities=checkint(L,3);
return 0;
}

static int CvStereoGCState_numberOfDisparities(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoGCState(L,1)->numberOfDisparities);
return 1;
}


static int CvStereoGCState_nIthreshold(lua_State *L)
{
  const char f_msg[]=CVSTEREOGCSTATE_NAME".Ithreshold=int";
  checkCvStereoGCState(L,1)->Ithreshold=checkint(L,3);
return 0;
}

static int CvStereoGCState_Ithreshold(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoGCState(L,1)->Ithreshold);
return 1;
}
static int CvStereoGCState_ninteractionRadius(lua_State *L)
{
  const char f_msg[]=CVSTEREOGCSTATE_NAME".interactionRadius=int";
  checkCvStereoGCState(L,1)->interactionRadius=checkint(L,3);
return 0;
}

static int CvStereoGCState_interactionRadius(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoGCState(L,1)->interactionRadius);
return 1;
}
static int CvStereoGCState_nK(lua_State *L)
{
  const char f_msg[]=CVSTEREOGCSTATE_NAME".K=num";
  checkCvStereoGCState(L,1)->K=checknumber(L,3);
return 0;
}

static int CvStereoGCState_K(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoGCState(L,1)->K);
return 1;
}

static int CvStereoGCState_nlambda(lua_State *L)
{
  const char f_msg[]=CVSTEREOGCSTATE_NAME".lambda=num";
  checkCvStereoGCState(L,1)->lambda=checknumber(L,3);
return 0;
}

static int CvStereoGCState_lambda(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoGCState(L,1)->lambda);
return 1;
}

static int CvStereoGCState_nlambda1(lua_State *L)
{
  const char f_msg[]=CVSTEREOGCSTATE_NAME".lambda1=num";
  checkCvStereoGCState(L,1)->lambda1=checknumber(L,3);
return 0;
}

static int CvStereoGCState_lambda1(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoGCState(L,1)->lambda1);
return 1;
}

static int CvStereoGCState_nlambda2(lua_State *L)
{
  const char f_msg[]=CVSTEREOGCSTATE_NAME".lambda2=num";
  checkCvStereoGCState(L,1)->lambda2=checknumber(L,3);
return 0;
}

static int CvStereoGCState_lambda2(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoGCState(L,1)->lambda2);
return 1;
}

static int CvStereoGCState_nocclusionCost(lua_State *L)
{
  const char f_msg[]=CVSTEREOGCSTATE_NAME".occlusionCost=int";
  checkCvStereoGCState(L,1)->occlusionCost=checkint(L,3);
return 0;
}

static int CvStereoGCState_occlusionCost(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoGCState(L,1)->occlusionCost);
return 1;
}
static int CvStereoGCState_nminDisparity(lua_State *L)
{
  const char f_msg[]=CVSTEREOGCSTATE_NAME".minDisparity=int";
  checkCvStereoGCState(L,1)->minDisparity=checkint(L,3);
return 0;
}

static int CvStereoGCState_minDisparity(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoGCState(L,1)->minDisparity);
return 1;
}

static int CvStereoGCState_nmaxIters(lua_State *L)
{
  const char f_msg[]=CVSTEREOGCSTATE_NAME".maxIters=int";
  checkCvStereoGCState(L,1)->maxIters=checkint(L,3);
return 0;
}

static int CvStereoGCState_maxIters(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoGCState(L,1)->maxIters);
return 1;
}



static int CvStereoGCState_nleft(lua_State *L)
{
  checkCvStereoGCState(L,1)->left=checkCvMat(L,3);
return 0;
}

static int CvStereoGCState_left(lua_State *L)
{
  pushUserData(L,checkCvStereoGCState(L,1)->left);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
return 1;
}

static int CvStereoGCState_nright(lua_State *L)
{
  checkCvStereoGCState(L,1)->right=checkCvMat(L,3);
return 0;
}

static int CvStereoGCState_right(lua_State *L)
{
  pushUserData(L,checkCvStereoGCState(L,1)->right);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
return 1;
}

static int CvStereoGCState_ndispLeft(lua_State *L)
{
  checkCvStereoGCState(L,1)->dispLeft=checkCvMat(L,3);
return 0;
}

static int CvStereoGCState_dispLeft(lua_State *L)
{
  pushUserData(L,checkCvStereoGCState(L,1)->dispLeft);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
return 1;
}

static int CvStereoGCState_ndispRight(lua_State *L)
{
  checkCvStereoGCState(L,1)->dispRight=checkCvMat(L,3);
return 0;
}

static int CvStereoGCState_dispRight(lua_State *L)
{
  pushUserData(L,checkCvStereoGCState(L,1)->dispRight);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
return 1;
}

static int CvStereoGCState_nptrLeft(lua_State *L)
{
  checkCvStereoGCState(L,1)->ptrLeft=checkCvMat(L,3);
return 0;
}

static int CvStereoGCState_ptrLeft(lua_State *L)
{
  pushUserData(L,checkCvStereoGCState(L,1)->ptrLeft);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
return 1;
}

static int CvStereoGCState_nptrRight(lua_State *L)
{
  checkCvStereoGCState(L,1)->ptrRight=checkCvMat(L,3);
return 0;
}

static int CvStereoGCState_ptrRight(lua_State *L)
{
  pushUserData(L,checkCvStereoGCState(L,1)->ptrRight);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
return 1;
}

static int CvStereoGCState_nvtxBuf(lua_State *L)
{
  checkCvStereoGCState(L,1)->vtxBuf=checkCvMat(L,3);
return 0;
}

static int CvStereoGCState_vtxBuf(lua_State *L)
{
  pushUserData(L,checkCvStereoGCState(L,1)->vtxBuf);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
return 1;
}

static int CvStereoGCState_nedgeBuf(lua_State *L)
{
  checkCvStereoGCState(L,1)->edgeBuf=checkCvMat(L,3);
return 0;
}

static int CvStereoGCState_edgeBuf(lua_State *L)
{
  pushUserData(L,checkCvStereoGCState(L,1)->edgeBuf);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
return 1;
}

static const luacv_method CvStereoGCState_v[]=
{
  {NULL,19,NULL,NULL},
  methodReg(Ithreshold,CvStereoGCState),
  methodReg(K,CvStereoGCState),
  methodReg(dispLeft,CvStereoGCState),
  methodReg(dispRight,CvStereoGCState),
  methodReg(edgeBuf,CvStereoGCState),
  methodReg(interactionRadius,CvStereoGCState),
  methodReg(lambda,CvStereoGCState),
  methodReg(lambda1,CvStereoGCState),
  methodReg(lambda2,CvStereoGCState),
  methodReg(left,CvStereoGCState),
  methodReg(minDisparity,CvStereoGCState),
  methodReg(maxIters,CvStereoGCState),
  methodReg(numberOfDisparities,CvStereoGCState),
  methodReg(occlusionCost,CvStereoGCState),
  methodReg(ptrLeft,CvStereoGCState),
  methodReg(ptrRight,CvStereoGCState),
  methodReg(right,CvStereoGCState),
  methodReg(vtxBuf,CvStereoGCState),
};

makeIndexFunctions(CvStereoGCState)
makeObjectCallback(CvStereoGCState);
