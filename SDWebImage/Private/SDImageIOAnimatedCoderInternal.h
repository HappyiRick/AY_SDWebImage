/*
* This file is part of the SDWebImage package.
* (c) Olivier Poitrey <rs@dailymotion.com>
*
* For the full copyright and license information, please view the LICENSE
* file that was distributed with this source code.
*/

#import <Foundation/Foundation.h>
#import "SDImageIOAnimatedCoder.h"

// AVFileTypeHEIC/AVFileTypeHEIF is defined in AVFoundation via iOS 11, we use this without import AVFoundation
/// AVFileTypeHEIC/AVFileTypeHEIF是通过iOS 11在AVFoundation中定义的，我们使用这个而不导入AVFoundation
#define kSDUTTypeHEIC  ((__bridge CFStringRef)@"public.heic")
#define kSDUTTypeHEIF  ((__bridge CFStringRef)@"public.heif")
// HEIC Sequence (Animated Image)
/// HEIC序列(动画图像)
#define kSDUTTypeHEICS ((__bridge CFStringRef)@"public.heics")
// kSDUTTypeWebP seems not defined in public UTI framework, Apple use the hardcode string, we define them :)
/// kSDUTTypeWebP似乎没有在公共UTI框架中定义，苹果使用硬编码字符串，是我们定义的它们:)
#define kSDUTTypeWebP  ((__bridge CFStringRef)@"org.webmproject.webp")

#define kSDUTTypeImage ((__bridge CFStringRef)@"public.image")
#define kSDUTTypeJPEG  ((__bridge CFStringRef)@"public.jpeg")
#define kSDUTTypePNG   ((__bridge CFStringRef)@"public.png")
#define kSDUTTypeTIFF  ((__bridge CFStringRef)@"public.tiff")
#define kSDUTTypeSVG   ((__bridge CFStringRef)@"public.svg-image")
#define kSDUTTypeGIF   ((__bridge CFStringRef)@"com.compuserve.gif")
#define kSDUTTypePDF   ((__bridge CFStringRef)@"com.adobe.pdf")

@interface SDImageIOAnimatedCoder ()
/// 指定图像源 在指定位置 的帧时长
+ (NSTimeInterval)frameDurationAtIndex:(NSUInteger)index source:(nonnull CGImageSourceRef)source;
/// 指定图像源的图像循环次数
+ (NSUInteger)imageLoopCountWithSource:(nonnull CGImageSourceRef)source;
/// 创建帧图像
+ (nullable UIImage *)createFrameAtIndex:(NSUInteger)index source:(nonnull CGImageSourceRef)source scale:(CGFloat)scale preserveAspectRatio:(BOOL)preserveAspectRatio thumbnailSize:(CGSize)thumbnailSize options:(nullable NSDictionary *)options;
/// 支持编码格式
+ (BOOL)canEncodeToFormat:(SDImageFormat)format;
/// 支持解码格式
+ (BOOL)canDecodeFromFormat:(SDImageFormat)format;

@end
