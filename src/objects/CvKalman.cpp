#include "CvKalman.h"

static int CvKalman_tostring(lua_State *L)
{
  CvKalman *k=checkCvKalman(L,1);
  lua_pushfstring(L,CVKALMAN_NAME" object %p\n\tint MP=%d\n\tint DP=%d\n\t"\
  "int CP=%d\n\tnum[] PosterState=%p\n\tnum[] PriorState=%p\n\tnum[] DynamMatr=%p\n\t"\
  "num[] MeasurementMatr=%p\n\tnum[] MNCovariance=%p\n\tnum[] PNCovariance=%p\n\t"\
  "num[] KalmGainMatr=%p\n\tnum[] PriorErrorCovariance=%p\n\tnum[] PosterErrorCovariance=%p\n\tnum[] Temp1=%p\n\tnum[] Temp2=%p\n\t"CVMAT_NAME" state_pre\n\t"CVMAT_NAME" state_post\n\t"CVMAT_NAME" state_transition_matrix\n\t"CVMAT_NAME" control_matrix\n\t"\
 CVMAT_NAME" measurement_matrix\n\t"CVMAT_NAME" process_noise_cov\n\t"CVMAT_NAME" measurement_noise_cov\n\t"CVMAT_NAME" error_cov_pre\n\t"CVMAT_NAME" state_pre\n\t"\
CVMAT_NAME" gain\n\t"CVMAT_NAME" error_cov_post\n\t"CVMAT_NAME" temp1\n\t"CVMAT_NAME" temp2\n\t"CVMAT_NAME" temp3\n\t"CVMAT_NAME" temp4\n\t"CVMAT_NAME" temp5",k,k->MP,k->DP,k->CP,k->PosterState,k->PriorState,k->DynamMatr,k->MeasurementMatr,k->MNCovariance,k->PNCovariance,k->KalmGainMatr,k->PriorErrorCovariance,k->PosterErrorCovariance,k->Temp1,k->Temp2,k->state_pre,k->state_post,k->transition_matrix,k->control_matrix,k->measurement_matrix,k->process_noise_cov,k->measurement_noise_cov,k->error_cov_pre,k->gain,k->error_cov_post,k->temp1,k->temp2,k->temp3,k->temp4);
  return 1;
}

static int CvKalman_gc(lua_State *L)
{

  luacv_obj<CvKalman>*obj=(luacv_obj<CvKalman>*)luaL_checkudata(L,1,CVKALMAN_NAME);
  if (obj->dealloc_data)
    cvReleaseKalman(&obj->data);
  return 0;
}

static int CvKalman_nMP(lua_State *L)
{
  const char f_msg[]=CVKALMAN_NAME".MP=int";
  checkCvKalman(L,1)->MP=checkint(L,3);
  return 0;
}

static int CvKalman_MP(lua_State *L)
{
  lua_pushnumber(L,checkCvKalman(L,1)->MP);
  return 1;
}

static int CvKalman_nDP(lua_State *L)
{
  const char f_msg[]=CVKALMAN_NAME".DP=int";
  checkCvKalman(L,1)->DP=checkint(L,3);
  return 0;
}

static int CvKalman_DP(lua_State *L)
{
  lua_pushnumber(L,checkCvKalman(L,1)->DP);
  return 1;
}
static int CvKalman_nCP(lua_State *L)
{
  const char f_msg[]=CVKALMAN_NAME".CP=int";
  checkCvKalman(L,1)->CP=checkint(L,3);
  return 0;
}

static int CvKalman_CP(lua_State *L)
{
  lua_pushnumber(L,checkCvKalman(L,1)->CP);
  return 1;
}

static int CvKalman_nstate_pre(lua_State *L)
{
  //const char f_msg[]=CVKALMAN_NAME".state_pre="CVMAT_NAME;
  checkCvKalman(L,1)->state_pre=checkCvMat(L,3);
  return 0;
}

static int CvKalman_state_pre(lua_State *L)
{
  pushUserData(L,checkCvKalman(L,1)->state_pre);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
  return 1;
}
static int CvKalman_nstate_post(lua_State *L)
{
  //const char f_msg[]=CVKALMAN_NAME".state_pos="CVMAT_NAME;
  checkCvKalman(L,1)->state_post=checkCvMat(L,3);
  return 0;
}

static int CvKalman_state_post(lua_State *L)
{
  pushUserData(L,checkCvKalman(L,1)->state_post);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
  return 1;
}
static int CvKalman_ntransition_matrix(lua_State *L)
{
  //const char f_msg[]=CVKALMAN_NAME".transition_matrix="CVMAT_NAME;
  checkCvKalman(L,1)->transition_matrix=checkCvMat(L,3);
  return 0;
}

static int CvKalman_transition_matrix(lua_State *L)
{
  pushUserData(L,checkCvKalman(L,1)->transition_matrix);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
  return 1;
}
static int CvKalman_ncontrol_matrix(lua_State *L)
{
  //const char f_msg[]=CVKALMAN_NAME".control_matrix="CVMAT_NAME;
  checkCvKalman(L,1)->control_matrix=checkCvMat(L,3);
  return 0;
}

static int CvKalman_control_matrix(lua_State *L)
{
  pushUserData(L,checkCvKalman(L,1)->control_matrix);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
  return 1;
}
static int CvKalman_nmeasurement_matrix(lua_State *L)
{
  //const char f_msg[]=CVKALMAN_NAME".measurement_matrix="CVMAT_NAME;
  checkCvKalman(L,1)->measurement_matrix=checkCvMat(L,3);
  return 0;
}

static int CvKalman_measurement_matrix(lua_State *L)
{
  pushUserData(L,checkCvKalman(L,1)->measurement_matrix);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
  return 1;
}
static int CvKalman_nprocess_noise_cov(lua_State *L)
{
  //const char f_msg[]=CVKALMAN_NAME".process_noise_cov="CVMAT_NAME;
  checkCvKalman(L,1)->process_noise_cov=checkCvMat(L,3);
  return 0;
}

static int CvKalman_process_noise_cov(lua_State *L)
{
  pushUserData(L,checkCvKalman(L,1)->process_noise_cov);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
  return 1;
}
static int CvKalman_nmeasurement_noise_cov(lua_State *L)
{
  //const char f_msg[]=CVKALMAN_NAME".measurement_noise_cov="CVMAT_NAME;
  checkCvKalman(L,1)->measurement_noise_cov=checkCvMat(L,3);
  return 0;
}

static int CvKalman_measurement_noise_cov(lua_State *L)
{
  pushUserData(L,checkCvKalman(L,1)->measurement_noise_cov);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
  return 1;
}
static int CvKalman_nerror_cov_pre(lua_State *L)
{
  //const char f_msg[]=CVKALMAN_NAME".error_cov_pre="CVMAT_NAME;
  checkCvKalman(L,1)->error_cov_pre=checkCvMat(L,3);
  return 0;
}

static int CvKalman_error_cov_pre(lua_State *L)
{
  pushUserData(L,checkCvKalman(L,1)->error_cov_pre);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
  return 1;
}
static int CvKalman_ngain(lua_State *L)
{
  //const char f_msg[]=CVKALMAN_NAME".gain="CVMAT_NAME;
  checkCvKalman(L,1)->gain=checkCvMat(L,3);
  return 0;
}

static int CvKalman_gain(lua_State *L)
{
  pushUserData(L,checkCvKalman(L,1)->gain);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
  return 1;
}
static int CvKalman_nerror_cov_post(lua_State *L)
{
  //const char f_msg[]=CVKALMAN_NAME".error_cov_post="CVMAT_NAME;
  checkCvKalman(L,1)->error_cov_post=checkCvMat(L,3);
  return 0;
}

static int CvKalman_error_cov_post(lua_State *L)
{
  pushUserData(L,checkCvKalman(L,1)->error_cov_post);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
  return 1;
}
static int CvKalman_ntemp1(lua_State *L)
{
  //const char f_msg[]=CVKALMAN_NAME".temp1="CVMAT_NAME;
  checkCvKalman(L,1)->temp1=checkCvMat(L,3);
  return 0;
}

static int CvKalman_temp1(lua_State *L)
{
  pushUserData(L,checkCvKalman(L,1)->temp1);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
  return 1;
}
static int CvKalman_ntemp2(lua_State *L)
{
  //const char f_msg[]=CVKALMAN_NAME".temp2="CVMAT_NAME;
  checkCvKalman(L,1)->temp2=checkCvMat(L,3);
  return 0;
}

static int CvKalman_temp2(lua_State *L)
{
  pushUserData(L,checkCvKalman(L,1)->temp2);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
  return 1;
}
static int CvKalman_ntemp3(lua_State *L)
{
  //const char f_msg[]=CVKALMAN_NAME".temp3="CVMAT_NAME;
  checkCvKalman(L,1)->temp3=checkCvMat(L,3);
  return 0;
}

static int CvKalman_temp3(lua_State *L)
{
  pushUserData(L,checkCvKalman(L,1)->temp3);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
  return 1;
}
static int CvKalman_ntemp4(lua_State *L)
{
  //const char f_msg[]=CVKALMAN_NAME".temp4="CVMAT_NAME;
  checkCvKalman(L,1)->temp4=checkCvMat(L,3);
  return 0;
}

static int CvKalman_temp4(lua_State *L)
{
  pushUserData(L,checkCvKalman(L,1)->temp4);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
  return 1;
}
static int CvKalman_ntemp5(lua_State *L)
{
  //const char f_msg[]=CVKALMAN_NAME".temp5="CVMAT_NAME;
  checkCvKalman(L,1)->temp5=checkCvMat(L,3);
  return 0;
}

static int CvKalman_temp5(lua_State *L)
{
  pushUserData(L,checkCvKalman(L,1)->temp5);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
  return 1;
}

static const luacv_method CvKalman_v[]=
{
  {NULL,19,NULL,NULL},
  methodReg(CP,CvKalman),
  methodReg(DP,CvKalman),
  methodReg(MP,CvKalman),
  methodReg(control_matrix,CvKalman),
  methodReg(error_cov_pre,CvKalman),
  methodReg(error_cov_post,CvKalman),
  methodReg(gain,CvKalman),
  methodReg(measurement_noise_cov,CvKalman),
  methodReg(measurement_matrix,CvKalman),
  methodReg(process_noise_cov,CvKalman),
  methodReg(state_pre,CvKalman),
  methodReg(state_post,CvKalman),
  methodReg(transition_matrix,CvKalman),
  methodReg(temp1,CvKalman),
  methodReg(temp2,CvKalman),
  methodReg(temp3,CvKalman),
  methodReg(temp4,CvKalman),
  methodReg(temp5,CvKalman)
};

makeIndexFunctions(CvKalman)
makeObjectCallback(CvKalman);
