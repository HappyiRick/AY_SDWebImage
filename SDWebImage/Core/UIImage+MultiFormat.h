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
 UIImage category for convenient image format decoding/encoding.
 为了方便图像格式解码/编码
 */
@interface UIImage (MultiFormat)
#pragma mark - Decode
/**
 Create and decode a image with the specify image data - 用指定的图像数据创建和解码图像

 @param data The image data - 图像数据
 @return The created image - 创建的图像
 */
+ (nullable UIImage *)sd_imageWithData:(nullable NSData *)data;

/**
 Create and decode a image with the specify image data and scale - 用指定的图像数据和缩放比创建和解码图像
 
 @param data The image data - 图像数据
 @param scale The image scale factor. Should be greater than or equal to 1.0. - 图像缩放因子，应该大于等于1.0
 @return The created image - 创建的图像
 */
+ (nullable UIImage *)sd_imageWithData:(nullable NSData *)data scale:(CGFloat)scale;

/**
 Create and decode a image with the specify image data and scale, allow specify animate/static control
 用指定的图像数据和缩放比创建和解码图像，允许指定控制动画或静态
 
 @param data The image data - 图像数据
 @param scale The image scale factor. Should be greater than or equal to 1.0. - 图像缩放比因子,应该大于等于1.0
 @param firstFrameOnly Even if the image data is animated image format, decode the first frame only as static image. - 是否只解码第一帧
 @return The created image - 创建的图像
 */
+ (nullable UIImage *)sd_imageWithData:(nullable NSData *)data scale:(CGFloat)scale firstFrameOnly:(BOOL)firstFrameOnly;

#pragma mark - Encode
/**
 Encode the current image to the data, the image format is unspecified
 
 将当前图像编码为二进制数据，图像格式为unspecified

 @note If the receiver is `SDAnimatedImage`, this will return the animated image data if available. No more extra encoding process.
 如果接收器是' sanimatedimage '，这将返回动画图像数据(如果可用)。不再有额外的编码过程。
 @return The encoded data. If can't encode, return nil - 编码的数据，如果不能编码，则返回nil
 */
- (nullable NSData *)sd_imageData;

/**
 Encode the current image to data with the specify image format
 
 将当前图像编码为指定图像格式的数据

 @param imageFormat The specify image format
 @return The encoded data. If can't encode, return nil
 */
- (nullable NSData *)sd_imageDataAsFormat:(SDImageFormat)imageFormat NS_SWIFT_NAME(sd_imageData(as:));

/**
 Encode the current image to data with the specify image format and compression quality
 
 使用指定的图像格式和压缩质量将当前图像编码为数据

 @param imageFormat The specify image format - 指定图像格式
 @param compressionQuality The quality of the resulting image data. Value between 0.0-1.0. Some coders may not support compression quality.
 压缩率，0.0 - 1.0之间，一些编码器可能不支持压缩质量
 @return The encoded data. If can't encode, return nil
 */
- (nullable NSData *)sd_imageDataAsFormat:(SDImageFormat)imageFormat compressionQuality:(double)compressionQuality NS_SWIFT_NAME(sd_imageData(as:compressionQuality:));

/**
 Encode the current image to data with the specify image format and compression quality, allow specify animate/static control
 
 将当前图像编码为指定图像格式和压缩质量的数据，允许指定动画/静态控制
 
 @param imageFormat The specify image format
 @param compressionQuality The quality of the resulting image data. Value between 0.0-1.0. Some coders may not support compression quality.
 @param firstFrameOnly Even if the image is animated image, encode the first frame only as static image.
 @return The encoded data. If can't encode, return nil
 */
- (nullable NSData *)sd_imageDataAsFormat:(SDImageFormat)imageFormat compressionQuality:(double)compressionQuality firstFrameOnly:(BOOL)firstFrameOnly NS_SWIFT_NAME(sd_imageData(as:compressionQuality:firstFrameOnly:));

@end
