#include "lua_highgui.h"

#include <iostream>
#include "opencv2/gpu/gpu.hpp"

using namespace std;
using namespace cv;

static int luacv_cvInitSystem(lua_State *L)
{
  const char f_msg[]="int InitSystem(int argc, string[] argv)";
  const int len=lua_objlen(L,2);
  char **argv=NULL; 
  if ((lua_gettop(L)!=2)||(!lua_istable(L,2))) luaL_error(L,f_msg);
  for(int i=1;i<=len;i++)
  {
    lua_rawgeti(L,2,i);
    argv[(i-1)]=(char*)checkstring(L,3);
    lua_pop(L,1);
  }
  lua_pushnumber(L,cvInitSystem(checkint(L,1),argv));
  return 1;
}

static int luacv_cvNamedWindow(lua_State *L)
{
  const char f_msg[]="int NamedWindow(string name, int flags=CV_WINDOW_AUTOSIZE)";
  int flags=CV_WINDOW_AUTOSIZE;
  switch(lua_gettop(L))
  {
    case 1:
      break;
    case 2:
      flags=checkint(L,2);
      break;
    default:
      luaL_error(L,f_msg);
  }
  lua_pushnumber(L,cvNamedWindow(checkstring(L,1),flags));
  return 1;
}

static int luacv_cvShowImage(lua_State *L)
{
  const char f_msg[]="ShowImage(string name, "CVARR_NAME" image)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvShowImage(checkstring(L,1),checkCvArr(L,2));
  return 0;
}

static int luacv_cvMoveWindow(lua_State *L)
{
  const char f_msg[]="MoveWindow(string name,int x, int y)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvMoveWindow(checkstring(L,1),checkint(L,2),checkint(L,3));
  return 0;
}

static int luacv_cvDestroyWindow(lua_State *L)
{
  const char f_msg[]="DestroyWindow(string name)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  const char *name=checkstring(L,1);
  cvDestroyWindow(name); 

  lua_getglobal(L,"package");
  lua_getfield(L,-1,"loaded");
  lua_getfield(L,-1,LIBNAME);
  lua_getfield(L,-1,LIBCALLBACKS);
  int len=lua_objlen(L,-1);
  luacv_callback *call=callbackTable[0];

  for(int i=0;i<len;i++)
    if (call!=NULL)
      if (!strcmp(call->wname,name))
      {
        luacv_free(&call); 
        break;
      }

  return 0;
}
static int luacv_cvDestroyAllWindows(lua_State *L)
{
  const char f_msg[]="DestroyAllWindows()";
  if (lua_gettop(L)!=0) luaL_error(L,f_msg);
  cvDestroyAllWindows(); 

  lua_getglobal(L,"package");
  lua_getfield(L,-1,"loaded");
  lua_getfield(L,-1,LIBNAME);
  lua_getfield(L,-1,LIBCALLBACKS);
  int len=lua_objlen(L,-1);

  luacv_callback *call=callbackTable[0];
  for(int i=0;i<len;i++)
    luacv_free(&call); 

  return 0;
}

static int luacv_cvGetWindowHandle(lua_State *L)
{
  const char f_msg[]="userdata GetWindowHandle(string name)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  pushUserData(L,cvGetWindowHandle(checkstring(L,1))); 
  return 1;
}

static int luacv_cvGetTrackbarPos(lua_State *L)
{
  const char f_msg[]="int GetTrackbarPos(string trackbar_name, string window_name)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGetTrackbarPos(checkstring(L,1),checkstring(L,2))); 
  return 1;
}

static int luacv_cvSetTrackbarPos(lua_State *L)
{
  const char f_msg[]="SetTrackbarPos(string trackbar_name, string window_name, int pos)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvSetTrackbarPos(checkstring(L,1),checkstring(L,2),checkint(L,3)); 
  return 0;
}

static int luacv_cvLoadImage(lua_State *L)
{
  const char f_msg[]=IPLIMAGE_NAME" LoadImage(string filename, int iscolor=CV_LOAD_IMAGE_COLOR)";
  int iscolor=CV_LOAD_IMAGE_COLOR;
  switch(lua_gettop(L))
  {
    case 1:
      break;
    case 2:
      iscolor=checkint(L,2);
      break;
    default:
      luaL_error(L,f_msg);
  }
  pushIplImage(L,cvLoadImage(checkstring(L,1),iscolor));
  return 1;
} 

static int luacv_cvLoadImageM(lua_State *L)
{
  const char f_msg[]=CVMAT_NAME" LoadImageM(string filename, int iscolor=CV_LOAD_IMAGE_COLOR)";
  int iscolor=CV_LOAD_IMAGE_COLOR;
  switch(lua_gettop(L))
  {
    case 1:
      break;
    case 2:
      iscolor=checkint(L,2);
      break;
    default:
      luaL_error(L,f_msg);
  }
  pushCvMat(L,cvLoadImageM(checkstring(L,1),iscolor));
  return 1;
}

static int luacv_cvSaveImage(lua_State *L)
{
  const char f_msg[]="int SaveImage(string filename, "CVARR_NAME" image, int[] params)";
  const int top=lua_gettop(L);
  int *params=NULL,len=0,t_params[LUAI_MAXVARS];
  switch(top)
  {
    case 2:
      break;
    case 3:
      if ((lua_istable(L,3))&&((len=lua_objlen(L,3))!=0))
      {
        for(int i=1;i<=len;i++)
        {
          lua_rawgeti(L,3,i);      
          t_params[i-1]=checkint(L,top+1);
          lua_pop(L,1);
        }
        params=(int *)&t_params;
        break;
      }
    default:
      luaL_error(L,f_msg);
  }
  lua_pushnumber(L,cvSaveImage(checkstring(L,1),checkCvArr(L,2),params));
  return 1;
}

static int luacv_cvGetCudaEnabledDeviceCount(lua_State *L)
{
  lua_pushnumber(L,cv::gpu::getCudaEnabledDeviceCount());
  return 1;
}

  static int
luacv_cvGpuGetBlob(lua_State *L)
{
  luaL_Buffer b;

  const char f_msg[]="int GpuGetBlob(string path, int width, int height, int interpolation=CV_INTER_LINEAR)";
  int inter=CV_INTER_LINEAR;
  switch(lua_gettop(L))
  {
    case 3:
      break;
    case 4:
      inter=checkint(L,4);
      break;
    default:
      luaL_error(L,f_msg);
  }
  // lua_pushnumber(L,cvSaveImage(checkstring(L,1),checkCvArr(L,2),params));

  // cv::Mat **m = (cv::Mat **) lua_touserdata(L,1);
  // if (m == NULL) {
  //     return 0;
  // }

  int width = checkint(L,2);
  int height = checkint(L,3);

  const cv::Mat src_ = cv::imread(checkstring(L,1));
  //int iscolor=CV_LOAD_IMAGE_COLOR;
  //const cv::Mat m = cvLoadImageM(checkstring(L,1),iscolor);
  cv::vector<uchar> buf;

  // IplImage* img = cvLoadImage(checkstring(L,1), iscolor);
  cv::Mat dst_(width,height, CV_8UC3, cv::Scalar(0,0,255));

  cv::gpu::GpuMat src(src_);
  cv::gpu::GpuMat dst(dst_);

  cv::gpu::resize(src, dst, cv::Size(width, height), 0, 0, inter);

  cv::Mat final(dst);

  cv::imencode(".png", final, buf);

  luaL_buffinit(L, &b);
  luaL_addlstring(&b, (const char*) &buf[0], buf.size());
  cout << buf.size() << endl;
  luaL_pushresult(&b);
  return 1;
}

  static int
luacv_cvGetBlob(lua_State *L)
{
  luaL_Buffer b;

  const char f_msg[]="int GetBlob(string path, int width, int height, int interpolation=CV_INTER_LINEAR)";
  int inter=CV_INTER_LINEAR;
  switch(lua_gettop(L))
  {
    case 3:
      break;
    case 4:
      inter=checkint(L,4);
      break;
    default:
      luaL_error(L,f_msg);
  }
  // lua_pushnumber(L,cvSaveImage(checkstring(L,1),checkCvArr(L,2),params));

  // cv::Mat **m = (cv::Mat **) lua_touserdata(L,1);
  // if (m == NULL) {
  //     return 0;
  // }

  int width = checkint(L,2);
  int height = checkint(L,3);

  const cv::Mat m = cv::imread(checkstring(L,1));
  //int iscolor=CV_LOAD_IMAGE_COLOR;
  //const cv::Mat m = cvLoadImageM(checkstring(L,1),iscolor);
  cv::vector<uchar> buf;

  // IplImage* img = cvLoadImage(checkstring(L,1), iscolor);

  cv::Mat u(width,height, CV_8UC3, cv::Scalar(0,0,255));
  resize(m,u,cv::Size(width, height),0,0, inter);
  cv::imencode(".png", u, buf);

  luaL_buffinit(L, &b);
  luaL_addlstring(&b, (const char*) &buf[0], buf.size());
  cout << buf.size() << endl;
  luaL_pushresult(&b);
  return 1;
}


void overlayImage(const cv::Mat &background, const cv::Mat &foreground, 
  cv::Mat &output, cv::Point2i location)
{
  background.copyTo(output);


  // start at the row indicated by location, or at row 0 if location.y is negative.
  for(int y = std::max(location.y , 0); y < background.rows; ++y)
  {
    int fY = y - location.y; // because of the translation

    // we are done of we have processed all rows of the foreground image.
    if(fY >= foreground.rows)
      break;

    // start at the column indicated by location, 

    // or at column 0 if location.x is negative.
    for(int x = std::max(location.x, 0); x < background.cols; ++x)
    {
      int fX = x - location.x; // because of the translation.

      // we are done with this row if the column is outside of the foreground image.
      if(fX >= foreground.cols)
        break;

      // determine the opacity of the foregrond pixel, using its fourth (alpha) channel.
      double opacity =
        ((double)foreground.data[fY * foreground.step + fX * foreground.channels() + 3])

        / 255.;


      // and now combine the background and foreground pixel, using the opacity, 

      // but only if opacity > 0.
      for(int c = 0; opacity > 0 && c < output.channels(); ++c)
      {
        unsigned char foregroundPx =
          foreground.data[fY * foreground.step + fX * foreground.channels() + c];
        unsigned char backgroundPx =
          background.data[y * background.step + x * background.channels() + c];
        output.data[y*output.step + output.channels()*x + c] =
          backgroundPx * (1.-opacity) + foregroundPx * opacity;
      }
    }
  }
}


static int luacv_cvMergeImages(lua_State *L)
{
  const char f_msg[]="int MergeImages(string file1, string file2)";
  switch(lua_gettop(L))
  {
    case 2:
      break;
    default:
      luaL_error(L,f_msg);
  }

  const char *file1 = checkstring(L,1);
  const char *file2 = checkstring(L,2);

  Mat background = imread(file1);
  Mat foreground = imread(file2, -1);
  Mat result;

  overlayImage(background, foreground, result, cv::Point(0,0));

  // C: void cvAddWeighted(const CvArr* src1, double alpha, const CvArr* src2, double beta, double gamma, CvArr* dst)
  // addWeighted(img1, 0.5, img2, 0.5, 0.0, dst);


  luaL_Buffer b;
  cv::vector<uchar> buf;
  cv::imencode(".png", result, buf);

  luaL_buffinit(L, &b);
  luaL_addlstring(&b, (const char*) &buf[0], buf.size());
  luaL_pushresult(&b);
  return 1;
}

  static int
luacv_cvScaleData(lua_State *L)
{
  luaL_Buffer b;

  const char f_msg[]="int ScaleData(string data, int data_length, int width, int height, int interpolation, string extension, int png_compression, int jpeg_compression)";
  switch(lua_gettop(L))
  {
    case 8:
      break;
    default:
      luaL_error(L,f_msg);
  }

  const char *contents = checkstring(L,1);
  int data_length = checkint(L,2);
  int width = checkint(L,3);
  int height = checkint(L,4);
  int inter = checkint(L,5);
  const char *extension = checkstring(L,6);
  int png_compression = checkint(L,7);
  int jpeg_compression = checkint(L,8);
  // check the type of a passed-in value like this:
  // cout << "type: " << lua_type(L,1) << endl;
  std::vector<char> data(data_length);
  for (int i=0; i < data_length; i++) {
    data[i] = contents[i];
  }
  // std::vector<char> * data = (std::vector<char> *) lua_touserdata(L,1);
  // std::string cppContents(contents, data_length);
  // cout << cppContents << endl;

  cv::Mat m = cv::imdecode(data, CV_LOAD_IMAGE_COLOR);

  // cv::Mat m(816, 650, CV_8UC3, lua_touserdata(L,1), cv::Mat::AUTO_STEP);

  // const cv::Mat m = cv::imread(checkstring(L,1));
  //int iscolor=CV_LOAD_IMAGE_COLOR;
  //const cv::Mat m = cvLoadImageM(checkstring(L,1),iscolor);
  cv::vector<uchar> buf;

  // IplImage* img = cvLoadImage(checkstring(L,1), iscolor);

  cv::vector<int> params;
  params.push_back(CV_IMWRITE_PNG_COMPRESSION);
  params.push_back(png_compression);
  params.push_back(CV_IMWRITE_JPEG_QUALITY);
  params.push_back(jpeg_compression);
  cv::Mat u(width,height, CV_8UC3, cv::Scalar(0,0,255));
  resize(m,u,cv::Size(width, height),0,0, inter);
  cv::imencode(extension, u, buf, params);

  luaL_buffinit(L, &b);
  luaL_addlstring(&b, (const char*) &buf[0], buf.size());
  cout << buf.size() << endl;
  luaL_pushresult(&b);
  return 1;
}



static int luacv_cvDecodeImage(lua_State *L)
{
  const char f_msg[]=IPLIMAGE_NAME" DecodeImage("CVMAT_NAME" buf, int iscolor=CV_LOAD_IMAGE_COLOR)";
  int iscolor=CV_LOAD_IMAGE_COLOR;
  switch(lua_gettop(L))
  {
    case 1:
      break;
    case 2:
      iscolor=checkint(L,2);
      break;
    default:
      luaL_error(L,f_msg);
  }
  pushIplImage(L,cvDecodeImage(checkCvMat(L,1),iscolor));
  return 1;
}

static int luacv_cvDecodeImageM(lua_State *L)
{
  const char f_msg[]=CVMAT_NAME" DecodeImageM("CVMAT_NAME" buf, int iscolor=CV_LOAD_IMAGE_COLOR)";
  int iscolor=CV_LOAD_IMAGE_COLOR;
  switch(lua_gettop(L))
  {
    case 1:
      break;
    case 2:
      iscolor=checkint(L,2);
      break;
    default:
      luaL_error(L,f_msg);
  }
  pushCvMat(L,cvDecodeImageM(checkCvMat(L,1),iscolor));

  return 1;
}
static int luacv_cvConvertImage(lua_State *L)
{
  const char f_msg[]="ConvertImage("CVARR_NAME" src, "CVARR_NAME" dst, int flags=0);";
  int flags=0;
  switch(lua_gettop(L))
  {
    case 1:
      break;
    case 2:
      flags=checkint(L,2);
      break;
    default:
      luaL_error(L,f_msg);
  }
  cvConvertImage(checkCvArr(L,1),checkCvArr(L,2),flags);

  return 0;
}

static int luacv_cvWaitKey(lua_State *L)
{
  const char f_msg[]="int WaitKey(int delay=0)";
  int delay=0;
  switch(lua_gettop(L))
  {
    case 0:
      break;
    case 1:
      delay=checkint(L,1);
      break;
    default:
      luaL_error(L,f_msg);
  }
  lua_pushnumber(L,cvWaitKey(delay));
  return 1;
}

static int luacv_cvCreateFileCapture(lua_State *L)
{
  const char f_msg[]=CVCAPTURE_NAME" CreateFileCapture(string filename)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  pushCvCapture(L,cvCreateFileCapture(checkstring(L,1)));
  return 1;
}

static int luacv_cvCaptureFromFile(lua_State *L)
{
  const char f_msg[]=CVCAPTURE_NAME" CaptureFromFile(string filename)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  pushCvCapture(L,cvCaptureFromFile(checkstring(L,1)));
  return 1;
}

static int luacv_cvCreateCameraCapture(lua_State *L)
{
  const char f_msg[]=CVCAPTURE_NAME" CreateCameraCapture(int index)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  pushCvCapture(L,cvCreateCameraCapture(checkint(L,1)));
  return 1;
}
static int luacv_cvGrabFrame(lua_State *L)
{
  const char f_msg[]="int GrabFram("CVCAPTURE_NAME" capture)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGrabFrame(checkCvCapture(L,1)));
  return 1;
}

static int luacv_cvRetrieveFrame(lua_State *L)
{
  const char f_msg[]=IPLIMAGE_NAME" RetrieveFrame("CVCAPTURE_NAME" capture, int streamIdx=0)";
  int streamIdx=0;
  switch(lua_gettop(L))
  {
    case 1:
      break;
    case 2:
      streamIdx=checkint(L,2);
      break;
    default:
      luaL_error(L,f_msg);
  }
  pushUserData(L,cvRetrieveFrame(checkCvCapture(L,1),streamIdx)); 
  luaL_getmetatable(L,IPLIMAGE_NAME);
  lua_setmetatable(L,-2);


  return 1;
}

static int luacv_cvQueryFrame(lua_State *L)
{
  const char f_msg[]=IPLIMAGE_NAME" QueryFrame("CVCAPTURE_NAME" capture)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  pushUserData(L,cvQueryFrame(checkCvCapture(L,1))); 
  luaL_getmetatable(L,IPLIMAGE_NAME);
  lua_setmetatable(L,-2);

  return 1;
}

static int luacv_cvReleaseCapture(lua_State *L)
{
  const char f_msg[]="ReleaseCapture("CVCAPTURE_NAME" capture)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvCapture(L,1);
  luacv_releaseObject(L,1);
  return 0;
}

static int luacv_cvGetCaptureProperty(lua_State *L)
{
  const char f_msg[]="num GetCaptureProperty("CVCAPTURE_NAME" capture, int property_id)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGetCaptureProperty(checkCvCapture(L,1),checkint(L,2)));
  return 1;
}
static int luacv_cvSetCaptureProperty(lua_State *L)
{
  const char f_msg[]="int SetCaptureProperty("CVCAPTURE_NAME" capture, int property_id, num value)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  lua_pushnumber(L,cvSetCaptureProperty(checkCvCapture(L,1),checkint(L,2),checknumber(L,3)));

  return 1;
}
static int luacv_cvGetCaptureDomain(lua_State *L)
{
  const char f_msg[]="int GetCaptureDomain("CVCAPTURE_NAME" capture)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGetCaptureDomain(checkCvCapture(L,1)));
  return 1;
}

static int luacv_cvWriteFrame(lua_State *L)
{
  const char f_msg[]="int WriteFrame("CVVIDEOWRITER_NAME" writer, "IPLIMAGE_NAME" image)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  lua_pushnumber(L,cvWriteFrame(checkCvVideoWriter(L,1),checkIplImage(L,2)));
  return 1;
}
static int luacv_cvReleaseVideoWriter(lua_State *L)
{
  const char f_msg[]="ReleaseVideoWriter("CVVIDEOWRITER_NAME" writer)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvVideoWriter(L,1);
  luacv_releaseObject(L,1);

  return 0;
}

static int luacv_cvCreateVideoWriter(lua_State *L)
{
  const char f_msg[]=CVVIDEOWRITER_NAME" CreateVideoWriter(string filename, int fourcc, num fps, "CVSIZE_NAME" frame_size, int is_color=1)";
  int is_color=1;
  switch(lua_gettop(L))
  {
    case 4:
      break;
    case 5:
      is_color=checkint(L,5);
      break;
    default:
      luaL_error(L,f_msg);
  }
  pushCvVideoWriter(L,cvCreateVideoWriter(checkstring(L,1),checkint(L,2),checknumber(L,3),*checkCvSize(L,4),is_color));
  return 1;
}

static int luacv_cvCV_FOURCC(lua_State *L)
{
  const char f_msg[]="num CV_FOURCC(string[4] codec)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  char *str=(char*)checkstring(L,1);
  lua_pushnumber(L,CV_FOURCC(str[0],str[1],str[2],str[3]));
  return 1;
}

static int luacv_cvStartWindowThread(lua_State *L)
{
  const char f_msg[]="int StartWindowThread()";
  if (lua_gettop(L)!=0) luaL_error(L,f_msg);
  lua_pushnumber(L,cvStartWindowThread());
  return 1;
}
static int luacv_cvSetWindowProperty(lua_State*L)
{
  const char f_msg[]="SetWindowProperty(string name, int prop_id, num prop_value)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvSetWindowProperty(checkstring(L,1),checkint(L,2),checknumber(L,3)); 
  return 0;
}

static int luacv_cvGetWindowProperty(lua_State*L)
{
  const char f_msg[]="num GetWindowProperty(string name, int prop_id)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGetWindowProperty(checkstring(L,1),checkint(L,2))); 
  return 1;
}

static int luacv_cvResizeWindow(lua_State*L)
{
  const char f_msg[]="ResizeWindow(string name, int width, int height)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvResizeWindow(checkstring(L,1),checkint(L,2),checkint(L,3));
  return 0;
}

static int luacv_cvGetWindowName(lua_State*L)
{
  const char f_msg[]="string GetWindowName(userdata handle)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushstring(L,cvGetWindowName(checkldata(L,1)));
  return 1;
}

void luacv_cvTrackbarCallback(int pos,void *userdata)
{

  luacv_callback *call=(luacv_callback*)userdata;
  lua_State *L=call->stack;
  lua_getglobal(L,"package");
  lua_getfield(L,-1,"loaded");
  lua_remove(L,-2);
  lua_getfield(L,-1,LIBNAME);
  lua_remove(L,-2);
  lua_getfield(L,-1,LIBCALLBACKS);
  lua_remove(L,-2);
  lua_rawgeti(L,-1,call->pos);
  lua_remove(L,-2);
  lua_pushnumber(L,pos);

  lua_call(L,1,0);
}

static int luacv_cvCreateTrackbar(lua_State*L)
{
  const char f_msg[]="int CreateTrackbar(string tbarname, string wname, int count, func on_change(int pos))";
  int len=0;
  const char *name=checkstring(L,2);
  if ((lua_gettop(L)!=4)||(!lua_isfunction(L,4))) luaL_error(L,f_msg);
  lua_getglobal(L,"package");
  lua_getfield(L,-1,"loaded");
  lua_getfield(L,-1,LIBNAME);
  lua_getfield(L,-1,LIBCALLBACKS);

  lua_remove(L,5);
  lua_remove(L,5);

  lua_insert(L,5);
  lua_insert(L,4);
  lua_insert(L,5);

  len=lua_objlen(L,-2); 
  lua_rawseti(L,-2,len+1);
  lua_rawset(L,-2);

  callbackTable[len]=(luacv_callback*)luacv_alloc(sizeof(luacv_callback));
  luacv_callback *call=callbackTable[len];
  call->stack=L;
  call->pos=len+1;
  sprintf(call->wname,"%s",name);

  int val=0;
  lua_pushnumber(L,cvCreateTrackbar2(checkstring(L,1),name,&val,checkint(L,3),luacv_cvTrackbarCallback,call));
  return 1;
}

static int luacv_cvCreateTrackbar2(lua_State*L)
{
  UNIMPL(L);
  return 1;
}

void luacv_cvMouseCallback(int event,int x,int y,int flags, void *param)
{
  luacv_callback *call=(luacv_callback*)param;
  lua_State *L=call->stack;
  lua_getglobal(L,"package");
  lua_getfield(L,-1,"loaded");
  lua_remove(L,-2);
  lua_getfield(L,-1,LIBNAME);
  lua_remove(L,-2);
  lua_getfield(L,-1,LIBCALLBACKS);
  lua_remove(L,-2);
  lua_rawgeti(L,-1,call->pos);
  lua_remove(L,-2);

  lua_pushnumber(L,event);
  lua_pushnumber(L,x);
  lua_pushnumber(L,y);
  lua_pushnumber(L,flags);
  lua_call(L,4,0);
}

static int luacv_cvSetMouseCallback(lua_State*L)
{
  const char f_msg[]="SetMouseCallback(string wname, func on_mouse(int event,int x,int y, int flags)";
  if ((lua_gettop(L)!=2)||(!lua_isfunction(L,2)))  luaL_error(L,f_msg);
  char *wname=(char*)checkstring(L,1);
  lua_getglobal(L,"package");
  lua_getfield(L,-1,"loaded");
  lua_remove(L,-2);
  lua_getfield(L,-1,LIBNAME);
  lua_remove(L,-2);
  lua_getfield(L,-1,LIBCALLBACKS);
  lua_insert(L,2);
  lua_replace(L,1);

  int len=lua_objlen(L,-2); 
  lua_rawseti(L,-2,len+1);
  lua_rawset(L,-2);

  callbackTable[len]=(luacv_callback*)luacv_alloc(sizeof(luacv_callback));
  luacv_callback *call=callbackTable[len];
  call->stack=L;
  call->pos=len+1;
  sprintf(call->wname,"%s",wname);

  cvSetMouseCallback(wname,luacv_cvMouseCallback,call);
  return 0;
}

/*
   static int luacv_cvFontQt(lua_State*L)
   {
   const char f_msg[]=CVFONT_NAME" FontQt(string nameFont, int pointSize=-1,"CVSCALAR_NAME" color=ScalarAll(0), int weight=CV_FONT_NORMAL, int style=CV_STYLE_NORMAL, int spacing=0)";
   int pointSize=-1, weight=CV_FONT_NORMAL, style=CV_STYLE_NORMAL, spacing=0;
   CvScalar color=cvScalarAll(0);

   switch(lua_gettop(L))
   {
   case 1:
   break;
   case 2:
   pointSize=checkint(L,2);
   break;
   case 3:
   pointSize=checkint(L,2);
   color=*checkCvScalar(L,3);
   break;
   case 4:
   pointSize=checkint(L,2);
   color=*checkCvScalar(L,3);
   weight=checkint(L,4);
   break;
   case 5:
   pointSize=checkint(L,2);
   color=*checkCvScalar(L,3);
   weight=checkint(L,4);
   style=checkint(L,5);
   break;
   case 6:
   pointSize=checkint(L,2);
   color=*checkCvScalar(L,3);
   weight=checkint(L,4);
   style=checkint(L,5);
   spacing=checkint(L,6);
   break;

   default:
   luaL_error(L,f_msg);
   }
   CvFont font=cvFontQt(checkstring(L,1),pointSize,color,weight,style,spacing);
   pushCvFont(L,&font);
   return 1;
   }

   static int luacv_cvAddText(lua_State*L)
   {
   const char f_msg[]="AddText("CVARR_NAME"img, string text, "CVPOINT_NAME" org, "CVFONT_NAME" arg2)";
   if (lua_gettop(L)!=4) luaL_error(L,f_msg);
   cvAddText(checkCvArr(L,1),checkstring(L,2),*checkCvPoint(L,3),checkCvFont(L,4));
   return 0;
   }
   static int luacv_cvDisplayOverlay(lua_State*L)
   {
   const char f_msg[]="DisplayOverlay(string name, string text, int delayms)";
   if (lua_gettop(L)!=3) luaL_error(L,f_msg);
   cvDisplayOverlay(checkstring(L,1),checkstring(L,2),checkint(L,3));
   return 0;
   }

   static int luacv_cvDisplayStatusBar(lua_State*L)
   {
   const char f_msg[]="DisplayStatusBar(string name, string text, int delayms)";
   if (lua_gettop(L)!=3) luaL_error(L,f_msg);
   cvDisplayStatusBar(checkstring(L,1),checkstring(L,2),checkint(L,3));
   return 0;
   }
   static int luacv_cvCreateOpenGLCallback(lua_State*L)
   {
   UNIMPL(L);
   return 1;
}

static int luacv_cvSaveWindowParameters(lua_State*L)
{
  const char f_msg[]="SaveWindowParameters(string name)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  cvSaveWindowParameters(checkstring(L,1));
  return 0;
}
static int luacv_cvLoadWindowParameters(lua_State*L)
{
  const char f_msg[]="LoadWindowParameters(string name)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  cvLoadWindowParameters(checkstring(L,1));
  return 0;
}

static int luacv_cvStartLoop(lua_State*L)
{
  UNIMPL(L);
  return 1;
}
static int luacv_cvStopLoop(lua_State*L)
{
  UNIMPL(L);
  return 1;
}
static int luacv_cvCreateButton(lua_State*L)
{
  UNIMPL(L);
  return 1;
}
*/
const luaL_Reg highgui[]=
{
  /*  funcReg(FontQt),          funcReg(AddText),           funcReg(DisplayOverlay),
      funcReg(DisplayStatusBar),  funcReg(CreateOpenGLCallback),funcReg(SaveWindowParameters),
      funcReg(LoadWindowParameters),funcReg(StartLoop),       funcReg(StopLoop),
      funcReg(CreateButton),*/
  funcReg(SetWindowProperty), funcReg(GetWindowProperty), funcReg(ResizeWindow),
  funcReg(GetWindowName),     funcReg(CreateTrackbar),    funcReg(CreateTrackbar2),
  funcReg(SetMouseCallback),  funcReg(StartWindowThread), funcReg(InitSystem),
  funcReg(NamedWindow),       funcReg(ShowImage),         funcReg(MoveWindow),
  funcReg(DestroyWindow),     funcReg(DestroyAllWindows), funcReg(GetWindowHandle),
  funcReg(GetTrackbarPos),    funcReg(SetTrackbarPos),    funcReg(LoadImage),
  funcReg(LoadImageM),        funcReg(SaveImage),         funcReg(DecodeImage), funcReg(GetBlob), funcReg(GpuGetBlob),
  funcReg(GetCudaEnabledDeviceCount), funcReg(ScaleData), funcReg(MergeImages),
  funcReg(DecodeImageM),      funcReg(ConvertImage),      funcReg(WaitKey),
  funcReg(CreateFileCapture), funcReg(CreateCameraCapture),funcReg(GrabFrame),
  funcReg(RetrieveFrame),     funcReg(QueryFrame),        funcReg(ReleaseCapture),
  funcReg(GetCaptureProperty),funcReg(SetCaptureProperty),funcReg(GetCaptureDomain),
  funcReg(WriteFrame),        funcReg(ReleaseVideoWriter),funcReg(CreateVideoWriter),funcReg(CV_FOURCC),
  funcReg(CaptureFromFile),
  {NULL,NULL}
};
luacv_callback *callbackTable[MAX_CALLBACKS];
const luacv_var highgui_var[]=
{
  varReg(CV_WINDOW_AUTOSIZE), varReg(CV_EVENT_MOUSEMOVE), varReg(CV_EVENT_LBUTTONDOWN),
  varReg(CV_EVENT_RBUTTONDOWN),varReg(CV_EVENT_MBUTTONDOWN),varReg(CV_EVENT_LBUTTONUP),
  varReg(CV_EVENT_RBUTTONUP), varReg(CV_EVENT_MBUTTONUP), varReg(CV_EVENT_LBUTTONDBLCLK),
  varReg(CV_EVENT_RBUTTONDBLCLK),varReg(CV_EVENT_MBUTTONDBLCLK),varReg(CV_EVENT_FLAG_LBUTTON),
  varReg(CV_EVENT_FLAG_RBUTTON),varReg(CV_EVENT_FLAG_MBUTTON),varReg(CV_EVENT_FLAG_CTRLKEY),
  varReg(CV_EVENT_FLAG_SHIFTKEY),varReg(CV_EVENT_FLAG_ALTKEY),varReg(CV_LOAD_IMAGE_UNCHANGED),
  varReg(CV_LOAD_IMAGE_GRAYSCALE),varReg(CV_LOAD_IMAGE_COLOR),varReg(CV_LOAD_IMAGE_ANYDEPTH),
  varReg(CV_LOAD_IMAGE_ANYCOLOR),varReg(CV_IMWRITE_JPEG_QUALITY),varReg(CV_IMWRITE_PNG_COMPRESSION),
  varReg(CV_IMWRITE_PXM_BINARY),  varReg(CV_CVTIMG_FLIP),   varReg(CV_CVTIMG_SWAP_RB),
  varReg(CV_CAP_ANY),             varReg(CV_CAP_MIL),       varReg(CV_CAP_VFW),
  varReg(CV_CAP_V4L),             varReg(CV_CAP_V4L2),      varReg(CV_CAP_FIREWARE),
  varReg(CV_CAP_FIREWIRE),        varReg(CV_CAP_IEEE1394),  varReg(CV_CAP_DC1394),
  varReg(CV_CAP_CMU1394),         varReg(CV_CAP_STEREO),    varReg(CV_CAP_TYZX),
  varReg(CV_TYZX_LEFT),           varReg(CV_TYZX_RIGHT),    varReg(CV_TYZX_COLOR),
  varReg(CV_TYZX_Z),              varReg(CV_CAP_QT),        varReg(CV_CAP_UNICAP),
  varReg(CV_CAP_DSHOW),           varReg(CV_CAP_PROP_POS_MSEC),varReg(CV_CAP_PROP_POS_FRAMES),
  varReg(CV_CAP_PROP_POS_AVI_RATIO),varReg(CV_CAP_PROP_FRAME_WIDTH),varReg(CV_CAP_PROP_FRAME_HEIGHT),
  varReg(CV_CAP_PROP_FPS),        varReg(CV_CAP_PROP_FOURCC),varReg(CV_CAP_PROP_FRAME_COUNT),
  varReg(CV_CAP_PROP_FORMAT),     varReg(CV_CAP_PROP_MODE), varReg(CV_CAP_PROP_BRIGHTNESS),
  varReg(CV_CAP_PROP_CONTRAST),   varReg(CV_CAP_PROP_SATURATION),varReg(CV_CAP_PROP_HUE),
  varReg(CV_CAP_PROP_GAIN),       varReg(CV_CAP_PROP_EXPOSURE),varReg(CV_CAP_PROP_CONVERT_RGB),
  //varReg(CV_CAP_PROP_WHITE_BALANCE),
  varReg(CV_CAP_PROP_RECTIFICATION), varReg(CV_FOURCC_PROMPT),varReg(CV_FOURCC_DEFAULT),
  varReg(HG_AUTOSIZE),
  {NULL,0}
};
