/*
* This file is part of the SDWebImage package.
* (c) Olivier Poitrey <rs@dailymotion.com>
*
* For the full copyright and license information, please view the LICENSE
* file that was distributed with this source code.
*/

#import "SDImageHEICCoder.h"
#import "SDImageIOAnimatedCoderInternal.h"

// These constants are available from iOS 13+ and Xcode 11. This raw value is used for toolchain and firmware compatibility
// 这些常量在iOS 13+和Xcode 11中可用。这个原始值用于工具链和固件兼容性
static NSString * kSDCGImagePropertyHEICSDictionary = @"{HEICS}";
static NSString * kSDCGImagePropertyHEICSLoopCount = @"LoopCount";
static NSString * kSDCGImagePropertyHEICSDelayTime = @"DelayTime";
static NSString * kSDCGImagePropertyHEICSUnclampedDelayTime = @"UnclampedDelayTime";

@implementation SDImageHEICCoder
/// 初始化
+ (void)initialize {
    if (@available(iOS 13, tvOS 13, macOS 10.15, watchOS 6, *)) {
        // Use SDK instead of raw value
        /// 用SDK 替代原始值
        kSDCGImagePropertyHEICSDictionary = (__bridge NSString *)kCGImagePropertyHEICSDictionary;
        kSDCGImagePropertyHEICSLoopCount = (__bridge NSString *)kCGImagePropertyHEICSLoopCount;
        kSDCGImagePropertyHEICSDelayTime = (__bridge NSString *)kCGImagePropertyHEICSDelayTime;
        kSDCGImagePropertyHEICSUnclampedDelayTime = (__bridge NSString *)kCGImagePropertyHEICSUnclampedDelayTime;
    }
}
/// 单例
+ (instancetype)sharedCoder {
    static SDImageHEICCoder *coder;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        coder = [[SDImageHEICCoder alloc] init];
    });
    return coder;
}

#pragma mark - SDImageCoder
/// 是否支持解码格式
- (BOOL)canDecodeFromData:(nullable NSData *)data {
    switch ([NSData sd_imageFormatForImageData:data]) {
        case SDImageFormatHEIC:
            // Check HEIC decoding compatibility
            return [self.class canDecodeFromFormat:SDImageFormatHEIC];
        case SDImageFormatHEIF:
            // Check HEIF decoding compatibility
            return [self.class canDecodeFromFormat:SDImageFormatHEIF];
        default:
            return NO;
    }
}
/// 是否支持增量解码数据
- (BOOL)canIncrementalDecodeFromData:(NSData *)data {
    return [self canDecodeFromData:data];
}
/// 是否支持编码格式
- (BOOL)canEncodeToFormat:(SDImageFormat)format {
    switch (format) {
        case SDImageFormatHEIC:
            // Check HEIC encoding compatibility
            return [self.class canEncodeToFormat:SDImageFormatHEIC];
        case SDImageFormatHEIF:
            // Check HEIF encoding compatibility
            return [self.class canEncodeToFormat:SDImageFormatHEIF];
        default:
            return NO;
    }
}

#pragma mark - Subclass Override
/// 图片格式
+ (SDImageFormat)imageFormat {
    return SDImageFormatHEIC;
}
/// 图片格式唯一标识
+ (NSString *)imageUTType {
    return (__bridge NSString *)kSDUTTypeHEIC;
}
/// 词典属性
+ (NSString *)dictionaryProperty {
    return kSDCGImagePropertyHEICSDictionary;
}
///unclampedDelayTime属性
+ (NSString *)unclampedDelayTimeProperty {
    return kSDCGImagePropertyHEICSUnclampedDelayTime;
}
/// 延时属性
+ (NSString *)delayTimeProperty {
    return kSDCGImagePropertyHEICSDelayTime;
}
/// 循环次数属性
+ (NSString *)loopCountProperty {
    return kSDCGImagePropertyHEICSLoopCount;
}
/// 默认循环次数
+ (NSUInteger)defaultLoopCount {
    return 0;
}

@end
