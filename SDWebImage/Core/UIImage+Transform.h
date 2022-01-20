/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

typedef NS_ENUM(NSUInteger, SDImageScaleMode) {
    SDImageScaleModeFill = 0,
    SDImageScaleModeAspectFit = 1,
    SDImageScaleModeAspectFill = 2
};

#if SD_UIKIT || SD_WATCH
typedef UIRectCorner SDRectCorner;
#else
typedef NS_OPTIONS(NSUInteger, SDRectCorner) {
    SDRectCornerTopLeft     = 1 << 0,
    SDRectCornerTopRight    = 1 << 1,
    SDRectCornerBottomLeft  = 1 << 2,
    SDRectCornerBottomRight = 1 << 3,
    SDRectCornerAllCorners  = ~0UL
};
#endif

/**
 Provide some common method for `UIImage`. - 为`UIImage`提供一些寻常方法，基于Core Graphics和vImage进行图像处理
 Image process is based on Core Graphics and vImage.
 */
@interface UIImage (Transform)

#pragma mark - Image Geometry 图像几何

/**
 Returns a new image which is resized from this image. - 返回一张重新设置尺寸的心图片
 You can specify a larger or smaller size than the image size. The image content will be changed with the scale mode.
 您可以指定比图像大小大或小的大小。图像内容将随着缩放模式而改变。
 
 @param size        The new size to be resized, values should be positive. - 要调整的新大小，值应该为正
 @param scaleMode   The scale mode for image content. - 缩放模式
 @return The new image with the given size. - 给定尺寸的新图片
 */
- (nullable UIImage *)sd_resizedImageWithSize:(CGSize)size scaleMode:(SDImageScaleMode)scaleMode;

/**
 Returns a new image which is cropped from this image. - 返回从该图像中裁剪出来的新图像
 
 @param rect     Image's inner rect. - 图像内部尺寸
 @return         The new image with the cropping rect. - 按指定尺寸裁出来的新图像
 */
- (nullable UIImage *)sd_croppedImageWithRect:(CGRect)rect;

/**
 Rounds a new image with a given corner radius and corners.
 
 用给定的角半径和角将新图像圆角化
 
 @param cornerRadius The radius of each corner oval. Values larger than half the rectangle's width or height are clamped appropriately to half the width or height. - 每个角的椭圆半径。大于矩形宽度或高度一半的值将被适当地固定到宽度或高度的一半。
 @param corners      A bitmask value that identifies the corners that you want rounded. You can use this parameter to round only a subset of the corners of the rectangle. - 位掩码值，它标识要圆角的角。您可以使用此参数只圆角矩形的一个子集
 @param borderWidth  The inset border line width. Values larger than half the rectangle's width or height are clamped appropriately to half the width or height. - 插入的边框线宽度。大于矩形宽度或高度一半的值将被适当地固定到宽度或高度的一半
 @param borderColor  The border stroke color. nil means clear color. - 边框颜色，nil表示透明
 @return The new image with the round corner.
 */
- (nullable UIImage *)sd_roundedCornerImageWithRadius:(CGFloat)cornerRadius
                                              corners:(SDRectCorner)corners
                                          borderWidth:(CGFloat)borderWidth
                                          borderColor:(nullable UIColor *)borderColor;

/**
 Returns a new rotated image (relative to the center).
 返回一个新的旋转后的图像(相对于中心)
 
 @param angle     Rotated radians in counterclockwise.⟲ -  逆时针旋转的弧度
 @param fitSize   YES: new image's size is extend to fit all content. - 是:新图像的大小被扩展到适合所有内容。
                  NO: image's size will not change, content may be clipped. - 否:图像大小不会改变，内容可能会被剪切。
 @return The new image with the rotation. - 带角度的新图像
 */
- (nullable UIImage *)sd_rotatedImageWithAngle:(CGFloat)angle fitSize:(BOOL)fitSize;

/**
 Returns a new horizontally(vertically) flipped image.
 
 返回一个新的水平(垂直)翻转图像
 
 @param horizontal YES to flip the image horizontally. ⇋ - 水平翻转图像
 @param vertical YES to flip the image vertically. ⥯ - 垂直翻转图像
 @return The new image with the flipping. - 翻转后的图像
 */
- (nullable UIImage *)sd_flippedImageWithHorizontal:(BOOL)horizontal vertical:(BOOL)vertical;

#pragma mark - Image Blending 图像混合

/**
 Return a tinted image with the given color. This actually use alpha blending of current image and the tint color.
 
 返回具有给定颜色的着色图像。这实际上使用了当前图像和色调颜色的alpha混合。
 
 @param tintColor  The tint color. - 给定颜色
 @return The new image with the tint color.
 */
- (nullable UIImage *)sd_tintedImageWithColor:(nonnull UIColor *)tintColor;

/**
 Return the pixel color at specify position. The point is from the top-left to the bottom-right and 0-based. The returned the color is always be RGBA format. The image must be CG-based.
 @note The point's x/y should not be smaller than 0, or greater than or equal to width/height.
 @note The overhead of object creation means this method is best suited for infrequent color sampling. For heavy image processing, grab the raw bitmap data and process yourself.

 返回指定位置的像素颜色。这个点从左上到右下，以0为基础。返回的颜色总是RGBA格式。图像必须是基于CG的。
 点的x/y不应小于0，或大于或等于宽/高。
 创建对象的开销意味着这个方法最适合不频繁的颜色采样。对于繁重的图像处理，抓取原始位图数据并自己处理。
 
 @param point The position of pixel - 像素位置
 @return The color for specify pixel, or nil if any error occur - 指定像素的颜色，如果出现任何错误，则为nil
 */
- (nullable UIColor *)sd_colorAtPoint:(CGPoint)point;

/**
 Return the pixel color array with specify rectangle. The rect is from the top-left to the bottom-right and 0-based. The returned the color is always be RGBA format. The image must be CG-based.
 @note The rect's width/height should not be smaller than or equal to 0. The minX/minY should not be smaller than 0. The maxX/maxY should not be greater than width/height. Attention this limit is different from `sd_colorAtPoint:` (point: (0, 0) like rect: (0, 0, 1, 1))
 @note The overhead of object creation means this method is best suited for infrequent color sampling. For heavy image processing, grab the raw bitmap data and process yourself.
 
 返回指定矩形的像素颜色数组。rect是从左上到右下，并且以0为基础。返回的颜色总是RGBA格式。图像必须是基于cg的。
 矩形的宽度/高度不应该小于等于0。minX/minY不能小于0。maxX/maxY不应该大于宽度/高度。注意，这个限制不同于' sd_colorAtPoint: ' (point: (0,0) like rect:(0,0,1,1))。
 创建对象的开销意味着这个方法最适合不频繁的颜色采样。对于繁重的图像处理，抓取原始位图数据并自己处理。
 
 @param rect The rectangle of pixels - 像素矩形尺寸
 @return The color array for specify pixels, or nil if any error occur - 指定像素的颜色数组，如果出现任何错误，则为nil
 */
- (nullable NSArray<UIColor *> *)sd_colorsWithRect:(CGRect)rect;

#pragma mark - Image Effect 图像效果

/**
 Return a new image applied a blur effect.  - 返回一个应用了模糊效果的新图像
 
 @param blurRadius     The radius of the blur in points, 0 means no blur effect. - 模糊的半径以点为单位，0表示没有模糊效果
 
 @return               The new image with blur effect, or nil if an error occurs (e.g. no enough memory). - 带有模糊效果的新图像，如果出现错误(例如内存不足)，则为nil
 */
- (nullable UIImage *)sd_blurredImageWithRadius:(CGFloat)blurRadius;

#if SD_UIKIT || SD_MAC
/**
 Return a new image applied a CIFilter.
 
 返回一个应用了CIFilter的新图像。

 @param filter The CIFilter to be applied to the image. - 应用于图像的CIFilter
 @return The new image with the CIFilter, or nil if an error occurs (e.g. no enough memory). - 使用CIFilter的新图像，如果出现错误(例如内存不足)，则为nil。
 */
- (nullable UIImage *)sd_filteredImageWithFilter:(nonnull CIFilter *)filter;
#endif

@end
