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
#import "FixCvVideoCamera.h"
using namespace cv;
@interface OpenCVWrapper() <CvVideoCameraDelegate>
@property (nonatomic, retain) FixCvVideoCamera* cvVideoCamera;
@property (nonatomic) Stabilizer *stabilizer;
@end
@implementation OpenCVWrapper
-(id) initWithWithVideoParentView:(UIView*) videoParentView{
    self = [super init];
    if (self) {
        self.cvVideoCamera = [[FixCvVideoCamera alloc] initWithParentView:videoParentView];
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
- (void) assignWrapperDelegate:(id<OpenCVWrapperDelegate> _Nonnull)wrapperDelegate {
    self.wrapperDelegate = wrapperDelegate;
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
    if (self.wrapperDelegate && [self.wrapperDelegate respondsToSelector:@selector(didReceiveOriginalImage:processedImage:)]) {
        UIImage *originalImage = [self UIImageFromCVMat:image_copy];
        UIImage *processedImage = [self UIImageFromCVMat:result];
        [self.wrapperDelegate didReceiveOriginalImage:originalImage processedImage:processedImage];
    }
}
#endif
#pragma mark - UI Actions
- (void)actionStart:(id)sender
{
    [self.cvVideoCamera start];
}

- (UIImage *) UIImageFromCVMat:(cv::Mat)cvMat {
    NSData *data = [NSData dataWithBytes:cvMat.data length:cvMat.elemSize()*cvMat.total()];
    
    CGColorSpaceRef colorSpace;
    CGBitmapInfo bitmapInfo;
    
    if (cvMat.elemSize() == 1) {
        colorSpace = CGColorSpaceCreateDeviceGray();
        bitmapInfo = kCGImageAlphaNone | kCGBitmapByteOrderDefault;
    } else {
        colorSpace = CGColorSpaceCreateDeviceRGB();
        bitmapInfo = kCGBitmapByteOrder32Little | (
                                                   cvMat.elemSize() == 3? kCGImageAlphaNone : kCGImageAlphaNoneSkipFirst
                                                   );
    }
    
    CGDataProviderRef provider = CGDataProviderCreateWithCFData((__bridge CFDataRef)data);
    
    // Creating CGImage from cv::Mat
    CGImageRef imageRef = CGImageCreate(
                                        cvMat.cols,                 //width
                                        cvMat.rows,                 //height
                                        8,                          //bits per component
                                        8 * cvMat.elemSize(),       //bits per pixel
                                        cvMat.step[0],              //bytesPerRow
                                        colorSpace,                 //colorspace
                                        bitmapInfo,                 // bitmap info
                                        provider,                   //CGDataProviderRef
                                        NULL,                       //decode
                                        false,                      //should interpolate
                                        kCGRenderingIntentDefault   //intent
                                        );
    
    // Getting UIImage from CGImage
    UIImage *finalImage = [UIImage imageWithCGImage:imageRef];
    CGImageRelease(imageRef);
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(colorSpace);
    return finalImage;
}
@end
