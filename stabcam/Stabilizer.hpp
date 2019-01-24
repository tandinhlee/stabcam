//
//  Stabilizer.hpp
//  stabcam
//
//  Created by Dinh Le on 1/23/19.
//  Copyright © 2019 Dinh Le. All rights reserved.
//

#ifndef Stabilizer_hpp
#define Stabilizer_hpp
#include <stdio.h>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cassert>
#include <fstream>
using namespace std;
using namespace cv;
struct TransformParam
{
    TransformParam() {}
    TransformParam(double _dx, double _dy, double _da) {
        dx = _dx;
        dy = _dy;
        da = _da;
    }
    
    double dx;
    double dy;
    double da; // angle
};

struct Trajectory
{
    Trajectory() {}
    Trajectory(double _x, double _y, double _a) {
        x = _x;
        y = _y;
        a = _a;
    }
    // "+"
    friend Trajectory operator+(const Trajectory &c1,const Trajectory  &c2){
        return Trajectory(c1.x+c2.x,c1.y+c2.y,c1.a+c2.a);
    }
    //"-"
    friend Trajectory operator-(const Trajectory &c1,const Trajectory  &c2){
        return Trajectory(c1.x-c2.x,c1.y-c2.y,c1.a-c2.a);
    }
    //"*"
    friend Trajectory operator*(const Trajectory &c1,const Trajectory  &c2){
        return Trajectory(c1.x*c2.x,c1.y*c2.y,c1.a*c2.a);
    }
    //"/"
    friend Trajectory operator/(const Trajectory &c1,const Trajectory  &c2){
        return Trajectory(c1.x/c2.x,c1.y/c2.y,c1.a/c2.a);
    }
    //"="
    Trajectory operator =(const Trajectory &rx){
        x = rx.x;
        y = rx.y;
        a = rx.a;
        return Trajectory(x,y,a);
    }
    
    double x;
    double y;
    double a; // angle
};
class Stabilizer {
private:
    Mat prev;
    // Step 1 - Get previous to current frame transformation (dx, dy, da) for all frames
//    vector <TransformParam> prev_to_cur_transform; // previous to current
    // Accumulated frame to frame transform
    double a = 0;
    double x = 0;
    double y = 0;
    // Step 2 - Accumulate the transformations to get the image trajectory
    vector <Trajectory> trajectory; // trajectory at all frames
    //
    // Step 3 - Smooth out the trajectory using an averaging window
    vector <Trajectory> smoothed_trajectory; // trajectory at all frames
    Trajectory X;//posteriori state estimate
    Trajectory    X_;//priori estimate
    Trajectory P;// posteriori estimate error covariance
    Trajectory P_;// priori estimate error covariance
    Trajectory K;//gain
    Trajectory    z;//actual measurement
    double pstd = 4e-3;//can be changed
    double cstd = 0.25;//can be changed
    Trajectory Q;// process noise covariance
    Trajectory R;// measurement noise covariance
    // Step 4 - Generate new set of previous to current transform, such that the trajectory ends up being the same as the smoothed trajectory
//    vector <TransformParam> new_prev_to_cur_transform;
    //
    // Step 5 - Apply the new transformation to the video
    //cap.set(CV_CAP_PROP_POS_FRAMES, 0);
    int vert_border; // get the aspect ratio correct
    Mat last_rigidtransform;
public:
    Stabilizer();
    Mat stablelize(Mat commingMat);
};
#endif /* Stabilizer_hpp */
