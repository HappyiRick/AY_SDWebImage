/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDImageAPNGCoder.h"
#import "SDImageIOAnimatedCoderInternal.h"
#if SD_MAC
#import <CoreServices/CoreServices.h>
#else
#import <MobileCoreServices/MobileCoreServices.h>
#endif

@implementation SDImageAPNGCoder
/// 单例创建
+ (instancetype)sharedCoder {
    static SDImageAPNGCoder *coder;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        coder = [[SDImageAPNGCoder alloc] init];
    });
    return coder;
}

#pragma mark - Subclass Override
/// 图像格式
+ (SDImageFormat)imageFormat {
    return SDImageFormatPNG;
}
/// 唯一标识类型
+ (NSString *)imageUTType {
    return (__bridge NSString *)kSDUTTypePNG;
}
/// 词典属性
+ (NSString *)dictionaryProperty {
    return (__bridge NSString *)kCGImagePropertyPNGDictionary;
}
/// unclampedDelayTime 属性
+ (NSString *)unclampedDelayTimeProperty {
    return (__bridge NSString *)kCGImagePropertyAPNGUnclampedDelayTime;
}
/// 延时属性
+ (NSString *)delayTimeProperty {
    return (__bridge NSString *)kCGImagePropertyAPNGDelayTime;
}
/// 循环次数属性
+ (NSString *)loopCountProperty {
    return (__bridge NSString *)kCGImagePropertyAPNGLoopCount;
}
/// 默认循环次数
+ (NSUInteger)defaultLoopCount {
    return 0;
}

@end
