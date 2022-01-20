/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

#if SD_MAC

/**
 This category is provided to easily write cross-platform(AppKit/UIKit) code. For common usage, see `UIImage+Metadata.h`.
 这个分类为为简单跨平台代码提供支持.使用可以查看`UIImage+Metadata.h`
 */
@interface NSImage (Compatibility)

/**
The underlying Core Graphics image object. This will actually use `CGImageForProposedRect` with the image size.
 底层Core Graphics图像对象。这将实际使用' CGImageForProposedRect '与图像大小
 */
@property (nonatomic, readonly, nullable) CGImageRef CGImage;
/**
 The underlying Core Image data. This will actually use `bestRepresentationForRect` with the image size to find the `NSCIImageRep`.
 底层的Core Image数据。这将使用带有图像大小的" bestRepresentationForRect "来找到" NSCIImageRep "
 */
@property (nonatomic, readonly, nullable) CIImage *CIImage;
/**
 The scale factor of the image. This wil actually use `bestRepresentationForRect` with image size and pixel size to calculate the scale factor. If failed, use the default value 1.0. Should be greater than or equal to 1.0.
 图像的比例因子。这实际上将使用' bestRepresentationForRect '与图像大小和像素大小来计算比例因子。如果失败，则使用默认值1.0。应该大于或等于1.0
 */
@property (nonatomic, readonly) CGFloat scale;

// These are convenience methods to make AppKit's `NSImage` match UIKit's `UIImage` behavior. The scale factor should be greater than or equal to 1.0.
// 这些是让AppKit的“NSImage”匹配UIKit的“UIImage”行为的便利方法。比例系数应大于或等于1.0

/**
 Returns an image object with the scale factor and orientation. The representation is created from the Core Graphics image object.
 @note The difference between this and `initWithCGImage:size` is that `initWithCGImage:size` will actually create a `NSCGImageSnapshotRep` representation and always use `backingScaleFactor` as scale factor. So we should avoid it and use `NSBitmapImageRep` with `initWithCGImage:` instead.
 @note The difference between this and UIKit's `UIImage` equivalent method is the way to process orientation. If the provided image orientation is not equal to Up orientation, this method will firstly rotate the CGImage to the correct orientation to work compatible with `NSImageView`. However, UIKit will not actually rotate CGImage and just store it as `imageOrientation` property.

 返回具有比例因子和方向的图像对象。表示是从Core Graphics图像对象创建的。
 这个和initWithCGImage:size的区别是，initWithCGImage:size会创建一个“NSCGImageSnapshotRep”表示，并且总是使用“backingScaleFactor”作为比例因子。所以我们应该避免它，使用' NSBitmapImageRep '和' initWithCGImage: '代替
 这个方法和UIKit的“UIImage”等效方法的区别在于处理方向的方式。如果提供的图像方向不等于向上的方向，该方法将首先旋转CGImage到正确的方向，以兼容“NSImageView”。然而，UIKit实际上不会旋转CGImage，只是将它存储为imageOrientation属性。
 
 @param cgImage A Core Graphics image object - 一个Core Graphics图像对象
 @param scale The image scale factor - 图像缩放比因子
 @param orientation The orientation of the image data - 图像数据的方向
 @return The image object - 图像对象
 */
- (nonnull instancetype)initWithCGImage:(nonnull CGImageRef)cgImage scale:(CGFloat)scale orientation:(CGImagePropertyOrientation)orientation;

/**
 Initializes and returns an image object with the specified Core Image object. The representation is `NSCIImageRep`.
 
 使用指定的Core image对象初始化并返回一个图像对象。表示是' NSCIImageRep '
 
 @param ciImage A Core Image image object - 一个Core Image图像对象
 @param scale The image scale factor - 图像缩放比因子
 @param orientation The orientation of the image data - 图像数据的方向
 @return The image object - 图形对象
 */
- (nonnull instancetype)initWithCIImage:(nonnull CIImage *)ciImage scale:(CGFloat)scale orientation:(CGImagePropertyOrientation)orientation;

/**
 Returns an image object with the scale factor. The representation is created from the image data.
 @note The difference between these this and `initWithData:` is that `initWithData:` will always use `backingScaleFactor` as scale factor.
 
 返回带有比例因子的图像对象。表示是根据图像数据创建的
 this和' initWithData: '之间的区别是' initWithData: '将始终使用' backingScaleFactor '作为缩放因子。

 @param data The image data - 图像数据
 @param scale The image scale factor - 图像缩放比因子
 @return The image object - 图像对象
 */
- (nullable instancetype)initWithData:(nonnull NSData *)data scale:(CGFloat)scale;

@end

#endif
