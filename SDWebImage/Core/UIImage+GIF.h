/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 * (c) Laurin Brandner
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

/**
 This category is just use as a convenience method. For more detail control, use methods in `UIImage+MultiFormat.h` or directly use `SDImageCoder`.
 这个类别只是作为一个方便的方法使用。更多细节控制，使用' UIImage+MultiFormat.h '中的方法或直接使用' SDImageCoder '。
 */
@interface UIImage (GIF)

/**
 Creates an animated UIImage from an NSData. - 从二进制数据创建一个动画图像
 This will create animated image if the data is Animated GIF. And will create a static image is the data is Static GIF.
 这将创建动画图像，如果数据是动画GIF。会创建一个静态图像数据是静态GIF

 @param data The GIF data - GIF 数据
 @return The created image - 创建的图像
 */
+ (nullable UIImage *)sd_imageWithGIFData:(nullable NSData *)data;

@end
