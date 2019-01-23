//
//  OpenCVWrapper.m
//  stabcam
//
//  Created by Dinh Le on 1/21/19.
//  Copyright © 2019 Dinh Le. All rights reserved.
//

#import "OpenCVWrapper.h"
#import <opencv2/opencv.hpp>
#import <opencv2/videoio/cap_ios.h>
using namespace cv;
@interface OpenCVWrapper() <CvVideoCameraDelegate>
@property (nonatomic, retain) CvVideoCamera* cvVideoCamera;
@end
@implementation OpenCVWrapper
-(id) initWithWithVideoParentView:(UIView*) videoParentView{
    self = [super init];
    if (self) {
        self.cvVideoCamera = [[CvVideoCamera alloc] initWithParentView:videoParentView];
        self.cvVideoCamera.defaultAVCaptureDevicePosition = AVCaptureDevicePositionFront;
        self.cvVideoCamera.defaultAVCaptureSessionPreset = AVCaptureSessionPreset352x288;
        self.cvVideoCamera.defaultAVCaptureVideoOrientation = AVCaptureVideoOrientationPortrait;
        self.cvVideoCamera.defaultFPS = 30;
        self.cvVideoCamera.grayscaleMode = NO;
        self.cvVideoCamera.delegate = self;
    }
    return self;
}

#pragma mark - Protocol CvVideoCameraDelegate
#ifdef __cplusplus
- (void) processImage:(Mat&)image
{
    // Do some OpenCV stuff with the image
    // Do some OpenCV stuff with the image
    Mat image_copy;
    cvtColor(image, image_copy, COLOR_BGR2GRAY);
    // invert image
    bitwise_not(image_copy, image_copy);
    //Convert BGR to BGRA (three channel to four channel)
    Mat bgr;
    cvtColor(image_copy, bgr, COLOR_GRAY2BGR);
    cvtColor(bgr, image, COLOR_BGR2BGRA);
}
#endif
#pragma mark - UI Actions
- (void)actionStart:(id)sender
{
    [self.cvVideoCamera start];
}

@end
