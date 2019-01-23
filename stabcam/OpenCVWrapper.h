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

@interface OpenCVWrapper : NSObject
-(id) initWithWithVideoParentView:(UIView*) videoParentView;
- (void)actionStart:(id)sender;
@end

NS_ASSUME_NONNULL_END
