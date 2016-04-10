#include "opencv2/opencv.hpp"
#include "luacvaux.h"

#include "CvRNG.h"
#include "CvArr.h"
#include "IplTileInfo.h"
#include "IplROI.h"
#include "IplImage.h"
#include "IplConvKernel.h"
#include "IplConvKernelFP.h"
#include "CvMat.h" //neni komplet
#include "CvMatND.h" //neni komplet
#include "CvSparseMat.h" //neni komplet
#include "CvSparseNode.h" //neni komplet
#include "CvSparseMatIterator.h" //neni komplet
#include "CvHistogram.h" //neni komplet
#include "CvRect.h"
#include "CvTermCriteria.h"
#include "CvPoint.h"
#include "CvPoint2D32f.h"
#include "CvPoint3D32f.h"
#include "CvPoint2D64f.h"
#include "CvPoint3D64f.h"
#include "CvSize.h"
#include "CvSize2D32f.h"
#include "CvBox2D.h"
#include "CvLineIterator.h"
#include "CvSlice.h"
#include "CvScalar.h"
#include "CvMemBlock.h"
#include "CvMemStorage.h"
#include "CvMemStoragePos.h"
#include "CvSeqBlock.h"
#include "CvSeq.h"
#include "CvSetElem.h"
#include "CvSet.h"
#include "CvGraphEdge.h"
#include "CvGraphVtx.h"
#include "CvGraphVtx2D.h"
#include "CvGraph.h"
#include "CvChain.h"
#include "CvContour.h"
#include "CvSeqWriter.h"
#include "CvSeqReader.h"
#include "CvNArrayIterator.h"
#include "CvFont.h"
#include "CvTreeNodeIterator.h"
#include "CvGraphScanner.h"

#define coreReg()\
  objectReg(CVNARRAYITERATOR_NAME,CvNArrayIterator_m);\
  objectReg(CVFONT_NAME,CvFont_m);\
  objectReg(CVTREENODEITERATOR_NAME,CvTreeNodeIterator_m);\
  objectReg(CVRNG_NAME,CvRNG_m);\
  objectReg(IPLIMAGE_NAME,IplImage_m);\
  objectReg(IPLROI_NAME,IplROI_m);\
  objectReg(IPLCONVKERNEL_NAME,IplConvKernel_m);\
  objectReg(IPLCONVKERNELFP_NAME,IplConvKernelFP_m);\
  objectReg(CVMAT_NAME,CvMat_m);\
  objectReg(CVMATND_NAME,CvMatND_m);\
  objectReg(CVSPARSEMAT_NAME,CvSparseMat_m);\
  objectReg(CVSPARSENODE_NAME,CvSparseNode_m);\
  objectReg(CVSPARSEMATITERATOR_NAME,CvSparseMatIterator_m);\
  objectReg(CVHISTOGRAM_NAME,CvHistogram_m);\
  objectReg(CVRECT_NAME,CvRect_m);\
  objectReg(CVTERMCRITERIA_NAME,CvTermCriteria_m);\
  objectReg(CVPOINT_NAME,CvPoint_m);\
  objectReg(CVPOINT2D32F_NAME,CvPoint2D32f_m);\
  objectReg(CVPOINT3D32F_NAME,CvPoint3D32f_m);\
  objectReg(CVPOINT2D64F_NAME,CvPoint2D64f_m);\
  objectReg(CVPOINT3D64F_NAME,CvPoint3D64f_m);\
  objectReg(CVSIZE_NAME,CvSize_m);\
  objectReg(CVSIZE2D32F_NAME,CvSize2D32f_m);\
  objectReg(CVBOX2D_NAME,CvBox2D_m);\
  objectReg(CVLINEITERATOR_NAME,CvLineIterator_m);\
  objectReg(CVSLICE_NAME,CvSlice_m);\
  objectReg(CVSCALAR_NAME,CvScalar_m);\
  objectReg(CVMEMBLOCK_NAME,CvMemBlock_m);\
  objectReg(CVMEMSTORAGE_NAME,CvMemStorage_m);\
  objectReg(CVMEMSTORAGEPOS_NAME,CvMemStoragePos_m);\
  objectReg(CVSEQBLOCK_NAME,CvSeqBlock_m);\
  objectReg(CVSEQ_NAME,CvSeq_m);\
  objectReg(CVSETELEM_NAME,CvSetElem_m);\
  objectReg(CVSET_NAME,CvSet_m);\
  objectReg(CVGRAPHEDGE_NAME,CvGraphEdge_m);\
  objectReg(CVGRAPHVTX_NAME,CvGraphVtx_m);\
  objectReg(CVGRAPHVTX2D_NAME,CvGraphVtx2D_m);\
  objectReg(CVGRAPH_NAME,CvGraph_m);\
  objectReg(CVCHAIN_NAME,CvChain_m);\
  objectReg(CVCONTOUR_NAME,CvContour_m);\
  objectReg(CVSEQWRITER_NAME,CvSeqWriter_m);\
  objectReg(CVSEQREADER_NAME,CvSeqReader_m);\
  objectReg(CVGRAPHSCANNER_NAME,CvGraphScanner_m);

extern const luaL_ConvFunc core_matconv[];
extern const luaL_ConvFunc core_imageconv[];
extern const luacv_var core_object[];
extern const luaL_Reg core[];
extern const luacv_var core_var[];

