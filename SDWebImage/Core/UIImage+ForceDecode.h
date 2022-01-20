/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

/**
 UIImage category about force decode feature (avoid Image/IO's lazy decoding during rendering behavior).
 关于强制解码特性的UIImage类别(避免渲染过程中Image/IO的延迟解码)。
 */
@interface UIImage (ForceDecode)

/**
 A bool value indicating whether the image has already been decoded. This can help to avoid extra force decode.
 一个bool值，指示图像是否已被解码。这可以帮助避免额外的强制解码。
 */
@property (nonatomic, assign) BOOL sd_isDecoded;

/**
 Decode the provided image. This is useful if you want to force decode the image before rendering to improve performance.
 解码提供的图像.如果您想在渲染前强制解码图像以提高性能，这是很有用的

 @param image The image to be decoded - 要解码的图像
 @return The decoded image
 */
+ (nullable UIImage *)sd_decodedImageWithImage:(nullable UIImage *)image;

/**
 Decode and scale down the provided image
 解码并缩放提供的图像

 @param image The image to be decoded
 @return The decoded and scaled down image
 */
+ (nullable UIImage *)sd_decodedAndScaledDownImageWithImage:(nullable UIImage *)image;

/**
 Decode and scale down the provided image with limit bytes
 解码并用指定字节缩放提供的图像
 
 @param image The image to be decoded
 @param bytes The limit bytes size. Provide 0 to use the build-in limit. - 约束的字节大小，传0会使用内置的限制
 @return The decoded and scaled down image
 */
+ (nullable UIImage *)sd_decodedAndScaledDownImageWithImage:(nullable UIImage *)image limitBytes:(NSUInteger)bytes;

@end
