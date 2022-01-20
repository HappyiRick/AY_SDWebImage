/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"
/// 无参数回调
typedef void(^SDWebImageNoParamsBlock)(void);
/// 上下文选项
typedef NSString * SDWebImageContextOption NS_EXTENSIBLE_STRING_ENUM;
/// 上下文
typedef NSDictionary<SDWebImageContextOption, id> SDWebImageContext;
/// 可变上下文
typedef NSMutableDictionary<SDWebImageContextOption, id> SDWebImageMutableContext;

#pragma mark - Image scale

/**
 Return the image scale factor for the specify key, supports file name and url key.
 This is the built-in way to check the scale factor when we have no context about it. Because scale factor is not stored in image data (It's typically from filename).
 However, you can also provide custom scale factor as well, see `SDWebImageContextImageScaleFactor`.
 
 返回指定键的图像比例因子，支持文件名和url键
 这是在没有上下文的情况下检查比例因子的内置方法。因为缩放因子不存储在图像数据中(它通常来自filename)。
 但是，你也可以提供自定义的缩放因子，参见“SDWebImageContextImageScaleFactor”。
 
 @param key The image cache key 图像缓存键
 @return The scale factor for image 图像缩放因子
 */
FOUNDATION_EXPORT CGFloat SDImageScaleFactorForKey(NSString * _Nullable key);

/**
 Scale the image with the scale factor for the specify key. If no need to scale, return the original image.
 This works for `UIImage`(UIKit) or `NSImage`(AppKit). And this function also preserve the associated value in `UIImage+Metadata.h`.
 @note This is actually a convenience function, which firstly call `SDImageScaleFactorForKey` and then call `SDScaledImageForScaleFactor`, kept for backward compatibility.
 
 使用指定键的缩放因子缩放图像。如果不需要缩放，返回原始图像。
 这适用于' UIImage ' (UIKit)或' NSImage ' (AppKit)。这个函数也保留了' UIImage+ metadata.h '中的关联值。
 这实际上是一个方便的函数，它首先调用' SDImageScaleFactorForKey '，然后调用' SDScaledImageForScaleFactor '，保持向后兼容。

 @param key The image cache key - 图像缓存key
 @param image The image - 图像
 @return The scaled image - 缩放后的图像
 */
FOUNDATION_EXPORT UIImage * _Nullable SDScaledImageForKey(NSString * _Nullable key, UIImage * _Nullable image);

/**
 Scale the image with the scale factor. If no need to scale, return the original image.
 This works for `UIImage`(UIKit) or `NSImage`(AppKit). And this function also preserve the associated value in `UIImage+Metadata.h`.
 
 用比例因子缩放图像。如果不需要缩放，返回原始图像。
 这适用于' UIImage ' (UIKit)或' NSImage ' (AppKit)。这个函数也保留了" UIImage+ metadata.h "中的相关值
 
 @param scale The image scale factor 图像缩放因子
 @param image The image 图像
 @return The scaled image 缩放后的图像
 */
FOUNDATION_EXPORT UIImage * _Nullable SDScaledImageForScaleFactor(CGFloat scale, UIImage * _Nullable image);

#pragma mark - WebCache Options

/// WebCache options
/// 网络缓存选项
typedef NS_OPTIONS(NSUInteger, SDWebImageOptions) {
    /**
     * By default, when a URL fail to be downloaded, the URL is blacklisted so the library won't keep trying.
     * This flag disable this blacklisting.
     * 默认情况下，当URL下载失败时，该URL将被列入黑名单，因此库不会继续尝试。此标志将禁用此黑名单
     */
    SDWebImageRetryFailed = 1 << 0,
    
    /**
     * By default, image downloads are started during UI interactions, this flags disable this feature,leading to delayed download on UIScrollView deceleration for instance.
     * 默认情况下，图片下载是在UI交互过程中启动的，该标志禁用该功能，导致延迟下载，例如UIScrollView减速。
     */
    SDWebImageLowPriority = 1 << 1,
    
    /**
     * This flag enables progressive download, the image is displayed progressively during download as a browser would do.
     * By default, the image is only displayed once completely downloaded.
     * 这个标志支持渐进式下载，图像会像浏览器一样在下载过程中逐步显示
     * 默认情况下，图像只在下载完成时显示一次
     */
    SDWebImageProgressiveLoad = 1 << 2,
    
    /**
     * Even if the image is cached, respect the HTTP response cache control, and refresh the image from remote location if needed.
     * The disk caching will be handled by NSURLCache instead of SDWebImage leading to slight performance degradation.
     * This option helps deal with images changing behind the same request URL, e.g. Facebook graph api profile pics.
     * If a cached image is refreshed, the completion block is called once with the cached image and again with the final image.
     *
     * 即使缓存了图像，也要遵守HTTP响应缓存控制，并在需要时从远程位置刷新图像
     * 磁盘缓存将由NSURLCache处理，而不是SDWebImage，这会导致轻微的性能下降
     * 这个选项可以帮助处理相同请求URL后的图像变化，例如Facebook图形api配置文件图片。
     * 如果缓存的图像被刷新，完成块会被缓存的图像调用一次，最终的图像会被再次调用
     *
     * Use this flag only if you can't make your URLs static with embedded cache busting parameter.
     * 只有当你不能使用内嵌的缓存中断参数使你的url静态时，才使用这个标志
     */
    SDWebImageRefreshCached = 1 << 3,
    
    /**
     * In iOS 4+, continue the download of the image if the app goes to background. This is achieved by asking the system for extra time in background to let the request finish. If the background task expires the operation will be cancelled.
     * 在ios4 +中，如果应用程序进入后台，继续下载图像。这是通过请求系统在后台提供额外的时间来完成请求来实现的。当后台任务过期时，操作将被取消。
     */
    SDWebImageContinueInBackground = 1 << 4,
    
    /**
     * Handles cookies stored in NSHTTPCookieStore by setting NSMutableURLRequest.HTTPShouldHandleCookies = YES;
     * 通过设置NSMutableURLRequest.HTTPShouldHandleCookies = YES 来处理存储在NSHTTPCookieStore中的cookie
     */
    SDWebImageHandleCookies = 1 << 5,
    
    /**
     * Enable to allow untrusted SSL certificates.
     * Useful for testing purposes. Use with caution in production.
     *
     * 允许使用不信任的SSL证书
     * 测试时使用
     */
    SDWebImageAllowInvalidSSLCertificates = 1 << 6,
    
    /**
     * By default, images are loaded in the order in which they were queued. This flag moves them to the front of the queue.
     * 默认情况下，图像是按照排队的顺序加载的。这个标志将他们移到队列的前面
     */
    SDWebImageHighPriority = 1 << 7,
    
    /**
     * By default, placeholder images are loaded while the image is loading. This flag will delay the loading of the placeholder image until after the image has finished loading.
     * 默认情况下，占位符图像是在图像加载时加载的。这个标志将延迟占位符图片的加载，直到图片加载完成。
     */
    SDWebImageDelayPlaceholder = 1 << 8,
    
    /**
     * We usually don't apply transform on animated images as most transformers could not manage animated images.
     * Use this flag to transform them anyway.
     *
     * 我们通常不会在动画图像上应用transform，因为大多数转换器无法管理动画图像
     * 无论如何，使用这个标志来转换它们
     */
    SDWebImageTransformAnimatedImage = 1 << 9,
    
    /**
     * By default, image is added to the imageView after download. But in some cases, we want to have the hand before setting the image (apply a filter or add it with cross-fade animation for instance)
     * Use this flag if you want to manually set the image in the completion when success
     *
     * 默认情况下，下载后image会被添加到imageView中。但在某些情况下，我们希望在设置图像之前有操作(应用滤镜或添加交叉淡入动画)
     * 如果您想在成功完成时手动设置图像，请使用此标志
     *
     */
    SDWebImageAvoidAutoSetImage = 1 << 10,
    
    /**
     * By default, images are decoded respecting their original size.
     * This flag will scale down the images to a size compatible with the constrained memory of devices.
     * To control the limit memory bytes, check `SDImageCoderHelper.defaultScaleDownLimitBytes` (Defaults to 60MB on iOS)
     * This will actually translate to use context option `.imageThumbnailPixelSize` from v5.5.0 (Defaults to (3966, 3966) on iOS). Previously does not.
     * This flags effect the progressive and animated images as well from v5.5.0. Previously does not.
     * @note If you need detail controls, it's better to use context option `imageThumbnailPixelSize` and `imagePreserveAspectRatio` instead.
     *
     * 默认情况下，图像根据其原始大小进行解码。
     * 该标志将图像缩小到与设备受限内存兼容的大小
     * 要控制限制内存字节，检查' SDImageCoderHelper.defaultScaleDownLimitBytes ' (iOS默认为60MB)
     * 这将实际转换为使用上下文选项'。imageThumbnailPixelSize ' from v5.5.0 (iOS上默认为(3966,3966))。以前没有。
     * 从v5.5.0开始，这个标志也会影响渐进图像和动画图像。以前没有。
     * 如果你需要细节控制，最好使用上下文选项' imageThumbnailPixelSize '和' imagePreserveAspectRatio '代替。
     *
     */
    SDWebImageScaleDownLargeImages = 1 << 11,
    
    /**
     * By default, we do not query image data when the image is already cached in memory. This mask can force to query image data at the same time. However, this query is asynchronously unless you specify `SDWebImageQueryMemoryDataSync`
     *
     * 默认情况下，当映像已经缓存到内存中时，我们不查询映像数据。此掩码可以强制同时查询图像数据。然而，这个查询是异步的，除非你指定' SDWebImageQueryMemoryDataSync '
     */
    SDWebImageQueryMemoryData = 1 << 12,
    
    /**
     * By default, when you only specify `SDWebImageQueryMemoryData`, we query the memory image data asynchronously. Combined this mask as well to query the memory image data synchronously.
     * @note Query data synchronously is not recommend, unless you want to ensure the image is loaded in the same runloop to avoid flashing during cell reusing.
     *
     * 默认情况下，当你只指定' SDWebImageQueryMemoryData '时，我们异步查询内存图像数据。结合这个掩码也可以同步查询内存图像数据。
     * 不建议同步查询数据，除非你想确保图像被加载在同一个运行循环中，以避免在单元格重用期间闪烁。
     */
    SDWebImageQueryMemoryDataSync = 1 << 13,
    
    /**
     * By default, when the memory cache miss, we query the disk cache asynchronously. This mask can force to query disk cache (when memory cache miss) synchronously.
     * @note These 3 query options can be combined together. For the full list about these masks combination, see wiki page.
     * @note Query data synchronously is not recommend, unless you want to ensure the image is loaded in the same runloop to avoid flashing during cell reusing.
     * 默认情况下，当内存缓存丢失时，我们将异步查询磁盘缓存。这个掩码可以强制同步查询磁盘缓存(当内存缓存丢失时)。
     * 这3个查询选项可以组合在一起。有关这些蒙版组合的完整列表，请参阅wiki页面。
     * 不建议同步查询数据，除非你想确保图像被加载在同一个运行循环中，以避免在单元格重用期间闪烁。
     *
     */
    SDWebImageQueryDiskDataSync = 1 << 14,
    
    /**
     * By default, when the cache missed, the image is load from the loader. This flag can prevent this to load from cache only.
     * 默认情况下，当缓存丢失时，图像将从加载器中加载。这个标志可以防止只从缓存加载。
     */
    SDWebImageFromCacheOnly = 1 << 15,
    
    /**
     * By default, we query the cache before the image is load from the loader. This flag can prevent this to load from loader only.
     * 默认情况下，在图像从加载器加载之前会查询缓存。该标志可以防止值从加载器加载。
     */
    SDWebImageFromLoaderOnly = 1 << 16,
    
    /**
     * By default, when you use `SDWebImageTransition` to do some view transition after the image load finished, this transition is only applied for image when the callback from manager is asynchronous (from network, or disk cache query)
     * This mask can force to apply view transition for any cases, like memory cache query, or sync disk cache query.
     *
     * 默认情况下，当你使用' SDWebImageTransition '在图像加载完成后做一些视图转换，这种转换只适用于从管理器异步回调(从网络，或磁盘缓存查询)的图像
     * 这个掩码可以强制在任何情况下应用视图转换，比如内存缓存查询，或者同步磁盘缓存查询。
     */
    SDWebImageForceTransition = 1 << 17,
    
    /**
     * By default, we will decode the image in the background during cache query and download from the network. This can help to improve performance because when rendering image on the screen, it need to be firstly decoded. But this happen on the main queue by Core Animation.
     * However, this process may increase the memory usage as well. If you are experiencing a issue due to excessive memory consumption, This flag can prevent decode the image.
     * 默认情况下，我们会在缓存查询和从网络下载时在后台解码图像。这有助于提高性能，因为在屏幕上呈现图像时，首先需要对图像进行解码。但这发生在Core Animation的主队列上
     * 但是，这个进程也会增加内存的使用。如果您遇到了由于过度内存消耗而引起的问题，此标志可以阻止解码图像。
     */
    SDWebImageAvoidDecodeImage = 1 << 18,
    
    /**
     * By default, we decode the animated image. This flag can force decode the first frame only and produce the static image.
     * 默认情况下，我们解码动画图像.该标志可以强制仅解码第一帧且创建一个静态图像
     */
    SDWebImageDecodeFirstFrameOnly = 1 << 19,
    
    /**
     * By default, for `SDAnimatedImage`, we decode the animated image frame during rendering to reduce memory usage. However, you can specify to preload all frames into memory to reduce CPU usage when the animated image is shared by lots of imageViews.
     * This will actually trigger `preloadAllAnimatedImageFrames` in the background queue(Disk Cache & Download only).
     * 默认情况下，对于“SDAnimatedImage”，我们在渲染过程中解码动画图像帧以减少内存的使用。然而，当动画图像被大量imageViews共享时，你可以指定将所有帧预加载到内存中以减少CPU占用。
     * 这将在后台队列中触发' preloadAllAnimatedImageFrames '(仅限磁盘缓存和下载)。
     */
    SDWebImagePreloadAllFrames = 1 << 20,
    
    /**
     * By default, when you use `SDWebImageContextAnimatedImageClass` context option (like using `SDAnimatedImageView` which designed to use `SDAnimatedImage`), we may still use `UIImage` when the memory cache hit, or image decoder is not available to produce one exactlly matching your custom class as a fallback solution.
     * Using this option, can ensure we always callback image with your provided class. If failed to produce one, a error with code `SDWebImageErrorBadImageData` will been used.
     * Note this options is not compatible with `SDWebImageDecodeFirstFrameOnly`, which always produce a UIImage/NSImage.
     *
     * 默认情况下,当您使用“SDWebImageContextAnimatedImageClass”上下文选项(如使用“SDAnimatedImageView”旨在使用“SDAnimatedImage”),我们可能仍在使用的用户界面图像内存缓存命中时,或图像解码器没有产生一个确切匹配您的自定义类作为后备方案。
     * 使用这个选项，可以确保我们总是用你提供的类回调图像。如果没有生成，则会使用代码' SDWebImageErrorBadImageData '的错误。
     * 注意这个选项不兼容' SDWebImageDecodeFirstFrameOnly '，它总是产生一个UIImage/NSImage。
     */
    SDWebImageMatchAnimatedImageClass = 1 << 21,
    
    /**
     * By default, when we load the image from network, the image will be written to the cache (memory and disk, controlled by your `storeCacheType` context option)
     * This maybe an asynchronously operation and the final `SDInternalCompletionBlock` callback does not guarantee the disk cache written is finished and may cause logic error. (For example, you modify the disk data just in completion block, however, the disk cache is not ready)
     * If you need to process with the disk cache in the completion block, you should use this option to ensure the disk cache already been written when callback.
     * Note if you use this when using the custom cache serializer, or using the transformer, we will also wait until the output image data written is finished.
     *
     * 默认情况下，当我们从网络加载图像时，图像将被写入缓存(内存和磁盘，由您的' storeCacheType '上下文选项控制)
     * 这可能是一个异步操作，并且最终的' SDInternalCompletionBlock '回调不保证磁盘缓存写入完成，可能会导致逻辑错误。(例如，您只是在完成块中修改磁盘数据，然而，磁盘缓存还没有准备好)
     * 如果您需要在完成块中处理磁盘缓存，您应该使用这个选项来确保在回调时磁盘缓存已经被写入。
     * 请注意，如果您在使用自定义缓存序列化器或转换器时使用此方法，我们也将等待输出图像数据写入完成。
     */
    SDWebImageWaitStoreCache = 1 << 22,
    
    /**
     * We usually don't apply transform on vector images, because vector images supports dynamically changing to any size, rasterize to a fixed size will loss details. To modify vector images, you can process the vector data at runtime (such as modifying PDF tag / SVG element).
     * Use this flag to transform them anyway.
     *
     * 我们通常不会在矢量图像上应用transform，因为矢量图像支持动态改变到任何大小，栅格化到固定大小会丢失细节。要修改矢量图像，可以在运行时处理矢量数据(例如修改PDF标记/ SVG元素)。
     * 无论如何，使用这个标志来转换它们
     */
    SDWebImageTransformVectorImage = 1 << 23
};


#pragma mark - Context Options

/**
 A String to be used as the operation key for view category to store the image load operation. This is used for view instance which supports different image loading process. If nil, will use the class name as operation key. (NSString *)
 一个字符串，用作视图类别的操作键，以存储图像加载操作。这是用于视图实例，支持不同的图像加载过程。如果为nil，将使用类名作为操作键。(NSString *)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextSetImageOperationKey;

/**
 A SDWebImageManager instance to control the image download and cache process using in UIImageView+WebCache category and likes. If not provided, use the shared manager (SDWebImageManager *)
 @deprecated Deprecated in the future. This context options can be replaced by other context option control like `.imageCache`, `.imageLoader`, `.imageTransformer` (See below), which already matches all the properties in SDWebImageManager.
 
 一个SDWebImageManager实例，用来控制图像下载和缓存的过程，使用UIImageView+WebCache类。如果没有提供，使用共享管理器(SDWebImageManager *)
 @deprecated将来已弃用。这个上下文选项可以被其他上下文选项控制代替，比如'。imageCache ', '。imageLoader ', '。imageTransformer的(见下文)，它已经匹配了SDWebImageManager中的所有属性。
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextCustomManager API_DEPRECATED("Use individual context option like .imageCache, .imageLoader and .imageTransformer instead", macos(10.10, API_TO_BE_DEPRECATED), ios(8.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED));

/**
 A id<SDImageCache> instance which conforms to `SDImageCache` protocol. It's used to override the image manager's cache during the image loading pipeline.
 In other word, if you just want to specify a custom cache during image loading, you don't need to re-create a dummy SDWebImageManager instance with the cache. If not provided, use the image manager's cache (id<SDImageCache>)
 一个id<SDImageCache>实例，它符合' SDImageCache '协议。它用于在图像加载过程中覆盖图像管理器的缓存。
 换句话说，如果你只是想在图像加载过程中指定一个自定义缓存，你不需要用缓存重新创建一个虚拟SDWebImageManager实例。如果没有提供，使用映像管理器的缓存(id<SDImageCache>)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextImageCache;

/**
 A id<SDImageLoader> instance which conforms to `SDImageLoader` protocol. It's used to override the image manager's loader during the image loading pipeline.
 In other word, if you just want to specify a custom loader during image loading, you don't need to re-create a dummy SDWebImageManager instance with the loader. If not provided, use the image manager's cache (id<SDImageLoader>)
 一个遵循'SDImageLoader'协议的实例.用来在图像加载时间线中重写图像管理类的加载
 换句话说，如果你只想在图像加载过程中指定一个自定义加载器，你不需要用这个加载器重新创建一个虚拟的SDWebImageManager实例。如果没有提供，请使用映像管理器的缓存(id<SDImageLoader>)
*/
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextImageLoader;

/**
 A id<SDImageCoder> instance which conforms to `SDImageCoder` protocol. It's used to override the default image coder for image decoding(including progressive) and encoding during the image loading process.
 If you use this context option, we will not always use `SDImageCodersManager.shared` to loop through all registered coders and find the suitable one. Instead, we will arbitrarily use the exact provided coder without extra checking (We may not call `canDecodeFromData:`).
 @note This is only useful for cases which you can ensure the loading url matches your coder, or you find it's too hard to write a common coder which can used for generic usage. This will bind the loading url with the coder logic, which is not always a good design, but possible. (id<SDImageCache>)
 一个遵循‘SDImageCoder’协议的实例，用来在图像加载过程中重写默认解码器来做编解码
 如果您使用此上下文选项，我们将不总是使用' SDImageCodersManager.Shared '循环遍历所有注册的编码器，并找到合适的。
 相反，我们将使用精确提供的编码器，而不需要额外的检查(我们可能不会调用' canDecodeFromData: ')
 @note 这只在你可以确保加载url匹配你的编码器的情况下有用，或者你发现很难写一个通用的编码器可以用于通用用法。这将绑定加载url与编码器逻辑，这不是一个好的设计，但可能存在。(id < SDImageCache >)
*/
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextImageCoder;

/**
 A id<SDImageTransformer> instance which conforms `SDImageTransformer` protocol. It's used for image transform after the image load finished and store the transformed image to cache. If you provide one, it will ignore the `transformer` in manager and use provided one instead. If you pass NSNull, the transformer feature will be disabled. (id<SDImageTransformer>)
 
 一个id<SDImageTransformer>实例，它符合' SDImageTransformer '协议。它用于图像加载完成后的图像转换，并将转换后的图像存储到缓存中。如果您提供了一个，它将忽略管理器中的“transformer”，而使用提供的一个。如果传递了NSNull, transformer特性将被禁用。(id < SDImageTransformer >)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextImageTransformer;

/**
 A CGFloat raw value which specify the image scale factor. The number should be greater than or equal to 1.0. If not provide or the number is invalid, we will use the cache key to specify the scale factor. (NSNumber)
 
 CGFloat原始值，指定图像比例因子。该值应该大于或等于1.0。如果没有提供或者数字无效，我们将使用缓存键来指定比例因子。(NSNumber)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextImageScaleFactor;

/**
 A Boolean value indicating whether to keep the original aspect ratio when generating thumbnail images (or bitmap images from vector format).
 Defaults to YES. (NSNumber)
 
 一个布尔值，指示在生成缩略图(或矢量格式的位图)时是否保留原始的纵横比
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextImagePreserveAspectRatio;

/**
 A CGSize raw value indicating whether or not to generate the thumbnail images (or bitmap images from vector format). When this value is provided, the decoder will generate a thumbnail image which pixel size is smaller than or equal to (depends the `.imagePreserveAspectRatio`) the value size.
 @note When you pass `.preserveAspectRatio == NO`, the thumbnail image is stretched to match each dimension. When `.preserveAspectRatio == YES`, the thumbnail image's width is limited to pixel size's width, the thumbnail image's height is limited to pixel size's height. For common cases, you can just pass a square size to limit both.
 Defaults to CGSizeZero, which means no thumbnail generation at all. (NSValue)
 
 CGSize原始值，指示是否生成缩略图图像(或从矢量格式生成的位图图像)。当提供这个值时，解码器将生成一个像素大小小于或等于该值大小(取决于'.imagePreserveAspectRatio)的缩略图。
 @note当你通过'.preserveAspectRatio == NO '，缩略图图像被拉伸以匹配每个维度。当”.preserveAspectRatio == YES '，缩略图的宽度限制为像素大小的宽度，缩略图的高度限制为像素大小的高度。在一般情况下，您可以传递一个正方形大小来限制这两者。
 默认为CGSizeZero, 意味着没有缩略图产生 (NSValue)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextImageThumbnailPixelSize;

/**
 A SDImageCacheType raw value which specify the source of cache to query. Specify `SDImageCacheTypeDisk` to query from disk cache only; `SDImageCacheTypeMemory` to query from memory only. And `SDImageCacheTypeAll` to query from both memory cache and disk cache. Specify `SDImageCacheTypeNone` is invalid and totally ignore the cache query.
 If not provide or the value is invalid, we will use `SDImageCacheTypeAll`. (NSNumber)
 
 指定要查询的缓存源的SDImageCacheType原始值。指定' SDImageCacheTypeDisk '只从磁盘缓存查询;' SDImageCacheTypeMemory '仅从内存中查询。和' SDImageCacheTypeAll '查询从内存缓存和磁盘缓存。指定' SDImageCacheTypeNone '是无效的，并且完全忽略缓存查询。
 如果没有提供或者该值不合规，我们将使用`SDImageCacheTypeAll`. (NSNumber)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextQueryCacheType;

/**
 A SDImageCacheType raw value which specify the store cache type when the image has just been downloaded and will be stored to the cache. Specify `SDImageCacheTypeNone` to disable cache storage; `SDImageCacheTypeDisk` to store in disk cache only; `SDImageCacheTypeMemory` to store in memory only. And `SDImageCacheTypeAll` to store in both memory cache and disk cache.
 If you use image transformer feature, this actually apply for the transformed image, but not the original image itself. Use `SDWebImageContextOriginalStoreCacheType` if you want to control the original image's store cache type at the same time.
 If not provide or the value is invalid, we will use `SDImageCacheTypeAll`. (NSNumber)
 
 指定当前图像刚被下载时的原始值，并将其存储到缓存中。指定`SDImageCacheTypeNone`会使缓存失效，`SDImageCacheTypeDisk`表示仅存储到硬盘缓存中，`SDImageCacheTypeMemory`表示仅存储到内存中，`SDImageCacheTypeAll`表示硬盘和内存中都存储
 如果使用图像变换特性，这实际上适用于变换后的图像，而不是原始图像本身。如果你想同时控制原始图像的存储缓存类型，使用“SDWebImageContextOriginalStoreCacheType”。
 如果没有使用或者该值不合规，我们将使用`SDImageCacheTypeAll`. (NSNumber)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextStoreCacheType;

/**
 The same behavior like `SDWebImageContextQueryCacheType`, but control the query cache type for the original image when you use image transformer feature. This allows the detail control of cache query for these two images. For example, if you want to query the transformed image from both memory/disk cache, query the original image from disk cache only, use `[.queryCacheType : .all, .originalQueryCacheType : .disk]`
 If not provide or the value is invalid, we will use `SDImageCacheTypeDisk`, which query the original full image data from disk cache after transformed image cache miss. This is suitable for most common cases to avoid re-downloading the full data for different transform variants. (NSNumber)
 @note Which means, if you set this value to not be `.none`, we will query the original image from cache, then do transform with transformer, instead of actual downloading, which can save bandwidth usage.
 
 相同的行为像' SDWebImageContextQueryCacheType '，但当你使用图像转换器功能时，控制原始图像的查询缓存类型。这允许对这两个图像的缓存查询进行详细控制。例如，如果你想从内存/磁盘缓存中查询转换后的图像，只查询磁盘缓存中的原始图像，使用'[.queryCacheType: .all， .originalQueryCacheType: .disk] '
 如果没有提供或值无效，我们将使用' SDImageCacheTypeDisk '，它将在转换后的图像缓存丢失后从磁盘缓存中查询原始的完整图像数据。这适用于大多数常见的情况，以避免重新下载不同转换变体的完整数据。(NSNumber)
 这意味着，如果你设置这个值不是'。None '，我们将从缓存中查询原始图像，然后用转换器进行转换，而不是实际下载，这样可以节省带宽的使用。
 
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextOriginalQueryCacheType;

/**
 The same behavior like `SDWebImageContextStoreCacheType`, but control the store cache type for the original image when you use image transformer feature. This allows the detail control of cache storage for these two images. For example, if you want to store the transformed image into both memory/disk cache, store the original image into disk cache only, use `[.storeCacheType : .all, .originalStoreCacheType : .disk]`
 If not provide or the value is invalid, we will use `SDImageCacheTypeDisk`, which store the original full image data into disk cache after storing the transformed image. This is suitable for most common cases to avoid re-downloading the full data for different transform variants. (NSNumber)
 @note This only store the original image, if you want to use the original image without downloading in next query, specify `SDWebImageContextOriginalQueryCacheType` as well.
 
 相同的行为像' SDWebImageContextStoreCacheType '，但当你使用图像转换器功能时，控制原始图像的存储缓存类型。这允许对这两个图像的缓存存储进行详细控制。例如，如果你想将转换后的图像存储到内存/磁盘缓存中，只将原始图像存储到磁盘缓存中，使用'[.storeCacheType: .all， .originalStoreCacheType: .disk]
 如果没有提供或值无效，我们将使用' SDImageCacheTypeDisk '，它在存储转换后将原始完整图像数据存储到磁盘缓存中。这适用于大多数常见情况，以避免重新下载不同转换变体的完整数据。(NSNumber)
 @note这只存储原始图像，如果你想使用原始图像而不下载在下次查询，也指定' SDWebImageContextOriginalQueryCacheType '。
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextOriginalStoreCacheType;

/**
 A id<SDImageCache> instance which conforms to `SDImageCache` protocol. It's used to control the cache for original image when using the transformer. If you provide one, the original image (full size image) will query and write from that cache instance instead, the transformed image will query and write from the default `SDWebImageContextImageCache` instead. (id<SDImageCache>)
 
 一个id<SDImageCache>实例，它符合' SDImageCache '协议。它用于在使用转换器时控制原始图像的缓存。如果你提供了一个，原始图像(全尺寸图像)将查询和写入缓存实例，转换后的图像将查询和写入默认的' SDWebImageContextImageCache '。(id < SDImageCache >)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextOriginalImageCache;

/**
 A Class object which the instance is a `UIImage/NSImage` subclass and adopt `SDAnimatedImage` protocol. We will call `initWithData:scale:options:` to create the instance (or `initWithAnimatedCoder:scale:` when using progressive download) . If the instance create failed, fallback to normal `UIImage/NSImage`.
 This can be used to improve animated images rendering performance (especially memory usage on big animated images) with `SDAnimatedImageView` (Class).
 
 一个Class对象，它的实例是' UIImage/NSImage '的子类，并采用' SDAnimatedImage '协议。我们将调用' initWithData:scale:options: '来创建实例(或' initWithAnimatedCoder:scale: '当使用渐进式下载时)。如果实例创建失败，回退到正常的' UIImage/NSImage '。
 这可以用' SDAnimatedImageView ' (Class)来提高动画图像的渲染性能(特别是大型动画图像的内存使用)。
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextAnimatedImageClass;

/**
 A id<SDWebImageDownloaderRequestModifier> instance to modify the image download request. It's used for downloader to modify the original request from URL and options. If you provide one, it will ignore the `requestModifier` in downloader and use provided one instead. (id<SDWebImageDownloaderRequestModifier>)
 
 一个id<SDWebImageDownloaderRequestModifier>实例，用于修改图片下载请求。它用于下载程序修改来自URL和选项的原始请求。如果你提供了一个，它将忽略在downloader中的' requestModifier '和使用提供的代替。(id < SDWebImageDownloaderRequestModifier >)
 
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextDownloadRequestModifier;

/**
 A id<SDWebImageDownloaderResponseModifier> instance to modify the image download response. It's used for downloader to modify the original response from URL and options.  If you provide one, it will ignore the `responseModifier` in downloader and use provided one instead. (id<SDWebImageDownloaderResponseModifier>)
 
 一个id<SDWebImageDownloaderResponseModifier>实例，用于修改图像下载响应。它用于下载程序修改来自URL和选项的原始响应。如果你提供一个，它将忽略在downloader中的' responseModifier '和使用提供的代替。(id < SDWebImageDownloaderResponseModifier >)
 
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextDownloadResponseModifier;

/**
 A id<SDWebImageContextDownloadDecryptor> instance to decrypt the image download data. This can be used for image data decryption, such as Base64 encoded image. If you provide one, it will ignore the `decryptor` in downloader and use provided one instead. (id<SDWebImageContextDownloadDecryptor>)
 
 一个id<SDWebImageContextDownloadDecryptor>实例来解密图像下载数据。这可以用于图像数据解密，例如Base64编码的图像。如果你提供了一个，它将忽略在downloader中的' decryptor '而使用提供的一个。(id < SDWebImageContextDownloadDecryptor >)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextDownloadDecryptor;

/**
 A id<SDWebImageCacheKeyFilter> instance to convert an URL into a cache key. It's used when manager need cache key to use image cache. If you provide one, it will ignore the `cacheKeyFilter` in manager and use provided one instead. (id<SDWebImageCacheKeyFilter>)
 
 一个id<SDWebImageCacheKeyFilter>实例，用于将URL转换为缓存键。当管理器需要缓存键来使用图像缓存时使用。如果你提供了一个，它会忽略管理器中的' cacheKeyFilter '，而使用提供的一个。(id < SDWebImageCacheKeyFilter >)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextCacheKeyFilter;

/**
 A id<SDWebImageCacheSerializer> instance to convert the decoded image, the source downloaded data, to the actual data. It's used for manager to store image to the disk cache. If you provide one, it will ignore the `cacheSerializer` in manager and use provided one instead. (id<SDWebImageCacheSerializer>)
 
 一个id<SDWebImageCacheSerializer>实例，将解码后的图像、源下载的数据转换为实际的数据。它用于管理器将映像存储到磁盘缓存中。如果你提供了一个，它会忽略manager中的' cacheSerializer '，而使用提供的一个。(id < SDWebImageCacheSerializer >)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextCacheSerializer;
