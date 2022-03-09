/*
* This file is part of the SDWebImage package.
* (c) Olivier Poitrey <rs@dailymotion.com>
*
* For the full copyright and license information, please view the LICENSE
* file that was distributed with this source code.
*/

#import "SDWebImageCompat.h"

/**
 These following class are provided to use `UIGraphicsImageRenderer` with polyfill, which allows write cross-platform(AppKit/UIKit) code and avoid runtime version check.
 Compared to `UIGraphicsBeginImageContext`, `UIGraphicsImageRenderer` use dynamic bitmap from your draw code to generate CGContext, not always use ARGB8888, which is more performant on RAM usage.
 Which means, if you draw CGImage/CIImage which contains grayscale only, the underlaying bitmap context use grayscale, it's managed by system and not a fixed type. (actually, the `kCGContextTypeAutomatic`)
 For usage, See more in Apple's documentation: https://developer.apple.com/documentation/uikit/uigraphicsimagerenderer
 For UIKit on iOS/tvOS 10+, these method just use the same `UIGraphicsImageRenderer` API.
 For others (macOS/watchOS or iOS/tvOS 10-), these method use the `SDImageGraphics.h` to implements the same behavior (but without dynamic bitmap support)
*/

typedef void (^SDGraphicsImageDrawingActions)(CGContextRef _Nonnull context);
typedef NS_ENUM(NSInteger, SDGraphicsImageRendererFormatRange) {
    SDGraphicsImageRendererFormatRangeUnspecified = -1,
    SDGraphicsImageRendererFormatRangeAutomatic = 0,
    SDGraphicsImageRendererFormatRangeExtended,
    SDGraphicsImageRendererFormatRangeStandard
};

/// A set of drawing attributes that represent the configuration of an image renderer context.
/// 表示图像渲染环境配置的绘制属性集合
@interface SDGraphicsImageRendererFormat : NSObject

/// The display scale of the image renderer context.
/// 图像渲染环境的展示缩放因子
/// The default value is equal to the scale of the main screen.
/// 默认等于显示屏的缩放因子
@property (nonatomic) CGFloat scale;

/// A Boolean value indicating whether the underlying Core Graphics context has an alpha channel.
/// The default value is NO.
/// 一个布尔值，指示底层Core Graphics上下文是否有alpha通道。默认为NO
@property (nonatomic) BOOL opaque;

/// Specifying whether the bitmap context should use extended color.
/// For iOS 12+, the value is from system `preferredRange` property
/// For iOS 10-11, the value is from system `prefersExtendedRange` property
/// For iOS 9-, the value is `.standard`
/// 指定位图上下文是否应使用扩展颜色。
/// 对于iOS 12+，该值来自system ' preferredRange '属性
/// 对于iOS 10-11，该值来自system ' prefersExtendedRange '属性
/// 对于iOS 9-，值为' .standard '
@property (nonatomic) SDGraphicsImageRendererFormatRange preferredRange;

/// Init the default format. See each properties's default value.
/// 默认初始化
- (nonnull instancetype)init;

/// Returns a new format best suited for the main screen’s current configuration.
/// 返回最适合主屏幕当前配置的新格式
+ (nonnull instancetype)preferredFormat;

@end

/// A graphics renderer for creating Core Graphics-backed images.
/// 用于创建核心图形支持的图像的图形渲染器
@interface SDGraphicsImageRenderer : NSObject

/// Creates an image renderer for drawing images of a given size.
/// 创建用于绘制给定大小的图像的图像渲染器
/// @param size The size of images output from the renderer, specified in points. // 从渲染器输出的图像的大小，以点指定.
/// @return An initialized image renderer.
/// @return 一个初始化的图像渲染器
- (nonnull instancetype)initWithSize:(CGSize)size;

/// Creates a new image renderer with a given size and format. // 用给定的大小和格式创建一个新的图像渲染器
/// @param size The size of images output from the renderer, specified in points. // 从渲染器输出的图像的大小，以点指定
/// @param format A SDGraphicsImageRendererFormat object that encapsulates the format used to create the renderer context. // 一个SDGraphicsImageRendererFormat对象，它封装了用于创建渲染器上下文的格式
/// @return An initialized image renderer. // 一个初始化的图像渲染器
- (nonnull instancetype)initWithSize:(CGSize)size format:(nonnull SDGraphicsImageRendererFormat *)format;

/// Creates an image by following a set of drawing instructions. // 按照一组绘图说明创建图像
/// @param actions A SDGraphicsImageDrawingActions block that, when invoked by the renderer, executes a set of drawing instructions to create the output image. // 一个SDGraphicsImageDrawingActions块，当被渲染器调用时，执行一组绘图指令来创建输出图像
/// @note You should not retain or use the context outside the block, it's non-escaping. // 你不应该保留或使用区块外的上下文，它是非转义的
/// @return A UIImage object created by the supplied drawing actions. // 一个由提供的绘图动作创建的UIImage对象
- (nonnull UIImage *)imageWithActions:(nonnull NS_NOESCAPE SDGraphicsImageDrawingActions)actions;

@end
