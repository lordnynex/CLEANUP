#!/usr/bin/env lua
cv=require('luacv')
A = { 1, 1, 0, 1 }

img = cv.CreateImage( cv.Size(500,500), cv.IPL_DEPTH_8U, 3 )
kalman = cv.CreateKalman( 2, 1, 0 )
state = cv.CreateMat( 2, 1, cv.CV_32FC1 )
process_noise = cv.CreateMat( 2, 1, cv.CV_32FC1 )
measurement = cv.CreateMat( 1, 1, cv.CV_32FC1 )
rng = cv.RNG(-1)
code = -1

cv.Zero( measurement )
cv.NamedWindow( "Kalman", 1 )

function calc_point(angle)                                      
  return cv.Point( cv.Round(img.width/2 + img.width/3*math.cos(angle)), 
  cv.Round(img.height/2 - img.width/3*math.sin(angle)))
end

function draw_cross( center, color, d )                                 
  cv.Line( img, cv.Point( center.x - d, center.y - d ),
  cv.Point( center.x + d, center.y + d ), color, 1, cv.CV_AA, 0)
  cv.Line( img, cv.Point( center.x + d, center.y - d ),cv.Point( center.x - d, center.y + d ), color, 1, cv.CV_AA, 0 )
end

while true do
  cv.RandArr( rng, state, cv.CV_RAND_NORMAL, cv.RealScalar(0), cv.RealScalar(0.1) )

  for i=0,#A-1 do
    cv.Set1D(kalman.transition_matrix,i,cv.Scalar(A[i+1]))
  end
  cv.SetIdentity( kalman.measurement_matrix, cv.RealScalar(1) )
  cv.SetIdentity( kalman.process_noise_cov, cv.RealScalar(1e-5) )
  cv.SetIdentity( kalman.measurement_noise_cov, cv.RealScalar(1e-1) )
  cv.SetIdentity( kalman.error_cov_post, cv.RealScalar(1))
  cv.RandArr( rng, kalman.state_post, cv.CV_RAND_NORMAL, cv.RealScalar(0), cv.RealScalar(0.1) )

  while true do

    state_angle=cv.GetReal2D(state,0,0)
    state_pt = calc_point(state_angle)

    prediction = cv.KalmanPredict(kalman)
    predict_angle=cv.GetReal2D(prediction,0,0)
    predict_pt = calc_point(predict_angle)

    cv.RandArr( rng, measurement, cv.CV_RAND_NORMAL, cv.RealScalar(0), cv.RealScalar(math.sqrt(cv.GetReal2D(kalman.measurement_noise_cov,0,0))))

    cv.MatMulAdd(kalman.measurement_matrix,state,measurement,measurement)

    measurement_angle = cv.GetReal2D(measurement,0,0)
    measurement_pt = calc_point(measurement_angle)

    cv.Zero(img)
    draw_cross( state_pt, cv.CV_RGB(255,255,255), 3 )
    draw_cross( measurement_pt, cv.CV_RGB(255,0,0), 3 )
    draw_cross( predict_pt, cv.CV_RGB(0,255,0), 3 )
    cv.Line( img, state_pt, measurement_pt, cv.CV_RGB(255,0,0), 3, cv.CV_AA, 0 )
    cv.Line( img, state_pt, predict_pt, cv.CV_RGB(255,255,0), 3, cv.CV_AA, 0 )

    cv.KalmanCorrect( kalman, measurement )

    cv.RandArr( rng, process_noise, cv.CV_RAND_NORMAL, cv.RealScalar(0), cv.RealScalar(math.sqrt(cv.GetReal2D(kalman.process_noise_cov,0,0))))
    cv.MatMulAdd( kalman.transition_matrix, state, process_noise, state )

    cv.ShowImage( "Kalman", img )

    if cv.WaitKey(25)==27 then break end
  end
  if cv.WaitKey(25)==27 then break end
end
cv.DestroyWindow("Kalman")
