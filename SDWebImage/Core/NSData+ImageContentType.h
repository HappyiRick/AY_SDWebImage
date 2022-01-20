/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 * (c) Fabrice Aneche
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"

/**
 You can use switch case like normal enum. It's also recommended to add a default case. You should not assume anything about the raw value.
 For custom coder plugin, it can also extern the enum for supported format. See `SDImageCoder` for more detailed information.
 
 你可以像普通枚举一样使用switch case。还建议添加一个默认大小写。你不应该对原始值做任何假设。
 对于自定义编码器插件，它也可以为支持的格式扩展枚举。请参阅' SDImageCoder '获取更多详细信息
 */
typedef NSInteger SDImageFormat NS_TYPED_EXTENSIBLE_ENUM;
static const SDImageFormat SDImageFormatUndefined = -1;
static const SDImageFormat SDImageFormatJPEG      = 0;
static const SDImageFormat SDImageFormatPNG       = 1;
static const SDImageFormat SDImageFormatGIF       = 2;
static const SDImageFormat SDImageFormatTIFF      = 3;
static const SDImageFormat SDImageFormatWebP      = 4;
static const SDImageFormat SDImageFormatHEIC      = 5;
static const SDImageFormat SDImageFormatHEIF      = 6;
static const SDImageFormat SDImageFormatPDF       = 7;
static const SDImageFormat SDImageFormatSVG       = 8;

/**
 NSData category about the image content type and UTI.
 关于图像内容类型和唯一标识的NSData分类
 */
@interface NSData (ImageContentType)

/**
 *  Return image format - 返回图像格式
 *
 *  @param data the input image data - 输入图像数据
 *
 *  @return the image format as `SDImageFormat` (enum) - SDImageFormat类型表达的图像格式（枚举）
 */
+ (SDImageFormat)sd_imageFormatForImageData:(nullable NSData *)data;

/**
 *  Convert SDImageFormat to UTType - 将 SDImageFormat 格式转化成 UTType
 *
 *  @param format Format as SDImageFormat - SDImageFormat格式
 *  @return The UTType as CFStringRef -  CFStringRef类型表达的UTType
 *  @note For unknown format, `kSDUTTypeImage` abstract type will return - 对于未知的格式，将返回' kSDUTTypeImage '抽象类型
 */
+ (nonnull CFStringRef)sd_UTTypeFromImageFormat:(SDImageFormat)format CF_RETURNS_NOT_RETAINED NS_SWIFT_NAME(sd_UTType(from:));

/**
 *  Convert UTType to SDImageFormat - 将UTType转化为 SDImageFormat 格式
 *
 *  @param uttype The UTType as CFStringRef - CFStringRef类型表达的UTType
 *  @return The Format as SDImageFormat - SDImageFormat格式
 *  @note For unknown type, `SDImageFormatUndefined` will return - 对于未知的格式，将返回' SDImageFormatUndefined '
 */
+ (SDImageFormat)sd_imageFormatFromUTType:(nonnull CFStringRef)uttype;

@end
