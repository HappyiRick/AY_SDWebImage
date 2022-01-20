/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"
#import "NSData+ImageContentType.h"

/**
 UIImage category for image metadata, including animation, loop count, format, incremental, etc.
 图像元数据的图像分类，包括动画、循环数、格式、增量等
 */
@interface UIImage (Metadata)

/**
 * UIKit:
 * For static image format, this value is always 0. - 对于静态图像格式，该值永远为0
 * For animated image format, 0 means infinite looping. - 对于动态图像格式，0意味着无限循环
 * Note that because of the limitations of categories this property can get out of sync if you create another instance with CGImage or other methods.
 * 请注意，由于类别的限制，如果您使用CGImage或其他方法创建另一个实例，这个属性可能会失去同步
 * AppKit:
 * NSImage currently only support animated via `NSBitmapImageRep`(GIF) or `SDAnimatedImageRep`(APNG/GIF/WebP) unlike UIImage.
 * 与UIImage不同，NSImage目前只支持通过' NSBitmapImageRep ' (GIF)或' sanimatedimagerep ' (APNG/GIF/WebP)进行动画。
 * The getter of this property will get the loop count from animated imageRep - 这个属性的getter将从动画imageRep中获取循环计数
 * The setter of this property will set the loop count from animated imageRep - 这个属性的setter将从动画imageRep中设置循环计数
 */
@property (nonatomic, assign) NSUInteger sd_imageLoopCount;

/**
 * UIKit:
 * Returns the `images`'s count by unapply the patch for the different frame durations. Which matches the real visible frame count when displaying on UIImageView.
 * 通过取消应用不同帧的补丁，返回“图像”的计数。当在UIImageView上显示时，它匹配真实的可见帧数。
 * See more in `SDImageCoderHelper.animatedImageWithFrames`.
 * Returns 1 for static image. - 对于静态图像返回1
 * AppKit:
 * Returns the underlaying `NSBitmapImageRep` or `SDAnimatedImageRep` frame count. -  返回底层的' NSBitmapImageRep '或' SDAnimatedImageRep '帧数
 * Returns 1 for static image.
 */
@property (nonatomic, assign, readonly) NSUInteger sd_imageFrameCount;

/**
 * UIKit:
 * Check the `images` array property. - 检查 ‘images’ 数组属性
 * AppKit:
 * NSImage currently only support animated via GIF imageRep unlike UIImage. It will check the imageRep's frame count.
 * NSImage目前只支持通过GIF imageRep动画，不像UIImage。它会检查imageRep的帧数。
 */
@property (nonatomic, assign, readonly) BOOL sd_isAnimated;

/**
 * UIKit:
 * Check the `isSymbolImage` property. Also check the system PDF(iOS 11+) && SVG(iOS 13+) support.
 * 检查“isSymbolImage”属性。此外，检查系统PDF(iOS 11+)和SVG(iOS 13+)支持。
 * AppKit:
 * NSImage supports PDF && SVG && EPS imageRep, check the imageRep class.
 * NSImage支持PDF && SVG && EPS imageRep，检查imageRep类。
 */
@property (nonatomic, assign, readonly) BOOL sd_isVector;

/**
 * The image format represent the original compressed image data format. - 图像格式表示原始的压缩图像数据格式
 * If you don't manually specify a format, this information is retrieve from CGImage using `CGImageGetUTType`, which may return nil for non-CG based image. At this time it will return `SDImageFormatUndefined` as default value.
 * @note Note that because of the limitations of categories this property can get out of sync if you create another instance with CGImage or other methods.
 * 如果你没有手动指定格式，这个信息是使用' CGImageGetUTType '从CGImage检索的，这可能会返回nil的非cg图像。此时，它将返回' SDImageFormatUndefined '作为默认值。
 * 注意，由于类别的限制，如果你用CGImage或其他方法创建另一个实例，这个属性可能会不同步。
 */
@property (nonatomic, assign) SDImageFormat sd_imageFormat;

/**
 A bool value indicating whether the image is during incremental decoding and may not contains full pixels.
 一个bool值，指示图像是否处于增量解码期间，且可能不包含完整像素。
 */
@property (nonatomic, assign) BOOL sd_isIncremental;

@end
