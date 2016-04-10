#include "lua_features2d.h"

static int luacv_cvSURFPoint(lua_State *L)
{  
  const char f_msg[]=CVSURFPOINT_NAME" SURFPoint("CVPOINT2D32F_NAME" pt, int laplacian, int size, num dir=0, float hessian=0)";
  float dir=0, hessian=0;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            dir=checknumber(L,4);
            break;
    case 5:
            dir=checknumber(L,4);
            hessian=checknumber(L,5);
            break;
    default:
            luaL_error(L,f_msg);
  }
  CvSURFPoint p=cvSURFPoint(*checkCvPoint2D32f(L,1),checkint(L,2),checkint(L,3),dir,hessian);
  pushCvSURFPoint(L,&p); 
  return 1;
}

static int luacv_cvSURFParams(lua_State *L)
{
const char f_msg[]=CVSURFPARAMS_NAME" SURFParams(num hessianThreshild, int extended=0)";
  lua_Integer ext=0;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            ext=checkint(L,2);
            break;
    default:
            luaL_error(L,f_msg);
  }
  CvSURFParams p=cvSURFParams(checknumber(L,1),ext);
  pushCvSURFParams(L,&p); 
  return 1;
}

static int luacv_cvExtractSURF(lua_State *L)
{//zmena
  //const char f_msg[]="SURFParams("CVARR_NAME" img, "CVARR_NAME" mask, "CVSEQ_NAME" keypoints, "CVSEQ_NAME" descriptors, "CVMEMSTORAGE_NAME" storage, "CVSURFPARAMS_NAME" params, int useProvidedKeyPts=0)";
  const char f_msg[]=CVSEQ_NAME", "CVSEQ_NAME" SURFParams("CVARR_NAME" img, "CVARR_NAME" mask, "CVMEMSTORAGE_NAME" storage, "CVSURFPARAMS_NAME" params, int useProvidedKeyPts=0)";

  lua_Integer key=0;
  switch(lua_gettop(L))
  {
    case 4:
            break;
    case 5:
            key=checkint(L,5);
            break;
    default:
            luaL_error(L,f_msg);
  }

  CvSeq *keypoints,*descriptors;
  cvExtractSURF(checkCvArr(L,1),checkCvArr(L,2),&keypoints,&descriptors,checkCvMemStorage(L,3),*checkCvSURFParams(L,4),key);
  pushCvSeq(L,keypoints);
  pushCvSeq(L,descriptors);
  return 2;
}

/*
static int luacv_cvMSERParams(lua_State *L)
{
  const char f_msg[]=CVMSERPARAMS_NAME" MSERParams(int delta=5, int min_area=60, int max_area=14400, num max_variation=0.25, num min_diversity=0.2, int max_evolution=200, num area_threhold=1.01, num min_margin=0.003, int edge_blur_size=5)";

  lua_Integer delta=5,min_area=60,max_area=14400,max_evolution=200,edge_blur=5;
  lua_Number max_variation=0.25,min_diversity=0.2,area_threshold=1.01,min_margin=0.003;
  switch(lua_gettop(L))
  {
    case 0:
            break;
    case 1:
            delta=checkint(L,1);
            break;
    case 2:
            delta=checkint(L,1);
            min_area=checkint(L,2);
            break;
    case 3:
            delta=checkint(L,1);
            min_area=checkint(L,2);
            max_area=checkint(L,3);
            break;
    case 4:
            delta=checkint(L,1);
            min_area=checkint(L,2);
            max_area=checkint(L,3);
            max_variation=(float)checknumber(L,4);
            break;
    case 5:
            delta=checkint(L,1);
            min_area=checkint(L,2);
            max_area=checkint(L,3);
            max_variation=(float)checknumber(L,4);
            min_diversity=(float)checknumber(L,5);
            break;
    case 6:
            delta=checkint(L,1);
            min_area=checkint(L,2);
            max_area=checkint(L,3);
            max_variation=(float)checknumber(L,4);
            min_diversity=(float)checknumber(L,5);
            max_evolution=checkint(L,6);
            break;
    case 7:
            delta=checkint(L,1);
            min_area=checkint(L,2);
            max_area=checkint(L,3);
            max_variation=(float)checknumber(L,4);
            min_diversity=(float)checknumber(L,5);
            max_evolution=checkint(L,6);
            area_threshold=checknumber(L,7);
            break;
    case 8:
            delta=checkint(L,1);
            min_area=checkint(L,2);
            max_area=checkint(L,3);
            max_variation=(float)checknumber(L,4);
            min_diversity=(float)checknumber(L,5);
            max_evolution=checkint(L,6);
            area_threshold=checknumber(L,7);
            min_margin=checknumber(L,8);
            break;
    case 9:
            delta=checkint(L,1);
            min_area=checkint(L,2);
            max_area=checkint(L,3);
            max_variation=(float)checknumber(L,4);
            min_diversity=(float)checknumber(L,5);
            max_evolution=checkint(L,6);
            area_threshold=checknumber(L,7);
            min_margin=checknumber(L,8);
            edge_blur=checkint(L,9);
            break;
    default:
            luaL_error(L,f_msg);
  }

  CvMSERParams p=cvMSERParams(delta,min_area,max_area,max_variation,min_diversity,max_evolution,area_threshold,min_margin,edge_blur);
  pushCvMSERParams(L,&p);
  return 1;
}
*/

/*
static int luacv_cvExtractMSER(lua_State *L)
{//zmena
  const char f_msg[]=CVSEQ_NAME" ExtractMSER("CVARR_NAME" img, "CVARR_NAME" mask, "CVMEMSTORAGE_NAME" storage, "CVMSERPARAMS_NAME" params)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  CvSeq *seq;
  cvExtractMSER(checkCvArr(L,1),checkCvArr(L,2),&seq,checkCvMemStorage(L,3),*checkCvMSERParams(L,4));
  pushCvSeq(L,seq);   
  return 1;
}
*/

static int luacv_cvStarKeypoint(lua_State *L)
{
  const char f_msg[]=CVSTARKEYPOINT_NAME" StarKeypoint("CVPOINT_NAME" pt, int size, num response)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  CvStarKeypoint p=cvStarKeypoint(*checkCvPoint(L,1),checkint(L,2),checknumber(L,3));
  pushCvStarKeypoint(L,&p);
  return 1;
}

static int luacv_cvStarDetectorParams(lua_State *L)
{
  const char f_msg[]=CVSTARDETECTORPARAMS_NAME" StarDetectorParams(int maxSize=45, int responseThreshold=30, int lineThresholdProjected=10, int lineThresholdBinarized=8, int suppressNonmaxSize=5)";

  lua_Integer maxSize=45, response=30, projected=10,binarized=8,suppress=5; 
  switch(lua_gettop(L))
  {
    case 0:
            break;
    case 1:
            maxSize=checkint(L,1); 
            break;
    case 2:
            maxSize=checkint(L,1); 
            response=checkint(L,2);
            break;
    case 3:
            maxSize=checkint(L,1); 
            response=checkint(L,2);
            projected=checkint(L,3);
            break;
    case 4:
            maxSize=checkint(L,1); 
            response=checkint(L,2);
            projected=checkint(L,3);
            binarized=checkint(L,4);
            break;
    case 5:
            maxSize=checkint(L,1); 
            response=checkint(L,2);
            projected=checkint(L,3);
            binarized=checkint(L,4);
            suppress=checkint(L,5);
            break;
     default:
            luaL_error(L,f_msg);
  }

  CvStarDetectorParams p=cvStarDetectorParams(maxSize,response,projected,binarized,suppress);
  pushCvStarDetectorParams(L,&p);
  return 1;
}

static int luacv_cvGetStarKeypoints(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" GetStarKeypoints("CVARR_NAME" img, "CVMEMSTORAGE_NAME" storage, "CVSTARDETECTORPARAMS_NAME" params=cv.StarDetectorParams())";
  CvStarDetectorParams p=cvStarDetectorParams();
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            p=*checkCvStarDetectorParams(L,3);
            break;
     default:
            luaL_error(L,f_msg);
  }

  pushCvSeq(L,cvGetStarKeypoints(checkCvArr(L,1),checkCvMemStorage(L,2),p));
  return 1;
}




const luaL_Reg features2d[]=
{
  funcReg(SURFPoint),         funcReg(SURFParams), //     funcReg(MSERParams),
  funcReg(ExtractSURF),      /* funcReg(ExtractMSER),*/     funcReg(StarKeypoint),
  funcReg(StarDetectorParams),funcReg(GetStarKeypoints),
  {NULL,NULL}
};

const luacv_var features2d_object[]=
{
  {CVSURFPOINT_NAME,sizeof(CvSURFPoint)},
  {CVSURFPARAMS_NAME,sizeof(CvSURFParams)},
 // {CVMSERPARAMS_NAME,sizeof(CvMSERParams)},
  {CVSTARKEYPOINT_NAME,sizeof(CvStarKeypoint)},
  {CVSTARDETECTORPARAMS_NAME,sizeof(CvStarDetectorParams)},
  {NULL,0}
};

const luacv_var features2d_var[]=
{
    {NULL,0}
};
