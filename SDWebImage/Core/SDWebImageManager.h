/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"
#import "SDWebImageOperation.h"
#import "SDImageCacheDefine.h"
#import "SDImageLoader.h"
#import "SDImageTransformer.h"
#import "SDWebImageCacheKeyFilter.h"
#import "SDWebImageCacheSerializer.h"
#import "SDWebImageOptionsProcessor.h"

/// 外部完成回调
typedef void(^SDExternalCompletionBlock)(UIImage * _Nullable image, NSError * _Nullable error, SDImageCacheType cacheType, NSURL * _Nullable imageURL);
/// 内部完成回调
typedef void(^SDInternalCompletionBlock)(UIImage * _Nullable image, NSData * _Nullable data, NSError * _Nullable error, SDImageCacheType cacheType, BOOL finished, NSURL * _Nullable imageURL);

/**
 A combined operation representing the cache and loader operation. You can use it to cancel the load process.
 一种表示缓存和加载器操作的组合操作。您可以使用它来取消加载过程。
 */
@interface SDWebImageCombinedOperation : NSObject <SDWebImageOperation>

/**
 Cancel the current operation, including cache and loader process
 取消当前操作，包括缓存和加载过程
 */
- (void)cancel;

/**
 The cache operation from the image cache query
 从图像缓存查询的缓存操作
 */
@property (strong, nonatomic, nullable, readonly) id<SDWebImageOperation> cacheOperation;

/**
 The loader operation from the image loader (such as download operation)
 图像加载中的加载操作
 */
@property (strong, nonatomic, nullable, readonly) id<SDWebImageOperation> loaderOperation;

@end


@class SDWebImageManager;

/**
 The manager delegate protocol.
 管理器代理协议
 */
@protocol SDWebImageManagerDelegate <NSObject>

@optional

/**
 * Controls which image should be downloaded when the image is not found in the cache.
 * 当图像在缓存中没找到的时候控制哪一个图像应该被下载
 *
 * @param imageManager The current `SDWebImageManager`
 * @param imageURL     The url of the image to be downloaded
 *
 * @return Return NO to prevent the downloading of the image on cache misses. If not implemented, YES is implied. - 返回NO以防止在缓存失败时下载图像。如果没有实现，则隐含YES
 */
- (BOOL)imageManager:(nonnull SDWebImageManager *)imageManager shouldDownloadImageForURL:(nonnull NSURL *)imageURL;

/**
 * Controls the complicated logic to mark as failed URLs when download error occur. - 控制在发生下载错误时将复杂的逻辑标记为失败的url
 * If the delegate implement this method, we will not use the built-in way to mark URL as failed based on error code;
 * 如果代理实现了这个方法，我们不会使用内置的方法根据错误代码将URL标记为失败
 *
 @param imageManager The current `SDWebImageManager`
 @param imageURL The url of the image
 @param error The download error for the url
 @return Whether to block this url or not. Return YES to mark this URL as failed.
 */
- (BOOL)imageManager:(nonnull SDWebImageManager *)imageManager shouldBlockFailedURL:(nonnull NSURL *)imageURL withError:(nonnull NSError *)error;

@end

/**
 * The SDWebImageManager is the class behind the UIImageView+WebCache category and likes. - SDWebImageManager是UIImageView+WebCache类后面的类
 * It ties the asynchronous downloader (SDWebImageDownloader) with the image cache store (SDImageCache). - 它将异步下载器(SDWebImageDownloader)与图像缓存存储(SDImageCache)绑定在一起。
 * You can use this class directly to benefit from web image downloading with caching in another context than a UIView. - 你可以直接用这个类从获取网络下载并缓存在其他上下文中的网络图像
 *
 * Here is a simple example of how to use SDWebImageManager:
 *
 * @code

SDWebImageManager *manager = [SDWebImageManager sharedManager];
[manager loadImageWithURL:imageURL
                  options:0
                 progress:nil
                completed:^(UIImage *image, NSData *data, NSError *error, SDImageCacheType cacheType, BOOL finished, NSURL *imageURL) {
                    if (image) {
                        // do something with image
                    }
                }];

 * @endcode
 */
@interface SDWebImageManager : NSObject

/**
 * The delegate for manager. Defaults to nil. - 管理器的代理，默认为nil
 */
@property (weak, nonatomic, nullable) id <SDWebImageManagerDelegate> delegate;

/**
 * The image cache used by manager to query image cache. -  管理器查询镜像缓存时使用的镜像缓存
 */
@property (strong, nonatomic, readonly, nonnull) id<SDImageCache> imageCache;

/**
 * The image loader used by manager to load image. - 管理器用来加载图像的图像加载器
 */
@property (strong, nonatomic, readonly, nonnull) id<SDImageLoader> imageLoader;

/**
 The image transformer for manager. It's used for image transform after the image load finished and store the transformed image to cache, see `SDImageTransformer`. - 管理器的图像转换器，它用于图像加载完成后的图像转换，并将转换后的图像存储到缓存中，参见“SDImageTransformer”。
 Defaults to nil, which means no transform is applied. - 默认为nil,意味着没有转换器应用
 @note This will affect all the load requests for this manager if you provide. However, you can pass `SDWebImageContextImageTransformer` in context arg to explicitly use that transformer instead. - 如果您提供，这将影响此管理器的所有加载请求。然而，你可以在context参数中传递' SDWebImageContextImageTransformer '来显式地使用该转换器
 */
@property (strong, nonatomic, nullable) id<SDImageTransformer> transformer;

/**
 * The cache filter is used to convert an URL into a cache key each time SDWebImageManager need cache key to use image cache.
 * 每当SDWebImageManager需要缓存键来使用图像缓存时，缓存过滤器会将URL转换为缓存键
 *
 * The following example sets a filter in the application delegate that will remove any query-string from the URL before to use it as a cache key:
 * 下面的示例在应用程序委托中设置了一个过滤器，该过滤器将从URL中删除之前的任何查询字符串，以将其用作缓存键
 * @code
 SDWebImageManager.sharedManager.cacheKeyFilter =[SDWebImageCacheKeyFilter cacheKeyFilterWithBlock:^NSString * _Nullable(NSURL * _Nonnull url) {
    url = [[NSURL alloc] initWithScheme:url.scheme host:url.host path:url.path];
    return [url absoluteString];
 }];
 * @endcode
 */
@property (nonatomic, strong, nullable) id<SDWebImageCacheKeyFilter> cacheKeyFilter;

/**
 * The cache serializer is used to convert the decoded image, the source downloaded data, to the actual data used for storing to the disk cache. If you return nil, means to generate the data from the image instance, see `SDImageCache`.
 * 缓存序列化用来讲解码图像、源下载数据转化成用来存储在硬盘缓存中的实际数据，如果返回nil，意味着从图像实例生成数据，see `SDImageCache`
 * For example, if you are using WebP images and facing the slow decoding time issue when later retrieving from disk cache again. You can try to encode the decoded image to JPEG/PNG format to disk cache instead of source downloaded data.
 * 例如，如果您正在使用WebP图像，并且在以后再次从磁盘缓存中检索时面临解码时间缓慢的问题。您可以尝试将已解码的图像编码为JPEG/PNG格式，而不是将下载的源数据编码到磁盘缓存中。
 * @note The `image` arg is nonnull, but when you also provide an image transformer and the image is transformed, the `data` arg may be nil, take attention to this case. - ' image '参数是非空的，但当你也提供一个图像转换器和图像被转换时，' data '参数可能是空的，请注意这种情况。
 * @note This method is called from a global queue in order to not to block the main thread. - 这个方法在全局队列中调用以避免阻塞主线程
 * @code
 SDWebImageManager.sharedManager.cacheSerializer = [SDWebImageCacheSerializer cacheSerializerWithBlock:^NSData * _Nullable(UIImage * _Nonnull image, NSData * _Nullable data, NSURL * _Nullable imageURL) {
    SDImageFormat format = [NSData sd_imageFormatForImageData:data];
    switch (format) {
        case SDImageFormatWebP:
            return image.images ? data : nil;
        default:
            return data;
    }
}];
 * @endcode
 * The default value is nil. Means we just store the source downloaded data to disk cache.
 */
@property (nonatomic, strong, nullable) id<SDWebImageCacheSerializer> cacheSerializer;

/**
 The options processor is used, to have a global control for all the image request options and context option for current manager.
 用来为当前管理器全局控制所有的图像请求选项和上下文选项的选项处理器
 
 @note If you use `transformer`, `cacheKeyFilter` or `cacheSerializer` property of manager, the input context option already apply those properties before passed. This options processor is a better replacement for those property in common usage.
 如果你使用manager的' transformer '， ' cacheKeyFilter '或' cacheSerializer '属性，输入上下文选项已经在传递之前应用了那些属性。这个选项处理器可以更好地替代那些常用的属性。
 
 For example, you can control the global options, based on the URL or original context option like the below code.
 
 @code
 SDWebImageManager.sharedManager.optionsProcessor = [SDWebImageOptionsProcessor optionsProcessorWithBlock:^SDWebImageOptionsResult * _Nullable(NSURL * _Nullable url, SDWebImageOptions options, SDWebImageContext * _Nullable context) {
     // Only do animation on `SDAnimatedImageView`
     if (!context[SDWebImageContextAnimatedImageClass]) {
        options |= SDWebImageDecodeFirstFrameOnly;
     }
     // Do not force decode for png url
     if ([url.lastPathComponent isEqualToString:@"png"]) {
        options |= SDWebImageAvoidDecodeImage;
     }
     // Always use screen scale factor
     SDWebImageMutableContext *mutableContext = [NSDictionary dictionaryWithDictionary:context];
     mutableContext[SDWebImageContextImageScaleFactor] = @(UIScreen.mainScreen.scale);
     context = [mutableContext copy];
 
     return [[SDWebImageOptionsResult alloc] initWithOptions:options context:context];
 }];
 @endcode
 */
@property (nonatomic, strong, nullable) id<SDWebImageOptionsProcessor> optionsProcessor;

/**
 * Check one or more operations running - 检查一个或更多操作运行
 */
@property (nonatomic, assign, readonly, getter=isRunning) BOOL running;

/**
 The default image cache when the manager which is created with no arguments. Such as shared manager or init. - 当不带参数创建管理器时的默认图像缓存。例如shared manager或init
 Defaults to nil. Means using `SDImageCache.sharedImageCache` - 默认为nil，表示使用SDImageCache.sharedImageCache
 */
@property (nonatomic, class, nullable) id<SDImageCache> defaultImageCache;

/**
 The default image loader for manager which is created with no arguments. Such as shared manager or init.
 不带参数创建管理器时的默认图像加载器。例如共享管理器或初始化
 
 Defaults to nil. Means using `SDWebImageDownloader.sharedDownloader` - 默认为nil, 意味着使用`SDWebImageDownloader.sharedDownloader`
 */
@property (nonatomic, class, nullable) id<SDImageLoader> defaultImageLoader;

/**
 * Returns global shared manager instance. - 全局共享管理器实例
 */
@property (nonatomic, class, readonly, nonnull) SDWebImageManager *sharedManager;

/**
 * Allows to specify instance of cache and image loader used with image manager.
 * 允许指定与映像管理器一起使用的缓存和映像加载器的实例
 *
 * @return new instance of `SDWebImageManager` with specified cache and loader. - 带有指定缓存和加载器的SDWebImageManager的新实例
 */
- (nonnull instancetype)initWithCache:(nonnull id<SDImageCache>)cache loader:(nonnull id<SDImageLoader>)loader NS_DESIGNATED_INITIALIZER;

/**
 * Downloads the image at the given URL if not present in cache or return the cached version otherwise.
 * 如果图片不在缓存中，则在给定的URL处下载图片，否则返回缓存的版本
 *
 * @param url            The URL to the image
 * @param options        A mask to specify options to use for this request
 * @param progressBlock  A block called while image is downloading
 *                       @note the progress block is executed on a background queue
 * @param completedBlock A block called when operation has been completed.
 *
 *   This parameter is required.
 * 
 *   This block has no return value and takes the requested UIImage as first parameter and the NSData representation as second parameter.
 *   In case of error the image parameter is nil and the third parameter may contain an NSError.
 *
 *   The forth parameter is an `SDImageCacheType` enum indicating if the image was retrieved from the local cache
 *   or from the memory cache or from the network.
 *
 *   The fifth parameter is set to NO when the SDWebImageProgressiveLoad option is used and the image is
 *   downloading. This block is thus called repeatedly with a partial image. When image is fully downloaded, the
 *   block is called a last time with the full image and the last parameter set to YES.
 *
 *   The last parameter is the original image URL
 *
 * @return Returns an instance of SDWebImageCombinedOperation, which you can cancel the loading process.
 */
- (nullable SDWebImageCombinedOperation *)loadImageWithURL:(nullable NSURL *)url
                                                   options:(SDWebImageOptions)options
                                                  progress:(nullable SDImageLoaderProgressBlock)progressBlock
                                                 completed:(nonnull SDInternalCompletionBlock)completedBlock;

/**
 * Downloads the image at the given URL if not present in cache or return the cached version otherwise.
 * 如果图片不在缓存中，则在给定的URL处下载图片，否则返回缓存的版本。
 *
 * @param url            The URL to the image
 * @param options        A mask to specify options to use for this request
 * @param context        A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
 * @param progressBlock  A block called while image is downloading
 *                       @note the progress block is executed on a background queue
 * @param completedBlock A block called when operation has been completed.
 *
 * @return Returns an instance of SDWebImageCombinedOperation, which you can cancel the loading process.
 */
- (nullable SDWebImageCombinedOperation *)loadImageWithURL:(nullable NSURL *)url
                                                   options:(SDWebImageOptions)options
                                                   context:(nullable SDWebImageContext *)context
                                                  progress:(nullable SDImageLoaderProgressBlock)progressBlock
                                                 completed:(nonnull SDInternalCompletionBlock)completedBlock;

/**
 * Cancel all current operations - 取消当前所有操作
 */
- (void)cancelAll;

/**
 * Remove the specify URL from failed black list. - 从失败黑名单中删除指定URL
 * @param url The failed URL.
 */
- (void)removeFailedURL:(nonnull NSURL *)url;

/**
 * Remove all the URL from failed black list. - 删除失败黑明但中的所有URL
 */
- (void)removeAllFailedURLs;

/**
 * Return the cache key for a given URL, does not considerate transformer or thumbnail.
 * 返回给定URL的缓存键，不考虑转换或缩略图
 *
 * @note This method does not have context option, only use the url and manager level cacheKeyFilter to generate the cache key.
 * 这个方法没有上下文选项，只使用url和管理器级别的cacheKeyFilter来生成缓存键。
 */
- (nullable NSString *)cacheKeyForURL:(nullable NSURL *)url;

/**
 * Return the cache key for a given URL and context option. - 通过给定的URL和上下文选项生成缓存key
 * @note The context option like `.thumbnailPixelSize` and `.imageTransformer` will effect the generated cache key, using this if you have those context associated.
 * 上下文选项像'.thumbnailPixelSize’和‘.imageTransformer '将影响生成的缓存键，如果您有这些上下文相关联，则使用这个。
*/
- (nullable NSString *)cacheKeyForURL:(nullable NSURL *)url context:(nullable SDWebImageContext *)context;

@end
