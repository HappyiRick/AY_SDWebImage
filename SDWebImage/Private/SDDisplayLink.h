/*
* This file is part of the SDWebImage package.
* (c) Olivier Poitrey <rs@dailymotion.com>
*
* For the full copyright and license information, please view the LICENSE
* file that was distributed with this source code.
*/

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"

/// Cross-platform display link wrapper. Do not retain the target
/// Use `CADisplayLink` on iOS/tvOS, `CVDisplayLink` on macOS, `NSTimer` on watchOS
/// 跨平台显示链接包装器。不保留目标，在iOS/tvOS上使用CADisplayLink，macOS上使用CVDisplayLink，watchOS上使用NSTimer
@interface SDDisplayLink : NSObject
/// 目标
@property (readonly, nonatomic, weak, nullable) id target;
/// 选择器
@property (readonly, nonatomic, assign, nonnull) SEL selector;
/// 时间
@property (readonly, nonatomic) CFTimeInterval duration;
/// 是否正在运行
@property (readonly, nonatomic) BOOL isRunning;
/// 初始化
+ (nonnull instancetype)displayLinkWithTarget:(nonnull id)target selector:(nonnull SEL)sel;
/// 添加到运行循环
- (void)addToRunLoop:(nonnull NSRunLoop *)runloop forMode:(nonnull NSRunLoopMode)mode;
/// 从运行循环中删除
- (void)removeFromRunLoop:(nonnull NSRunLoop *)runloop forMode:(nonnull NSRunLoopMode)mode;
/// 开始
- (void)start;
/// 停止
- (void)stop;

@end
