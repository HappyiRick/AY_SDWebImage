/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDImageCodersManager.h"
#import "SDImageIOCoder.h"
#import "SDImageGIFCoder.h"
#import "SDImageAPNGCoder.h"
#import "SDImageHEICCoder.h"
#import "SDInternalMacros.h"

@interface SDImageCodersManager ()
/// 私有变量
@property (nonatomic, strong, nonnull) NSMutableArray<id<SDImageCoder>> *imageCoders;

@end

@implementation SDImageCodersManager {
    /// 锁的宏
    SD_LOCK_DECLARE(_codersLock);
}
/// 创建单例
+ (nonnull instancetype)sharedManager {
    static dispatch_once_t once;
    static id instance;
    dispatch_once(&once, ^{
        instance = [self new];
    });
    return instance;
}
/// 默认初始化
- (instancetype)init {
    if (self = [super init]) {
        // initialize with default coders
        _imageCoders = [NSMutableArray arrayWithArray:@[[SDImageIOCoder sharedCoder], [SDImageGIFCoder sharedCoder], [SDImageAPNGCoder sharedCoder]]];
        SD_LOCK_INIT(_codersLock);
    }
    return self;
}
/// 重写coders getter方法
- (NSArray<id<SDImageCoder>> *)coders {
    SD_LOCK(_codersLock);
    NSArray<id<SDImageCoder>> *coders = [_imageCoders copy];
    SD_UNLOCK(_codersLock);
    return coders;
}
/// 重写coders setter方法
- (void)setCoders:(NSArray<id<SDImageCoder>> *)coders {
    SD_LOCK(_codersLock);
    [_imageCoders removeAllObjects];
    if (coders.count) {
        [_imageCoders addObjectsFromArray:coders];
    }
    SD_UNLOCK(_codersLock);
}

#pragma mark - Coder IO operations
/// 添加编码器
- (void)addCoder:(nonnull id<SDImageCoder>)coder {
    /// 如果不遵守编码器protocol则终止
    if (![coder conformsToProtocol:@protocol(SDImageCoder)]) {
        return;
    }
    SD_LOCK(_codersLock);
    [_imageCoders addObject:coder];
    SD_UNLOCK(_codersLock);
}
/// 删除编码器
- (void)removeCoder:(nonnull id<SDImageCoder>)coder {
    /// 如果不遵守编码器protocol则终止
    if (![coder conformsToProtocol:@protocol(SDImageCoder)]) {
        return;
    }
    SD_LOCK(_codersLock);
    [_imageCoders removeObject:coder];
    SD_UNLOCK(_codersLock);
}

#pragma mark - SDImageCoder
/// 是否可以解码数据
- (BOOL)canDecodeFromData:(NSData *)data {
    NSArray<id<SDImageCoder>> *coders = self.coders;
    for (id<SDImageCoder> coder in coders.reverseObjectEnumerator) {
        if ([coder canDecodeFromData:data]) {
            return YES;
        }
    }
    return NO;
}
/// 是否支持编码格式
- (BOOL)canEncodeToFormat:(SDImageFormat)format {
    NSArray<id<SDImageCoder>> *coders = self.coders;
    for (id<SDImageCoder> coder in coders.reverseObjectEnumerator) {
        if ([coder canEncodeToFormat:format]) {
            return YES;
        }
    }
    return NO;
}
/// 从数据中解码图片
- (UIImage *)decodedImageWithData:(NSData *)data options:(nullable SDImageCoderOptions *)options {
    if (!data) {
        return nil;
    }
    UIImage *image;
    NSArray<id<SDImageCoder>> *coders = self.coders;
    for (id<SDImageCoder> coder in coders.reverseObjectEnumerator) {
        if ([coder canDecodeFromData:data]) {
            image = [coder decodedImageWithData:data options:options];
            break;
        }
    }
    
    return image;
}
/// 编码图片为数据
- (NSData *)encodedDataWithImage:(UIImage *)image format:(SDImageFormat)format options:(nullable SDImageCoderOptions *)options {
    if (!image) {
        return nil;
    }
    NSArray<id<SDImageCoder>> *coders = self.coders;
    for (id<SDImageCoder> coder in coders.reverseObjectEnumerator) {
        if ([coder canEncodeToFormat:format]) {
            return [coder encodedDataWithImage:image format:format options:options];
        }
    }
    return nil;
}

@end
