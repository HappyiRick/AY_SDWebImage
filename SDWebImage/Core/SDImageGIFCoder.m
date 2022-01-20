/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDImageGIFCoder.h"
#import "SDImageIOAnimatedCoderInternal.h"
#if SD_MAC
#import <CoreServices/CoreServices.h>
#else
#import <MobileCoreServices/MobileCoreServices.h>
#endif

@implementation SDImageGIFCoder
/// 获取编码器
+ (instancetype)sharedCoder {
    static SDImageGIFCoder *coder;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        coder = [[SDImageGIFCoder alloc] init];
    });
    return coder;
}

#pragma mark - Subclass Override
/// 图片格式
+ (SDImageFormat)imageFormat {
    return SDImageFormatGIF;
}
/// 图片源格式
+ (NSString *)imageUTType {
    return (__bridge NSString *)kSDUTTypeGIF;
}
/// 属性字典
+ (NSString *)dictionaryProperty {
    return (__bridge NSString *)kCGImagePropertyGIFDictionary;
}
/// unclampedDelayTimeProperty 属性
+ (NSString *)unclampedDelayTimeProperty {
    return (__bridge NSString *)kCGImagePropertyGIFUnclampedDelayTime;
}
/// 延时属性
+ (NSString *)delayTimeProperty {
    return (__bridge NSString *)kCGImagePropertyGIFDelayTime;
}
/// 循环次数属性
+ (NSString *)loopCountProperty {
    return (__bridge NSString *)kCGImagePropertyGIFLoopCount;
}
/// 默认循环次数
+ (NSUInteger)defaultLoopCount {
    return 1;
}

@end
