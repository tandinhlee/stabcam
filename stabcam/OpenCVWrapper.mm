//
//  OpenCVWrapper.m
//  stabcam
//
//  Created by Dinh Le on 1/21/19.
//  Copyright © 2019 Dinh Le. All rights reserved.
//

#import "OpenCVWrapper.h"
#import "Stabilizer.hpp"
#import <opencv2/opencv.hpp>
#import <opencv2/videoio/cap_ios.h>
using namespace cv;
@interface OpenCVWrapper() <CvVideoCameraDelegate>
@property (nonatomic, retain) CvVideoCamera* cvVideoCamera;
@property (nonatomic) Stabilizer *stabilizer;
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
        self.stabilizer = new Stabilizer();
    }
    return self;
}

#pragma mark - Protocol CvVideoCameraDelegate
#ifdef __cplusplus
- (void) processImage:(Mat&) image
{
//    Stabilizer *stabilizer = new Stabilizer();
    Mat image_copy;
    image.copyTo(image_copy);
    Mat result = self.stabilizer->stablelize(image_copy);
    result.copyTo(image);
}
#endif
#pragma mark - UI Actions
- (void)actionStart:(id)sender
{
    [self.cvVideoCamera start];
}

@end
