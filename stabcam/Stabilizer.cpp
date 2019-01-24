//
//  Stabilizer.cpp
//  stabcam
//
//  Created by Dinh Le on 1/23/19.
//  Copyright © 2019 Dinh Le. All rights reserved.
//

#include "Stabilizer.hpp"
const int HORIZONTAL_BORDER_CROP = 40; // In pixels. Crops the border to reduce the black borders from stabilisation being too noticeable.

using namespace std;
Stabilizer::Stabilizer() {
    prev = Mat();
    Q = Trajectory(pstd,pstd,pstd);// process noise covariance
    R = Trajectory(cstd,cstd,cstd);// measurement noise covariance
    //T = Mat(2,3,CV_64F);
    X = Trajectory(0,0,0); //Initial estimate,  set 0
    P =Trajectory(1,1,1); //set error variance,set 1
    
}
Mat Stabilizer::stablelize(Mat commingMat) {
    Mat cur, cur_grey;
    Mat prev_grey;
    commingMat.copyTo(cur);
    if (prev.data == NULL) {
        cur.copyTo(prev);
        cvtColor(prev, prev_grey, COLOR_BGR2GRAY);
    } else {
        cvtColor(prev, prev_grey, COLOR_BGR2GRAY);
    }
    cvtColor(cur, cur_grey, COLOR_BGR2GRAY);
    goodFeaturesToTrack(prev_grey, prev_corner, 200, 0.01, 30);
    calcOpticalFlowPyrLK(prev_grey, cur_grey, prev_corner, cur_corner, status, err);
    
    // weed out bad matches
    for(size_t i=0; i < status.size(); i++) {
        if(status[i]) {
            prev_corner2.push_back(prev_corner[i]);
            cur_corner2.push_back(cur_corner[i]);
        }
    }
    
    // translation + rotation only
//    T = estimateRigidTransform(prev_corner2, cur_corner2, false); // false = rigid transform, no scaling/shearing
    Mat rigidtrans=estimateRigidTransform(prev_corner,cur_corner,false); // false = rigid transform, no scaling/shearing
    
    // in rare cases no transform is found. We'll just use the last known good transform.
    if(rigidtrans.empty()) {
        rigidtrans=last_rigidtransform.clone();
    }
    rigidtrans.copyTo(last_rigidtransform);
    
    // decompose T
    double dx = rigidtrans.at<double>(0,2);
    double dy = rigidtrans.at<double>(1,2);
    double da = atan2(rigidtrans.at<double>(1,0), rigidtrans.at<double>(0,0));
    //
    //prev_to_cur_transform.push_back(TransformParam(dx, dy, da));
    
//    out_transform << k << " " << dx << " " << dy << " " << da << endl;
    //
    // Accumulated frame to frame transform
    x += dx;
    y += dy;
    a += da;
    //trajectory.push_back(Trajectory(x,y,a));
    //
//    out_trajectory << k << " " << x << " " << y << " " << a << endl;
    //
    z = Trajectory(x,y,a);
    //
    
    //time update£¨prediction£©
    X_ = X; //X_(k) = X(k-1);
    P_ = P+Q; //P_(k) = P(k-1)+Q;
    // measurement update£¨correction£©
    K = P_/( P_+R ); //gain;K(k) = P_(k)/( P_(k)+R );
    X = X_+K*(z-X_); //z-X_ is residual,X(k) = X_(k)+K(k)*(z(k)-X_(k));
    P = (Trajectory(1,1,1)-K)*P_; //P(k) = (1-K(k))*P_(k);
    
    //smoothed_trajectory.push_back(X);
//    out_smoothed_trajectory << k << " " << X.x << " " << X.y << " " << X.a << endl;
    //-
    // target - current
    double diff_x = X.x - x;//
    double diff_y = X.y - y;
    double diff_a = X.a - a;
    
    dx = dx + diff_x;
    dy = dy + diff_y;
    da = da + diff_a;
    
    //new_prev_to_cur_transform.push_back(TransformParam(dx, dy, da));
    //
//    out_new_transform << k << " " << dx << " " << dy << " " << da << endl;
    //
    rigidtrans.at<double>(0,0) = cos(da);
    rigidtrans.at<double>(0,1) = -sin(da);
    rigidtrans.at<double>(1,0) = sin(da);
    rigidtrans.at<double>(1,1) = cos(da);
    
    rigidtrans.at<double>(0,2) = dx;
    rigidtrans.at<double>(1,2) = dy;
    
    Mat cur2;
    
    warpAffine(prev, cur2, rigidtrans, cur.size());
    int vert_border = HORIZONTAL_BORDER_CROP * prev.rows / prev.cols; // get the aspect ratio correct
    cur2 = cur2(Range(vert_border, cur2.rows-vert_border), Range(HORIZONTAL_BORDER_CROP, cur2.cols-HORIZONTAL_BORDER_CROP));
    
    // Resize cur2 back to cur size, for better side by side comparison
    resize(cur2, cur2, cur.size());
    
    
    // Now draw the original and stablised side by side for coolness
//    Mat canvas = Mat::zeros(cur.rows, cur.cols*2+10, cur.type());
    
//    prev.copyTo(canvas(Range::all(), Range(0, cur2.cols)));
//    cur2.copyTo(canvas(Range::all(), Range(cur2.cols+10, cur2.cols*2+10)));
    
    // If too big to fit on the screen, then scale it down by 2, hopefully it'll fit :)
//    if(canvas.cols > 1920) {
//        resize(canvas, canvas, Size(canvas.cols/2, canvas.rows/2));
//    }
    //outputVideo<<canvas;
//    imshow("before and after", canvas);
    
//    waitKey(10);
    //
    prev = cur.clone();//cur.copyTo(prev);
//    cur.deallocate();
//    cur_grey.deallocate();
//    cur_grey.copyTo(prev_grey);
    
//    cout << "Frame: " << k << "/" << max_frames << " - good optical flow: " << prev_corner2.size() << endl;
//    k++;
    return cur2;
}
