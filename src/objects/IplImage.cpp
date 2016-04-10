#include "IplImage.h"

static int IplImage_tostring(lua_State *L)
{
  IplImage *im=checkIplImage(L,1);
  lua_pushfstring(L,IPLIMAGE_NAME" object %p\n\tint nSize=%d\n\tint ID=%d\n\t"\
  "int nChannels=%d\n\tint alphaChannel=%d\n\tint depth=%d\n\tint origin=%d\n\t"\
  "int align=%d\n\tint width=%d\n\tint height=%d\n\t"IPLROI_NAME" roi=%p\n\t"\
  "int imageSize=%d\n\tnum[] imageData=%p\n\tint widthStep=%d"\
  ,im,im->nSize,im->ID,im->nChannels,im->alphaChannel,im->depth,im->origin,im->align\
  ,im->width,im->height,im->roi,im->imageSize,im->imageData,im->widthStep);
  return 1;
}

static int IplImage_gc(lua_State *L)
{

  luacv_obj<IplImage>*obj=(luacv_obj<IplImage>*)luaL_checkudata(L,1,IPLIMAGE_NAME);
  if (obj->dealloc_data)
    cvReleaseImage(&obj->data);

  return 0;
}

static  int IplImage_nnSize(lua_State *L)
{
    const char f_msg[]=IPLIMAGE_NAME".nSize=int";
    checkIplImage(L,1)->nSize=checkint(L,3);
    return 0;
}

static  int IplImage_nSize(lua_State *L)
{
    lua_pushnumber(L,checkIplImage(L,1)->nSize);
    return 1;
}

static  int IplImage_ID(lua_State *L)
{
    lua_pushnumber(L,checkIplImage(L,1)->ID);
    return 1;
}

static  int IplImage_nID(lua_State *L)
{
    const char f_msg[]=IPLIMAGE_NAME".ID=int";
    checkIplImage(L,1)->ID=checkint(L,3);
    return 0;
}

static  int IplImage_nChannels(lua_State *L)
{
    lua_pushnumber(L,checkIplImage(L,1)->nChannels);
    return 1;
}

static  int IplImage_nnChannels(lua_State *L)
{
    const char f_msg[]=IPLIMAGE_NAME".nChannels=int";
    checkIplImage(L,1)->nChannels=checkint(L,3);
    return 0;
}

static  int IplImage_depth(lua_State *L)
{
    lua_pushnumber(L,checkIplImage(L,1)->depth);
    return 1;
}

static  int IplImage_ndepth(lua_State *L)
{
    const char f_msg[]=IPLIMAGE_NAME".depth=int";
    checkIplImage(L,1)->depth=checkint(L,3);
    return 0;
}

static  int IplImage_dataOrder(lua_State *L)
{
    lua_pushnumber(L,checkIplImage(L,1)->dataOrder);
    return 1;
}

static  int IplImage_ndataOrder(lua_State *L)
{
    const char f_msg[]=IPLIMAGE_NAME".dataOrder=int";
    checkIplImage(L,1)->dataOrder=checkint(L,3);
    return 0;
}

static  int IplImage_origin(lua_State *L)
{
    lua_pushnumber(L,checkIplImage(L,1)->origin);
    return 1;
}

static  int IplImage_norigin(lua_State *L)
{
    const char f_msg[]=IPLIMAGE_NAME".origin=int";
    checkIplImage(L,1)->origin=checkint(L,3);
    return 0;
}

static int IplImage_align(lua_State *L)
{
    lua_pushnumber(L,checkIplImage(L,1)->align);
    return 1;
}

static int IplImage_nalign(lua_State *L)
{
    const char f_msg[]=IPLIMAGE_NAME".align=int";
    checkIplImage(L,1)->align=checkint(L,3);
    return 0;
}

static int IplImage_width(lua_State *L)
{
    lua_pushnumber(L,checkIplImage(L,1)->width);
    return 1;
}

static int IplImage_nwidth(lua_State *L)
{
    const char f_msg[]=IPLIMAGE_NAME".width=int";
    checkIplImage(L,1)->width=checkint(L,3);
    return 0;
}

static int IplImage_height(lua_State *L)
{
    lua_pushnumber(L,checkIplImage(L,1)->height);
    return 1;
}

static int IplImage_nheight(lua_State *L)
{
    const char f_msg[]=IPLIMAGE_NAME".height=int";
    checkIplImage(L,1)->height=checkint(L,3);
    return 0;
}

static int IplImage_roi(lua_State *L)
{
    IplROI *roi=checkIplImage(L,1)->roi;
    pushIplROI(L,roi);
    return 1;
}

static int IplImage_nroi(lua_State *L)
{
    //const char f_msg[]=IPLIMAGE_NAME".roi="IPLROI_NAME;
    checkIplImage(L,1)->roi=checkIplROI(L,3);
    return 0;
}

static int IplImage_imageSize(lua_State *L)
{
    lua_pushnumber(L,checkIplImage(L,1)->imageSize);
    return 1;
}

static int IplImage_nimageSize(lua_State *L)
{
    const char f_msg[]=IPLIMAGE_NAME".imageSize=int";
    checkIplImage(L,1)->imageSize=checkint(L,3);
    return 0;
}

static int IplImage_imageData(lua_State *L)
{
  UNIMPL(L);
#if 0
  IplImage *image=checkIplImage(L,1);
  if (!CV_IS_IMAGE(image)) 
  {
    lua_pushnil(L);
    return 1;
  }
  lua_newtable(L);
  const size_t len=image->width*image->height;
  for(size_t i=0;i<len;i++)
  {
    lua_pushnumber(L,(lua_Number)image->imageData[i]);
    lua_rawseti(L,-2,i+1);
  }
#endif
  return 1;
}


static int IplImage_nimageData(lua_State *L)
{
    UNIMPL(L);
   #if 0
    const char f_msg[]=IPLIMAGE_NAME".imageData=int[]";
    IplImage *image=checkIplImage(L,1);
    size_t len;
    if ((!lua_istable(L,3))||(!(len=lua_objlen(L,3)))) luaL_error(L,"You are trying to rewrite image data by non-matrix data type or blank matrix.");
    if (len!=((image->height)*(image->width))) luaL_error(L,"Matrix length don't match with imageData.");
    if (!image->imageData)
    {
      image->imageData=(char*)lua_newuserdata(L,sizeof(char)*image->height*image->width);
      lua_pop(L,1);
    }
    for (size_t i=0;i<len;i++) 
    {
      lua_rawgeti(L,3,i+1);
      image->imageData[i]=checkint(L,4);
      lua_pop(L,1);
    }
#endif
    return 0;
}

static int IplImage_widthStep(lua_State *L)
{
    lua_pushnumber(L,checkIplImage(L,1)->widthStep);
    return 1;
}

static int IplImage_nwidthStep(lua_State *L)
{
    const char f_msg[]=IPLIMAGE_NAME".widthStep=int";
    checkIplImage(L,1)->widthStep=checkint(L,3);
    return 0;
}

static const luacv_method IplImage_v[]=
{
  {NULL,14,NULL,NULL},
  methodReg(ID,IplImage),
  methodReg(align,IplImage),
  methodReg(depth,IplImage),
  methodReg(dataOrder,IplImage),
  methodReg(height,IplImage),
  methodReg(imageData,IplImage),
  methodReg(imageSize,IplImage),
  methodReg(nSize,IplImage),
  methodReg(nChannels,IplImage),
  methodReg(origin,IplImage),
  methodReg(roi,IplImage),
  methodReg(widthStep,IplImage),
  methodReg(width,IplImage)
};

makeIndexFunctions(IplImage)
makeObjectCallback(IplImage);
