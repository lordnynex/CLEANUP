#include "lua_core.h"
static int luacv_cvCreateSeqReader(lua_State *L)
{
  const char f_msg[]=CVSEQREADER_NAME" CreateSeqReader()";  
  if (lua_gettop(L)!=0) luaL_error(L,f_msg);
  CvSeqReader reader;
  pushCvSeqReader(L,&reader);
  return 1;
}

static int luacv_cvGetTickCount(lua_State *L)
{
  const char f_msg[]="num GetTickCount()";  
  if (lua_gettop(L)!=0) luaL_error(L,f_msg);
  lua_pushnumber(L,(double)cvGetTickCount());
  return 1;
}
static int luacv_cvGetTickFrequency(lua_State *L)
{
  const char f_msg[]="num GetTickFrequency()";  
  if (lua_gettop(L)!=0) luaL_error(L,f_msg);
  lua_pushnumber(L,(double)cvGetTickFrequency());
  return 1;
}
static int luacv_cvRound(lua_State *L)
{
  const char f_msg[]="int Round(num value)";  
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushinteger(L,cvRound(checknumber(L,1)));
  return 1;
}

static int luacv_cvFloor(lua_State *L)
{
  const char f_msg[]="int Floor(num value)";  
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushinteger(L,cvFloor(checknumber(L,1)));
  return 1;
}

static int luacv_cvCeil(lua_State *L)
{
  const char f_msg[]="int Ceil(num value)";  
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushinteger(L,cvCeil(checknumber(L,1)));
  return 1;
}

static int luacv_cvInvSqrt(lua_State *L)
{
  const char f_msg[]="num InvSqrt(num value)";  
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushnumber(L,cvInvSqrt(checknumber(L,1)));
  return 1;
}

static int luacv_cvSqrt(lua_State *L)
{
  const char f_msg[]="num Sqrt(num value)";  
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushnumber(L,cvSqrt(checknumber(L,1)));
  return 1;
}

static int luacv_cvIsNaN(lua_State *L)
{
  const char f_msg[]="int IsNaN(num value)";  
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushinteger(L,cvIsNaN(checknumber(L,1)));
  return 1;
}

static int luacv_cvIsInf(lua_State *L)
{
  const char f_msg[]="int IsInf(num value)";  
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushinteger(L,cvIsInf(checknumber(L,1)));
  return 1;
}

//*************************** random number generation ***********************//

static int luacv_cvRNG(lua_State *L)
{
  const char f_msg[]=CVRNG_NAME" RNG(int seed)";  
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  CvRNG rng=cvRNG(checkint(L,1));
  pushCvRNG(L,&rng);
  return 1;
}

static int luacv_cvRandInt(lua_State *L)
{
  const char f_msg[]="int RandInt("CVRNG_NAME" rng)";  
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushinteger(L,cvRandInt(checkCvRNG(L,1)));
  return 1;
}

static int luacv_cvRandReal(lua_State *L)
{
  const char f_msg[]="num RandReal("CVRNG_NAME" rng)";  
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushnumber(L,cvRandReal(checkCvRNG(L,1)));
  return 1;
}
//*************************** image type **********************************//
static int luacv_iGet(lua_State *L)
{//nove
  const char f_msg[]="num[] iGet("IPLIMAGE_NAME" image, table type, int row, int col)";  
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  IplImage *image=checkIplImage(L,1);
  if (!image->imageData) luaL_error(L,"You are trying acces image header with no data.");
  int row=checkint(L,3),col=checkint(L,4);
  if ((row>=image->height)||(col>=image->width)) luaL_error(L,"You are trying to acces wrong dimension.");
  lua_pushstring(L,"value");
  lua_rawget(L,2);
  if ((image->depth)!=cvIplDepth(checkint(L,5))) luaL_error(L,"You are trying to acces image data with wrong type.");
  lua_pop(L,1);
  lua_pushstring(L,"from");
  lua_rawget(L,2);
  pushUserData(L,image);
  lua_call(L,1,1); 
  lua_newtable(L); 
  for(size_t i=1;i<=(size_t)image->nChannels;i++)
  {
    lua_rawgeti(L,5,image->nChannels*image->width*row+col*image->nChannels+i);
    lua_rawseti(L,-2,i);
  }
  return 1;
}

static int luacv_iSet(lua_State *L)
{//nove
  const char f_msg[]="iSet("IPLIMAGE_NAME" image, table type, int row, int col, num[] value)";  
  if (lua_gettop(L)!=5) luaL_error(L,f_msg);
  IplImage *image=checkIplImage(L,1);
  if (!image->imageData) luaL_error(L,"You are trying acces image header with no data.");
  if ((size_t)image->nChannels!=lua_objlen(L,5)) luaL_error(L,"New value vector has incorrect size.");
  int row=checkint(L,3),col=checkint(L,4);
  if ((row>=image->height)||(col>=image->width)) luaL_error(L,"You are trying to acces wrong dimension.");
  lua_getfield(L,2,"value");
  if (image->depth!=checkint(L,6)) luaL_error(L,"You are trying to acces image data with wrong type.");
  lua_pop(L,1);
  lua_getfield(L,2,"to");
  lua_remove(L,2);
  lua_insert(L,1);
  lua_insert(L,3);
  lua_call(L,4,0);
  return 0;
}

#define makeImageConvFunctionFrom(name,type_t)\
static int luacv_##name##ToTable(lua_State *L)\
{\
  const char f_msg[]="num[] "#name"ToTable(userdata image, userdata data=nil)";\
  IplImage *image=(IplImage*)checkldata(L,1);\
  size_t len=image->nChannels*image->width*image->height;\
  char *data=image->imageData;\
  if (lua_gettop(L)==2) data=(char*)checkldata(L,2);\
  lua_newtable(L);\
    for(size_t i=0;i<len;i++)\
  {\
    lua_pushnumber(L,((type_t*)data)[i]);\
    lua_rawseti(L,-2,i+1);\
  }\
  return 1;\
}

#define makeImageConvFunctionTo(name,type_t)\
static int luacv_TableTo##name(lua_State *L)\
{\
  const char f_msg[]="userdata TableTo"#name"("IPLIMAGE_NAME" image, num[] data, int row=0, int col=0)";\
  size_t len=0;\
  if (!(len=lua_objlen(L,2))) luaL_error(L,f_msg);\
  IplImage *image=(IplImage*)checkldata(L,1);\
  size_t top=lua_gettop(L);\
  int row,col;\
  switch (top)\
  {\
    case 2:\
      image->imageData=(char*)lua_newuserdata(L,image->imageSize);\
      for(size_t i=0;i<len;i++)\
      {\
        lua_rawgeti(L,2,i+1);\
        ((type_t*)image->imageData)[i]=(type_t)checknumber(L,top+2);\
        lua_pop(L,1);\
      }\
      break;\
    case 4:\
          row=checkint(L,3);\
          col=checkint(L,4);\
          for(size_t i=0;i<len;i++)\
          {\
            lua_rawgeti(L,2,i+1);\
            ((type_t*)image->imageData)[image->width*len*row+len*col+i]=(type_t)checknumber(L,top+1);\
            lua_pop(L,1);\
          }\
          break;\
    default:\
            luaL_error(L,f_msg);\
  }\
  return 1;\
}
//*************************** matrix type *********************************//
#define makeMatConvFunctionFrom(name,type_t)\
static int luacv_##name##ToTable(lua_State *L)\
{\
  const char f_msg[]="num[] "#name"ToTable(userdata data)";\
  CvMat *mat=(CvMat*)checkldata(L,1);\
  size_t len=(size_t)(CV_MAT_CN(mat->type)*mat->rows*mat->cols);\
  lua_newtable(L);\
  for(size_t i=0;i<len;i++)\
  {\
    lua_pushnumber(L,((type_t*)mat->data.ptr)[i]);\
    lua_rawseti(L,-2,i+1);\
  }\
  return 1;\
}

#define makeMatConvFunctionTo(name,type_t)\
static int luacv_TableTo##name(lua_State *L)\
{\
  const char f_msg[]="userdata TableTo"#name"("CVMAT_NAME" mat, num[] data, int row=0, int col=0)";\
  size_t len=0;\
  if (!(len=lua_objlen(L,2))) luaL_error(L,f_msg);\
  CvMat *mat=(CvMat*)checkldata(L,1);\
  size_t top=lua_gettop(L);\
  int row,col;\
  switch (top)\
  {\
    case 2:\
      mat->data.ptr=(uchar*)lua_newuserdata(L,CV_ELEM_SIZE(mat->type)*len);\
      for(size_t i=0;i<len;i++)\
      {\
        lua_rawgeti(L,2,i+1);\
        ((type_t*)mat->data.ptr)[i]=(type_t)checknumber(L,top+2);\
        lua_pop(L,1);\
      }\
      break;\
    case 4:\
          row=checkint(L,3);\
          col=checkint(L,4);\
          for(size_t i=0;i<len;i++)\
          {\
            lua_rawgeti(L,2,i+1);\
            ((type_t*)mat->data.ptr)[mat->cols*len*row+len*col+i]=(type_t)checknumber(L,top+1);\
            lua_pop(L,1);\
          }\
          break;\
    default:\
            luaL_error(L,f_msg);\
  }\
  return 0;\
}

//********************** iplimage conv func ****************//
makeImageConvFunctionFrom(IPL_DEPTH_1U,bool)
makeImageConvFunctionFrom(IPL_DEPTH_8U,unsigned char)
makeImageConvFunctionFrom(IPL_DEPTH_8S,signed char)
makeImageConvFunctionFrom(IPL_DEPTH_16U,unsigned short int)
makeImageConvFunctionFrom(IPL_DEPTH_16S,signed short int)
makeImageConvFunctionFrom(IPL_DEPTH_32S,int)
makeImageConvFunctionFrom(IPL_DEPTH_32F,float)
makeImageConvFunctionFrom(IPL_DEPTH_64F,double)

makeImageConvFunctionTo(IPL_DEPTH_1U,bool)
makeImageConvFunctionTo(IPL_DEPTH_8U,unsigned char)
makeImageConvFunctionTo(IPL_DEPTH_8S,signed char)
makeImageConvFunctionTo(IPL_DEPTH_16U,unsigned short int)
makeImageConvFunctionTo(IPL_DEPTH_16S,signed short int)
makeImageConvFunctionTo(IPL_DEPTH_32S,int)
makeImageConvFunctionTo(IPL_DEPTH_32F,float)
makeImageConvFunctionTo(IPL_DEPTH_64F,double)

//******************* matrix conv func *************************//
makeMatConvFunctionFrom(CV_8U,unsigned char)
makeMatConvFunctionFrom(CV_8S,signed char)
makeMatConvFunctionFrom(CV_16U,unsigned short int)
makeMatConvFunctionFrom(CV_16S,signed short int)
makeMatConvFunctionFrom(CV_32S,int)
makeMatConvFunctionFrom(CV_32F,float)
makeMatConvFunctionFrom(CV_64F,double)

makeMatConvFunctionTo(CV_8U,unsigned char)
makeMatConvFunctionTo(CV_8S,signed char)
makeMatConvFunctionTo(CV_16U,unsigned short int)
makeMatConvFunctionTo(CV_16S,signed short int)
makeMatConvFunctionTo(CV_32S,int)
makeMatConvFunctionTo(CV_32F,float)
makeMatConvFunctionTo(CV_64F,double)

static int luacv_cvMat(lua_State *L)
{
  const char f_msg[]=CVMAT_NAME" Mat(int rows, int cols, table type, num data[]=nil)";
  if (!lua_istable(L,3)) luaL_error(L,"Table is not valid conversion table.");
  uchar *data=NULL;
  int rows=checkint(L,1),cols=checkint(L,2),type;
  size_t top=lua_gettop(L);
  lua_pushstring(L,"value");
  lua_rawget(L,3);
  if (lua_isnil(L,top+1)) luaL_error(L,"Table is not valid conversion table.");
  type=checkint(L,top+1);
  lua_pop(L,1);
  CvMat mat=cvMat(rows,cols,type,data);
  switch (top)
  {
    case 3:
            break;
    case 4:
            if (!lua_istable(L,4)) luaL_error(L,f_msg);
            if ((size_t)CV_MAT_CN(CV_MAT_TYPE(CV_MAT_MAGIC_VAL|CV_MAT_CONT_FLAG|type))*rows*cols!=lua_objlen(L,4)) luaL_error(L,"Length of new matrix data don't match with matrix.");
            lua_pushstring(L,"to");
            lua_rawget(L,3);
            if (lua_isnil(L,5)) luaL_error(L,"Table is not valid conversion table.");
            lua_replace(L,2);
            pushUserData(L,&mat);
            lua_replace(L,3);
            lua_call(L,2,0);
            break;
    default:
            luaL_error(L,f_msg);
  }
  CvMat *copy=cvCloneMat(&mat);
  //luacv_pushObject<CvMat>(L,&mat,CVMAT_NAME,true);
  pushCvMat(L,copy);
  return 1;
}

static int luacv_cvmGet(lua_State *L)
{//zmena
  const char f_msg[]="num[] mGet("CVMAT_NAME" mat, table type, int row, int col)";  
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  CvMat *mat=checkCvMat(L,1);
  if (!mat->data.ptr) luaL_error(L,"You are trying acces matrix header with no data.");
  int row=checkint(L,3),col=checkint(L,4);
  if ((row>=mat->rows)||(col>=mat->cols)) luaL_error(L,"You are trying to acces wrong dimension.");
  lua_pushstring(L,"value");
  lua_rawget(L,2);
  if (CV_MAT_TYPE(mat->type)!=checkint(L,5)) luaL_error(L,"You are trying to acces mat data with wrong type.");
  lua_pop(L,1);
  lua_pushstring(L,"from");
  lua_rawget(L,2);
  pushUserData(L,mat);
  lua_call(L,1,1); 
  size_t len=CV_MAT_CN(mat->type);
  lua_newtable(L); 
  for(size_t i=1;i<=len;i++)
  {
    lua_rawgeti(L,5,len*mat->cols*row+col*len+i);
    lua_rawseti(L,-2,i);
  }
  return 1;
}

static int luacv_cvmSet(lua_State *L)
{//zmena
  const char f_msg[]="mSet("CVMAT_NAME" mat, table type, int row, int col, num[] value)";  
  if (lua_gettop(L)!=5) luaL_error(L,f_msg);
  CvMat *mat=checkCvMat(L,1);
  if (!mat->data.ptr) luaL_error(L,"You are trying acces matrix header with no data.");
  int row=checkint(L,3),col=checkint(L,4);
  if ((row>=mat->rows)||(col>=mat->cols)) luaL_error(L,"You are trying to acces wrong dimension.");
  lua_getfield(L,2,"value");
  if (CV_MAT_TYPE(mat->type)!=checkint(L,6)) luaL_error(L,"You are trying to acces mat data with wrong type.");
  lua_pop(L,1);
  lua_getfield(L,2,"to");
  lua_remove(L,2);
  lua_insert(L,1);
  lua_insert(L,3);
  lua_call(L,4,0);
  return 0;
}

static int luacv_cvIplDepth(lua_State *L)
{
  const char f_msg[]="int IplDepth(int type)";  
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushinteger(L,cvIplDepth(checkint(L,1)));
  return 1;
}


static int luacv_cvRect(lua_State *L)
{
  const char f_msg[]=CVRECT_NAME" Rect(int x, int y, int width, int height)";  
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  CvRect rect=cvRect(checkint(L,1),checkint(L,2),checkint(L,3),checkint(L,4));
  pushCvRect(L,&rect);
  return 1;
}

static int luacv_cvRectToROI(lua_State *L)
{
  const char f_msg[]=IPLROI_NAME" RectToROI("CVRECT_NAME" rect, int coi)";  
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  CvRect *rect=checkCvRect(L,1);
  IplROI roi=cvRectToROI(*rect,checkint(L,2));
  pushIplROI(L,&roi);
  return 1;
}

static int luacv_cvROIToRect(lua_State *L)
{
  const char f_msg[]=CVRECT_NAME" ROIToRect("IPLROI_NAME" roi)";  
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  IplROI *roi=checkIplROI(L,1);
  CvRect rect=cvROIToRect(*roi);
  pushCvRect(L,&rect);
  return 1;
}

//**************************** term criteria **************************//

static int luacv_cvTermCriteria(lua_State *L)
{
  const char f_msg[]=CVTERMCRITERIA_NAME" TermCriteria(int type, int max_iter, num epsilon)";  
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  CvTermCriteria crit=cvTermCriteria(checkint(L,1),checkint(L,2), checknumber(L,3));
  pushCvTermCriteria(L,&crit);
  return 1;
}

//****************************  CvPoint **************************//

static int luacv_cvPoint2D32f(lua_State *L)
{
  const char f_msg[]=CVPOINT2D32F_NAME" Point2D32f(num x, num y)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  CvPoint2D32f point=cvPoint2D32f(checknumber(L,1),checknumber(L,2));
  pushCvPoint2D32f(L,&point);
  return 1;
}

static int luacv_cvPoint(lua_State *L)
{
 const char f_msg[]=CVPOINT_NAME" Point(int x, int y)";
 if (lua_gettop(L)!=2) luaL_error(L,f_msg);
 CvPoint p=cvPoint(checkint(L,1),checkint(L,2));
 pushCvPoint(L,&p);
 return 1;
}

static int luacv_cvPointTo32f(lua_State *L)
{
  const char f_msg[]=CVPOINT2D32F_NAME" PointTo32f("CVPOINT_NAME" point)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  CvPoint *p1=checkCvPoint(L,1);
  CvPoint2D32f p2=cvPointTo32f(*p1);
  pushCvPoint2D32f(L,&p2);
  return 1;
}

static int luacv_cvPointFrom32f(lua_State *L)
{
  const char f_msg[]=CVPOINT_NAME" PointFrom32f("CVPOINT2D32F_NAME" point)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  CvPoint2D32f *p1=checkCvPoint2D32f(L,1);
  CvPoint p2=cvPointFrom32f(*p1);
  pushCvPoint(L,&p2);
  return 1;
}

static int luacv_cvPoint3D32f(lua_State *L)
{
  const char f_msg[]=CVPOINT3D32F_NAME" Point3D32f(num x, num y, num z)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  CvPoint3D32f point=cvPoint3D32f(checknumber(L,1),checknumber(L,2),checknumber(L,3));
  pushCvPoint3D32f(L,&point);
  return 1;
}

static int luacv_cvPoint2D64f(lua_State *L)
{
  const char f_msg[]=CVPOINT2D64F_NAME" Point2D64f(num x, num y)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  CvPoint2D64f point=cvPoint2D64f(checknumber(L,1),checknumber(L,2));
  pushCvPoint2D64f(L,&point);
  return 1;
}

static int luacv_cvPoint3D64f(lua_State *L)
{
  const char f_msg[]=CVPOINT3D64F_NAME" Point3D64f(num x, num y, num z)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  CvPoint3D64f point=cvPoint3D64f(checknumber(L,1),checknumber(L,2),checknumber(L,3));
  pushCvPoint3D64f(L,&point);
  return 1;
}

static int luacv_cvSize(lua_State *L)
{
  const char f_msg[]=CVSIZE_NAME" Size(int width, int height)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  CvSize size=cvSize(checkint(L,1),checkint(L,2));
  pushCvSize(L,&size);
  return 1;
}

static int luacv_cvSize2D32f(lua_State *L)
{
  const char f_msg[]=CVSIZE2D32F_NAME" Size2D32f(num width, num height)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  CvSize2D32f size=cvSize2D32f(checkint(L,1),checkint(L,2));
  pushCvSize2D32f(L,&size);
  return 1;
}

static int luacv_cvSlice(lua_State *L)
{
  const char f_msg[]=CVSLICE_NAME" Slice(int start, int end)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  CvSlice slice=cvSlice(checkint(L,1),checkint(L,2));
  pushCvSlice(L,&slice);
  return 1;
}


static int luacv_cvScalar(lua_State *L)
{
  const char f_msg[]=CVSCALAR_NAME" Scalar(num v1, num v2=0, num v3=0, num v4=0)";
  lua_Number v[4]={checknumber(L,1),0,0,0};
   switch (lua_gettop(L))
  {
   case 1:
          break;
   case 2:
          v[1]=checknumber(L,2);
          break;
   case 3:
          v[1]=checknumber(L,2);
          v[2]=checknumber(L,3);
          break;
   case 4:
          v[1]=checknumber(L,2);
          v[2]=checknumber(L,3);
          v[3]=checknumber(L,4);
          break;

   default: 
          luaL_error(L,f_msg);
  }
  CvScalar scalar=cvScalar(v[0],v[1],v[2],v[3]);
  pushCvScalar(L,&scalar);
  return 1;
}

static int luacv_cvScalarAll(lua_State *L)
{
  const char f_msg[]=CVSCALAR_NAME" ScalarAll(num val)"; 
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  CvScalar s=cvScalarAll(checknumber(L,1));
  pushCvScalar(L,&s); 
  return 1;
}

static int luacv_cvRealScalar(lua_State *L)
{
  const char f_msg[]=CVSCALAR_NAME" RealScalar(num val0)"; 
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);

  CvScalar s=cvRealScalar(checknumber(L,1));
  pushCvScalar(L,&s); 
  return 1;
}
//************* Array initialization and access to elements ***********//
static int luacv_cvCreateImageHeader(lua_State *L)
{
 const char f_msg[]=IPLIMAGE_NAME" CreateImageHeader("CVSIZE_NAME" size, table depth, int channels)";
 if (lua_gettop(L)!=3) luaL_error(L,f_msg);
 if (!lua_istable(L,2)) luaL_error(L,"Table is not correct conversion table");
 lua_getfield(L,2,"value");
 pushUserData(L,cvCreateImageHeader(*checkCvSize(L,1),checkint(L,4),checkint(L,3))); 
 luaL_getmetatable(L,IPLIMAGE_NAME);
 lua_setmetatable(L,-2);

 return 1;
}

static int luacv_cvInitImageHeader(lua_State *L)
{
 const char f_msg[]=IPLIMAGE_NAME" InitImageHeader("IPLIMAGE_NAME" image, "CVSIZE_NAME" size, table depth, int channels, int origin=0, int align=4)";
 int depth,channels=checkint(L,4),origin=0,align=4;
if (!lua_istable(L,3)) luaL_error(L,"Table is not correct conversion table");
 switch(lua_gettop(L))
 {
   case 4:
        break; 
   case 5:
        origin=checkint(L,5);
        break;
   case 6:
        origin=checkint(L,5);
        align=checkint(L,6);
        break;

   default:
          luaL_error(L,f_msg);
 } 
 lua_getfield(L,3,"value");
 depth=checkint(L,7);
 pushIplImage(L,cvInitImageHeader(checkIplImage(L,1),*checkCvSize(L,2),depth,channels,origin,align));
 return 1;
}

static int luacv_cvCreateImage(lua_State *L)
{
  const char f_msg[]=IPLIMAGE_NAME" CreateImage("CVSIZE_NAME" size, table depth, int channels)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  if (!lua_istable(L,2)) luaL_error(L,"Table is not correct conversion table.");
  lua_getfield(L,2,"value");
  pushIplImage(L,cvCreateImage(*checkCvSize(L,1),checkint(L,4),checkint(L,3)));
 return 1;
}

static int luacv_cvReleaseImageHeader(lua_State *L)
{
  const char f_msg[]="ReleaseImageHeader("IPLIMAGE_NAME" image)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkIplImage(L,1);
  luacv_releaseObject(L,1);
  return 0;
}
static int luacv_cvReleaseImage(lua_State *L)
{
  const char f_msg[]="ReleaseImage("IPLIMAGE_NAME" image)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkIplImage(L,1);
  luacv_releaseObject(L,1);

  return 0;
}

static int luacv_cvCloneImage(lua_State *L)
{
  const char f_msg[]=IPLIMAGE_NAME" CloneImage("IPLIMAGE_NAME" image)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  pushIplImage(L,cvCloneImage(checkIplImage(L,1)));
 return 1;
}
static int luacv_cvSetImageCOI(lua_State *L)
{
  const char f_msg[]="SetImageCOI("IPLIMAGE_NAME" image, int coi)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvSetImageCOI(checkIplImage(L,1),checkint(L,2));
 return 0;
}
static int luacv_cvGetImageCOI(lua_State *L)
{
  const char f_msg[]="int GetImageCOI("IPLIMAGE_NAME" image)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGetImageCOI(checkIplImage(L,1)));
 return 1;
}

static int luacv_cvSetImageROI(lua_State *L)
{
  const char f_msg[]="SetImageROI("IPLIMAGE_NAME" image, "CVRECT_NAME" rect)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  CvRect *rect=checkCvRect(L,2);
  cvSetImageROI(checkIplImage(L,1),*rect);
 return 0;
}
static int luacv_cvResetImageROI(lua_State *L)
{
  const char f_msg[]="ResetImageROI("IPLIMAGE_NAME" image)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  cvResetImageROI(checkIplImage(L,1));
 return 0;
}

static int luacv_cvGetImageROI(lua_State *L)
{
  const char f_msg[]=CVRECT_NAME" GetImageROI("IPLIMAGE_NAME" image)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  CvRect r=cvGetImageROI(checkIplImage(L,1));
  pushCvRect(L,&r);
 return 1;
}
static int luacv_cvCreateMatHeader(lua_State *L)
{
  const char f_msg[]=CVMAT_NAME" CreateMatHeader(int rows, int cols, table type)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  if (!lua_istable(L,3)) luaL_error(L,"Table is not valid conversion table."); 
  lua_getfield(L,3,"value");
  pushUserData(L,cvCreateMatHeader(checkint(L,1),checkint(L,2),checkint(L,4)));
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);

  return 1;
}
static int luacv_cvInitMatHeader(lua_State *L)
{
  const char f_msg[]=CVMAT_NAME" InitMatHeader("CVMAT_NAME" mat, int rows, int cols, int type, userdata data=nil, int step=CV_AUTOSTEP)";
  void *data=NULL;
  if (!lua_istable(L,4)) luaL_error(L,"Table is not valid conversion table."); 
  lua_getfield(L,3,"value");
  size_t top=lua_gettop(L);
  lua_getfield(L,3,"value");
  lua_Integer val[4]={checkint(L,2),checkint(L,3),checkint(L,top+1),CV_AUTOSTEP};
  lua_pop(L,1);
  switch (top)
  {
    case 4:
            break;
    case 5:
            data=checkldata(L,5);
            break;
    case 6:
            data=checkldata(L,5);
            val[3]=checkint(L,6);
            break;
    default:
            luaL_error(L,f_msg);
  }
  pushCvMat(L,cvInitMatHeader(checkCvMat(L,1),val[0],val[1],val[2],data,val[3]));
 return 1;
}

static int luacv_cvCreateMat(lua_State *L)
{
  const char f_msg[]=CVMAT_NAME" CreateMat(int rows, int cols, table type)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  if (!lua_istable(L,3)) luaL_error(L,"Table is not valid conversion table."); 
  lua_getfield(L,3,"value");
  pushCvMat(L,cvCreateMat(checkint(L,1),checkint(L,2),checkint(L,4)));
 return 1;
}

static int luacv_cvReleaseMat(lua_State *L)
{
  const char f_msg[]="ReleaseMat("CVMAT_NAME" mat)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvMat(L,1);
  luacv_releaseObject(L,1);
 return 0;
}



static int luacv_cvDecRefData(lua_State *L)
{//pozor
  const char f_msg[]="DecRefData("CVARR_NAME" arr)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  cvDecRefData(checkCvArr(L,1));
  return 0;
}

static int luacv_cvIncRefData(lua_State *L)
{
  const char f_msg[]="IncRefData("CVARR_NAME" arr)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushnumber(L,cvIncRefData(checkCvArr(L,1)));
  return 1;
}

static int luacv_cvCloneMat(lua_State *L)
{
  const char f_msg[]=CVMAT_NAME" CloneMat("CVMAT_NAME" mat)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  pushCvMat(L,cvCloneMat(checkCvMat(L,1)));
 return 1;
}
static int luacv_cvGetSubRect(lua_State *L)
{
  const char f_msg[]=CVMAT_NAME" GetSubRect("CVARR_NAME" arr, "CVMAT_NAME" submat, "CVRECT_NAME" rect)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  CvRect *rect=checkCvRect(L,3);
  pushUserData(L,cvGetSubRect(checkCvArr(L,1),checkCvMat(L,2),*rect));
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);


 return 1;
}
static int luacv_cvGetRows(lua_State *L)
{
  const char f_msg[]=CVMAT_NAME" GetRows("CVARR_NAME" arr, "CVMAT_NAME" submat, int start_row, int end_row, int delta_row=1)";
  lua_Integer var[3]={checkint(L,3),checkint(L,4),1};
  switch(lua_gettop(L))
  {
    case 4:
          break;
    case 5:
          var[2]=checkint(L,5);
          break;
    default:
          luaL_error(L,f_msg);
  }
  pushUserData(L,cvGetRows(checkCvArr(L,1),checkCvMat(L,2),var[0],var[1],var[2]));
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);

 return 1;
}

static int luacv_cvGetRow(lua_State *L)
{
  const char f_msg[]=CVMAT_NAME" GetRow("CVARR_NAME" arr, "CVMAT_NAME" submat, int row)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  pushUserData(L,cvGetRow(checkCvArr(L,1),checkCvMat(L,2),checkint(L,3)));
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);

 return 1;
}

static int luacv_cvGetCols(lua_State *L)
{
  const char f_msg[]=CVMAT_NAME" GetCols("CVARR_NAME" arr, "CVMAT_NAME" submat, int start_col, int end_col)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
   pushUserData(L,cvGetCols(checkCvArr(L,1),checkCvMat(L,2),checkint(L,3),checkint(L,4)));
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);

 return 1;
}

static int luacv_cvGetCol(lua_State *L)
{
  const char f_msg[]=CVMAT_NAME" GetCol("CVARR_NAME" arr, "CVMAT_NAME" submat, int col)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  pushUserData(L,cvGetCol(checkCvArr(L,1),checkCvMat(L,2),checkint(L,3)));
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
 return 1;
}

static int luacv_cvGetDiag(lua_State *L)
{
  const char f_msg[]=CVMAT_NAME" GetDiag("CVARR_NAME" arr, "CVARR_NAME" submat, int diag=0)";
  lua_Integer diag=0;
  switch(lua_gettop(L))
  {
    case 2:
          break;
    case 3:
          diag=checkint(L,3);
          break;
    default:
          luaL_error(L,f_msg);
  }
  pushUserData(L,cvGetDiag(checkCvArr(L,1),checkCvMat(L,2),diag));
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);

 return 1;
}
static int luacv_cvScalarToRawData(lua_State *L)
{
  luaL_error(L,"Unimplemented function %s. Use CvScalar.val index/newindex callback instead.",__FUNCTION__);
  return 1;
}

static int luacv_cvRawDataToScalar(lua_State *L)
{
  luaL_error(L,"Unimplemented function %s. Use CvScalar.val index/newindex callback instead.",__FUNCTION__);

 return 1;
}

static int luacv_cvCreateMatNDHeader(lua_State *L)
{
 const char f_msg[]=CVMATND_NAME" CreateMatNDHeader(int dims, int[] sizes, int type)";
  const int top=lua_gettop(L),len=lua_objlen(L,2);
  if ((top!=3)||(!lua_istable(L,2))||(!len)) 
    luaL_error(L,f_msg);
  int sizes[CV_MAX_DIM];
  for(int i=1;i<=len;i++)
  {
     lua_rawgeti(L,2,i);
     sizes[(i-1)]=checkint(L,top+1);
     lua_pop(L,1);
  }
  if (!lua_istable(L,3)) luaL_error(L,"Table is not valid conversion table."); 
  lua_getfield(L,3,"value");

  pushUserData(L,cvCreateMatNDHeader(checkint(L,1),sizes,checkint(L,4)));
  luaL_getmetatable(L,CVMATND_NAME);
  lua_setmetatable(L,-2);

 return 1;
}

static int luacv_cvCreateMatND(lua_State *L)
{
  const char f_msg[]=CVMATND_NAME" CreateMatND(int dims, int[] sizes, int type)";
  const int top=lua_gettop(L),len=lua_objlen(L,2);
  if ((top!=3)||(!lua_istable(L,2))||(!len)) 
    luaL_error(L,f_msg);
  int sizes[CV_MAX_DIM];
  for(int i=1;i<=len;i++)
  {
     lua_rawgeti(L,2,i);
     sizes[(i-1)]=checkint(L,top+1);
     lua_pop(L,1);
  }
  if (!lua_istable(L,3)) luaL_error(L,"Table is not valid conversion table."); 
  lua_getfield(L,3,"value");


  pushCvMatND(L,cvCreateMatND(checkint(L,1),sizes,checkint(L,4)));
 return 1;
}

static int luacv_cvInitMatNDHeader(lua_State *L)
{
 const char f_msg[]=CVMATND_NAME" InitMatNDHeader("CVMATND_NAME" mat, int dims, int[] sizes, int type, userdata data=nil)";
 void *data=NULL;
 const int top=lua_gettop(L),len=lua_objlen(L,3);
 if ((!lua_istable(L,3))||(!len)) luaL_error(L,f_msg);
 switch(top)
 {
   case 4:
        break; 
   case 5:
        data=checkldata(L,5);
        break;
   default:
        luaL_error(L,f_msg);
 } 
  int sizes[CV_MAX_DIM];
  for(int i=1;i<=len;i++)
  {
     lua_rawgeti(L,2,i);
     sizes[(i-1)]=checkint(L,top+1);
     lua_pop(L,1);
  }
 pushCvMatND(L,cvInitMatNDHeader(checkCvMatND(L,1),checkint(L,2),sizes,checkint(L,4),data));
 return 1;
}

static int luacv_cvReleaseMatND(lua_State *L)
{
  const char f_msg[]="ReleaseMatND("CVMATND_NAME" mat)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvMatND(L,1);
  luacv_releaseObject(L,1);
  return 0;
}

static int luacv_cvCloneMatND(lua_State *L)
{
  const char f_msg[]=CVMATND_NAME" CloneMatND("CVMATND_NAME" mat)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  pushCvMatND(L,cvCloneMatND(checkCvMatND(L,1)));
 return 1;
}

static int luacv_cvCreateSparseMat(lua_State *L)
{
  const char f_msg[]=CVSPARSEMAT_NAME" CreateSparseMat(int dims, int[] sizes, int type)";
  const int top=lua_gettop(L),len=lua_objlen(L,2);
  if ((top!=3)||(!lua_istable(L,2))||(!len)) 
    luaL_error(L,f_msg);
  int sizes[CV_MAX_DIM];
  for(int i=1;i<=len;i++)
  {
     lua_rawgeti(L,2,i);
     sizes[(i-1)]=checkint(L,top+1);
     lua_pop(L,1);
  }
  pushCvSparseMat(L,cvCreateSparseMat(checkint(L,1),sizes,checkint(L,3)));
 return 1;
}

static int luacv_cvReleaseSparseMat(lua_State *L)
{
  const char f_msg[]="ReleaseSparseMat("CVSPARSEMAT_NAME" mat)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvSparseMat(L,1);
  luacv_releaseObject(L,1);

  return 0;
}

static int luacv_cvCloneSparseMat(lua_State *L)
{
  const char f_msg[]=CVSPARSEMAT_NAME" CloneMatND("CVSPARSEMAT_NAME" mat)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  pushCvSparseMat(L,cvCloneSparseMat(checkCvSparseMat(L,1)));
 return 1;
}

static int luacv_cvInitSparseMatIterator(lua_State *L)
{
  const char f_msg[]=CVSPARSENODE_NAME"|nil InitSparseMatIterator("CVSPARSEMAT_NAME" mat,"CVSPARSEMATITERATOR_NAME" mat_iterator)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  CvSparseNode *node=cvInitSparseMatIterator(checkCvSparseMat(L,1),checkCvSparseMatIterator(L,2));
  if (!node) lua_pushnil(L);
  else
    pushCvSparseNode(L,node);
  return 1;
}

static int luacv_cvGetNextSparseNode(lua_State *L)
{
  const char f_msg[]=CVSPARSENODE_NAME"|nil GetNextSparseNode("CVSPARSEMATITERATOR_NAME" mat_iterator)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  CvSparseNode *node=cvGetNextSparseNode(checkCvSparseMatIterator(L,1));
  if (!node) lua_pushnil(L);
  else
    pushCvSparseNode(L,node);
  return 1;
}

//************* matrix iterator used for n-ary operations on dense arrays *******/
static int luacv_cvCreateNArrayIterator(lua_State *L)
{//nove
  const char f_msg[]=CVNARRAYITERATOR_NAME" CreateNArrayIterator(int count, "CVARR_NAME" arrs[], "CVARR_NAME" mask, "CVMATND_NAME" stubs, int flags=0)";
  lua_Integer flags=0;
  const int top=lua_gettop(L),len=lua_objlen(L,2);
  if ((!lua_istable(L,2))||(!len)) luaL_error(L,f_msg);
  switch(top)
  {
    case 4:
            break;
    case 5:
            flags=checkint(L,5);
            break;
    default:
            luaL_error(L,f_msg);
  }
  CvArr **arrs=(CvArr**)luacv_alloc(sizeof(CvArr*)*len);
  for(int i=1;i<=len;i++)
  {
     lua_rawgeti(L,2,i);
     arrs[(i-1)]=checkCvArr(L,top+1);
     lua_pop(L,1);
  }
  CvNArrayIterator iter; 
  cvInitNArrayIterator(checkint(L,1),arrs,checkCvArr(L,3),checkCvMatND(L,4),&iter,flags);
  pushCvNArrayIterator(L,&iter);
  luacv_free(&arrs);
  return 1;
}

static int luacv_cvInitNArrayIterator(lua_State *L)
{
  const char f_msg[]="int InitNArrayIterator(int count, "CVARR_NAME" arrs[], "CVARR_NAME" mask, "CVMATND_NAME" stubs, "CVNARRAYITERATOR_NAME" array_iterator, int flags=0)";
  lua_Integer flags=0;
  const int top=lua_gettop(L),len=lua_objlen(L,2);
  if ((!lua_istable(L,2))||(!len)) luaL_error(L,f_msg);
  switch(top)
  {
    case 5:
            break;
    case 6:
            flags=checkint(L,6);
            break;
    default:
            luaL_error(L,f_msg);
  }
  CvArr **arrs=(CvArr**)luacv_alloc(sizeof(CvArr*)*len);
  for(int i=1;i<=len;i++)
  {
     lua_rawgeti(L,2,i);
     arrs[(i-1)]=checkCvArr(L,top+1);
     lua_pop(L,1);
  }

  lua_pushnumber(L,cvInitNArrayIterator(checkint(L,1),arrs,checkCvArr(L,3),checkCvMatND(L,4),checkCvNArrayIterator(L,5),flags));
  luacv_free(&arrs);
  return 1;
}

static int luacv_cvNextNArraySlice(lua_State *L)
{
  const char f_msg[]="int NextNArraySlice("CVNARRAYITERATOR_NAME" array_iterator)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushnumber(L,cvNextNArraySlice(checkCvNArrayIterator(L,1)));
 return 1;
}
static int luacv_cvGetElemType(lua_State *L)
{
  const char f_msg[]="int GetElemType("CVARR_NAME" arr)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGetElemType(checkCvArr(L,1)));
 return 1;
}
static int luacv_cvGetDims(lua_State *L)
{
  const char f_msg[]="int GetDims("CVARR_NAME" arr, int[] sizes=nil)";
  int *sizes=NULL,t_sizes[CV_MAX_DIM];
  const int top=lua_gettop(L);
  switch(top)
  {
    case 1:
          break;
    case 2:
          sizes=t_sizes;
          break;
    default:
      luaL_error(L,f_msg);
  }
  int dims=cvGetDims(checkCvArr(L,1),sizes);
  if (sizes!=NULL)
    for(int i=1;i<=dims;i++)
    {
      lua_pushnumber(L,sizes[i-1]);
      lua_rawseti(L,2,i); 
    }
  lua_pushnumber(L,dims);
 return 1;
}

static int luacv_cvGetDimSize(lua_State *L)
{
  const char f_msg[]="int GetDimsSize("CVARR_NAME" arr,int index)";
  if(lua_gettop(L)!=2) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGetDimSize(checkCvArr(L,1),checkint(L,2)));
 return 1;
}
static int luacv_cvPtr1D(lua_State *L)
{//zmena
  const char f_msg[]="userdata, int Ptr1D("CVARR_NAME" arr,int idx0, int gettype=0)";
  int t_type,gettype=0,*type=NULL;
  switch(lua_gettop(L))
  {
    case 2:
          break;
    case 3:
          gettype=checkint(L,3);
          break;
    default:
      luaL_error(L,f_msg);
  }
  if (gettype) type=&t_type;
  pushUserData(L,cvPtr1D(checkCvArr(L,1),checkint(L,2),type));
  if (!gettype) return 1;
  lua_pushnumber(L,*type);
  return 2;
}

static int luacv_cvPtr2D(lua_State *L)
{//zmena
  const char f_msg[]="userdata, int Ptr2D("CVARR_NAME" arr,int idx0, int idx1, int gettype=0)";
  int t_type,gettype=0,*type=NULL;

  switch(lua_gettop(L))
  {
    case 3:
          break;
    case 4:
          gettype=checkint(L,4);
          break;
    default:
      luaL_error(L,f_msg);
  }
  if (gettype) type=&t_type;
  pushUserData(L,cvPtr2D(checkCvArr(L,1),checkint(L,2),checkint(L,3),type));
  if (!gettype) return 1;
  lua_pushnumber(L,*type);
 return 2;
}
static int luacv_cvPtr3D(lua_State *L)
{//zmena
  const char f_msg[]="userdata, int Ptr3D("CVARR_NAME" arr,int idx0, int idx1, int idx2, int gettype=0)";
  int t_type,gettype=0,*type=NULL;
  switch(lua_gettop(L))
  {
    case 4:
          break;
    case 5:
          gettype=checkint(L,5);
          break;
    default:
      luaL_error(L,f_msg);
  }
  if (gettype) type=&t_type;
  pushUserData(L,cvPtr3D(checkCvArr(L,1),checkint(L,2),checkint(L,3),checkint(L,4),type));
  if (!gettype) return 1;
  lua_pushnumber(L,*type);
  return 2;
}
static int luacv_cvPtrND(lua_State *L)
{//zmena
  const char f_msg[]="userdata, int, int PtrND("CVARR_NAME" arr,int[] idx, int gettype=0, int create_node=1, int gethashval=0)";
  int *type=NULL,t_type,gettype=0,gethashval=0,create_node=1,idx[CV_MAX_DIM];
  unsigned int *hashval=NULL,t_hash;
  const int len=lua_objlen(L,2),top=lua_gettop(L);
  if ((!lua_istable(L,2))||(!len)) luaL_error(L,f_msg);
  switch(top)
  {
    case 3:
          break;
    case 4:
          gettype=checkint(L,4);
          break;
    case 5:
          gettype=checkint(L,4);
          create_node=checkint(L,5);
          break;
    case 6:
          gettype=checkint(L,4);
          create_node=checkint(L,5);
          gethashval=checkint(L,6);
          break;

    default:
      luaL_error(L,f_msg);
  }
  for(int i=1;i<=len;i++)
  {
      lua_rawgeti(L,2,i);
      idx[(i-1)]=checkint(L,top+1);
      lua_pop(L,1);
  }
  if (gettype) type=&t_type;
  if (gethashval) hashval=&t_hash;
  pushUserData(L,cvPtrND(checkCvArr(L,1),idx,type,create_node,hashval));
  if ((!gettype)&&(!gethashval)) return 1;
  if (gettype)
  {
    lua_pushnumber(L,*type);
    if(!gethashval) return 2;
  }
  lua_pushnumber(L,*hashval);
 return 3;
}

static int luacv_cvGet1D(lua_State *L)
{
  const char f_msg[]=CVSCALAR_NAME" Get1D("CVARR_NAME" arr, int idx0)";
  if(lua_gettop(L)!=2) luaL_error(L,f_msg);
  CvScalar s=cvGet1D(checkCvArr(L,1),checkint(L,2));
  pushCvScalar(L,&s);
 return 1;
}

static int luacv_cvGet2D(lua_State *L)
{
  const char f_msg[]=CVSCALAR_NAME" Get2D("CVARR_NAME" arr, int idx0, int idx1)";
  if(lua_gettop(L)!=3) luaL_error(L,f_msg);
  CvScalar s=cvGet2D(checkCvArr(L,1),checkint(L,2),checkint(L,3));
  pushCvScalar(L,&s);
 return 1;
}

static int luacv_cvGet3D(lua_State *L)
{
  const char f_msg[]=CVSCALAR_NAME" Get3D("CVARR_NAME" arr, int idx0, int idx1, int idx2)";
  if(lua_gettop(L)!=4) luaL_error(L,f_msg);
  CvScalar s=cvGet3D(checkCvArr(L,1),checkint(L,2),checkint(L,3),checkint(L,4));
  pushCvScalar(L,&s);
 return 1;
}

static int luacv_cvGetND(lua_State *L)
{
  const char f_msg[]=CVSCALAR_NAME" GetND("CVARR_NAME" arr, int[] idx)";
  const int len=lua_objlen(L,2),top=lua_gettop(L);
  if((top!=2)||(!lua_istable(L,2))||(!len)) luaL_error(L,f_msg);
  int idx[CV_MAX_DIM];
  for(int i=1;i<=len;i++)
  {
        lua_rawgeti(L,2,i);
        idx[(i-1)]=checkint(L,top+1);
        lua_pop(L,1);
  }
  CvScalar s=cvGetND(checkCvArr(L,1),idx);
  pushCvScalar(L,&s);
 return 1;
}

static int luacv_cvGetReal1D(lua_State *L)
{
  const char f_msg[]="num GetReal1D("CVARR_NAME" arr, int idx0)";
  if(lua_gettop(L)!=2) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGetReal1D(checkCvArr(L,1),checkint(L,2)));
  return 1;
}
static int luacv_cvGetReal2D(lua_State *L)
{
  const char f_msg[]="num GetReal2D("CVARR_NAME" arr, int idx0, int idx1)";
  if(lua_gettop(L)!=3) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGetReal2D(checkCvArr(L,1),checkint(L,2),checkint(L,3)));
 return 1;
}

static int luacv_cvGetReal3D(lua_State *L)
{
  const char f_msg[]="num GetReal3D("CVARR_NAME" arr, int idx0, int idx1, int idx2)";
  if(lua_gettop(L)!=4) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGetReal3D(checkCvArr(L,1),checkint(L,2),checkint(L,3),checkint(L,4)));

 return 1;
}

static int luacv_cvGetRealND(lua_State *L)
{
  const char f_msg[]="num GetRealND("CVARR_NAME" arr, int[] idx)";
  const int top=lua_gettop(L),len=lua_objlen(L,2); 
  if((top!=2)||(!lua_istable(L,2))||(!len)) luaL_error(L,f_msg);
  int idx[CV_MAX_DIM];
  for(int i=1;i<=len;i++)
  {
        lua_rawgeti(L,2,i);
        idx[(i-1)]=checkint(L,top+1);
        lua_pop(L,1);
  }
  lua_pushnumber(L,cvGetRealND(checkCvArr(L,1),idx));
 return 1;
}

static int luacv_cvSet1D(lua_State *L)
{
  const char f_msg[]="Set1D("CVARR_NAME" arr, int idx0, "CVSCALAR_NAME" value)";
  if(lua_gettop(L)!=3) luaL_error(L,f_msg);
  CvScalar *s=checkCvScalar(L,3);
  cvSet1D(checkCvArr(L,1),checkint(L,2),*s);
 return 0;
}

static int luacv_cvSet2D(lua_State *L)
{
  const char f_msg[]="Set2D("CVARR_NAME" arr,int idx0, int idx1, "CVSCALAR_NAME" value)";
  if(lua_gettop(L)!=4) luaL_error(L,f_msg);
  CvScalar *s=checkCvScalar(L,4);
  cvSet2D(checkCvArr(L,1),checkint(L,2),checkint(L,3),*s);
 return 0;
}

static int luacv_cvSet3D(lua_State *L)
{
  const char f_msg[]="Set3D("CVARR_NAME" arr,int idx0, int idx1, int idx2, "CVSCALAR_NAME" value)";
  if(lua_gettop(L)!=5) luaL_error(L,f_msg);
  CvScalar *s=checkCvScalar(L,5);
  cvSet3D(checkCvArr(L,1),checkint(L,2),checkint(L,3),checkint(L,4),*s);
 return 0;
}

static int luacv_cvSetND(lua_State *L)
{
  const char f_msg[]="SetND("CVARR_NAME" arr, int[] idx, "CVSCALAR_NAME" value)";
  const int top=lua_gettop(L),len=lua_objlen(L,2);
  if((top!=3)||(!lua_istable(L,2))||(!len)) luaL_error(L,f_msg);
  int idx[CV_MAX_DIM];
  for(int i=1;i<=len;i++)
  {
        lua_rawgeti(L,2,i);
        idx[(i-1)]=checkint(L,top+1);
        lua_pop(L,1);
  }
  CvScalar *s=checkCvScalar(L,3);
  cvSetND(checkCvArr(L,1),idx,*s);
 return 0;
}

static int luacv_cvSetReal1D(lua_State *L)
{
  const char f_msg[]="SetReal1D("CVARR_NAME" arr, int idx0, num value)";
  if(lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvSetReal1D(checkCvArr(L,1),checkint(L,2),checknumber(L,3));
 return 0;
}

static int luacv_cvSetReal2D(lua_State *L)
{
  const char f_msg[]="SetReal2D("CVARR_NAME" arr, int idx0, int idx1, num value)";
  if(lua_gettop(L)!=4) luaL_error(L,f_msg);
  cvSetReal2D(checkCvArr(L,1),checkint(L,2),checkint(L,3),checknumber(L,4));
 return 0;
}

static int luacv_cvSetReal3D(lua_State *L)
{
  const char f_msg[]="SetReal3D("CVARR_NAME" arr, int idx0, int idx1, int idx2, num value)";
  if(lua_gettop(L)!=5) luaL_error(L,f_msg);
  cvSetReal3D(checkCvArr(L,1),checkint(L,2),checkint(L,3),checkint(L,4),checknumber(L,5));
 return 0;
}

static int luacv_cvSetRealND(lua_State *L)
{
  const char f_msg[]="SetRealND("CVARR_NAME" arr, int[] idx, num value)";
  const int top=lua_gettop(L),len=lua_objlen(L,2);
  if((top!=3)||(!lua_istable(L,2))||(!len)) luaL_error(L,f_msg);
  int idx[CV_MAX_DIM];
  for(int i=1;i<=len;i++)
  {
        lua_rawgeti(L,2,i);
        idx[(i-1)]=checkint(L,top+1);
        lua_pop(L,1);
  }
  cvSetRealND(checkCvArr(L,1),idx,checknumber(L,3));
 return 0;
}

static int luacv_cvClearND(lua_State *L)
{
  const char f_msg[]="ClearND("CVARR_NAME" arr, int[] idx)";
  const int top=lua_gettop(L),len=lua_objlen(L,2);
  if((top!=2)||(!lua_istable(L,2))||(!len)) luaL_error(L,f_msg);
  int idx[CV_MAX_DIM];
  for(int i=1;i<=len;i++)
  {
        lua_rawgeti(L,2,i);
        idx[(i-1)]=checkint(L,top+1);
        lua_pop(L,1);
  }
  cvClearND(checkCvArr(L,1),idx);
 return 0;
}

static int luacv_cvGetMat(lua_State *L)
{//zmena
  const char f_msg[]=CVMAT_NAME",int GetMat("CVARR_NAME" arr, "CVMAT_NAME" header, int getcoi=0, int allowND=0)";
  int getcoi=0,allownd=0,*coi=NULL,t_coi;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            getcoi=checkint(L,3);
    case 4:
            getcoi=checkint(L,3);
            allownd=checkint(L,4);
    default:
            luaL_error(L,f_msg);
  }
  if (getcoi) coi=&t_coi;
  pushCvMat(L,cvGetMat(checkCvArr(L,1),checkCvMat(L,2),coi,allownd));
  if (!getcoi) return 1;
  lua_pushnumber(L,*coi);
  return 2;
}

static int luacv_cvGetImage(lua_State *L)
{
  const char f_msg[]=IPLIMAGE_NAME" GetImage("CVARR_NAME" arr, "IPLIMAGE_NAME" image_header)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  pushUserData(L,cvGetImage(checkCvArr(L,1),checkIplImage(L,2)));
  luaL_getmetatable(L,IPLIMAGE_NAME);
  lua_setmetatable(L,-2);

  return 1;
}

static int luacv_cvReshapeMatND(lua_State *L)
{
  const char f_msg[]=CVARR_NAME" ReshapeMatND("CVARR_NAME" arr, int sizeof_header, "CVARR_NAME" header, int new_cn, int new_dims, int[] new_sizes )";
  const int top=lua_gettop(L),len=lua_objlen(L,6);
  if ((top!=6)||(!lua_istable(L,6))||(!len)) luaL_error(L,f_msg);
  int new_sizes[CV_MAX_DIM];
  for(int i=1;i<=len;i++)
  {
     lua_rawgeti(L,6,i);
     new_sizes[(i-1)]=checkint(L,top+1);
     lua_pop(L,1);
  }
  pushUserData(L,cvReshapeMatND(checkCvArr(L,1),checkint(L,2),checkCvArr(L,3),checkint(L,4),checkint(L,5),new_sizes)); 
  luaL_getmetatable(L,CVMATND_NAME);
  lua_setmetatable(L,-2);

  return 1;
}

static int luacv_cvReshapeND(lua_State *L)
{
  UNIMPL(L);
  return 1;
}

static int luacv_cvReshape(lua_State *L)
{
  const char f_msg[]=CVMAT_NAME" Reshape("CVARR_NAME" arr, "CVMAT_NAME" header, int new_cn, int new_rows=0)";
  int new_rows=0;
  switch(lua_gettop(L)) 
  {
    case 3:
            break;
    case 4:
            new_rows=checkint(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  pushCvMat(L,cvReshape(checkCvArr(L,1),checkCvMat(L,2),checkint(L,3),new_rows));
  return 1;
}

static int luacv_cvRepeat(lua_State *L)
{
  const char f_msg[]="Repeat("CVARR_NAME" src, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvRepeat(checkCvArr(L,1),checkCvArr(L,2));
 return 0;
}
static int luacv_cvCreateData(lua_State *L)
{
  const char f_msg[]="CreateData("CVARR_NAME" arr)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  cvCreateData(checkCvArr(L,1));
 return 0;
}

static int luacv_cvReleaseData(lua_State *L)
{
  UNIMPL(L);
   return 1;
}



static int luacv_cvSetData(lua_State *L)
{//zmena
  const char f_msg[]="SetData("CVARR_NAME" arr, num[] data, int step, table type)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  if (!lua_istable(L,4)) luaL_error(L,"Table is not valid conversion table.");
  int step=checkint(L,3);
  CvArr *arr=checkCvArr(L,1);
  lua_getfield(L,4,"to");
  lua_insert(L,1);
  lua_pop(L,2);
  lua_call(L,2,1);
  cvSetData(arr,checkldata(L,lua_gettop(L)),step);
 return 0;
}

static int luacv_cvGetRawData(lua_State *L)
{//zmena
//const char f_msg[]="userdata,int getrawdata("cvarr_name" arr, table type, "cvsize_name" roi_size=nil)";
  const char f_msg[]="userdata,int getrawdata("CVARR_NAME" arr, table type)";
  if (!lua_istable(L,2)) luaL_error(L,"Table is not valid conversion table.");
  //CvSize *size=NULL;
  uchar *data=NULL;
  int step=0;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
   //         size=checkCvSize(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvGetRawData(checkCvArr(L,1),&data,&step);
  lua_getfield(L,2,"from");
  if (lua_isnil(L,4)) luaL_error(L,"Table is not valid conversion table.");
    lua_insert(L,1);
  pushUserData(L,data);
  lua_insert(L,3);
  lua_remove(L,4);
  lua_call(L,2,1);
  lua_pushnumber(L,step);
 return 2;
}
static int luacv_cvGetSize(lua_State *L)
{
  const char f_msg[]=CVSIZE_NAME" GetSize("CVARR_NAME" arr)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  CvSize s=cvGetSize(checkCvArr(L,1));
  pushCvSize(L,&s);
 return 1;
}

static int luacv_cvCopy(lua_State *L)
{
  const char f_msg[]="Copy("CVARR_NAME" src, "CVARR_NAME" dst, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            mask=checkCvArr(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvCopy(checkCvArr(L,1),checkCvArr(L,2),mask);
  return 0;
}

static int luacv_cvSet(lua_State *L)
{
  const char f_msg[]="Set("CVARR_NAME" src, "CVSCALAR_NAME" value, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            mask=checkCvArr(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
  CvScalar *val=checkCvScalar(L,2);
  cvSet(checkCvArr(L,1),*val,mask);
  return 0;
}

static int luacv_cvSetZero(lua_State *L)
{
  const char f_msg[]="SetZero(CvArr arr)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  cvSetZero(checkCvArr(L,1));
 return 0;
}

static int luacv_cvSplit(lua_State *L)
{
  const char f_msg[]="Split("CVARR_NAME" src, "CVARR_NAME" dst0, "CVARR_NAME" dst1, "CVARR_NAME" dst2, "CVARR_NAME" dst3)";
  if (lua_gettop(L)!=5) luaL_error(L,f_msg);
  cvSplit(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checkCvArr(L,4),checkCvArr(L,5));
  return 0;
}

static int luacv_cvMerge(lua_State *L)
{
  const char f_msg[]="Merge("CVARR_NAME" src0, "CVARR_NAME" src1=nil, "CVARR_NAME" src2=nil, "CVARR_NAME" src3=nil, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=5) luaL_error(L,f_msg); 
  cvMerge(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checkCvArr(L,4),checkCvArr(L,5));
  return 0;
}

static int luacv_cvMixChannels(lua_State *L)
{
  const char f_msg[]="MixChannels("CVARR_NAME" src, int src_count, "CVARR_NAME" dst, int dst_count, int[] from_to, int pair_count)";
  const int top=lua_gettop(L),len=lua_objlen(L,5);
  if ((top!=6)||(!lua_istable(L,5))||(!len)) luaL_error(L,f_msg);
  CvArr *src=checkldata(L,1),*dst=checkldata(L,3);
  int from_to[CV_MAX_DIM];
  for(int i=1;i<=len;i++)
  {
     lua_rawgeti(L,2,i);
     from_to[(i-1)]=checkint(L,top+1);
     lua_pop(L,1);
  }
  cvMixChannels((const CvArr**)&src,checkint(L,2),&dst,checkint(L,4),from_to,checkint(L,6));
  return 0;
}

static int luacv_cvConvertScale(lua_State *L)
{
  const char f_msg[]="ConvertScale("CVARR_NAME" src, "CVARR_NAME" dst, num scale=1, num shift=0)";
  double scale=1,shift=0;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            scale=checknumber(L,3);
            break;
    case 4:
            scale=checknumber(L,3);
            shift=checknumber(L,4);
            break;

    default:
            luaL_error(L,f_msg);
  }
  cvConvertScale(checkCvArr(L,1),checkCvArr(L,2),scale,shift);
  return 0;
}

static int luacv_cvConvert(lua_State *L)
{
  const char f_msg[]="Convert("CVARR_NAME" src, "CVARR_NAME" dst)";
  if(lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvConvert(checkCvArr(L,1),checkCvArr(L,2));
  return 0;
}

static int luacv_cvConvertScaleAbs(lua_State *L)
{
  const char f_msg[]="ConvertScaleAbs("CVARR_NAME" src, "CVARR_NAME" dst, num scale=1, num shift=0)";
  double scale=1,shift=0;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            scale=checknumber(L,3);
            break;
    case 4:
            scale=checknumber(L,3);
            shift=checknumber(L,3);
            break;

    default:
            luaL_error(L,f_msg);
  }
  cvConvertScaleAbs(checkCvArr(L,1),checkCvArr(L,2),scale,shift);
  return 0;
}

static int luacv_cvCheckTermCriteria(lua_State *L)
{
  const char f_msg[]=CVTERMCRITERIA_NAME" CheckTermCriteria("CVTERMCRITERIA_NAME" citeria, num default_eps, int default_max_iters)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  CvTermCriteria *t=checkCvTermCriteria(L,1);
  CvTermCriteria term=cvCheckTermCriteria(*t,checknumber(L,2),checkint(L,3));
  pushCvTermCriteria(L,&term);
  return 1;
}

//***************** Arithmetic, logic and comparision operations ***********//

static int luacv_cvAdd(lua_State *L)
{
  const char f_msg[]="Add("CVARR_NAME" src1, "CVARR_NAME" src2, "CVARR_NAME" dst, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            mask=checkCvArr(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvAdd(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),mask);
  return 0;
}

static int luacv_cvAddS(lua_State *L)
{
  const char f_msg[]="AddS("CVARR_NAME" src1, "CVSCALAR_NAME" value, "CVARR_NAME" dst, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            mask=checkCvArr(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  CvScalar *s=checkCvScalar(L,2);
  cvAddS(checkCvArr(L,1),*s,checkCvArr(L,3),mask);
  return 0;
}

static int luacv_cvSub(lua_State *L)
{
  const char f_msg[]="Sub("CVARR_NAME" src1, "CVARR_NAME" src2, "CVARR_NAME" dst, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            mask=checkCvArr(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvSub(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),mask);
  return 0;
}

static int luacv_cvSubS(lua_State *L)
{
  const char f_msg[]="SubS("CVARR_NAME" src1, "CVSCALAR_NAME" value, "CVARR_NAME" dst, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            mask=checkCvArr(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  CvScalar *s=checkCvScalar(L,2);
  cvSubS(checkCvArr(L,1),*s,checkCvArr(L,3),mask);
  return 0;
}

static int luacv_cvSubRS(lua_State *L)
{
  const char f_msg[]="SubRS("CVARR_NAME" src1, "CVSCALAR_NAME" value, "CVARR_NAME" dst, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            mask=checkCvArr(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  CvScalar *s=checkCvScalar(L,2);
  cvSubRS(checkCvArr(L,1),*s,checkCvArr(L,3),mask);
  return 0;
}

static int luacv_cvMul(lua_State *L)
{
  const char f_msg[]="Mul("CVARR_NAME" src1, "CVARR_NAME" src2, "CVARR_NAME" dst, num scale=1)";
  lua_Number scale=1;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            scale=checknumber(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvMul(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),scale);
  return 0;
}

static int luacv_cvDiv(lua_State *L)
{
  const char f_msg[]="Div("CVARR_NAME" src1, "CVARR_NAME" src2, "CVARR_NAME" dst, num scale=1)";
  lua_Number scale=1;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            scale=checknumber(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvDiv(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),scale);
  return 0;
}

static int luacv_cvScaleAdd(lua_State *L)
{
  const char f_msg[]="ScaleAdd("CVARR_NAME" src1, "CVSCALAR_NAME" scale, "CVARR_NAME" src2, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  CvScalar *s=checkCvScalar(L,2);
  cvScaleAdd(checkCvArr(L,1),*s,checkCvArr(L,3),checkCvArr(L,4));
  return 0;
}

static int luacv_cvAXPY(lua_State *L)
{
  const char f_msg[]="AXPY("CVARR_NAME" src1, num real_scalar, "CVARR_NAME" src2, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  cvAXPY(checkCvArr(L,1),checknumber(L,2),checkCvArr(L,3),checkCvArr(L,4));
  return 0;
}

static int luacv_cvAddWeighted(lua_State *L)
{
  const char f_msg[]="AddWeighted("CVARR_NAME" src1, num alpha, "CVARR_NAME" src2, num beta, num gamma, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=6) luaL_error(L,f_msg);
  cvAddWeighted(checkCvArr(L,1),checknumber(L,2),checkCvArr(L,3),checknumber(L,4),checknumber(L,5),checkCvArr(L,6));
  return 0;
}

static int luacv_cvDotProduct(lua_State *L)
{
  const char f_msg[]="num DotProduct("CVARR_NAME" src1, "CVARR_NAME" src2)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  lua_pushnumber(L,cvDotProduct(checkCvArr(L,1),checkCvArr(L,2)));
 return 1;
}

static int luacv_cvAnd(lua_State *L)
{
  const char f_msg[]="And("CVARR_NAME" src1, "CVARR_NAME" src2, "CVARR_NAME" dst, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            mask=checkCvArr(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvAnd(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),mask);
  return 0;
}

static int luacv_cvAndS(lua_State *L)
{
  const char f_msg[]="AndS("CVARR_NAME" src1, "CVSCALAR_NAME" value, "CVARR_NAME" dst, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            mask=checkCvArr(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  CvScalar *s=checkCvScalar(L,2);
  cvAndS(checkCvArr(L,1),*s,checkCvArr(L,3),mask);
  return 0;
}

static int luacv_cvOr(lua_State *L)
{
  const char f_msg[]="Or("CVARR_NAME" src1, "CVARR_NAME" src2, "CVARR_NAME" dst, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            mask=checkCvArr(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvOr(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),mask);
  return 0;
}

static int luacv_cvOrS(lua_State *L)
{
  const char f_msg[]="OrS("CVARR_NAME" src1, "CVSCALAR_NAME" value, "CVARR_NAME" dst, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            mask=checkCvArr(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  CvScalar *s=checkCvScalar(L,2);
  cvOrS(checkCvArr(L,1),*s,checkCvArr(L,3),mask);
  return 0;
}

static int luacv_cvXor(lua_State *L)
{
  const char f_msg[]="Xor("CVARR_NAME" src1, "CVARR_NAME" src2, "CVARR_NAME" dst, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            mask=checkCvArr(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvXor(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),mask);
  return 0;
}

static int luacv_cvXorS(lua_State *L)
{
  const char f_msg[]="XorS("CVARR_NAME" src1, "CVSCALAR_NAME" value, "CVARR_NAME" dst, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            mask=checkCvArr(L,4);
            break;

    default:
            luaL_error(L,f_msg);
  }
  CvScalar *s=checkCvScalar(L,2);
  cvXorS(checkCvArr(L,1),*s,checkCvArr(L,3),mask);
  return 0;
}

static int luacv_cvNot(lua_State *L)
{
  const char f_msg[]="Not("CVARR_NAME" src1, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvNot(checkCvArr(L,1),checkCvArr(L,2));
 return 0;
}

static int luacv_cvInRange(lua_State *L)
{
  const char f_msg[]="InRange("CVARR_NAME" src1, "CVARR_NAME" lower, "CVARR_NAME" upper, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  cvInRange(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checkCvArr(L,4));
 return 0;
}

static int luacv_cvInRangeS(lua_State *L)
{
  const char f_msg[]="InRangeS("CVARR_NAME" src1, "CVSCALAR_NAME" lower, "CVSCALAR_NAME" upper, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  CvScalar *lower=checkCvScalar(L,2),*upper=checkCvScalar(L,3);
  cvInRangeS(checkCvArr(L,1),*lower,*upper,checkCvArr(L,4));
 return 0;
}

static int luacv_cvCmp(lua_State *L)
{
  const char f_msg[]="Cmp("CVARR_NAME" src1, "CVARR_NAME" src2, "CVARR_NAME" dst, int cmp_op)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  cvCmp(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checkint(L,4));
 return 0;
}

static int luacv_cvCmpS(lua_State *L)
{
  const char f_msg[]="CmpS("CVARR_NAME" src1, num value, "CVARR_NAME" dst, int cmp_op)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  cvCmpS(checkCvArr(L,1),checknumber(L,2),checkCvArr(L,3),checkint(L,4));
 return 0;
}

static int luacv_cvMin(lua_State *L)
{
  const char f_msg[]="Min("CVARR_NAME" src1, "CVARR_NAME" src2, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvMin(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3));
 return 0;
}

static int luacv_cvMax(lua_State *L)
{
  const char f_msg[]="Max("CVARR_NAME" src1, "CVARR_NAME" src2, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvMax(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3));
 return 0;
}

static int luacv_cvMinS(lua_State *L)
{
  const char f_msg[]="MinS("CVARR_NAME" src1, num value, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvMinS(checkCvArr(L,1),checknumber(L,2),checkCvArr(L,3));
 return 0;
}

static int luacv_cvMaxS(lua_State *L)
{
  const char f_msg[]="MaxS("CVARR_NAME" src1, num value, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvMaxS(checkCvArr(L,1),checknumber(L,2),checkCvArr(L,3));
 return 0;
}

static int luacv_cvAbsDiff(lua_State *L)
{
  const char f_msg[]="AbsDiff("CVARR_NAME" src1, "CVARR_NAME" src2, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvAbsDiff(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3));
 return 0;
}

static int luacv_cvAbsDiffS(lua_State *L)
{
  const char f_msg[]="AbsDiffS("CVARR_NAME" src1, "CVARR_NAME" dst, "CVSCALAR_NAME" value)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  CvScalar *s=checkCvScalar(L,3);
  cvAbsDiffS(checkCvArr(L,1),checkCvArr(L,2),*s);
 return 0;
}

static int luacv_cvAbs(lua_State *L)
{
  const char f_msg[]="Abs("CVARR_NAME" src1, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvAbs(checkCvArr(L,1),checkCvArr(L,2));
 return 0;
}

//******************************** math operations *******************************//

static int luacv_cvCartToPolar(lua_State *L)
{
  const char f_msg[]="CartToPolar("CVARR_NAME" x, "CVARR_NAME" y, "CVARR_NAME" magnitude, "CVARR_NAME" angle=nil, int angle_in_degrees=0)";
  CvArr *angle=NULL;
  lua_Integer angle_in_degrees=0;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            angle=checkCvArr(L,4);
            break;
    case 5:
            angle=checkCvArr(L,4);
            angle_in_degrees=checkint(L,5);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvCartToPolar(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),angle,angle_in_degrees);
  return 0;
}

static int luacv_cvPolarToCart(lua_State *L)
{
  const char f_msg[]="PolarToCart("CVARR_NAME" magnitude, "CVARR_NAME" angle, "CVARR_NAME" x, "CVARR_NAME" y, int angle_in_degrees=0)";
  lua_Integer angle_in_degrees=0;
  switch(lua_gettop(L))
  {
    case 4:
            break;
    case 5:
            angle_in_degrees=checkint(L,5);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvPolarToCart(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checkCvArr(L,4),angle_in_degrees);
  return 0;
}
static int luacv_cvPow(lua_State *L)
{
  const char f_msg[]="Pow("CVARR_NAME" src1, "CVARR_NAME" dst, num power)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvPow(checkCvArr(L,1),checkCvArr(L,2),checknumber(L,3));
 return 0;
}
static int luacv_cvExp(lua_State *L)
{
  const char f_msg[]="Exp("CVARR_NAME" src1, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvExp(checkCvArr(L,1),checkCvArr(L,2));
 return 0;
}

static int luacv_cvLog(lua_State *L)
{
  const char f_msg[]="Log("CVARR_NAME" src1, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvLog(checkCvArr(L,1),checkCvArr(L,2));
 return 0;
}

static int luacv_cvFastArctan(lua_State *L)
{
  const char f_msg[]="num FastArctan(num y, num x)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  lua_pushnumber(L,cvFastArctan((float)checknumber(L,1),(float)checknumber(L,2)));
  return 1;
}

static int luacv_cvCbrt(lua_State *L)
{
  const char f_msg[]="num Cbrt(num value)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushnumber(L,cvCbrt((float)checknumber(L,1)));
 return 1;
}

static int luacv_cvCheckArr(lua_State *L)
{
  const char f_msg[]="int CheckArr("CVARR_NAME" arr, int flags=0, num min_val=0, num max_val=0)";
  lua_Number var[2]={0,0};
  lua_Integer flags=0;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            flags=checkint(L,2); 
            break;
    case 3:
            flags=checkint(L,2); 
            var[0]=checknumber(L,3); 
            break;
    case 4:
            flags=checkint(L,2); 
            var[0]=checknumber(L,3); 
            var[1]=checknumber(L,4); 
            break;
    default:
            luaL_error(L,f_msg);
  }
  lua_pushnumber(L,cvCheckArr(checkCvArr(L,1),flags,var[0],var[1]));
  return 1;
}

static int luacv_cvRandArr(lua_State *L)
{
  const char f_msg[]="RandArr("CVRNG_NAME" rng, "CVARR_NAME" arr, int dist_type, "CVSCALAR_NAME" param1, "CVSCALAR_NAME" param2)";
  if (lua_gettop(L)!=5) luaL_error(L,f_msg);
  CvScalar *param1=checkCvScalar(L,4),*param2=checkCvScalar(L,5);
  cvRandArr(checkCvRNG(L,1),checkCvArr(L,2), checkint(L,3),*param1,*param2);
  return 0;
}

static int luacv_cvRandShuffle(lua_State *L)
{
  const char f_msg[]="RandShuffle("CVARR_NAME" mat, "CVRNG_NAME" rng, num iter_factor=1)";
  lua_Number iter=1;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            iter=checknumber(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvRandShuffle(checkCvArr(L,1),checkCvRNG(L,2),iter);
  return 0;
}

static int luacv_cvSort(lua_State *L)
{
  const char f_msg[]="Sort("CVARR_NAME" src, "CVARR_NAME" dst=nil, "CVARR_NAME" idxmat=nil, int flags=0)";
  lua_Integer flags=0;
  CvArr *dst=NULL,*idxmat=NULL;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            dst=checkCvArr(L,2);
            break;
    case 3:
            dst=checkCvArr(L,2);
            idxmat=checkCvArr(L,3);
            break;
    case 4:
            dst=checkCvArr(L,2);
            idxmat=checkCvArr(L,3);
            flags=checkint(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvSort(checkCvArr(L,1),dst,idxmat,flags);
 return 0;
}

static int luacv_cvSolveCubic(lua_State *L)
{
  const char f_msg[]="int SolveCubic("CVMAT_NAME" coeffs, "CVMAT_NAME" roots)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  lua_pushnumber(L,cvSolveCubic(checkCvMat(L,1),checkCvMat(L,2)));
 return 1;
}

static int luacv_cvSolvePoly(lua_State *L)
{
  const char f_msg[]="SolvePoly("CVMAT_NAME" coeffs , "CVMAT_NAME" roots2, int maxiter=20, int fig=100)";
  lua_Integer maxiter=20,fig=100;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            maxiter=checkint(L,3);
            break;
    case 4:
            maxiter=checkint(L,3);
            fig=checkint(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvSolvePoly(checkCvMat(L,1),checkCvMat(L,2),maxiter,fig);
 return 0;
}

//************************ matrix operations ***********************************//

static int luacv_cvCrossProduct(lua_State *L)
{
  const char f_msg[]="CrossProduct("CVARR_NAME" src1, "CVARR_NAME" src2, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
 cvCrossProduct(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3));
 return 0;
}

static int luacv_cvGEMM(lua_State *L)
{
  const char f_msg[]="GEMM("CVARR_NAME" src1, "CVARR_NAME" src2, num alpha, "CVARR_NAME" src3, num beta, "CVARR_NAME" dst, int tABC=0)";
  lua_Integer tABC=0;
  switch(lua_gettop(L))
  {
    case 6:
          break;
    case 7:
          tABC=checkint(L,7);
          break;
    default:
        luaL_error(L,f_msg);
  }
 cvGEMM(checkCvArr(L,1),checkCvArr(L,2),checknumber(L,3),checkCvArr(L,4),checknumber(L,5),checkCvArr(L,6),tABC);
 return 0;
}

static int luacv_cvMatMulAdd(lua_State *L)
{
  const char f_msg[]="MatMulAdd("CVARR_NAME" src1, "CVARR_NAME" src2, "CVARR_NAME" src3, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  cvMatMulAdd(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checkCvArr(L,4));
 return 0;
}

static int luacv_cvMatMul(lua_State *L)
{
  const char f_msg[]="MatMul("CVARR_NAME" src1, "CVARR_NAME" src2, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
 cvMatMul(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3));
 return 0;
}

static int luacv_cvTransform(lua_State *L)
{
  const char f_msg[]="Transform("CVARR_NAME" src1, "CVARR_NAME" dst, "CVMAT_NAME" transmat, "CVMAT_NAME" shiftvec=nil)";
  CvMat *shift=NULL;
  switch(lua_gettop(L))
  {
    case 3:
          break;
    case 4:
          shift=checkCvMat(L,4);
          break;
    default:
        luaL_error(L,f_msg);
  }
  cvTransform(checkCvArr(L,1),checkCvArr(L,2),checkCvMat(L,3),shift);
 return 0;
}

static int luacv_cvPerspectiveTransform(lua_State *L)
{
  const char f_msg[]="PerspectiveTransform("CVARR_NAME" src1, "CVARR_NAME" dst, "CVMAT_NAME" mat)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvPerspectiveTransform(checkCvArr(L,1),checkCvArr(L,2),checkCvMat(L,3));
 return 0;
}

static int luacv_cvMulTransposed(lua_State *L)
{
  const char f_msg[]="MulTransposed("CVARR_NAME" src1, "CVARR_NAME" dst, int order, "CVARR_NAME" delta=nil, num scale=1)";
  CvArr *delta=NULL;
  lua_Number scale=1;
  switch(lua_gettop(L))
  {
    case 3:
          break;
    case 4:
          delta=checkCvArr(L,4);
          break;
    case 5:
          delta=checkCvArr(L,4);
          scale=checknumber(L,5);
          break;
    default:
        luaL_error(L,f_msg);
  }
  cvMulTransposed(checkCvArr(L,1),checkCvArr(L,2),checkint(L,3),delta,scale);
 return 0;
}

static int luacv_cvTranspose(lua_State *L)
{
  const char f_msg[]="Transpose("CVARR_NAME" src1, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
 cvTranspose(checkCvArr(L,1),checkCvArr(L,2));
 return 0;
}

static int luacv_cvCompleteSymm(lua_State *L)
{
  const char f_msg[]="CompleteSymm("CVMAT_NAME" matrix, int LtoR=0)";
  lua_Integer ltor=0;
  switch(lua_gettop(L))
  {
    case 1:
          break;
    case 2:
          ltor=checkint(L,2);
          break;
    default:
        luaL_error(L,f_msg);
  }
  cvCompleteSymm(checkCvMat(L,1),ltor);
 return 0;
}

static int luacv_cvFlip(lua_State *L)
{
  const char f_msg[]="Flip("CVARR_NAME" src, "CVARR_NAME" dst=nil, int flip_mode=0)";
  lua_Integer flip=0;
  CvArr *dst=NULL;
  switch(lua_gettop(L))
  {
    case 1:
          break;
    case 2:
          dst=checkCvArr(L,2);
          break;
    case 3:
          dst=checkCvArr(L,2);
          flip=checkint(L,3);
          break;
    default:
        luaL_error(L,f_msg);
  }
  cvFlip(checkCvArr(L,1),dst,flip);
 return 0;
}

static int luacv_cvSVD(lua_State *L)
{
  const char f_msg[]="SVD("CVARR_NAME" A, "CVARR_NAME" W, "CVARR_NAME" U=nil, "CVARR_NAME" V=nil, int flags=0)";
  lua_Integer flags=0;
  CvArr *U=NULL,*V=NULL;
  switch(lua_gettop(L))
  {
    case 2:
          break;
    case 3:
          U=checkCvArr(L,3);
          break;
    case 4:
          U=checkCvArr(L,3);
          V=checkCvArr(L,4);
          break;
    case 5:
          U=checkCvArr(L,3);
          V=checkCvArr(L,4);
          flags=checkint(L,5);
          break;
    default:
        luaL_error(L,f_msg);
  }
  cvSVD(checkCvArr(L,1),checkCvArr(L,2),U,V,flags);
 return 0;
}

static int luacv_cvSVBkSb(lua_State *L)
{
  const char f_msg[]="SVBkSb("CVARR_NAME" W, "CVARR_NAME" U, "CVARR_NAME" V, "CVARR_NAME" B, "CVARR_NAME" X, int flags)";
  if (lua_gettop(L)!=6) luaL_error(L,f_msg);
 cvSVBkSb(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checkCvArr(L,4),checkCvArr(L,5),checkint(L,6));
 return 0;
}

static int luacv_cvInvert(lua_State *L)
{
  const char f_msg[]="num Invert("CVARR_NAME" src, "CVARR_NAME" dst, int method=CV_LU)";
  lua_Integer method=CV_LU;
  switch(lua_gettop(L))
  {
    case 2:
          break;
    case 3:
          method=checkint(L,3);
          break;
    default:
        luaL_error(L,f_msg);
  }
  lua_pushnumber(L,cvInvert(checkCvArr(L,1),checkCvArr(L,2),method));
  return 1;
}

static int luacv_cvSolve(lua_State *L)
{
  const char f_msg[]="int Solve("CVARR_NAME" src1, "CVARR_NAME" src2, "CVARR_NAME" dst, int method=CV_LU)";
  lua_Integer method=CV_LU;
  switch(lua_gettop(L))
  {
    case 3:
          break;
    case 4:
          method=checkint(L,4);
          break;
    default:
        luaL_error(L,f_msg);
  }
  lua_pushnumber(L,cvSolve(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),method));
  return 1;
}

static int luacv_cvDet(lua_State *L)
{
  const char f_msg[]="num Det("CVARR_NAME" mat)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
 lua_pushnumber(L,cvDet(checkCvArr(L,1)));
 return 1;
}

static int luacv_cvTrace(lua_State *L)
{
  const char f_msg[]=CVSCALAR_NAME" Trace("CVARR_NAME" mat)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  CvScalar s=cvTrace(checkCvArr(L,1));
  pushCvScalar(L,&s);
 return 1;
}

static int luacv_cvEigenVV(lua_State *L)
{
  const char f_msg[]="EigenVV("CVARR_NAME" mat, "CVARR_NAME" evects, "CVARR_NAME" evals, num eps=0, int lowindex=-1, int highindex=-1)";
  lua_Number eps=0;
  lua_Integer low=-1,high=-1;
  switch(lua_gettop(L))
  {
    case 3:
          break;
    case 4:
          eps=checknumber(L,4);
          break;
    case 5:
          eps=checknumber(L,4);
          low=checkint(L,5);
          break;
    case 6:
          eps=checknumber(L,4);
          low=checkint(L,5);
          high=checkint(L,6);
          break;
    default:
        luaL_error(L,f_msg);
  }
  cvEigenVV(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),eps,low,high);
  return 0;
}

static int luacv_cvSetIdentity(lua_State *L)
{
   const char f_msg[]="SetIdentity("CVARR_NAME" mat, "CVSCALAR_NAME" value=RealScalar(1))";
  CvScalar *val=NULL,v=cvRealScalar(1);
  val=&v;
  switch(lua_gettop(L))
  {
    case 1:
          break;
    case 2:
          val=checkCvScalar(L,2);
          break;
    default:
        luaL_error(L,f_msg);
  }
  cvSetIdentity(checkCvArr(L,1),*val);
 return 0;
}

static int luacv_cvRange(lua_State *L)
{
  const char f_msg[]=CVARR_NAME" Range("CVARR_NAME" mat, num start, num end)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  pushUserData(L,cvRange(checkCvArr(L,1),checknumber(L,2),checknumber(L,3)));
 return 1;
}

static int luacv_cvCalcCovarMatrix(lua_State *L)
{
  const char f_msg[]="CalcCovarMatrix("CVARR_NAME"[] vects, int count, "CVARR_NAME" cov_mat, "CVARR_NAME" avq, int flags)";
  const int top=lua_gettop(L),len=lua_objlen(L,1);
  if ((top!=5)||(!lua_istable(L,1))||(!len)) luaL_error(L,f_msg);
  CvArr *arrs[LUAI_MAXVARS];
  for(int i=1;i<=len;i++)
  {
        lua_rawgeti(L,1,i);
        arrs[(i-1)]=checkCvArr(L,top+1);
        lua_pop(L,1);
  }
  cvCalcCovarMatrix((const CvArr**)arrs,checkint(L,3),checkCvArr(L,4),checkCvArr(L,5),checkint(L,6));
 return 0;
}

static int luacv_cvCalcPCA(lua_State *L)
{
  const char f_msg[]="CalcPCA("CVARR_NAME" data, "CVARR_NAME" mean, "CVARR_NAME" eigenvals, "CVARR_NAME" eigenvects, int flags)";
  if (lua_gettop(L)!=5) luaL_error(L,f_msg);
  cvCalcPCA(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checkCvArr(L,4),checkint(L,5));
 return 0;
}

static int luacv_cvProjectPCA(lua_State *L)
{
  const char f_msg[]="ProjectPCA("CVARR_NAME" data, "CVARR_NAME" mean, "CVARR_NAME" eigenvals, "CVARR_NAME" result)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  cvProjectPCA(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checkCvArr(L,4));
 return 0;
}

static int luacv_cvBackProjectPCA(lua_State *L)
{
  const char f_msg[]="BackProjectPCA("CVARR_NAME" proj, "CVARR_NAME" mean, "CVARR_NAME" eigenvals, "CVARR_NAME" result)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  cvBackProjectPCA(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checkCvArr(L,4));
 return 0;
}

static int luacv_cvMahalanobis(lua_State *L)
{
  const char f_msg[]="num Mahalanobis("CVARR_NAME" vec1, "CVARR_NAME" vec2, "CVARR_NAME" mat)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  lua_pushnumber(L,cvMahalanobis(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3)));
 return 1;
}

//******************************** array statistics ***************************//

static int luacv_cvSum(lua_State *L)
{
  const char f_msg[]=CVSCALAR_NAME" Sum("CVARR_NAME" arr)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  CvScalar s=cvSum(checkCvArr(L,1));
  pushCvScalar(L,&s);
 return 1;
}

static int luacv_cvCountNonZero(lua_State *L)
{
  const char f_msg[]="int CountNonZero("CVARR_NAME" arr)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushnumber(L,cvCountNonZero(checkCvArr(L,1)));
 return 1;
}

static int luacv_cvAvg(lua_State *L)
{
  const char f_msg[]=CVSCALAR_NAME" Avg("CVARR_NAME" arr, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            mask=checkCvArr(L,2);
            break;
    default:
      luaL_error(L,f_msg);
  }
  CvScalar s=cvAvg(checkCvArr(L,1),mask);
  pushCvScalar(L,&s);
 return 1;
}

static int luacv_cvAvgSdv(lua_State *L)
{
  const char f_msg[]="AvgSdv("CVARR_NAME" arr, "CVSCALAR_NAME" mean, "CVSCALAR_NAME" std_dev, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            mask=checkCvArr(L,4);
            break;
    default:
      luaL_error(L,f_msg);
  }
  cvAvgSdv(checkCvArr(L,1),checkCvScalar(L,2),checkCvScalar(L,3),mask);
 return 0;
}

static int luacv_cvMinMaxLoc(lua_State *L)
{//zmena
  const char f_msg[]="num,num MinMaxLoc("CVARR_NAME" arr, "CVPOINT_NAME" min_loc=nil, "CVPOINT_NAME" max_loc=nil, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  CvPoint *min=NULL,*max=NULL;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            min=checkCvPoint(L,2);
            break;
    case 3:
            min=checkCvPoint(L,2);
            max=checkCvPoint(L,3);
            break;
    case 4:
            min=checkCvPoint(L,2);
            max=checkCvPoint(L,3);
            mask=checkCvArr(L,4);
            break;

    default:
      luaL_error(L,f_msg);
  }
  lua_Number min_d, max_d;
  cvMinMaxLoc(checkCvArr(L,1),&min_d,&max_d,min,max,mask);
  lua_pushnumber(L,min_d);
  lua_pushnumber(L,max_d);
 return 2;
}

static int luacv_cvNorm(lua_State *L)
{
  const char f_msg[]="num Norm("CVARR_NAME" arr1, "CVARR_NAME" arr2=nil, int norm_type=CV_L2, "CVARR_NAME" mask=nil)";
  CvArr *arr2=NULL,*mask=NULL;
  lua_Integer norm_type=CV_L2;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            arr2=checkCvArr(L,2);
            break;
    case 3:
            arr2=checkCvArr(L,2);
            norm_type=checkint(L,3);
            break;
    case 4:
            arr2=checkCvArr(L,2);
            norm_type=checkint(L,3);
            mask=checkCvArr(L,4);
            break;

    default:
      luaL_error(L,f_msg);
  }
  lua_pushnumber(L,cvNorm(checkCvArr(L,1),arr2,norm_type,mask));
 return 1;
}

static int luacv_cvNormalize(lua_State *L)
{
  const char f_msg[]="Normalize("CVARR_NAME" src, "CVARR_NAME" dst, num a=1, num b=0, int norm_type=CV_L2, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  lua_Number a=1,b=0;
  lua_Integer norm_type=CV_L2;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            a=checknumber(L,3);
            break;
    case 4:
            a=checknumber(L,3);
            b=checknumber(L,4);
            break;
    case 5:
            a=checknumber(L,3);
            b=checknumber(L,4);
            norm_type=checkint(L,5);
            break;
    case 6:
            a=checknumber(L,3);
            b=checknumber(L,4);
            norm_type=checkint(L,5);
            mask=checkCvArr(L,6);
            break;
    default:
      luaL_error(L,f_msg);
  }
  cvNormalize(checkCvArr(L,1),checkCvArr(L,2),a,b,norm_type,mask);
 return 0;
}

static int luacv_cvReduce(lua_State *L)
{
  const char f_msg[]="Reduce("CVARR_NAME" src, "CVARR_NAME" dst, int dim=-1, int op=CV_REDUCE_SUM)";
  lua_Integer dim=-1,op=CV_REDUCE_SUM;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            dim=checkint(L,3);
            break;
    case 4:
            dim=checkint(L,3);
            op=checkint(L,4);
            break;
    default:
      luaL_error(L,f_msg);
  }
  cvReduce(checkCvArr(L,1),checkCvArr(L,2),dim,op);
 return 0;
}

//**************** discrete linear transforms and related functions **************//

static int luacv_cvDFT(lua_State *L)
{
  const char f_msg[]="DFT("CVARR_NAME" src, "CVARR_NAME" dst, int flags, int nonzero_rows=0)";
  lua_Integer rows=0;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            rows=checkint(L,4);
            break;
    default:
      luaL_error(L,f_msg);
  }
  cvDFT(checkCvArr(L,1),checkCvArr(L,2),checkint(L,3),rows);
 return 0;
}

static int luacv_cvMulSpectrums(lua_State *L)
{
  const char f_msg[]="MulSpectrums("CVARR_NAME" src1, "CVARR_NAME" src2, "CVARR_NAME" dst, int flags)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  cvMulSpectrums(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checkint(L,4));
 return 0;
}

static int luacv_cvGetOptimalDFTSize(lua_State *L)
{
  const char f_msg[]="int GetOptimalDFTSize(int size0)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGetOptimalDFTSize(checkint(L,1)));
 return 1;
}

static int luacv_cvDCT(lua_State *L)
{
  const char f_msg[]="DCT("CVARR_NAME" src, "CVARR_NAME" dst, int flags)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvDCT(checkCvArr(L,1),checkCvArr(L,2),checkint(L,3));
 return 0;
}

//************************ dynamic data structures **************************//

static int luacv_cvSliceLength(lua_State *L)
{
  const char f_msg[]="int SliceLength("CVSLICE_NAME" slice, "CVSEQ_NAME" seq )";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  CvSlice *s=checkCvSlice(L,1);
  lua_pushnumber(L,cvSliceLength(*s,checkCvSeq(L,2)));
 return 1;
}

static int luacv_cvCreateMemStorage(lua_State *L)
{
  const char f_msg[]=CVMEMSTORAGE_NAME" CreateMemStorage(int block_size=0)";
  lua_Integer size=0;
  switch(lua_gettop(L))
  {
    case 0:
          break;
    case 1:
          size=checkint(L,1);
          break;
    default:
      luaL_error(L,f_msg);
  }
  pushCvMemStorage(L,cvCreateMemStorage(size));
 return 1;
}

static int luacv_cvCreateChildMemStorage(lua_State *L)
{
  const char f_msg[]=CVMEMSTORAGE_NAME" CreateChildMemStorage("CVMEMSTORAGE_NAME" parent)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  pushCvMemStorage(L,cvCreateChildMemStorage(checkCvMemStorage(L,1)));
 return 1;
}

static int luacv_cvReleaseMemStorage(lua_State *L)
{
  const char f_msg[]="ReleaseMemStorage("CVMEMSTORAGE_NAME" storage)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvMemStorage(L,1);
  luacv_releaseObject(L,1);
  return 0;
}

static int luacv_cvClearMemStorage(lua_State *L)
{
  const char f_msg[]="ClearMemStorage("CVMEMSTORAGE_NAME" storage)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  cvClearMemStorage(checkCvMemStorage(L,1));
 return 0;
}

static int luacv_cvSaveMemStoragePos(lua_State *L)
{
  const char f_msg[]="SaveMemStorage("CVMEMSTORAGE_NAME" storage, "CVMEMSTORAGE_NAME" pos)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvSaveMemStoragePos(checkCvMemStorage(L,1),checkCvMemStoragePos(L,2));
 return 0;
}

static int luacv_cvRestoreMemStoragePos(lua_State *L)
{
  const char f_msg[]="RestoreMemStoragePos("CVMEMSTORAGE_NAME" storage, "CVMEMSTORAGE_NAME" pos)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvRestoreMemStoragePos(checkCvMemStorage(L,1),checkCvMemStoragePos(L,2));
 return 0;
}

static int luacv_cvMemStorageAlloc(lua_State *L)
{
  UNIMPL(L);
  return 0;
}

static int luacv_cvMemStorageAllocString(lua_State *L)
{
  UNIMPL(L);
  return 0;
}

static int luacv_cvCreateSeq(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" CreateSeq(int seq_flags, int header_size, int elem_size, "CVMEMSTORAGE_NAME" storage)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  pushCvSeq(L,cvCreateSeq(checkint(L,1),checkint(L,2),checkint(L,3),checkCvMemStorage(L,4)));
 return 1;
}

static int luacv_cvSetSeqBlockSize(lua_State *L)
{
  const char f_msg[]="SetSeqBlockSize("CVSEQ_NAME" seq, int delta_elems)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvSetSeqBlockSize(checkCvSeq(L,1),checkint(L,2));
 return 0;
}

static int luacv_cvSeqPush(lua_State *L)
{
  const char f_msg[]="userdata SeqPush("CVSEQ_NAME" seq, userdata element=nil)";
  void *element=NULL;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            element=checkldata(L,2);
            break;
    default:
            luaL_error(L,f_msg);
  }
  pushUserData(L,cvSeqPush(checkCvSeq(L,1),element));
 return 1;
}

static int luacv_cvSeqPushFront(lua_State *L)
{
  const char f_msg[]="userdata SeqPushFront("CVSEQ_NAME" seq, userdata element=nil)";
  void *element=NULL;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            element=checkldata(L,2);
            break;
    default:
            luaL_error(L,f_msg);
  }
  pushUserData(L,cvSeqPushFront(checkCvSeq(L,1),element));
 return 1;
}

static int luacv_cvSeqPop(lua_State *L)
{
  const char f_msg[]="SeqPop("CVSEQ_NAME" seq, userdata element=nil)";
  void *element=NULL;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            element=checkldata(L,2);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvSeqPop(checkCvSeq(L,1),element);
 return 0;
}

static int luacv_cvSeqPopFront(lua_State *L)
{
  const char f_msg[]="SeqPopFront("CVSEQ_NAME" seq, userdata element=nil)";
  void *element=NULL;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            element=checkldata(L,2);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvSeqPopFront(checkCvSeq(L,1),element);
 return 0;
}

static int luacv_cvSeqPushMulti(lua_State *L)
{
  const char f_msg[]="SeqPushMulti("CVSEQ_NAME" seq, userdata[] elements, int count, int in_front=0 )";
  lua_Integer front=0; 
  const int top=lua_gettop(L),len=lua_objlen(L,2);
  if ((!len)||(!lua_istable(L,2))) luaL_error(L,f_msg);
  switch(top)
  {
    case 3:
            break;
    case 4:
            front=checkint(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }

  CvSeq *seq=checkCvSeq(L,1);
  void *array=(void*)luacv_alloc(seq->elem_size*len); 
  for(int i=1;i<=len;i++)
  {
       lua_rawgeti(L,2,i);
       memcpy(((void *)(((size_t)array)+(seq->elem_size*(i-1)))),checkldata(L,top+1),seq->elem_size); 
       lua_pop(L,1);
  }
  cvSeqPushMulti(seq,array,checkint(L,3),front);
  luacv_free(&array);
 return 0;
}

static int luacv_cvSeqPopMulti(lua_State *L)
{
  const char f_msg[]="SeqPopMulti("CVSEQ_NAME" seq, userdata[] elements, int count, int in_front=0, string convert_to='')";
  lua_Integer front=0; 
  const int top=lua_gettop(L);
  char *name=(char*)"";
  if (!lua_istable(L,2)) luaL_error(L,f_msg);
  switch(top)
  {
    case 3:
            break;
    case 4:
            front=checkint(L,4);
            break;
    case 5:
            front=checkint(L,4);
            name=(char*)checkstring(L,5);
            break;
    default:
            luaL_error(L,f_msg);
  }
  CvSeq *seq=checkCvSeq(L,1);
  const int count=checkint(L,3);
  void *arrs=(void*)lua_newuserdata(L,seq->elem_size*count);
  lua_pop(L,1);
  cvSeqPopMulti(seq,arrs,count,front);
  for(int i=1;i<=count;i++)
  {
    pushUserData(L,((void *)(((size_t)arrs)+(seq->elem_size*(i-1))))); 
    luaL_getmetatable(L,name);
    lua_setmetatable(L,top+1);
    lua_rawseti(L,2,i);
  }
  return 0;
}

static int luacv_cvSeqInsert(lua_State *L)
{
  const char f_msg[]="userdata SeqInsert("CVSEQ_NAME" seq, int before_index, userdata element=nil)";
  void* elem=NULL;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            elem=checkldata(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
 pushUserData(L,cvSeqInsert(checkCvSeq(L,1),checkint(L,2),elem));
 return 1;
}

static int luacv_cvSeqRemove(lua_State *L)
{
 const char f_msg[]="SeqRemove("CVSEQ_NAME" seq, int index)";
 if (lua_gettop(L)!=2) luaL_error(L,f_msg);
 cvSeqRemove(checkCvSeq(L,1),checkint(L,2));
 return 0;
}

static int luacv_cvClearSeq(lua_State *L)
{
 const char f_msg[]="ClearSeq("CVSEQ_NAME" seq)";
 if (lua_gettop(L)!=1) luaL_error(L,f_msg);
 checkCvSeq(L,1);
  luacv_releaseObject(L,1);
 return 0;
}

static int luacv_cvGetSeqElem(lua_State *L)
{
 const char f_msg[]="userdata GetSeqElem("CVSEQ_NAME" seq, int index, string convert_to='')";
 const int top=lua_gettop(L);
 char *name=NULL;
 switch(top)
 {
     case 2:
            break;
     case 3:
            name=(char*)checkstring(L,3);
            break;
     default: luaL_error(L,f_msg);
 }

 const int len=strlen(name);
 int count=luacv_checkmetaname(&name,len);

 CvSeq *seq=checkCvSeq(L,1);

 void *elem=(void*)cvGetSeqElem(seq,checkint(L,2));
 if (!len)
    pushUserData(L,elem);
 else if (count==-1) 
  {//pointer in sequence
    pushUserData(L,*(void**)elem);
    luaL_getmetatable(L,name);
    lua_setmetatable(L,top+1);
  }
  else if (count==1)
  {//object in sequence
    pushUserData(L,elem);
    luaL_getmetatable(L,name);
    lua_setmetatable(L,top+1);
  }
  else if (count>1)
  {//array of objects in sequence
    lua_newtable(L);
    int array_el=seq->elem_size/count;
    for(int i=1;i<=count;i++)
    {
      pushUserData(L,(void*)(((size_t)elem) + array_el*(i-1))); 
      luaL_getmetatable(L,name);
      lua_setmetatable(L,-2);
      lua_rawseti(L,-2,i);
    }
    luacv_free(&name);
  }

 return 1;
}

static int luacv_cvSeqElemIdx(lua_State *L)
{
  const char f_msg[]="int SeqElemIdx("CVSEQ_NAME" seq, userdata element, "CVSEQBLOCK_NAME" block=nil)";
  CvSeqBlock* block=NULL;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            block=checkCvSeqBlock(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
  lua_pushnumber(L,cvSeqElemIdx(checkCvSeq(L,1),checkldata(L,2),&block));
 return 1;
}

static int luacv_cvStartAppendToSeq(lua_State *L)
{
 const char f_msg[]="StartAppendToSeq("CVSEQ_NAME" seq, "CVSEQWRITER_NAME" writer)";
 if (lua_gettop(L)!=2) luaL_error(L,f_msg);
 cvStartAppendToSeq(checkCvSeq(L,1),checkCvSeqWriter(L,2));
 return 0;
}

static int luacv_cvStartWriteSeq(lua_State *L)
{
 const char f_msg[]="StartWriteSeq(int seq_flags, int header_size, int elem_size, "CVMEMSTORAGE_NAME" storage, "CVSEQWRITER_NAME" writer)";
 if (lua_gettop(L)!=5) luaL_error(L,f_msg);
 cvStartWriteSeq(checkint(L,1),checkint(L,2),checkint(L,3),checkCvMemStorage(L,4),checkCvSeqWriter(L,5));
 return 0;
}

static int luacv_cvEndWriteSeq(lua_State *L)
{
 const char f_msg[]=CVSEQ_NAME" EndWriteSeq("CVSEQWRITER_NAME" writer)";
 if (lua_gettop(L)!=1) luaL_error(L,f_msg);
 pushCvSeq(L,cvEndWriteSeq(checkCvSeqWriter(L,1)));
 return 1;
}

static int luacv_cvFlushSeqWriter(lua_State *L)
{
const char f_msg[]="FlushSeqWriter("CVSEQWRITER_NAME" writer)";
 if (lua_gettop(L)!=1) luaL_error(L,f_msg);
 cvFlushSeqWriter(checkCvSeqWriter(L,1));
 return 0;
}

static int luacv_cvStartReadSeq(lua_State *L)
{
  const char f_msg[]="StartReadSeq("CVSEQ_NAME" seq, "CVSEQREADER_NAME" reader, int reverse=0)";
  lua_Integer rev=0;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            rev=checkint(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvStartReadSeq(checkCvSeq(L,1),checkCvSeqReader(L,2),rev);
 return 0;
}

static int luacv_cvGetSeqReaderPos(lua_State *L)
{
 const char f_msg[]="int GetSeqReaderPos("CVSEQREADER_NAME" reader)";
 if (lua_gettop(L)!=1) luaL_error(L,f_msg);
 lua_pushnumber(L,cvGetSeqReaderPos(checkCvSeqReader(L,1)));
 return 1;
}

static int luacv_cvSetSeqReaderPos(lua_State *L)
{
  const char f_msg[]="SetSeqReaderPos("CVSEQREADER_NAME" reader, int index, int is_relative=0)";
  lua_Integer rel=0;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            rel=checkint(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvSetSeqReaderPos(checkCvSeqReader(L,1),checkint(L,2),rel);
 return 0;
}

static int luacv_cvCvtSeqToArray(lua_State *L)
{//zmena
  const char f_msg[]="userdata CvtSeqToArray("CVSEQ_NAME" seq, userdata[] elements, "CVSLICE_NAME" slice=CV_WHOLE_SEQ, string convert_to='')";
  CvSlice slice=CV_WHOLE_SEQ,*s=&slice;
  const int top=lua_gettop(L);
  char *name=(char*)"";
  if (!lua_istable(L,2)) luaL_error(L,f_msg);
  switch(top)
  {
    case 2:
            break;
    case 3:
            s=checkCvSlice(L,3);
            break;
    case 4:
            s=checkCvSlice(L,3);
            name=(char*)checkstring(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }

  const int len=s->end_index-s->start_index;
  CvSeq *seq=checkCvSeq(L,1);
  void *arrs=(void*)lua_newuserdata(L,seq->elem_size*len);
  lua_pop(L,1);
  pushUserData(L,cvCvtSeqToArray(seq,arrs,*s));
  for(int i=1;i<=len;i++)
  {
    pushUserData(L,((void *)(((size_t)arrs)+(seq->elem_size*(i-1))))); 
    luaL_getmetatable(L,name);
    lua_setmetatable(L,-2);
    lua_rawseti(L,2,i);
  }
 return 1;
}

static int luacv_cvMakeSeqHeaderForArray(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" MakeSeqHeaderForArray(int seq_type, int header_size, int elem_size, userdata[] elements, int total, "CVSEQ_NAME" seq, "CVSEQBLOCK_NAME" block)";
  const int len=lua_objlen(L,4),top=lua_gettop(L);
 if ((top!=7)||(!len)||(!lua_istable(L,4))) luaL_error(L,f_msg); 
  CvSeq *seq=checkCvSeq(L,6);
  void *elems=(void*)luacv_alloc(seq->elem_size*len); 
  for(int i=1;i<=len;i++)
  {
       lua_rawgeti(L,2,i);
       memcpy(((void *)(((size_t)elems)+(seq->elem_size*(i-1)))),checkldata(L,top+1),seq->elem_size); 
       lua_pop(L,1);
  }
 pushCvSeq(L,cvMakeSeqHeaderForArray(checkint(L,1),checkint(L,2),checkint(L,3),elems,checkint(L,5),seq,checkCvSeqBlock(L,7)));
 luacv_free(&elems);
 return 1;
}

static int luacv_cvSeqSlice(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" SeqSlice("CVSEQ_NAME" seq, "CVSLICE_NAME" slice, "CVMEMSTORAGE_NAME" storage=nil, int copy_data=0)";
  CvMemStorage *storage=NULL;
  lua_Integer copy=0;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            storage=checkCvMemStorage(L,3);
            break;
    case 4:
            storage=checkCvMemStorage(L,3);
            copy=checkint(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  CvSlice *s=checkCvSlice(L,2);
  pushCvSeq(L,cvSeqSlice(checkCvSeq(L,1),*s,storage,copy));
 return 1;
}

static int luacv_cvCloneSeq(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" CloneSeq("CVSEQ_NAME" seq, "CVMEMSTORAGE_NAME" storage=nil)";
  CvMemStorage *storage=NULL;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            storage=checkCvMemStorage(L,2);
            break;
    default:
            luaL_error(L,f_msg);
  }
  pushCvSeq(L,cvCloneSeq(checkCvSeq(L,1),storage));
 return 1;
}

static int luacv_cvSeqRemoveSlice(lua_State *L)
{
 const char f_msg[]="SeqRemoveSlice("CVSEQ_NAME" seq, "CVSLICE_NAME" slice)";
 if (lua_gettop(L)!=2) luaL_error(L,f_msg);
 CvSlice *s=checkCvSlice(L,2);
 cvSeqRemoveSlice(checkCvSeq(L,1),*s);
 return 0;
}

static int luacv_cvSeqInsertSlice(lua_State *L)
{
 const char f_msg[]="SeqInsertSlice("CVSEQ_NAME" seq, int before_index, "CVARR_NAME" from_arr)";
 if (lua_gettop(L)!=3) luaL_error(L,f_msg);
 cvSeqInsertSlice(checkCvSeq(L,1),checkint(L,2),checkCvArr(L,3));
 return 0;
}

static int luacv_cvSeqSort(lua_State *L)
{
  UNIMPL(L);
 return 0;
}
static int luacv_cvSeqSearch(lua_State *L)
{
  UNIMPL(L);
 return 0;
}

static int luacv_cvSeqInvert(lua_State *L)
{
 const char f_msg[]="SeqInvert("CVSEQ_NAME" seq)";
 if (lua_gettop(L)!=1) luaL_error(L,f_msg);
 cvSeqInvert(checkCvSeq(L,1));
 return 0;
}

static int luacv_cvSeqPartition(lua_State *L)
{
 UNIMPL(L);
 return 0;
}

/******************** Internal sequence functions ***************************/

static int luacv_cvChangeSeqBlock(lua_State *L)
{
 const char f_msg[]="ChangeSeqBlock(userdata reader, int direction)";
 if (lua_gettop(L)!=2) luaL_error(L,f_msg);
 cvChangeSeqBlock(checkldata(L,1),checkint(L,2));
 return 0;
}

static int luacv_cvCreateSeqBlock(lua_State *L)
{
 const char f_msg[]="CreateSeqBlock("CVSEQWRITER_NAME" writer)";
 if (lua_gettop(L)!=1) luaL_error(L,f_msg);
 cvCreateSeqBlock(checkCvSeqWriter(L,1));
 return 0;
}

static int luacv_cvCreateSet(lua_State *L)
{
 const char f_msg[]=CVSET_NAME" CreateSet(int set_flags, int header_size, int elem_size, "CVMEMSTORAGE_NAME" storage)";
 if (lua_gettop(L)!=4) luaL_error(L,f_msg);
 pushCvSet(L,cvCreateSet(checkint(L,1),checkint(L,2),checkint(L,3),checkCvMemStorage(L,4)));
 return 1;
}

static int luacv_cvSetAdd(lua_State *L)
{//zmena
  const char f_msg[]="int,userdata SetAdd("CVSET_NAME" set_header, "CVSETELEM_NAME" elem=nil, int getnewelem_ptr=0)";
  CvSetElem *elem=NULL,*t_new_elem=NULL,nelem;
  int new_elem=0;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            elem=checkCvSetElem(L,2); 
            break;
    case 3:
            elem=checkCvSetElem(L,2); 
            new_elem=checkint(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
  if (new_elem) t_new_elem=&nelem;
  lua_pushnumber(L,cvSetAdd(checkCvSet(L,1),elem,&t_new_elem));
  if (!new_elem) return 1;
  pushCvSetElem(L,t_new_elem);
  return 2;
}

static int luacv_cvSetNew(lua_State *L)
{
  const char f_msg[]=CVSETELEM_NAME" SetNew("CVSET_NAME" set_header)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  pushCvSetElem(L,cvSetNew(checkCvSet(L,1)));
 return 1;
}

static int luacv_cvSetRemoveByPtr(lua_State *L)
{
  const char f_msg[]="SetRemoveByPtr("CVSET_NAME" set_header, userdata elem)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvSetRemoveByPtr(checkCvSet(L,1),checkldata(L,2));
 return 0;
}

static int luacv_cvSetRemove(lua_State *L)
{
  const char f_msg[]="SetRemove("CVSET_NAME" set_header, int index)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvSetRemove(checkCvSet(L,1),checkint(L,2));
 return 0;
}
static int luacv_cvGetSetElem(lua_State *L)
{
  const char f_msg[]=CVSETELEM_NAME" GetSetElem("CVSET_NAME" set_header, int index)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  pushCvSetElem(L,cvGetSetElem(checkCvSet(L,1),checkint(L,2)));
  return 1;
}
static int luacv_cvClearSet(lua_State *L)
{
  const char f_msg[]="ClearSet("CVSET_NAME" set_header)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  cvClearSet(checkCvSet(L,1));
 return 0;
}

static int luacv_cvCreateGraph(lua_State *L)
{
  const char f_msg[]="CreateGraph(int graph_flags, int header_size, int vtx_size, int edge_size, "CVMEMSTORAGE_NAME" storage)";
  if (lua_gettop(L)!=5) luaL_error(L,f_msg);
  pushCvGraph(L,cvCreateGraph(checkint(L,1),checkint(L,2),checkint(L,3),checkint(L,4),checkCvMemStorage(L,5)));
 return 1;
}

static int luacv_cvGraphAddVtx(lua_State *L)
{
  const char f_msg[]="int GraphAddVtx("CVGRAPH_NAME" graph, "CVGRAPHVTX_NAME" vtx=nil, "CVGRAPHVTX_NAME" inserted_vtx=nil)";
  CvGraphVtx *vtx=NULL,*inserted_vtx=NULL;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            vtx=checkCvGraphVtx(L,2);
            break;
    case 3:
            vtx=checkCvGraphVtx(L,2);
            inserted_vtx=checkCvGraphVtx(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
  lua_pushnumber(L,cvGraphAddVtx(checkCvGraph(L,1),vtx,&inserted_vtx));

 return 1;
}

static int luacv_cvGraphAddEdge(lua_State *L)
{
  const char f_msg[]="int GraphAddEdge("CVGRAPH_NAME" graph, int start_idx, int end_idx, "CVGRAPHEDGE_NAME" edge=nil, "CVGRAPHEDGE_NAME" inserted_edge=nil)";
  CvGraphEdge *edge=NULL,*inserted_edge=NULL;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            edge=checkCvGraphEdge(L,4);
            break;
    case 5:
            edge=checkCvGraphEdge(L,4);
            inserted_edge=checkCvGraphEdge(L,5);
            break;
    default:
            luaL_error(L,f_msg);
  }
  lua_pushnumber(L,cvGraphAddEdge(checkCvGraph(L,1),checkint(L,2),checkint(L,3),edge,&inserted_edge));

 return 1;
}


static int luacv_cvGraphAddEdgeByPtr(lua_State *L)
{
  const char f_msg[]="int GraphAddEdgeByPtr("CVGRAPH_NAME" graph, "CVGRAPHVTX_NAME" start_vtx, "CVGRAPHVTX_NAME" end_vtx, "CVGRAPHEDGE_NAME" edge=nil, "CVGRAPHEDGE_NAME" inserted_edge=nil)";
  CvGraphEdge *edge=NULL,*inserted_edge=NULL;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            edge=checkCvGraphEdge(L,4);
            break;
    case 5:
            edge=checkCvGraphEdge(L,4);
            inserted_edge=checkCvGraphEdge(L,5);
            break;
    default:
            luaL_error(L,f_msg);
  }
  lua_pushnumber(L,cvGraphAddEdgeByPtr(checkCvGraph(L,1),checkCvGraphVtx(L,2),checkCvGraphVtx(L,3),edge,&inserted_edge));

 return 1;
}

static int luacv_cvGraphRemoveEdgeByPtr(lua_State *L)
{
   const char f_msg[]="GraphRemoveEdgeByPtr("CVGRAPH_NAME" graph, "CVGRAPHVTX_NAME" start_vtx, "CVGRAPHVTX_NAME" end_vtx )";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvGraphRemoveEdgeByPtr(checkCvGraph(L,1),checkCvGraphVtx(L,2),checkCvGraphVtx(L,3));
 return 0;
}

static int luacv_cvGraphRemoveEdge(lua_State *L)
{
   const char f_msg[]="GraphRemoveEdge("CVGRAPH_NAME" graph, int start_idx, int end_idx)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvGraphRemoveEdge(checkCvGraph(L,1),checkint(L,2),checkint(L,3));

 return 0;
}

static int luacv_cvGraphRemoveVtxByPtr(lua_State *L)
{
 const char f_msg[]="GraphRemoveVtxByPtr("CVGRAPH_NAME" graph, "CVGRAPHVTX_NAME" vtx)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGraphRemoveVtxByPtr(checkCvGraph(L,1),checkCvGraphVtx(L,2)));
 return 1;
}

static int luacv_cvGraphRemoveVtx(lua_State *L)
{
  const char f_msg[]="GraphRemoveVtx("CVGRAPH_NAME" graph, int index)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGraphRemoveVtx(checkCvGraph(L,1),checkint(L,2)));
 return 1;
}

static int luacv_cvFindGraphEdge(lua_State *L)
{
  const char f_msg[]=CVGRAPHEDGE_NAME" FindGraphEdge("CVGRAPH_NAME" graph, int start_idx, int end_idx)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  pushCvGraphEdge(L,cvFindGraphEdge(checkCvGraph(L,1),checkint(L,2),checkint(L,3)));

 return 1;
}
static int luacv_cvFindGraphEdgeByPtr(lua_State *L)
{
   const char f_msg[]=CVGRAPHEDGE_NAME" FindGraphEdgeByPtr("CVGRAPH_NAME" graph, "CVGRAPHVTX_NAME" start_vtx, "CVGRAPHVTX_NAME" end_vtx)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvFindGraphEdgeByPtr(checkCvGraph(L,1),checkCvGraphVtx(L,2),checkCvGraphVtx(L,3));
 return 1;
}

static int luacv_cvGetGraphVtx(lua_State *L)
{
 const char f_msg[]=CVGRAPHVTX_NAME" GetGraphVtx("CVGRAPH_NAME" graph, int idx)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  pushCvGraphVtx(L,cvGetGraphVtx(checkCvGraph(L,1),checkint(L,2)));
 return 1;
}
static int luacv_cvGetGraphVtxIdx(lua_State *L)
{
  const char f_msg[]="int GraphVtxIdx("CVGRAPH_NAME" graph, "CVGRAPHVTX_NAME" vtx)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGraphVtxIdx(checkCvGraph(L,1),checkCvGraphVtx(L,2)));
 return 1;
}

static int luacv_cvGraphEdgeIdx(lua_State *L)
{
  const char f_msg[]="int GraphEdgeIdx("CVGRAPH_NAME" graph, "CVGRAPHEDGE_NAME" edge)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGraphEdgeIdx(checkCvGraph(L,1),checkCvGraphEdge(L,2)));

 return 1;
}
static int luacv_cvGraphGetVtxCount(lua_State *L)
{
  const char f_msg[]="int GraphGetVtxCount("CVGRAPH_NAME" graph)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGraphGetVtxCount(checkCvGraph(L,1)));

 return 1;
}
static int luacv_cvGraphGetEdgeCount(lua_State *L)
{
  const char f_msg[]="int GraphGetEdgeCount("CVGRAPH_NAME" graph)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGraphGetEdgeCount(checkCvGraph(L,1)));

 return 1;
}

static int luacv_cvClearGraph(lua_State *L)
{
  const char f_msg[]="ClearGraph("CVGRAPH_NAME" graph)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  cvClearGraph(checkCvGraph(L,1));
 return 0;
}

static int luacv_cvGraphVtxDegree(lua_State *L)
{
  const char f_msg[]="int GraphVtxDegree("CVGRAPH_NAME" graph, int vtx_idx)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGraphVtxDegree(checkCvGraph(L,1),checkint(L,2)));

 return 1;
}
static int luacv_cvGraphVtxDegreeByPtr(lua_State *L)
{
  const char f_msg[]="int GraphVtxDegreeByPtr("CVGRAPH_NAME" graph, "CVGRAPHVTX_NAME" vtx)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGraphVtxDegreeByPtr(checkCvGraph(L,1),checkCvGraphVtx(L,2)));
 return 1;
}

static int luacv_cvCreateGraphScanner(lua_State *L)
{
  const char f_msg[]=CVGRAPHSCANNER_NAME" CreateGraphScanner("CVGRAPH_NAME" graph, "CVGRAPHVTX_NAME" vtx=nil, int mask=CV_GRAPH_ALL_ITEMS)";
  CvGraphVtx *vtx=NULL;
  int mask=CV_GRAPH_ALL_ITEMS;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            vtx=checkCvGraphVtx(L,2);
            break;
    case 3:
            vtx=checkCvGraphVtx(L,2);
            mask=checkint(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
  pushCvGraphScanner(L,cvCreateGraphScanner(checkCvGraph(L,1),vtx,mask));

 return 1;
}

static int luacv_cvReleaseGraphScanner(lua_State *L)
{
  const char f_msg[]="ReleaseGraphScanner("CVGRAPHSCANNER_NAME" scanner)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvGraphScanner(L,1);
  luacv_releaseObject(L,1);
 return 0;
}

static int luacv_cvNextGraphItem(lua_State *L)
{
  const char f_msg[]="int NextGraphItem("CVGRAPHSCANNER_NAME" scanner)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushnumber(L,cvNextGraphItem(checkCvGraphScanner(L,1)));
 return 1;
}
static int luacv_cvCloneGraph(lua_State *L)
{
  const char f_msg[]=CVGRAPH_NAME" CloneGraph("CVGRAPH_NAME" graph, "CVMEMSTORAGE_NAME" storage)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  pushCvGraph(L,cvCloneGraph(checkCvGraph(L,1),checkCvMemStorage(L,2)));
 return 1;
}

//**************************** Drawing functions ******************************/
static int luacv_cvLine(lua_State *L)
{
  const char f_msg[]="Line("CVARR_NAME" img, "CVPOINT_NAME" pt1, "CVPOINT_NAME" pt2, "CVSCALAR_NAME" color, int thickness=1, int line_type=8, int shift=0)";
  lua_Integer thick=1,line=8, shift=0;
  switch(lua_gettop(L))
  {
    case 4:
            break;
    case 5:
            thick=checkint(L,5);
            break;
    case 6:
            thick=checkint(L,5);
            line=checkint(L,6);
            break;
    case 7:
            thick=checkint(L,5);
            line=checkint(L,6);
            shift=checkint(L,7);
            break;

    default:
            luaL_error(L,f_msg);
  }
  CvPoint *p1=checkCvPoint(L,2),*p2=checkCvPoint(L,3);
  CvScalar *scalar=checkCvScalar(L,4);
  cvLine(checkCvArr(L,1),*p1,*p2,*scalar,thick,line,shift);
  return 0;
}

static int luacv_cvRectangle(lua_State *L)
{
  const char f_msg[]="Rectangle("CVARR_NAME" img, "CVPOINT_NAME" pt1, "CVPOINT_NAME" pt2, "CVSCALAR_NAME" color, int thickness=1, int line_type=8, int shift=0)";
  lua_Integer thick=1,line=8, shift=0;
  switch(lua_gettop(L))
  {
    case 4:
            break;
    case 5:
            thick=checkint(L,5);
            break;
    case 6:
            thick=checkint(L,5);
            line=checkint(L,6);
            break;
    case 7:
            thick=checkint(L,5);
            line=checkint(L,6);
            shift=checkint(L,7);
            break;

    default:
            luaL_error(L,f_msg);
  }
  CvPoint *p1=checkCvPoint(L,2),*p2=checkCvPoint(L,3);
  CvScalar *scalar=checkCvScalar(L,4);
  cvRectangle(checkCvArr(L,1),*p1,*p2,*scalar,thick,line,shift);
  return 0;
}

static int luacv_cvCircle(lua_State *L)
{
  const char f_msg[]="Circle("CVARR_NAME" img, "CVPOINT_NAME" center, int radius, "CVSCALAR_NAME" color, int thickness=1, int line_type=8, int shift=0)";
  lua_Integer thick=1,line=8, shift=0;
  switch(lua_gettop(L))
  {
    case 4:
            break;
    case 5:
            thick=checkint(L,5);
            break;
    case 6:
            thick=checkint(L,5);
            line=checkint(L,6);
            break;
    case 7:
            thick=checkint(L,5);
            line=checkint(L,6);
            shift=checkint(L,7);
            break;

    default:
            luaL_error(L,f_msg);
  }
  CvPoint *p1=checkCvPoint(L,2);
  CvScalar *scalar=checkCvScalar(L,4);
  cvCircle(checkCvArr(L,1),*p1,checkint(L,3),*scalar,thick,line,shift);
  return 0;
}

static int luacv_cvEllipse(lua_State *L)
{
  const char f_msg[]="Ellipse("CVARR_NAME" img, "CVPOINT_NAME" center, "CVSIZE_NAME" axes, num angle, num start_angle, num end_angle, "CVSCALAR_NAME" color, int thickness=1, int line_type=8, int shift=0)";
  lua_Integer thick=1,line=8, shift=0;
  switch(lua_gettop(L))
  {
    case 7:
            break;
    case 8:
            thick=checkint(L,8);
            break;
    case 9:
            thick=checkint(L,8);
            line=checkint(L,9);
            break;
    case 10:
            thick=checkint(L,8);
            line=checkint(L,9);
            shift=checkint(L,10);
            break;

    default:
            luaL_error(L,f_msg);
  }
  CvPoint *p1=checkCvPoint(L,2);
  CvSize *size=checkCvSize(L,3);
  CvScalar *scalar=checkCvScalar(L,7);
  cvEllipse(checkCvArr(L,1),*p1,*size,checknumber(L,4),checknumber(L,5),checknumber(L,6),*scalar,thick,line,shift);
  return 0;
}

static int luacv_cvEllipseBox(lua_State *L)
{
  const char f_msg[]="EllipseBox("CVARR_NAME" img, "CVBOX2D_NAME" box, "CVSCALAR_NAME" color, int thickness=1, int line_type=8, int shift=0)";
  lua_Integer thick=1,line=8, shift=0;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            thick=checkint(L,4);
            break;
    case 5:
            thick=checkint(L,4);
            line=checkint(L,5);
            break;
    case 6:
            thick=checkint(L,4);
            line=checkint(L,5);
            shift=checkint(L,6);
            break;

    default:
            luaL_error(L,f_msg);
  }
  cvEllipseBox(checkCvArr(L,1),*checkCvBox2D(L,2),*checkCvScalar(L,3),thick,line,shift);
  return 0;
}

static int luacv_cvFillConvexPoly(lua_State *L)
{
  const char f_msg[]="FillConvexPoly("CVARR_NAME" img, "CVPOINT_NAME" pts, int npts, "CVSCALAR_NAME" color, int line_type=8, int shift=0)";
  const int top=lua_gettop(L),len=lua_objlen(L,2);
  if ((!len)||(!lua_istable(L,2))) luaL_error(L,f_msg);
  lua_Integer line=8, shift=0;
  switch(top)
  {
    case 4:
            break;
    case 5:
            line=checkint(L,5);
            break;
    case 6:
            line=checkint(L,5);
            shift=checkint(L,6);
            break;
    default:
            luaL_error(L,f_msg);
  }
  CvPoint *tmp,*pts=(CvPoint*)luacv_alloc(sizeof(CvPoint)*len);
  for(int i=1;i<=len;i++)
  {
    lua_rawgeti(L,2,i);
    tmp=checkCvPoint(L,top+1);
    lua_pop(L,1);
    pts[i-1]=*tmp;
  }
  CvScalar *scalar=checkCvScalar(L,4);
  cvFillConvexPoly(checkCvArr(L,1),pts,checkint(L,3),*scalar,line,shift);
  luacv_free(&pts);
  return 0;
}

static int luacv_cvFillPoly(lua_State *L)
{
  const char f_msg[]="FillPoly("CVARR_NAME" img, "CVPOINT_NAME"[] pts, int[] npts, int contours, "CVSCALAR_NAME" color, int line_type=8, int shift=0)";
  const int top=lua_gettop(L),pts_len=lua_objlen(L,2),npts_len=lua_objlen(L,3);
  if ((!pts_len)||(!lua_istable(L,2))) luaL_error(L,f_msg);
  if ((!npts_len)||(!lua_istable(L,3))) luaL_error(L,f_msg);
  lua_Integer line=8, shift=0;
  switch(lua_gettop(L))
  {
    case 5:
            break;
    case 6:
            line=checkint(L,6);
            break;
    case 7:
            line=checkint(L,6);
            shift=checkint(L,7);
            break;
    default:
            luaL_error(L,f_msg);
  }
  int npts[LUAI_MAXVARS];
  for(int i=1;i<=npts_len;i++)
  {
    lua_rawgeti(L,3,i);
    npts[i-1]=checkint(L,top+1);
    lua_pop(L,1);
  }

  int sub_len=0;
  CvPoint **pts=(CvPoint**)luacv_alloc(sizeof(CvPoint*)*npts[0]);
  for(int i=1;i<=pts_len;i++)
  {
    lua_rawgeti(L,2,i);  
    if (npts_len>1)
    {
      if (!lua_istable(L,top+1)) luaL_error(L,f_msg);
      sub_len=lua_objlen(L,top+1);
      pts[i-1]=(CvPoint*)luacv_alloc(sizeof(CvPoint)*sub_len); 
      for(int j=1;j<=sub_len;j++)
      {
        lua_rawgeti(L,top+1,j);
        pts[i-1][j-1]=*checkCvPoint(L,top+2);
        lua_pop(L,1);
      }
    }
    else
    {
      pts[i-1]=(CvPoint*)luacv_alloc(sizeof(CvPoint)); 
      pts[i-1]=checkCvPoint(L,top+1);
    }

    lua_pop(L,1);
  }


  cvFillPoly(checkCvArr(L,1),pts,npts,checkint(L,4),*checkCvScalar(L,5),line,shift);
  if (sub_len>0)
    for(int i=0;i<pts_len;i++)
      luacv_free(&pts[i]);

  luacv_free(&pts);
  return 0;
}

static int luacv_cvPolyLine(lua_State *L)
{
  const char f_msg[]="PolyLine("CVARR_NAME" img, "CVPOINT_NAME"[] pts, int[] npts, int contours, int is_closed, "CVSCALAR_NAME" color, int thickness=1, int line_type=8, int shift=0)";
  const int top=lua_gettop(L),pts_len=lua_objlen(L,2),npts_len=lua_objlen(L,3);
  if ((!pts_len)||(!lua_istable(L,2))) luaL_error(L,f_msg);
  if ((!npts_len)||(!lua_istable(L,3))) luaL_error(L,f_msg);
  lua_Integer line=8, shift=0, thickness=1;
  switch(lua_gettop(L))
  {
    case 6:
            break;
    case 7:
            thickness=checkint(L,7);
            break;
    case 8:
            thickness=checkint(L,7);
            line=checkint(L,8);
            break;
    case 9:
            thickness=checkint(L,7);
            line=checkint(L,8);
            shift=checkint(L,9);
            break;
    default:
            luaL_error(L,f_msg);
  }
  int npts[LUAI_MAXVARS];
  for(int i=1;i<=npts_len;i++)
  {
    lua_rawgeti(L,3,i);
    npts[i-1]=checkint(L,top+1);
    lua_pop(L,1);
  }

  int sub_len=0;
  CvPoint **pts=(CvPoint**)luacv_alloc(sizeof(CvPoint*)*npts[0]);
  for(int i=1;i<=pts_len;i++)
  {
    lua_rawgeti(L,2,i);  
    if (npts_len>1)
    {
      if (!lua_istable(L,top+1)) luaL_error(L,f_msg);
      sub_len=lua_objlen(L,top+1);
      pts[i-1]=(CvPoint*)luacv_alloc(sizeof(CvPoint)*sub_len); 
      for(int j=1;j<=sub_len;j++)
      {
        lua_rawgeti(L,top+1,j);
        pts[i-1][j-1]=*checkCvPoint(L,top+2);
        lua_pop(L,1);
      }
    }
    else
    {
      pts[i-1]=(CvPoint*)luacv_alloc(sizeof(CvPoint)); 
      pts[i-1]=checkCvPoint(L,top+1);
    }

    lua_pop(L,1);
  }

  CvScalar *scalar=checkCvScalar(L,6);
  cvPolyLine(checkCvArr(L,1),pts,npts,checkint(L,4),checkint(L,5),*scalar,thickness,line,shift);
  if (sub_len>0)
    for(int i=0;i<pts_len;i++)
      luacv_free(&pts[i]);


  luacv_free(&pts);
  return 0;
}

static int luacv_cvClipLine(lua_State *L)
{
 const char f_msg[]="int ClipLine("CVSIZE_NAME" img_size, "CVPOINT_NAME" pt1, "CVPOINT_NAME" pt2)";
 if (lua_gettop(L)!=3) luaL_error(L,f_msg);
 CvSize *s=checkCvSize(L,1);
 lua_pushnumber(L,cvClipLine(*s,checkCvPoint(L,2),checkCvPoint(L,3)));
 return 1;
}

static int luacv_cvInitLineIterator(lua_State *L)
{
  const char f_msg[]="int InitLineIterator("CVARR_NAME" image, "CVPOINT_NAME" pt1, "CVPOINT_NAME" pt2, "CVLINEITERATOR_NAME" line_iterator, int connectivity=8, int left_to_righ=0)";
  lua_Integer conn=8,ltor=0;
  switch(lua_gettop(L))
  {
    case 4:
            break;
    case 5:
            conn=checkint(L,5);
            break;
    case 6:
            conn=checkint(L,5);
            ltor=checkint(L,6);
            break;
    default:
            luaL_error(L,f_msg);
  }
  CvPoint *p1=checkCvPoint(L,2),*p2=checkCvPoint(L,3);
  lua_pushnumber(L,cvInitLineIterator(checkCvArr(L,1),*p1,*p2,checkCvLineIterator(L,4),conn,ltor));
  return 1;
}

static int luacv_cvInitFont(lua_State *L)
{
  const char f_msg[]="InitFont("CVFONT_NAME" font, int font_face, num hscale, num vscale, num shear=0, int thickness=1, int line_type=8)";
  lua_Number shear=0;
  lua_Integer thickness=1, line_type=8;
  switch(lua_gettop(L))
  {
    case 4:
            break;
    case 5:
            shear=checknumber(L,5);
            break;
    case 6:
            shear=checknumber(L,5);
            thickness=checkint(L,6);
            break;
    case 7:
            shear=checknumber(L,5);
            thickness=checkint(L,6);
            line_type=checkint(L,7);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvInitFont(checkCvFont(L,1),checkint(L,2),checknumber(L,3),checknumber(L,4),shear,thickness,line_type);
  return 0;
}

static int luacv_cvFont(lua_State *L)
{
  const char f_msg[]=CVFONT_NAME" Font(num scale, int thickness=1)";
  lua_Integer thickness=1;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            thickness=checkint(L,2);
            break;
    default:
            luaL_error(L,f_msg);
  }
  CvFont f=cvFont(checknumber(L,1),thickness);
  pushCvFont(L,&f);
  return 1;
}

static int luacv_cvPutText(lua_State *L)
{
  const char f_msg[]="PutText("CVARR_NAME" img, string text, "CVPOINT_NAME" org, "CVFONT_NAME" font, "CVSCALAR_NAME" color)";
  if (lua_gettop(L)!=5)luaL_error(L,f_msg);
  CvScalar *s=checkCvScalar(L,5);
  CvPoint *p=checkCvPoint(L,3);
  cvPutText(checkCvArr(L,1),checkstring(L,2),*p,checkCvFont(L,4),*s);
  return 0;
}

static int luacv_cvGetTextSize(lua_State *L)
{//zmena
  const char f_msg[]="int GetTextSize(string text, CvFont font, CvSize text_size)";
  if (lua_gettop(L)!=4)luaL_error(L,f_msg);
  int baseline;
  cvGetTextSize(checkstring(L,1),checkCvFont(L,2),checkCvSize(L,3),&baseline);
  lua_pushnumber(L,baseline);
  return 1;
}

static int luacv_cvColorToScalar(lua_State *L)
{
   const char f_msg[]=CVSCALAR_NAME" ColorToScalar(num packed_color, int arrtype)";
  if (lua_gettop(L)!=2)luaL_error(L,f_msg);
  CvScalar s=cvColorToScalar(checknumber(L,1),checkint(L,2));
  pushCvScalar(L,&s);
  return 1;
}

static int luacv_cvEllipse2Poly(lua_State *L)
{
   const char f_msg[]="int Ellipse2Poly("CVPOINT_NAME" center, "CVSIZE_NAME" axes, int angle, int argc_end, "CVPOINT_NAME" pts, int delta)";
  if (lua_gettop(L)!=7)luaL_error(L,f_msg);
  CvPoint *center=checkCvPoint(L,1);
  CvSize *axes=checkCvSize(L,2);
  lua_pushnumber(L,cvEllipse2Poly(*center,*axes,checkint(L,3),checkint(L,4),checkint(L,5),checkCvPoint(L,6),checkint(L,7)));
  return 1;
}

static int luacv_cvDrawContours(lua_State *L)
{
  const char f_msg[]="DrawContours("CVARR_NAME" img, "CVSEQ_NAME" contour, "CVSCALAR_NAME" external_color, "CVSCALAR_NAME" hole_color, int max_level, int thickness=1, int line_type=8, "CVPOINT_NAME" offset=Point(0,0))";
  lua_Integer thickness=1,line_type=8;
  CvPoint p_temp=cvPoint(0,0),*p=&p_temp;
  switch(lua_gettop(L))
  {
    case 5:
            break;
    case 6:
            thickness=checkint(L,6);
            break;
    case 7:
            thickness=checkint(L,6);
            line_type=checkint(L,7);
            break;
    case 8:
            thickness=checkint(L,6);
            line_type=checkint(L,7);
            p=checkCvPoint(L,8);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvDrawContours(checkCvArr(L,1),checkCvSeq(L,2),*checkCvScalar(L,3),*checkCvScalar(L,4),checkint(L,5),thickness,line_type,*p);
  return 0;
}

static int luacv_cvLUT(lua_State *L)
{
  const char f_msg[]="LUT("CVARR_NAME" src, "CVARR_NAME" dst, "CVARR_NAME" lut)";
  if (lua_gettop(L)!=3)luaL_error(L,f_msg);
  cvLUT(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3));
 return 0;
}

//********************** iteration through the sequence tree *******************//

static int luacv_cvCreateTreeNodeIterator(lua_State *L)
{//New func
  const char f_msg[]=CVTREENODEITERATOR_NAME" CreateTreeNodeIterator()";
 if (lua_gettop(L)!=0)luaL_error(L,f_msg);
 CvTreeNodeIterator iter;
 iter.node=NULL;
 iter.level=0;
 iter.max_level=0;
 pushCvTreeNodeIterator(L,&iter);
 return 1;
}

static int luacv_cvInitTreeNodeIterator(lua_State *L)
{
   const char f_msg[]="InitTreeNodeIterator("CVTREENODEITERATOR_NAME" tree_iterator, userdata first, int max_level)";
 if (lua_gettop(L)!=3)luaL_error(L,f_msg);
 cvInitTreeNodeIterator(checkCvTreeNodeIterator(L,1),checkldata(L,2),checkint(L,3)); 
 return 0;
}

static int luacv_cvNextTreeNode(lua_State *L)
{
 const char f_msg[]="userdata NextTreeNode("CVTREENODEITERATOR_NAME" tree_iterator)";
 if (lua_gettop(L)!=1)luaL_error(L,f_msg);
 pushUserData(L,cvNextTreeNode(checkCvTreeNodeIterator(L,1))); 
 return 1;
}

static int luacv_cvPrevTreeNode(lua_State *L)
{
 const char f_msg[]="userdata PrevTreeNode("CVTREENODEITERATOR_NAME" tree_iterator)";
 if (lua_gettop(L)!=1)luaL_error(L,f_msg);
 pushUserData(L,cvPrevTreeNode(checkCvTreeNodeIterator(L,1))); 
 return 1;
}

static int luacv_cvInsertNodeIntoTree(lua_State *L)
{
 const char f_msg[]="InsertNodeIntoTree(userdata node, userdata parent, userdata frame)";
 if (lua_gettop(L)!=3)luaL_error(L,f_msg);
  cvInsertNodeIntoTree(checkldata(L,1),checkldata(L,2),checkldata(L,3));
 return 0;
}

static int luacv_cvRemoveNodeFromTree(lua_State *L)
{
const char f_msg[]="RemoveNodeFromTree(userdata node, userdata frame)";
 if (lua_gettop(L)!=2)luaL_error(L,f_msg);
  cvRemoveNodeFromTree(checkldata(L,1),checkldata(L,2));
 return 0;
}

static int luacv_cvTreeToNodeSeq(lua_State *L)
{
 const char f_msg[]=CVSEQ_NAME" TreeToNodeSeq(userdata first, int header_size, "CVMEMSTORAGE_NAME" storage)";
 if (lua_gettop(L)!=3)luaL_error(L,f_msg);
 pushCvSeq(L,cvTreeToNodeSeq(checkldata(L,1),checkint(L,2),checkCvMemStorage(L,3)));
 return 1;
}

static int luacv_cvKMeans2(lua_State *L)
{//zmena
  const char f_msg[]="int,num KMeans2("CVARR_NAME" samples, int cluster_count, "CVARR_NAME" labels, "CVTERMCRITERIA_NAME" termcrit, int attmepts=1, "CVRNG_NAME" rng=0, int flags=0, "CVARR_NAME" _centers=0, num compactness=0)";
  lua_Integer attempts=1,flags=1;
  lua_Number compactness=0;
  CvRNG* rng=NULL;
  CvArr *_centers=NULL;
  switch(lua_gettop(L))
  {
    case 4:
            break;
    case 5:
            attempts=checkint(L,5);
            break;
    case 6:
            attempts=checkint(L,5);
            rng=checkCvRNG(L,6);
            break;
    case 7:
            attempts=checkint(L,5);
            rng=checkCvRNG(L,6);
            flags=checkint(L,7);
            break;
    case 8:
            attempts=checkint(L,5);
            rng=checkCvRNG(L,6);
            flags=checkint(L,7);
            _centers=checkCvArr(L,8);
            break;
    case 9:
            attempts=checkint(L,5);
            rng=checkCvRNG(L,6);
            flags=checkint(L,7);
            _centers=checkCvArr(L,8);
            compactness=checknumber(L,9);
            break;
    default:
            luaL_error(L,f_msg);
  }
 CvTermCriteria *c=checkCvTermCriteria(L,4);
 lua_pushnumber(L,cvKMeans2(checkCvArr(L,1),checkint(L,2),checkCvArr(L,3),*c,attempts,rng,flags,_centers,&compactness));
 lua_pushnumber(L,compactness);
 return 2;
}

static int luacv_CVRGB(lua_State *L)
{
  const char f_msg[]=CVSCALAR_NAME" CV_RGB(int r, int g, int b)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  CvScalar s=CV_RGB(checkint(L,1),checkint(L,2),checkint(L,3)); 
  pushCvScalar(L,&s);
  return 1;
}

static int luacv_cvLoad(lua_State*L)
{
  const char f_msg[]="userdata,string Load(string filename, string convert_to="", "CVMEMSTORAGE_NAME" storage=nil, string name="")";
  char *name=NULL,*conv=NULL;
  CvMemStorage *storage=NULL;
  int top=lua_gettop(L);
  switch(top)
  {
    case 1:
            break;
    case 2:
            conv=(char*)checkstring(L,2);
            break;
    case 3:
            conv=(char*)checkstring(L,2);
            storage=checkCvMemStorage(L,3);
            break;
    case 4:
            conv=(char*)checkstring(L,2);
            storage=checkCvMemStorage(L,3);
            name=(char*)checkstring(L,4);
            break;

    default:
            luaL_error(L,f_msg);
  }
  char *ret;
 void *elem=cvLoad(checkstring(L,1),storage,name,(const char**)&ret);
  pushUserData(L,elem);
  luaL_getmetatable(L,conv);
  lua_setmetatable(L,top+1);
  lua_pushstring(L,ret);
  return 2;
}


static int luacv_cvBox2D(lua_State *L)
{//new function
  const char f_msg[]=CVBOX2D_NAME" Box2D("CVPOINT2D32F_NAME" center,"CVSIZE2D32F_NAME" size, num angle)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  CvBox2D box;
  box.center=*checkCvPoint2D32f(L,1);
  box.size=*checkCvSize2D32f(L,2);
  box.angle=checknumber(L,3);
  pushCvBox2D(L,&box);

  return 1;
}
const luaL_Reg core[]=
{//cxtypes
  funcReg(Round),               funcReg(Floor),             funcReg(Ceil),
  funcReg(InvSqrt),             funcReg(Sqrt),              funcReg(IsNaN),
  funcReg(IsInf),               funcReg(RNG),               funcReg(RandInt),
  funcReg(RandReal),            funcReg(Mat),               funcReg(mGet),
  funcReg(mSet),                funcReg(ScalarAll),         funcReg(RealScalar),
  funcReg(IplDepth),            funcReg(Rect),              funcReg(RectToROI),
  funcReg(ROIToRect),           funcReg(TermCriteria),      funcReg(Point),
  funcReg(Point2D32f),          funcReg(PointTo32f),        funcReg(PointFrom32f),
  funcReg(Point3D32f),          funcReg(Point2D64f),        funcReg(Point3D64f),
  funcReg(Size),                funcReg(Size2D32f),         funcReg(Slice),
  funcReg(Scalar),              funcReg(Box2D),             

//cxcore
  funcReg(CreateImageHeader),   funcReg(InitImageHeader),   funcReg(ReleaseImage),
  funcReg(ReleaseImageHeader),  funcReg(CreateImage),       funcReg(CloneImage),
  funcReg(SetImageCOI),         funcReg(GetImageCOI),       funcReg(SetImageROI),
  funcReg(ResetImageROI),       funcReg(GetImageROI),       funcReg(CreateMatHeader),
  funcReg(InitMatHeader),       funcReg(CreateMat),         funcReg(ReleaseMat),
  funcReg(DecRefData),          funcReg(IncRefData),        funcReg(CloneMat),
  funcReg(GetSubRect),          funcReg(GetRows),           funcReg(GraphRemoveEdge),     
  funcReg(GetRow),              funcReg(GetCols),           funcReg(GetCol),
  funcReg(GetDiag),             funcReg(ScalarToRawData),   funcReg(RawDataToScalar),
  funcReg(CreateMatNDHeader),   funcReg(CreateMatND),       funcReg(ReleaseMatND),
  funcReg(InitMatNDHeader),     funcReg(CloneMatND),        funcReg(CreateSparseMat),
  funcReg(ReleaseSparseMat),    funcReg(CloneSparseMat),    funcReg(InitSparseMatIterator),
  funcReg(GetNextSparseNode),   funcReg(CreateNArrayIterator),funcReg(InitNArrayIterator),
  funcReg(NextNArraySlice),     funcReg(GetElemType),       funcReg(GetDims),
  funcReg(GetDimSize),          funcReg(Ptr1D),             funcReg(Ptr2D),
  funcReg(Ptr3D),               funcReg(PtrND),             funcReg(GetND),
  funcReg(Get1D),               funcReg(Get2D),             funcReg(Get3D),
  funcReg(GetReal1D),           funcReg(GetReal2D),         funcReg(GetReal3D),
  funcReg(GetRealND),           funcReg(Set1D),             funcReg(Set2D),
  funcReg(Set3D),               funcReg(SetND),             funcReg(SetReal1D),
  funcReg(SetReal2D),           funcReg(SetReal3D),         funcReg(SetRealND),
  funcReg(ClearND),             funcReg(GetMat),            funcReg(GetImage),
  funcReg(ReshapeMatND),        funcReg(ReshapeND),         funcReg(Reshape),
  funcReg(Repeat),              funcReg(CreateData),        funcReg(SetData),
  funcReg(GetRawData),          funcReg(GetSize),           funcReg(Set),
  funcReg(Copy),                funcReg(SetZero),           funcReg(ClearGraph),
  funcReg(Split),               funcReg(Merge),             funcReg(MixChannels),
  funcReg(ConvertScale),        funcReg(GraphVtxDegree),    funcReg(GraphVtxDegreeByPtr), 
  funcReg(Convert),             funcReg(ConvertScaleAbs),   funcReg(GetGraphVtx),
  funcReg(CheckTermCriteria),   funcReg(Add),               funcReg(AddS),
  funcReg(Sub),                 funcReg(SubS),              funcReg(SubRS),
  funcReg(Mul),                 funcReg(Div),               funcReg(ScaleAdd),
  funcReg(AXPY),                funcReg(AddWeighted),       funcReg(DotProduct),
  funcReg(And),                 funcReg(AndS),              funcReg(Or),
  funcReg(OrS),                 funcReg(Xor),               funcReg(XorS),
  funcReg(Not),                 funcReg(InRange),           funcReg(InRangeS),
  funcReg(Cmp),                 funcReg(CmpS),              funcReg(Min),
  funcReg(Max),                 funcReg(MinS),              funcReg(MaxS),
  funcReg(AbsDiff),             funcReg(AbsDiffS),          funcReg(Abs),
  funcReg(CartToPolar),         funcReg(PolarToCart),       funcReg(Pow),
  funcReg(Exp),                 funcReg(Log),               funcReg(FastArctan),
  funcReg(Cbrt),                funcReg(CheckArr),          funcReg(GetGraphVtxIdx),
  funcReg(RandArr),             funcReg(RandShuffle),       funcReg(Sort),
  funcReg(SolveCubic),          funcReg(SolvePoly),         funcReg(CrossProduct),
  funcReg(GEMM),                funcReg(MatMulAdd),         funcReg(MatMul),
  funcReg(GraphGetVtxCount),    funcReg(Transform),         funcReg(GraphEdgeIdx),
  funcReg(PerspectiveTransform),funcReg(MulTransposed),     funcReg(Transpose),
  funcReg(GraphGetEdgeCount),   funcReg(CompleteSymm),      funcReg(Flip),
  funcReg(CreateGraphScanner),  funcReg(SVD),               funcReg(SVBkSb),
  funcReg(Invert),              funcReg(NextGraphItem),     funcReg(Solve),
  funcReg(Det),                 funcReg(Trace),             funcReg(SetIdentity),
  funcReg(EigenVV),             funcReg(Range),             funcReg(CalcCovarMatrix),
  funcReg(CalcPCA),             funcReg(ProjectPCA),        funcReg(BackProjectPCA),
  funcReg(Mahalanobis),         funcReg(Sum),               funcReg(CountNonZero),
  funcReg(Avg),                 funcReg(AvgSdv),            funcReg(MinMaxLoc),
  funcReg(Norm),                funcReg(Normalize),         funcReg(Reduce),
  funcReg(DFT),                 funcReg(CloneGraph),        funcReg(GetOptimalDFTSize),
  funcReg(MulSpectrums),        funcReg(DCT),               funcReg(SliceLength),
  funcReg(CreateMemStorage),    funcReg(CreateChildMemStorage),funcReg(ClearMemStorage),
  funcReg(SaveMemStoragePos),   funcReg(RestoreMemStoragePos),funcReg(ReleaseMemStorage),
  funcReg(MemStorageAlloc),     funcReg(MemStorageAllocString),funcReg(CreateSeq),
  funcReg(SetSeqBlockSize),     funcReg(SeqPush),           funcReg(SeqPushFront),
  funcReg(SeqPop),              funcReg(SeqPopFront),       funcReg(SeqPushMulti),
  funcReg(SeqPopMulti),         funcReg(SeqRemove),         funcReg(SeqInsert),
  funcReg(ClearSeq),            funcReg(GetSeqElem),        funcReg(SeqElemIdx),
  funcReg(StartAppendToSeq),    funcReg(StartWriteSeq),     funcReg(EndWriteSeq),
  funcReg(FlushSeqWriter),      funcReg(StartReadSeq),      funcReg(SetSeqReaderPos),
  funcReg(CvtSeqToArray),       funcReg(MakeSeqHeaderForArray),funcReg(GetSeqReaderPos),
  funcReg(SeqRemoveSlice),      funcReg(SeqInsertSlice),    funcReg(SeqSlice),
  funcReg(CloneSeq),            funcReg(SeqSearch),         funcReg(SeqPartition),
  funcReg(SeqSort),             funcReg(SeqInvert),         funcReg(ChangeSeqBlock),
  funcReg(CreateSeqBlock),      funcReg(CreateSet),         funcReg(SetAdd),
  funcReg(SetNew),              funcReg(SetRemoveByPtr),    funcReg(SetRemove),
  funcReg(GetSetElem),          funcReg(ClearSet),          funcReg(CreateGraph),
  funcReg(GraphAddVtx),         funcReg(GraphRemoveVtx),    funcReg(GraphRemoveVtxByPtr),
  funcReg(FindGraphEdge),     funcReg(FindGraphEdgeByPtr),
//******* drawing functions *************//
  funcReg(Line),                funcReg(Rectangle),         funcReg(Circle),
  funcReg(Ellipse),             funcReg(EllipseBox),        funcReg(FillConvexPoly),
  funcReg(FillPoly),            funcReg(PolyLine),
  funcReg(ClipLine),            funcReg(InitLineIterator),  funcReg(InitFont),
  funcReg(Font),                funcReg(PutText),           funcReg(GetTextSize),
  funcReg(ColorToScalar),       funcReg(Ellipse2Poly),      funcReg(DrawContours),
  funcReg(LUT),                 funcReg(CreateTreeNodeIterator),funcReg(InitTreeNodeIterator),
  funcReg(NextTreeNode),        funcReg(PrevTreeNode),      funcReg(InsertNodeIntoTree),
  funcReg(RemoveNodeFromTree),  funcReg(TreeToNodeSeq),     funcReg(KMeans2),
  funcReg(Load),                funcReg(ReleaseData),       funcReg(ReleaseGraphScanner),
  funcReg(CreateSeqReader),     funcReg(GetTickCount),      funcReg(GetTickFrequency),
  //***** aliases to other functions ******************//
  {"CV_RGB",luacv_CVRGB},
  {"iGet",luacv_iGet},
  {"iSet",luacv_iSet},
  {"DrawRect",luacv_cvRectangle},
  {"DrawLine",luacv_cvLine},
  {"DrawCircle",luacv_cvCircle},
  {"DrawEllipse",luacv_cvEllipse},
  {"DrawPolyLine",luacv_cvPolyLine},
  {"GetSubArr",luacv_cvGetSubRect},        
  {"Zero",luacv_cvSetZero},
  {"CvtScale",luacv_cvConvertScale},
  {"Scale",luacv_cvConvertScale},
  {"CvtScaleAbs",luacv_cvConvertScaleAbs},
  {"CheckArray",luacv_cvCheckArr},
  {"MatMulAddS",luacv_cvTransform},   
  {"MatMulAddEx",luacv_cvGEMM},
  {"T",luacv_cvTranspose},
  {"Mirror",luacv_cvFlip},      
  {"Inv",luacv_cvInvert},
  {"FFT",luacv_cvDFT},
  {"GrapAddEdge",luacv_cvGraphAddEdge},
  {"GrapAddEdgeByPtr",luacv_cvGraphAddEdgeByPtr},
  {"GraphRemoveEdge",luacv_cvGraphRemoveEdgeByPtr},
  {"GraphFindEdge",luacv_cvFindGraphEdge},
  {"GraphFindEdgeByPtr",luacv_cvFindGraphEdgeByPtr},

  {NULL,NULL}
};

const luaL_ConvFunc core_matconv[]=
{
  matTableReg(CV_8U),
  matTableReg(CV_8S),
  matTableReg(CV_16U),
  matTableReg(CV_16S),
  matTableReg(CV_32S),
  matTableReg(CV_32F),
  matTableReg(CV_64F),
  {NULL,0,NULL,NULL}
};

const luaL_ConvFunc core_imageconv[]=
{
  imTableReg(IPL_DEPTH_1U),
  imTableReg(IPL_DEPTH_8U),
  imTableReg(IPL_DEPTH_8S),
  imTableReg(IPL_DEPTH_16U),
  imTableReg(IPL_DEPTH_16S),
  imTableReg(IPL_DEPTH_32S),
  imTableReg(IPL_DEPTH_64F),
  imTableReg(IPL_DEPTH_32F),
  {NULL,0,NULL,NULL}
};

const luacv_var core_object[]=
{
  {CVRNG_NAME,sizeof(CvRNG)},
  {IPLIMAGE_NAME,sizeof(IplImage)},
  {IPLROI_NAME,sizeof(IplROI)},
  {IPLCONVKERNEL_NAME,sizeof(IplConvKernel)},
  {IPLCONVKERNELFP_NAME,sizeof(IplConvKernelFP)},
  {CVMAT_NAME,sizeof(CvMat)},
  {CVMATND_NAME,sizeof(CvMatND)},
  {CVSPARSEMAT_NAME,sizeof(CvSparseMat)},
  {CVSPARSENODE_NAME,sizeof(CvSparseNode)},
  {CVSPARSEMATITERATOR_NAME,sizeof(CvSparseMatIterator)},
  {CVHISTOGRAM_NAME,sizeof(CvHistogram)},
  {CVRECT_NAME,sizeof(CvRect)},
  {CVTERMCRITERIA_NAME,sizeof(CvTermCriteria)},
  {CVPOINT_NAME,sizeof(CvPoint)},
  {CVPOINT2D32F_NAME,sizeof(CvPoint2D32f)},
  {CVPOINT3D32F_NAME,sizeof(CvPoint3D32f)},
  {CVPOINT2D64F_NAME,sizeof(CvPoint2D64f)},
  {CVPOINT3D64F_NAME,sizeof(CvPoint3D64f)},
  {CVSIZE_NAME,sizeof(CvSize)},
  {CVSIZE2D32F_NAME,sizeof(CvSize2D32f)},
  {CVBOX2D_NAME,sizeof(CvBox2D)},
  {CVLINEITERATOR_NAME,sizeof(CvLineIterator)},
  {CVSLICE_NAME,sizeof(CvSlice)},
  {CVSCALAR_NAME,sizeof(CvScalar)},
  {CVMEMBLOCK_NAME,sizeof(CvMemBlock)},
  {CVMEMSTORAGE_NAME,sizeof(CvMemStorage)},
  {CVMEMSTORAGEPOS_NAME,sizeof(CvMemStoragePos)},
  {CVSEQBLOCK_NAME,sizeof(CvSeqBlock)},
  {CVSEQ_NAME,sizeof(CvSeq)},
  {CVSETELEM_NAME,sizeof(CvSetElem)},
  {CVSET_NAME,sizeof(CvSet)},
  {CVGRAPHEDGE_NAME,sizeof(CvGraphEdge)},
  {CVGRAPHVTX_NAME,sizeof(CvGraphVtx)},
  {CVGRAPHVTX2D_NAME,sizeof(CvGraphVtx2D)},
  {CVGRAPH_NAME,sizeof(CvGraph)},
  {CVCHAIN_NAME,sizeof(CvChain)},
  {CVCONTOUR_NAME,sizeof(CvContour)},
  {CVSEQWRITER_NAME,sizeof(CvSeqWriter)},
  {CVSEQREADER_NAME,sizeof(CvSeqReader)},
  {CVSIZE_NAME,sizeof(CvSize)},
  {CVGRAPHSCANNER_NAME,sizeof(CvGraphScanner)},
  {NULL,0}
};

const luacv_var core_var[]=
{//cxtypes
  varReg(CV_PI),                varReg(CV_LOG2),              varReg(IPL_DEPTH_SIGN),
  varReg(IPL_DATA_ORDER_PIXEL), varReg(IPL_DATA_ORDER_PLANE), varReg(IPL_ORIGIN_TL),
  varReg(IPL_ORIGIN_BL),        varReg(IPL_ALIGN_4BYTES),     varReg(IPL_ALIGN_8BYTES),
  varReg(IPL_ALIGN_16BYTES),    varReg(IPL_ALIGN_32BYTES),    varReg(IPL_ALIGN_DWORD),
  varReg(IPL_ALIGN_QWORD),      varReg(IPL_BORDER_CONSTANT),
  varReg(IPL_BORDER_REPLICATE), varReg(IPL_BORDER_REFLECT),   varReg(IPL_BORDER_WRAP),
  varReg(IPL_IMAGE_HEADER),     varReg(IPL_IMAGE_DATA),       varReg(IPL_IMAGE_ROI),
  varReg(IPL_BORDER_REFLECT_101),varReg(IPL_IMAGE_MAGIC_VAL), varReg(CV_CN_MAX),
  varReg(CV_CN_SHIFT),          varReg(CV_DEPTH_MAX),         varReg(CV_USRTYPE1),
  varReg(CV_MAT_DEPTH_MASK),    varReg(CV_AUTO_STEP),         varReg(CV_MAT_CN_MASK),
  varReg(CV_MAT_TYPE_MASK),     varReg(CV_MAT_CONT_FLAG_SHIFT),varReg(CV_MAT_CONT_FLAG),
  varReg(CV_MAGIC_MASK),        varReg(CV_MAT_MAGIC_VAL),     varReg(CV_MAX_DIM),
  varReg(CV_MAX_DIM_HEAP),      varReg(CV_SPARSE_MAT_MAGIC_VAL),varReg(CV_HIST_MAGIC_VAL),
  varReg(CV_HIST_UNIFORM_FLAG), varReg(CV_HIST_RANGES_FLAG),  varReg(CV_HIST_ARRAY),
  varReg(CV_HIST_SPARSE),       varReg(CV_HIST_TREE),         varReg(CV_HIST_UNIFORM),
  varReg(CV_TERMCRIT_ITER),     varReg(CV_TERMCRIT_NUMBER),   varReg(CV_TERMCRIT_EPS),
  varReg(CV_WHOLE_SEQ_END_INDEX),varReg(CV_STORAGE_MAGIC_VAL),varReg(CV_SET_ELEM_IDX_MASK),
  varReg(CV_SET_ELEM_FREE_FLAG),varReg(CV_SEQ_MAGIC_VAL),     varReg(CV_SET_MAGIC_VAL),
  varReg(CV_SEQ_ELTYPE_BITS),   varReg(CV_SEQ_ELTYPE_MASK),   varReg(CV_SEQ_ELTYPE_POINT),
  varReg(CV_SEQ_ELTYPE_CODE),   varReg(CV_SEQ_ELTYPE_GENERIC),varReg(CV_SEQ_ELTYPE_PTR),
  varReg(CV_SEQ_ELTYPE_PPOINT), varReg(CV_SEQ_ELTYPE_INDEX),  varReg(CV_SEQ_ELTYPE_GRAPH_EDGE),
  varReg(CV_SEQ_ELTYPE_GRAPH_VERTEX),varReg(CV_SEQ_ELTYPE_TRIAN_ATR),varReg(CV_SEQ_ELTYPE_CONNECTED_COMP),          
  varReg(CV_SEQ_ELTYPE_POINT3D),varReg(CV_SEQ_KIND_BITS),     varReg(CV_SEQ_KIND_MASK),
  varReg(CV_SEQ_KIND_GENERIC),  varReg(CV_SEQ_KIND_CURVE),    varReg(CV_SEQ_KIND_BIN_TREE),
  varReg(CV_SEQ_KIND_GRAPH),    varReg(CV_SEQ_KIND_SUBDIV2D), varReg(CV_SEQ_FLAG_SHIFT),
  varReg(CV_SEQ_FLAG_CLOSED),   varReg(CV_SEQ_FLAG_SIMPLE),   varReg(CV_SEQ_FLAG_CONVEX),
  varReg(CV_SEQ_FLAG_HOLE),     varReg(CV_GRAPH_FLAG_ORIENTED),varReg(CV_GRAPH),
  varReg(CV_ORIENTED_GRAPH),    varReg(CV_SEQ_POINT_SET),     varReg(CV_SEQ_POINT3D_SET),
  varReg(CV_SEQ_POLYLINE),      varReg(CV_SEQ_POLYGON),       varReg(CV_SEQ_CONTOUR),
  varReg(CV_SEQ_SIMPLE_POLYGON),varReg(CV_SEQ_CHAIN),         varReg(CV_SEQ_CHAIN_CONTOUR),
  varReg(CV_SEQ_POLYGON_TREE),  varReg(CV_SEQ_CONNECTED_COMP),varReg(CV_SEQ_INDEX),
  varReg(CV_AUTOSTEP),          varReg(CV_MAX_ARR),           varReg(CV_NO_DEPTH_CHECK),
  varReg(CV_NO_CN_CHECK),       varReg(CV_NO_SIZE_CHECK),     varReg(CV_CMP_EQ),
  varReg(CV_CMP_GT),            varReg(CV_CMP_GE),            varReg(CV_CMP_LT),
  varReg(CV_CMP_LE),            varReg(CV_CMP_NE),            varReg(CV_RAND_UNI),
  varReg(CV_RAND_NORMAL),       varReg(CV_SORT_EVERY_ROW),    varReg(CV_SORT_EVERY_COLUMN),
  varReg(CV_SORT_ASCENDING),    varReg(CV_SORT_DESCENDING),   varReg(CV_GEMM_A_T),
  varReg(CV_GEMM_B_T),          varReg(CV_GEMM_C_T),          varReg(CV_SVD_MODIFY_A),
  varReg(CV_SVD_U_T),           varReg(CV_SVD_V_T),           varReg(CV_LU),
  varReg(CV_SVD),               varReg(CV_SVD_SYM),           varReg(CV_CHOLESKY),
  varReg(CV_QR),                varReg(CV_NORMAL),            varReg(CV_COVAR_SCRAMBLED),
  varReg(CV_COVAR_NORMAL),      varReg(CV_COVAR_USE_AVG),     varReg(CV_COVAR_SCALE),
  varReg(CV_COVAR_ROWS),        varReg(CV_COVAR_COLS),        varReg(CV_PCA_DATA_AS_ROW),
  varReg(CV_PCA_DATA_AS_COL),   varReg(CV_PCA_USE_AVG),       varReg(CV_C),
  varReg(CV_L1),                varReg(CV_L2),                varReg(CV_NORM_MASK),
  varReg(CV_RELATIVE),          varReg(CV_DIFF),              varReg(CV_MINMAX),
  varReg(CV_DIFF_C),            varReg(CV_DIFF_L1),           varReg(CV_DIFF_L2),
  varReg(CV_RELATIVE_C),        varReg(CV_RELATIVE_L1),       varReg(CV_RELATIVE_L2),
  varReg(CV_REDUCE_SUM),        varReg(CV_REDUCE_AVG),        varReg(CV_REDUCE_MAX),
  varReg(CV_REDUCE_MIN),        varReg(CV_DXT_FORWARD),       varReg(CV_DXT_INVERSE),
  varReg(CV_DXT_SCALE),         varReg(CV_DXT_INVERSE_SCALE), varReg(CV_DXT_ROWS),
  varReg(CV_DXT_MUL_CONJ),      varReg(CV_FRONT),             varReg(CV_ErrModeSilent),
  varReg(CV_BACK),              varReg(CV_FILLED),            varReg(CV_AA),
  varReg(CV_FONT_HERSHEY_SIMPLEX),varReg(CV_FONT_HERSHEY_PLAIN),varReg(CV_FONT_HERSHEY_DUPLEX),
  varReg(CV_FONT_HERSHEY_COMPLEX),varReg(CV_FONT_HERSHEY_TRIPLEX),varReg(CV_FONT_HERSHEY_COMPLEX_SMALL),
  varReg(CV_FONT_HERSHEY_SCRIPT_SIMPLEX),varReg(CV_FONT_HERSHEY_SCRIPT_COMPLEX),varReg(CV_FONT_ITALIC),
  varReg(CV_FONT_VECTOR0),      varReg(CV_KMEANS_USE_INITIAL_LABELS),varReg(CV_ErrModeLeaf),
  varReg(CV_ErrModeParent),
  {NULL,0}
};
