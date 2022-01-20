/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"
#import "SDImageCoder.h"


/**
 This is the protocol for SDAnimatedImage class only but not for SDAnimatedImageCoder. If you want to provide a custom animated image class with full advanced function, you can conform to this instead of the base protocol.
 这是SDAnimatedImage类的协议，但不是SDAnimatedImageCoder。如果你想提供一个具有完整高级功能的自定义动画图像类，你可以遵循这个而不是基本协议。
 */
@protocol SDAnimatedImage <SDAnimatedImageProvider>

@required
/**
 Initializes and returns the image object with the specified data, scale factor and possible animation decoding options.
 
 用指定的数据、缩放因子和可能的动画解码选项初始化并放回图像对象
 
 @note We use this to create animated image instance for normal animation decoding.
 
 @param data The data object containing the image data.
 @param scale The scale factor to assume when interpreting the image data. Applying a scale factor of 1.0 results in an image whose size matches the pixel-based dimensions of the image. Applying a different scale factor changes the size of the image as reported by the `size` property.
 @param options A dictionary containing any animation decoding options.
 @return An initialized object
 */
- (nullable instancetype)initWithData:(nonnull NSData *)data scale:(CGFloat)scale options:(nullable SDImageCoderOptions *)options;

/**
 Initializes the image with an animated coder. You can use the coder to decode the image frame later.
 @note We use this with animated coder which conforms to `SDProgressiveImageCoder` for progressive animation decoding.
 
 用动画解码器初始化图像，可以稍后用解码器解码图像帧
 
 @param animatedCoder An animated coder which conform `SDAnimatedImageCoder` protocol
 @param scale The scale factor to assume when interpreting the image data. Applying a scale factor of 1.0 results in an image whose size matches the pixel-based dimensions of the image. Applying a different scale factor changes the size of the image as reported by the `size` property.
 @return An initialized object
 */
- (nullable instancetype)initWithAnimatedCoder:(nonnull id<SDAnimatedImageCoder>)animatedCoder scale:(CGFloat)scale;

@optional
// These methods are used for optional advanced feature, like image frame preloading. - 这些方法用于可选的高级特征，如图像帧预加载
/**
 Pre-load all animated image frame into memory. Then later frame image request can directly return the frame for index without decoding.
 This method may be called on background thread.
 
 预加载所有动画图像帧到内存。然后以后帧图像请求可以直接返回帧进行索引而无需解码。这个方法可以在后台线程中调用。
 
 @note If one image instance is shared by lots of imageViews, the CPU performance for large animated image will drop down because the request frame index will be random (not in order) and the decoder should take extra effort to keep it re-entrant. You can use this to reduce CPU usage if need. Attention this will consume more memory usage.
 */
- (void)preloadAllFrames;

/**
 Unload all animated image frame from memory if are already pre-loaded. Then later frame image request need decoding. You can use this to free up the memory usage if need.
 
 从内存中卸载所有动画图像帧，如果已经预加载。之后帧图像请求需要解码。如果需要，您可以使用这个来释放内存使用。
 */
- (void)unloadAllFrames;

/**
 Returns a Boolean value indicating whether all animated image frames are already pre-loaded into memory.
 返回一个布尔值，指示是否所有动画图像帧都已预加载到内存中
 */
@property (nonatomic, assign, readonly, getter=isAllFramesLoaded) BOOL allFramesLoaded;

/**
 Return the animated image coder if the image is created with `initWithAnimatedCoder:scale:` method.
 @note We use this with animated coder which conforms to `SDProgressiveImageCoder` for progressive animation decoding.
 
 如果图像是用' initWithAnimatedCoder:scale: '方法创建的，则返回动画图像编码器。
 */
@property (nonatomic, strong, readonly, nullable) id<SDAnimatedImageCoder> animatedCoder;

@end

/**
 The image class which supports animating on `SDAnimatedImageView`. You can also use it on normal UIImageView/NSImageView.
 支持在' SDAnimatedImageView '上进行动画的图像类。你也可以在普通的UIImageView/NSImageView上使用它。
 */
@interface SDAnimatedImage : UIImage <SDAnimatedImage>

// This class override these methods from UIImage(NSImage), and it supports NSSecureCoding. - 这个类覆盖了UIImage(NSImage)中的这些方法，并且它支持NSSecureCoding。
// You should use these methods to create a new animated image. Use other methods just call super instead. - 您应该使用这些方法创建一个新的动画图像。使用其他方法，只需调用super即可
// Pay attention, when the animated image frame count <= 1, all the `SDAnimatedImageProvider` protocol methods will return nil or 0 value, you'd better check the frame count before usage and keep fallback. -  注意，当动画图像帧数<= 1时，所有的' SDAnimatedImageProvider '协议方法将返回nil或0值，你最好在使用前检查帧数并保持后退
+ (nullable instancetype)imageNamed:(nonnull NSString *)name; // Cache in memory, no Asset Catalog support
#if __has_include(<UIKit/UITraitCollection.h>)
+ (nullable instancetype)imageNamed:(nonnull NSString *)name inBundle:(nullable NSBundle *)bundle compatibleWithTraitCollection:(nullable UITraitCollection *)traitCollection; // Cache in memory, no Asset Catalog support
#else
+ (nullable instancetype)imageNamed:(nonnull NSString *)name inBundle:(nullable NSBundle *)bundle; // Cache in memory, no Asset Catalog support
#endif
+ (nullable instancetype)imageWithContentsOfFile:(nonnull NSString *)path;
+ (nullable instancetype)imageWithData:(nonnull NSData *)data;
+ (nullable instancetype)imageWithData:(nonnull NSData *)data scale:(CGFloat)scale;
- (nullable instancetype)initWithContentsOfFile:(nonnull NSString *)path;
- (nullable instancetype)initWithData:(nonnull NSData *)data;
- (nullable instancetype)initWithData:(nonnull NSData *)data scale:(CGFloat)scale;

/**
 Current animated image format. - 当前动画图像格式
 */
@property (nonatomic, assign, readonly) SDImageFormat animatedImageFormat;

/**
 Current animated image data, you can use this to grab the compressed format data and create another animated image instance.
 If this image instance is an animated image created by using animated image coder (which means using the API listed above or using `initWithAnimatedCoder:scale:`), this property is non-nil.
 当前动画图像数据，您可以使用它来获取压缩格式数据并创建另一个动画图像实例。
 如果这个图像实例是一个使用动画图像编码器创建的动画图像(这意味着使用上面列出的API或使用' initWithAnimatedCoder:scale: ')，这个属性是非nil的。
 */
@property (nonatomic, copy, readonly, nullable) NSData *animatedImageData;

/**
 The scale factor of the image.
 图像的缩放因子
 
 @note For UIKit, this just call super instead.
 @note For AppKit, `NSImage` can contains multiple image representations with different scales. However, this class does not do that from the design. We process the scale like UIKit. This will actually be calculated from image size and pixel size.
 */
@property (nonatomic, readonly) CGFloat scale;

// By default, animated image frames are returned by decoding just in time without keeping into memory. But you can choose to preload them into memory as well, See the description in `SDAnimatedImage` protocol. - 默认情况下，动画图像帧通过解码及时返回而不保存到记忆中。但你也可以选择将它们预加载到内存中，参见“SDAnimatedImage”协议中的描述。
// After preloaded, there is no huge difference on performance between this and UIImage's `animatedImageWithImages:duration:`. But UIImage's animation have some issues such like blanking and pausing during segue when using in `UIImageView`. It's recommend to use only if need. - 在预加载后，这个和UIImage的' animatedImageWithImages:duration: '在性能上没有很大的区别。但UIImage的动画有一些问题，比如在' UIImageView '中使用segue时的空白和暂停。建议仅在需要时使用
- (void)preloadAllFrames;
- (void)unloadAllFrames;
@property (nonatomic, assign, readonly, getter=isAllFramesLoaded) BOOL allFramesLoaded;

@end
