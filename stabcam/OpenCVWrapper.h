//
//  OpenCVWrapper.h
//  stabcam
//
//  Created by Dinh Le on 1/21/19.
//  Copyright © 2019 Dinh Le. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN
@protocol OpenCVWrapperDelegate <NSObject>
@optional
- (void) didReceiveOriginalImage:(UIImage *) originalImage processedImage:(UIImage *) processImage;
@end
@interface OpenCVWrapper : NSObject
@property (nonatomic, assign) id<OpenCVWrapperDelegate> wrapperDelegate;
- (id) initWithWithVideoParentView:(UIView*) videoParentView;
- (void)actionStart:(id)sender;
- (void) assignWrapperDelegate:(id<OpenCVWrapperDelegate> _Nonnull)wrapperDelegate;
@end

NS_ASSUME_NONNULL_END
