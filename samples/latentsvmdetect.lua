#!/usr/bin/env lua
cv=require('luacv')
model_filename = "cat.xml";
image_filename = "cat.jpg";

function detect_and_draw_objects( image, detector, numThreads )
  storage = cv.CreateMemStorage(0);
  start = 0
  finish = 0;

  start = cv.GetTickCount();
  detections = cv.LatentSvmDetectObjects(image, detector, storage, 0.5, numThreads);
  finish = cv.GetTickCount();
  print("detection time = ", (finish - start) / (cv.GetTickFrequency() * 1000000));

  for i=0,detections.total-1 do
    detection = cv.GetSeqElem( detections, i,cv.CvObjectDetection["name"] );
    bounding_box = detection.rect;
    cv.Rectangle( image, cv.Point(bounding_box.x, bounding_box.y),
    cv.Point(bounding_box.x + bounding_box.width, 
    bounding_box.y + bounding_box.height),
    cv. CV_RGB(255,0,0), 3 );
  end
  cv.ReleaseMemStorage( storage );
end

if (#arg ==2 ) then
  image_filename = arg[1];
  model_filename = arg[2];
end

image = cv.LoadImage(image_filename) or error("Unable to load the image");

detector = cv.LoadLatentSvmDetector(model_filename);

detect_and_draw_objects( image, detector, -1 );
cv.NamedWindow( "test", 0 );
cv.ShowImage( "test", image );
cv.WaitKey(0);
cv.ReleaseLatentSvmDetector(detector );
cv.ReleaseImage(image );
cv.DestroyAllWindows();
