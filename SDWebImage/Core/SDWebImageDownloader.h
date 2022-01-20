/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"
#import "SDWebImageDefine.h"
#import "SDWebImageOperation.h"
#import "SDWebImageDownloaderConfig.h"
#import "SDWebImageDownloaderRequestModifier.h"
#import "SDWebImageDownloaderResponseModifier.h"
#import "SDWebImageDownloaderDecryptor.h"
#import "SDImageLoader.h"

/// Downloader options
/// 下载选项
typedef NS_OPTIONS(NSUInteger, SDWebImageDownloaderOptions) {
    /**
     * Put the download in the low queue priority and task priority.
     * 将下载任务置于低队列优先级和任务优先级中
     */
    SDWebImageDownloaderLowPriority = 1 << 0,
    
    /**
     * This flag enables progressive download, the image is displayed progressively during download as a browser would do.
     * 这个标志支持渐进式下载，图像会像浏览器一样在下载过程中逐步显示
     */
    SDWebImageDownloaderProgressiveLoad = 1 << 1,

    /**
     * By default, request prevent the use of NSURLCache. With this flag, NSURLCache is used with default policies.
     * 默认情况下，request阻止NSURLCache的使用。有了这个标志，NSURLCache在默认策略中被使用。
     */
    SDWebImageDownloaderUseNSURLCache = 1 << 2,

    /**
     * Call completion block with nil image/imageData if the image was read from NSURLCache
     * And the error code is `SDWebImageErrorCacheNotModified`
     * This flag should be combined with `SDWebImageDownloaderUseNSURLCache`.
     * 如果图像是从NSURLCache中读取的，调用带有nil image/imageData的完成块
     * 错误代码是" SDWebImageErrorCacheNotModified "
     * 这个标志应该与“SDWebImageDownloaderUseNSURLCache”相结合。
     */
    SDWebImageDownloaderIgnoreCachedResponse = 1 << 3,
    
    /**
     * In iOS 4+, continue the download of the image if the app goes to background. This is achieved by asking the system for
     * extra time in background to let the request finish. If the background task expires the operation will be cancelled.
     * 在iOS 4+中，如果应用程序进入后台，继续下载图像。这是通过请求系统在后台提供额外的时间来完成请求来实现的。当后台任务过期时，操作将被取消。
     */
    SDWebImageDownloaderContinueInBackground = 1 << 4,

    /**
     * Handles cookies stored in NSHTTPCookieStore by setting 
     * NSMutableURLRequest.HTTPShouldHandleCookies = YES;
     * 通过设置处理存储在NSHTTPCookieStore的cookie
     * NSMutableURLRequest.HTTPShouldHandleCookies = YES;
     */
    SDWebImageDownloaderHandleCookies = 1 << 5,

    /**
     * Enable to allow untrusted SSL certificates.
     * Useful for testing purposes. Use with caution in production.
     * 启用不受信任的SSL证书
     * 用于测试目的。在生产中谨慎使用
     */
    SDWebImageDownloaderAllowInvalidSSLCertificates = 1 << 6,

    /**
     * Put the download in the high queue priority and task priority.
     * 将下载放入高队列优先级和任务优先级中
     */
    SDWebImageDownloaderHighPriority = 1 << 7,
    
    /**
     * By default, images are decoded respecting their original size. On iOS, this flag will scale down the images to a size compatible with the constrained memory of devices.
     * This flag take no effect if `SDWebImageDownloaderAvoidDecodeImage` is set. And it will be ignored if `SDWebImageDownloaderProgressiveLoad` is set.
     * 默认情况下，图像根据其原始大小进行解码。在iOS上，这个标志将缩小图像到与设备有限的内存兼容的大小。
     * 如果设置了“SDWebImageDownloaderAvoidDecodeImage”，则此标志无效。如果设置了“SDWebImageDownloaderProgressiveLoad”，它将被忽略。
     */
    SDWebImageDownloaderScaleDownLargeImages = 1 << 8,
    
    /**
     * By default, we will decode the image in the background during cache query and download from the network. This can help to improve performance because when rendering image on the screen, it need to be firstly decoded. But this happen on the main queue by Core Animation.
     * However, this process may increase the memory usage as well. If you are experiencing a issue due to excessive memory consumption, This flag can prevent decode the image.
     * 默认情况下，我们会在缓存查询和从网络下载时在后台解码图像。这有助于提高性能，因为在屏幕上呈现图像时，首先需要对图像进行解码。但这发生在Core Animation的主队列上
     * 但是，这个进程也会增加内存的使用。如果您遇到了由于过度内存消耗而引起的问题，此标志可以阻止解码图像。
     */
    SDWebImageDownloaderAvoidDecodeImage = 1 << 9,
    
    /**
     * By default, we decode the animated image. This flag can force decode the first frame only and produce the static image.
     * 默认情况下，解码动画图像。这一标志可以强制仅解码第一帧并且创建静态图片。
     */
    SDWebImageDownloaderDecodeFirstFrameOnly = 1 << 10,
    
    /**
     * By default, for `SDAnimatedImage`, we decode the animated image frame during rendering to reduce memory usage. This flag actually trigger `preloadAllAnimatedImageFrames = YES` after image load from network
     * 默认情况下，对于“SDAnimatedImage”，我们在渲染过程中解码动画图像帧以减少内存的使用。这个标志实际上在从网络加载图像后触发' preloadAllAnimatedImageFrames = YES '
     */
    SDWebImageDownloaderPreloadAllFrames = 1 << 11,
    
    /**
     * By default, when you use `SDWebImageContextAnimatedImageClass` context option (like using `SDAnimatedImageView` which designed to use `SDAnimatedImage`), we may still use `UIImage` when the memory cache hit, or image decoder is not available, to behave as a fallback solution.
     * Using this option, can ensure we always produce image with your provided class. If failed, a error with code `SDWebImageErrorBadImageData` will been used.
     * Note this options is not compatible with `SDWebImageDownloaderDecodeFirstFrameOnly`, which always produce a UIImage/NSImage.
     * 默认情况下，当你使用' SDWebImageContextAnimatedImageClass '上下文选项(像使用' SDAnimatedImageView '设计使用' SDAnimatedImage ')，我们仍然可以使用' UIImage '时，内存缓存hit，或图像解码器不可用，以作为一个回退的解决方案。
     * 使用这个选项，可以确保我们总是用你提供的类生成图像。如果失败，一个代码' SDWebImageErrorBadImageData '的错误将被使用。
     * 注意这个选项不兼容“SDWebImageDownloaderDecodeFirstFrameOnly”，它总是产生一个UIImage/NSImage。
     */
    SDWebImageDownloaderMatchAnimatedImageClass = 1 << 12,
};
/// 开始下载通知
FOUNDATION_EXPORT NSNotificationName _Nonnull const SDWebImageDownloadStartNotification;
/// 收到响应通知
FOUNDATION_EXPORT NSNotificationName _Nonnull const SDWebImageDownloadReceiveResponseNotification;
/// 下载停止通知
FOUNDATION_EXPORT NSNotificationName _Nonnull const SDWebImageDownloadStopNotification;
/// 下载完成通知
FOUNDATION_EXPORT NSNotificationName _Nonnull const SDWebImageDownloadFinishNotification;

/// 下载进度回调
typedef SDImageLoaderProgressBlock SDWebImageDownloaderProgressBlock;
/// 下载完成回调
typedef SDImageLoaderCompletedBlock SDWebImageDownloaderCompletedBlock;

/**
 *  A token associated with each download. Can be used to cancel a download
 *  用来和下载器绑定的标识，可以用来取消下载
 */
@interface SDWebImageDownloadToken : NSObject <SDWebImageOperation>

/**
 Cancel the current download.
 取消当前下载
 */
- (void)cancel;

/**
 The download's URL.
 下载url
 */
@property (nonatomic, strong, nullable, readonly) NSURL *url;

/**
 The download's request.
 下载请求
 */
@property (nonatomic, strong, nullable, readonly) NSURLRequest *request;

/**
 The download's response.
 下载响应
 */
@property (nonatomic, strong, nullable, readonly) NSURLResponse *response;

/**
 The download's metrics. This will be nil if download operation does not support metrics.
 下载的指标。这将是nil，如果下载操作不支持指标
 */
@property (nonatomic, strong, nullable, readonly) NSURLSessionTaskMetrics *metrics API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@end


/**
 * Asynchronous downloader dedicated and optimized for image loading.
 * 异步下载器专用和优化的图像加载
 */
@interface SDWebImageDownloader : NSObject

/**
 * Downloader Config object - storing all kind of settings.
 * Most config properties support dynamic changes during download, except something like `sessionConfiguration`, see `SDWebImageDownloaderConfig` for more detail.
 * 下载器配置对象 - 存储所有的设置
 * 大多数配置属性在下载过程中支持动态更改，除了像“sessionConfiguration”这样的东西，请参阅“SDWebImageDownloaderConfig”获取更多细节
 */
@property (nonatomic, copy, readonly, nonnull) SDWebImageDownloaderConfig *config;

/**
 * Set the request modifier to modify the original download request before image load.
 * This request modifier method will be called for each downloading image request. Return the original request means no modification. Return nil will cancel the download request.
 * Defaults to nil, means does not modify the original download request.
 * @note If you want to modify single request, consider using `SDWebImageContextDownloadRequestModifier` context option.
 * 设置请求修饰符，在加载图像之前修改原始的下载请求。
 * 每个下载图片的请求都会调用这个请求修饰符方法。返回原始请求意味着没有修改。返回nil将取消下载请求。
 * 默认为nil, 意味着没有修改原始下载请求
 * @note 如果你想修改单个请求，考虑使用“SDWebImageContextDownloadRequestModifier”上下文选项
 */
@property (nonatomic, strong, nullable) id<SDWebImageDownloaderRequestModifier> requestModifier;

/**
 * Set the response modifier to modify the original download response during image load.
 * This response modifier method will be called for each downloading image response. Return the original response means no modification. Return nil will mark current download as cancelled.
 * Defaults to nil, means does not modify the original download response.
 * @note If you want to modify single response, consider using `SDWebImageContextDownloadResponseModifier` context option.
 * 设置相应修饰符，在图像加载过程中修改原始下载响应
 * 每次下载图像响应都会调用这个响应修饰器方法。返回原始响应意味着没有修改。返回nil将标记当前下载为取消
 * 默认为nil, 意味着没有修改原始下载响应
 * @note 如果像修改单个相应，考虑使用“SDWebImageContextDownloadResponseModifier”上下文选项
 */
@property (nonatomic, strong, nullable) id<SDWebImageDownloaderResponseModifier> responseModifier;

/**
 * Set the decryptor to decrypt the original download data before image decoding. This can be used for encrypted image data, like Base64.
 * This decryptor method will be called for each downloading image data. Return the original data means no modification. Return nil will mark this download failed.
 * Defaults to nil, means does not modify the original download data.
 * @note When using decryptor, progressive decoding will be disabled, to avoid data corrupt issue.
 * @note If you want to decrypt single download data, consider using `SDWebImageContextDownloadDecryptor` context option.
 * 设置解密器对原始下载数据进行解密后再解码。这可以用于加密的图像数据，比如Base64
 * 每次下载图像数据都会调用这个解密器方法。返回原始数据意味着没有修改。返回nil表示下载失败。
 * 默认为nil，意味着不修改原始下载数据
 * @note 当使用解密器时，将禁用渐进式解码，以避免数据损坏问题
 * @note 如果想解密单个下载数据, 考虑使用“SDWebImageContextDownloadDecryptor” 上下文选项
 */
@property (nonatomic, strong, nullable) id<SDWebImageDownloaderDecryptor> decryptor;

/**
 * The configuration in use by the internal NSURLSession. If you want to provide a custom sessionConfiguration, use `SDWebImageDownloaderConfig.sessionConfiguration` and create a new downloader instance.
 @note This is immutable according to NSURLSession's documentation. Mutating this object directly has no effect.
 * 内部NSURLSession使用的配置。如果你想提供一个自定义的sessionConfiguration，使用' SDWebImageDownloaderConfig.sessionConfiguration，并创建一个新的下载器实例
 * @note 根据NSURLSession的文档，这是不可变的。直接修改此对象不起作用
 */
@property (nonatomic, readonly, nonnull) NSURLSessionConfiguration *sessionConfiguration;

/**
 * Gets/Sets the download queue suspension state.
 * 获取/设置下载队列暂停状态
 */
@property (nonatomic, assign, getter=isSuspended) BOOL suspended;

/**
 * Shows the current amount of downloads that still need to be downloaded
 * 显示当前仍需下载的下载数量
 */
@property (nonatomic, assign, readonly) NSUInteger currentDownloadCount;

/**
 *  Returns the global shared downloader instance. Which use the `SDWebImageDownloaderConfig.defaultDownloaderConfig` config.
 *  返回全局共享下载器实例。使用“SDWebImageDownloaderConfig.defaultDownloaderConfig”配置
 */
@property (nonatomic, class, readonly, nonnull) SDWebImageDownloader *sharedDownloader;

/**
 Creates an instance of a downloader with specified downloader config.
 You can specify session configuration, timeout or operation class through downloader config.
 用指定的配置创建一个下载器实例，通过下载配置可以指定会话配置、超时或操作类

 @param config The downloader config. If you specify nil, the `defaultDownloaderConfig` will be used.  下载器配置，如果为nil，则使用默认配置
 @return new instance of downloader class 一个新的下载器实例
 */
- (nonnull instancetype)initWithConfig:(nullable SDWebImageDownloaderConfig *)config NS_DESIGNATED_INITIALIZER;

/**
 * Set a value for a HTTP header to be appended to each download HTTP request.
 * 为每个下载HTTP请求附加的HTTP头设置一个值
 *
 * @param value The value for the header field. Use `nil` value to remove the header field. http请求头的值, 使用nil时则删除HTTP头
 * @param field The name of the header field to set.  要设置的报头字段的名称
 */
- (void)setValue:(nullable NSString *)value forHTTPHeaderField:(nullable NSString *)field;

/**
 * Returns the value of the specified HTTP header field.
 * 返回指定HTTP头的值
 *
 * @return The value associated with the header field field, or `nil` if there is no corresponding header field.
 * @return 与报头字段相关的值，如果没有相应的报头字段，则为' nil '
 */
- (nullable NSString *)valueForHTTPHeaderField:(nullable NSString *)field;

/**
 * Creates a SDWebImageDownloader async downloader instance with a given URL
 * 根据给定的URL创建一个SDWebImageDownloader异步下载器实例
 *
 * The delegate will be informed when the image is finish downloaded or an error has happen.
 * 当图片下载完成或发生错误时将会调用代理方法
 *
 * @see SDWebImageDownloaderDelegate
 *
 * @param url            The URL to the image to download 下载图片的链接
 * @param completedBlock A block called once the download is completed. 下载完成时的回调
 *                       If the download succeeded, the image parameter is set, in case of error,
 *                       error parameter is set with the error. The last parameter is always YES
 *                       if SDWebImageDownloaderProgressiveDownload isn't use. With the
 *                       SDWebImageDownloaderProgressiveDownload option, this block is called
 *                       repeatedly with the partial image object and the finished argument set to NO
 *                       before to be called a last time with the full image and finished argument
 *                       set to YES. In case of error, the finished argument is always YES.
 *
 * @return A token (SDWebImageDownloadToken) that can be used to cancel this operation
 * 返回一个可以用来取消下载操作的token
 */
- (nullable SDWebImageDownloadToken *)downloadImageWithURL:(nullable NSURL *)url
                                                 completed:(nullable SDWebImageDownloaderCompletedBlock)completedBlock;

/**
 * Creates a SDWebImageDownloader async downloader instance with a given URL
 * 根据给定的URL创建一个SDWebImageDownloader异步下载器实例
 *
 * The delegate will be informed when the image is finish downloaded or an error has happen.
 * 当图片下载完成或发生错误时将会调用代理方法
 *
 * @see SDWebImageDownloaderDelegate
 *
 * @param url            The URL to the image to download 下载图片的链接
 * @param options        The options to be used for this download 下载选项
 * @param progressBlock  A block called repeatedly while the image is downloading 下载进度回调
 *                       @note the progress block is executed on a background queue
 * @param completedBlock A block called once the download is completed. 下载完成回调
 *                       If the download succeeded, the image parameter is set, in case of error,
 *                       error parameter is set with the error. The last parameter is always YES
 *                       if SDWebImageDownloaderProgressiveLoad isn't use. With the
 *                       SDWebImageDownloaderProgressiveLoad option, this block is called
 *                       repeatedly with the partial image object and the finished argument set to NO
 *                       before to be called a last time with the full image and finished argument
 *                       set to YES. In case of error, the finished argument is always YES.
 *
 * @return A token (SDWebImageDownloadToken) that can be used to cancel this operation
 */
- (nullable SDWebImageDownloadToken *)downloadImageWithURL:(nullable NSURL *)url
                                                   options:(SDWebImageDownloaderOptions)options
                                                  progress:(nullable SDWebImageDownloaderProgressBlock)progressBlock
                                                 completed:(nullable SDWebImageDownloaderCompletedBlock)completedBlock;

/**
 * Creates a SDWebImageDownloader async downloader instance with a given URL
 * 根据给定的URL创建一个SDWebImageDownloader异步下载器实例
 *
 * The delegate will be informed when the image is finish downloaded or an error has happen.
 * 当图片下载完成或发生错误时将会调用代理方法
 *
 * @see SDWebImageDownloaderDelegate
 *
 * @param url            The URL to the image to download 下载图片的链接
 * @param options        The options to be used for this download 下载选项
 * @param context        A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold. 上下文包括不同的选项来执行指定的改动或处理，`SDWebImageContextOption`保存了' options ' 枚举不能保存的额外对象
 * @param progressBlock  A block called repeatedly while the image is downloading 进度回调
 *                       @note the progress block is executed on a background queue
 * @param completedBlock A block called once the download is completed. 完成回调
 *
 * @return A token (SDWebImageDownloadToken) that can be used to cancel this operation
 */
- (nullable SDWebImageDownloadToken *)downloadImageWithURL:(nullable NSURL *)url
                                                   options:(SDWebImageDownloaderOptions)options
                                                   context:(nullable SDWebImageContext *)context
                                                  progress:(nullable SDWebImageDownloaderProgressBlock)progressBlock
                                                 completed:(nullable SDWebImageDownloaderCompletedBlock)completedBlock;

/**
 * Cancels all download operations in the queue
 * 取消所有的下载操作
 */
- (void)cancelAllDownloads;

/**
 * Invalidates the managed session, optionally canceling pending operations.
 * 使管理会话失效，可选地取消挂起的操作。
 *
 * @note If you use custom downloader instead of the shared downloader, you need call this method when you do not use it to avoid memory leak
 * 如果使用自定义下载程序而不是共享下载程序，则需要在不使用该方法时调用该方法，以避免内存泄漏
 *
 * @param cancelPendingOperations Whether or not to cancel pending operations. 是否取消挂起的操作
 * @note Calling this method on the shared downloader has no effect. 在共享下载程序上调用此方法没有任何效果
 */
- (void)invalidateSessionAndCancel:(BOOL)cancelPendingOperations;

@end


/**
 SDWebImageDownloader is the built-in image loader conform to `SDImageLoader`. Which provide the HTTP/HTTPS/FTP download, or local file URL using NSURLSession.
 However, this downloader class itself also support customization for advanced users. You can specify `operationClass` in download config to custom download operation, See `SDWebImageDownloaderOperation`.
 If you want to provide some image loader which beyond network or local file, consider to create your own custom class conform to `SDImageLoader`.
 
 SDWebImageDownloader是符合“SDImageLoader”的内置图像加载器。提供HTTP/HTTPS/FTP下载，或使用NSURLSession的本地文件URL。
 但是，这个下载器类本身也支持高级用户的定制。你可以在下载配置中指定' operationClass '来自定义下载操作，参见' SDWebImageDownloaderOperation '。
 如果你想提供一些网络或本地文件以外的图像加载器，考虑创建自己的自定义类符合' SDImageLoader '。
 */
@interface SDWebImageDownloader (SDImageLoader) <SDImageLoader>

@end
