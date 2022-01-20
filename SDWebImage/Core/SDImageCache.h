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
#import "SDImageCacheConfig.h"
#import "SDImageCacheDefine.h"
#import "SDMemoryCache.h"
#import "SDDiskCache.h"

/// Image Cache Options
/// 图像缓存选项
typedef NS_OPTIONS(NSUInteger, SDImageCacheOptions) {
    /**
     * By default, we do not query image data when the image is already cached in memory. This mask can force to query image data at the same time. However, this query is asynchronously unless you specify `SDImageCacheQueryMemoryDataSync`
     * 默认情况下，当映像已经缓存到内存中时，我们不查询映像数据。此掩码可以强制同时查询图像数据。但是，这个查询是异步的，除非你指定' SDImageCacheQueryMemoryDataSync '
     */
    SDImageCacheQueryMemoryData = 1 << 0,
    /**
     * By default, when you only specify `SDImageCacheQueryMemoryData`, we query the memory image data asynchronously. Combined this mask as well to query the memory image data synchronously.
     * 默认情况下，当您只指定' SDImageCacheQueryMemoryData '时，我们将异步查询内存图像数据。结合这个掩码也可以同步查询内存图像数据。
     */
    SDImageCacheQueryMemoryDataSync = 1 << 1,
    /**
     * By default, when the memory cache miss, we query the disk cache asynchronously. This mask can force to query disk cache (when memory cache miss) synchronously.
     @note These 3 query options can be combined together. For the full list about these masks combination, see wiki page.
     * 默认情况下，当内存缓存丢失时，我们将异步查询磁盘缓存。这个掩码可以强制同步查询磁盘缓存(当内存缓存丢失时)。
     * 这3个查询选项可以组合在一起。有关这些蒙版组合的完整列表，请参阅wiki页面
     */
    SDImageCacheQueryDiskDataSync = 1 << 2,
    /**
     * By default, images are decoded respecting their original size. On iOS, this flag will scale down the images to a size compatible with the constrained memory of devices.
     * 默认情况下，图像根据其原始大小进行解码。在iOS上，这个标志将缩小图像到与设备有限的内存兼容的大小。
     */
    SDImageCacheScaleDownLargeImages = 1 << 3,
    /**
     * By default, we will decode the image in the background during cache query and download from the network. This can help to improve performance because when rendering image on the screen, it need to be firstly decoded. But this happen on the main queue by Core Animation.
     * However, this process may increase the memory usage as well. If you are experiencing a issue due to excessive memory consumption, This flag can prevent decode the image.
     * 默认情况下，我们会在缓存查询和从网络下载时在后台解码图像。这有助于提高性能，因为在屏幕上呈现图像时，首先需要对图像进行解码。但这发生在Core Animation的主队列上。
     * 但是，这个进程也会增加内存的使用。如果您遇到了由于过度内存消耗而引起的问题，此标志可以阻止解码图像。
     */
    SDImageCacheAvoidDecodeImage = 1 << 4,
    /**
     * By default, we decode the animated image. This flag can force decode the first frame only and produce the static image.
     * 默认情况下，我们解码动画图像。这个标志可以强制只解码第一帧并生成静态图像
     */
    SDImageCacheDecodeFirstFrameOnly = 1 << 5,
    /**
     * By default, for `SDAnimatedImage`, we decode the animated image frame during rendering to reduce memory usage. This flag actually trigger `preloadAllAnimatedImageFrames = YES` after image load from disk cache
     * 默认情况下，对于“SDAnimatedImage”，我们在渲染过程中解码动画图像帧以减少内存的使用。这个标志实际上在从磁盘缓存加载图像后触发' preloadAllAnimatedImageFrames = YES '
     */
    SDImageCachePreloadAllFrames = 1 << 6,
    /**
     * By default, when you use `SDWebImageContextAnimatedImageClass` context option (like using `SDAnimatedImageView` which designed to use `SDAnimatedImage`), we may still use `UIImage` when the memory cache hit, or image decoder is not available, to behave as a fallback solution.
     * Using this option, can ensure we always produce image with your provided class. If failed, an error with code `SDWebImageErrorBadImageData` will be used.
     * Note this options is not compatible with `SDImageCacheDecodeFirstFrameOnly`, which always produce a UIImage/NSImage.
     * 默认情况下，当你使用' SDWebImageContextAnimatedImageClass '上下文选项(像使用' SDAnimatedImageView '设计使用' SDAnimatedImage ')，我们仍然可以使用' UIImage '时，内存缓存hit，或图像解码器不可用，以作为一个回退的解决方案。
     * 使用这个选项，可以确保我们总是用你提供的类生成图像。如果失败，将使用代码' SDWebImageErrorBadImageData '的错误。
     * 注意这个选项不兼容' SDImageCacheDecodeFirstFrameOnly '，它总是产生一个UIImage/NSImage。
     */
    SDImageCacheMatchAnimatedImageClass = 1 << 7,
};

/**
 * SDImageCache maintains a memory cache and a disk cache. Disk cache write operations are performed asynchronous so it doesn’t add unnecessary latency to the UI.
 * SDImageCache维护一个内存缓存和一个磁盘缓存。磁盘缓存写操作是异步执行的，因此不会给UI增加不必要的延迟。
 */
@interface SDImageCache : NSObject

#pragma mark - Properties

/**
 *  Cache Config object - storing all kind of settings.
 *  缓存配置对象-存储所有类型的设置
 *  The property is copy so change of current config will not accidentally affect other cache's config.
 *  这个属性是copy，所以当前配置的改变不会意外地影响其他缓存的配置
 */
@property (nonatomic, copy, nonnull, readonly) SDImageCacheConfig *config;

/**
 * The memory cache implementation object used for current image cache.
 * By default we use `SDMemoryCache` class, you can also use this to call your own implementation class method.
 * @note To customize this class, check `SDImageCacheConfig.memoryCacheClass` property.
 * 当前图像缓存对象的内存缓存对象
 * 默认使用`SDMemoryCache`类, 也可以使用自定义实现的类
 * @note 要定制这个类，请选中' SDImageCacheConfig.memoryCacheClass'的属性
 */
@property (nonatomic, strong, readonly, nonnull) id<SDMemoryCache> memoryCache;

/**
 * The disk cache implementation object used for current image cache.
 * By default we use `SDMemoryCache` class, you can also use this to call your own implementation class method.
 * @note To customize this class, check `SDImageCacheConfig.diskCacheClass` property.
 * @warning When calling method about read/write in disk cache, be sure to either make your disk cache implementation IO-safe or using the same access queue to avoid issues.
 * 当前图像缓存对象的硬盘缓存对象
 * 默认使用`SDMemoryCache`类，也可以自定义
 * @note 要定制这个类，请选中' SDImageCacheConfig.diskCacheClass'的属性
 * @warning 当调用关于磁盘缓存读/写的方法时，请确保磁盘缓存实现io安全，或者使用相同的访问队列以避免问题
 */
@property (nonatomic, strong, readonly, nonnull) id<SDDiskCache> diskCache;

/**
 *  The disk cache's root path
 *  硬盘缓存根目录
 */
@property (nonatomic, copy, nonnull, readonly) NSString *diskCachePath;

/**
 *  The additional disk cache path to check if the query from disk cache not exist;
 *  The `key` param is the image cache key. The returned file path will be used to load the disk cache. If return nil, ignore it.
 *  Useful if you want to bundle pre-loaded images with your app
 *  用于检查来自磁盘缓存的查询是否不存在的附加磁盘缓存路径
 *  “key”参数是图像缓存键。返回的文件路径将用于加载磁盘缓存。如果返回nil，则忽略它
 *  如果想在你的应用程序中捆绑预加载的图像，这很有用
 */
@property (nonatomic, copy, nullable) SDImageCacheAdditionalCachePathBlock additionalCachePathBlock;

#pragma mark - Singleton and initialization

/**
 * Returns global shared cache instance
 * 返回全局共享的缓存单例
 */
@property (nonatomic, class, readonly, nonnull) SDImageCache *sharedImageCache;

/**
 * Control the default disk cache directory. This will effect all the SDImageCache instance created after modification, even for shared image cache.
 * This can be used to share the same disk cache with the App and App Extension (Today/Notification Widget) using `- [NSFileManager.containerURLForSecurityApplicationGroupIdentifier:]`.
 * @note If you pass nil, the value will be reset to `~/Library/Caches/com.hackemist.SDImageCache`.
 * @note We still preserve the `namespace` arg, which means, if you change this property into `/path/to/use`,  the `SDImageCache.sharedImageCache.diskCachePath` should be `/path/to/use/default` because shared image cache use `default` as namespace.
 * Defaults to nil.
 * 控制默认硬盘缓存目录.这将影响修改后创建的所有SDImageCache实例，甚至包括共享映像缓存
 * 这可以使用' - [NSFileManager.containerURLForSecurityApplicationGroupIdentifier:] '与应用程序和应用程序扩展(Today/Notification Widget)共享相同的磁盘缓存
 * @note 如果传空，该属性值将被置为"~/Library/Caches/com.hackemist.SDImageCache"
 * @note 我们仍然保留名称空间的参数,这意味着,如果你改变这个属性为“/path/to/use”,“SDImageCache.sharedImageCache.diskCachePath '应该是' /path/to/use/default '，因为共享映像缓存使用' default '作为命名空间。
 * 默认为nil
 */
@property (nonatomic, class, readwrite, null_resettable) NSString *defaultDiskCacheDirectory;

/**
 * Init a new cache store with a specific namespace
 * The final disk cache directory should looks like ($directory/$namespace). And the default config of shared cache, should result in (~/Library/Caches/com.hackemist.SDImageCache/default/)
 * 用指定的命名空间初始化一个新的缓存实例
 * 最终的磁盘缓存目录应该类似于($directory/$namespace)。共享缓存的默认配置应该会导致(~/Library/Caches/com.hackemist.SDImageCache/default/)
 *
 * @param ns The namespace to use for this cache store
 * @param ns 为缓存存储的命名空间
 */
- (nonnull instancetype)initWithNamespace:(nonnull NSString *)ns;

/**
 * Init a new cache store with a specific namespace and directory.
 * The final disk cache directory should looks like ($directory/$namespace). And the default config of shared cache, should result in (~/Library/Caches/com.hackemist.SDImageCache/default/)
 * 用指定的命名空间和目录初始化一个新的缓存实例
 * 最终的磁盘缓存目录应该类似于($directory/$namespace)。共享缓存的默认配置应该会是(~/Library/Caches/com.hackemist.SDImageCache/default/)
 *
 * @param ns        The namespace to use for this cache store
 * @param directory Directory to cache disk images in
 * @param ns 缓存存储的命名空间
 * @param directory 用来存储硬盘图片的目录
 */
- (nonnull instancetype)initWithNamespace:(nonnull NSString *)ns
                       diskCacheDirectory:(nullable NSString *)directory;

/**
 * Init a new cache store with a specific namespace, directory and config.
 * The final disk cache directory should looks like ($directory/$namespace). And the default config of shared cache, should result in (~/Library/Caches/com.hackemist.SDImageCache/default/)
 * 用指定的命名空间，目录和配置初始化一个新的缓存存储
 * 最终的磁盘缓存目录应该类似于($directory/$namespace)。共享缓存的默认配置应该会是(~/Library/Caches/com.hackemist.SDImageCache/default/)
 *
 * @param ns          The namespace to use for this cache store
 * @param directory   Directory to cache disk images in
 * @param config      The cache config to be used to create the cache. You can provide custom memory cache or disk cache class in the cache config
 * @param ns 当前缓存对象的命名空间
 * @param directory 缓存硬盘图片的目录
 * @param config 用于创建缓存的缓存配置。你可以在缓存配置中提供自定义的内存缓存或磁盘缓存类
 */
- (nonnull instancetype)initWithNamespace:(nonnull NSString *)ns
                       diskCacheDirectory:(nullable NSString *)directory
                                   config:(nullable SDImageCacheConfig *)config NS_DESIGNATED_INITIALIZER;

#pragma mark - Cache paths

/**
 Get the cache path for a certain key
 // 获取某个键的缓存路径
 @param key The unique image cache key // 独一无二的图像缓存键
 @return The cache path. You can check `lastPathComponent` to grab the file name. // 缓存路径。你可以检查' lastPathComponent '来获取文件名
 */
- (nullable NSString *)cachePathForKey:(nullable NSString *)key;

#pragma mark - Store Ops

/**
 * Asynchronously store an image into memory and disk cache at the given key.
 * 异步存储，通过指定的键将图像存储到内存和硬盘缓存中
 *
 * @param image           The image to store // 存储的图像
 * @param key             The unique image cache key, usually it's image absolute URL // 独一无二的图像存储key, 通常是图像绝对URL
 * @param completionBlock A block executed after the operation is finished // 操作完成后的回调
 */
- (void)storeImage:(nullable UIImage *)image
            forKey:(nullable NSString *)key
        completion:(nullable SDWebImageNoParamsBlock)completionBlock;

/**
 * Asynchronously store an image into memory and disk cache at the given key.
 * 异步存储，通过指定的键将图像存储到内存和硬盘缓存中
 *
 * @param image           The image to store //存储的图像
 * @param key             The unique image cache key, usually it's image absolute URL //
 * @param toDisk          Store the image to disk cache if YES. If NO, the completion block is called synchronously // 如果YES的话存储图像到硬盘，NO的话同步调用完成block
 * @param completionBlock A block executed after the operation is finished // 操作完成时的回调
 * @note If no image data is provided and encode to disk, we will try to detect the image format (using either `sd_imageFormat` or `SDAnimatedImage` protocol method) and animation status, to choose the best matched format, including GIF, JPEG or PNG.
 * @note 如果没有提供图像数据并编码到磁盘，我们将尝试检测图像格式(使用' sd_imageFormat '或' SDAnimatedImage '协议方法)和动画状态，以选择最匹配的格式，包括GIF, JPEG或PNG
 */
- (void)storeImage:(nullable UIImage *)image
            forKey:(nullable NSString *)key
            toDisk:(BOOL)toDisk
        completion:(nullable SDWebImageNoParamsBlock)completionBlock;

/**
 * Asynchronously store an image into memory and disk cache at the given key.
 * 异步存储
 *
 * @param image           The image to store
 * @param imageData       The image data as returned by the server, this representation will be used for disk storage
 *                        instead of converting the given image object into a storable/compressed image format in order
 *                        to save quality and CPU // 从服务器返回的图像数据体，这种表示将用于磁盘存储，而不是将给定的图像对象转换为可存储/压缩的图像格式，以节省质量和CPU
 * @param key             The unique image cache key, usually it's image absolute URL
 * @param toDisk          Store the image to disk cache if YES. If NO, the completion block is called synchronously
 * @param completionBlock A block executed after the operation is finished
 * @note If no image data is provided and encode to disk, we will try to detect the image format (using either `sd_imageFormat` or `SDAnimatedImage` protocol method) and animation status, to choose the best matched format, including GIF, JPEG or PNG.
 */
- (void)storeImage:(nullable UIImage *)image
         imageData:(nullable NSData *)imageData
            forKey:(nullable NSString *)key
            toDisk:(BOOL)toDisk
        completion:(nullable SDWebImageNoParamsBlock)completionBlock;

/**
 * Synchronously store image into memory cache at the given key.
 * 同步存储，
 *
 * @param image  The image to store
 * @param key    The unique image cache key, usually it's image absolute URL
 */
- (void)storeImageToMemory:(nullable UIImage*)image
                    forKey:(nullable NSString *)key;

/**
 * Synchronously store image data into disk cache at the given key.
 * 同步存储
 *
 * @param imageData  The image data to store
 * @param key        The unique image cache key, usually it's image absolute URL
 */
- (void)storeImageDataToDisk:(nullable NSData *)imageData
                      forKey:(nullable NSString *)key;


#pragma mark - Contains and Check Ops

/**
 *  Asynchronously check if image exists in disk cache already (does not load the image)
 *  异步检查图像是否已经存在于硬盘(没有加载图像)
 *
 *  @param key             the key describing the url // 描述url的键
 *  @param completionBlock the block to be executed when the check is done. // 检查完成的时候执行block
 *  @note the completion block will be always executed on the main queue
 *  @note 完成回调的block总是在主线程执行
 */
- (void)diskImageExistsWithKey:(nullable NSString *)key completion:(nullable SDImageCacheCheckCompletionBlock)completionBlock;

/**
 *  Synchronously check if image data exists in disk cache already (does not load the image)
 *  同步检查图像是否已经存在于硬盘(没有加载图像)
 *
 *  @param key             the key describing the url // 描述url的键
 */
- (BOOL)diskImageDataExistsWithKey:(nullable NSString *)key;

#pragma mark - Query and Retrieve Ops

/**
 * Synchronously query the image data for the given key in disk cache. You can decode the image data to image after loaded.
 * 同步查找，根据给定的key查找图像数据是否在硬盘缓存中，在加载之后可以将图像数据解码为图像
 *
 *  @param key The unique key used to store the wanted image // 用来存储目标图像的key
 *  @return The image data for the given key, or nil if not found. // 给定key绑定的图像数据, 没找到的话为nil
 */
- (nullable NSData *)diskImageDataForKey:(nullable NSString *)key;

/**
 * Asynchronously query the image data for the given key in disk cache. You can decode the image data to image after loaded.
 * 异步查找, 根据给定的key查找图像数据是否在硬盘缓存中，在加载之后可以将图像数据解码为图像
 *
 *  @param key The unique key used to store the wanted image  // 用来存储目标图像的key
 *  @param completionBlock the block to be executed when the query is done. // 查找完成时执行的block
 *  @note the completion block will be always executed on the main queue // 回调block总是在主线程执行
 */
- (void)diskImageDataQueryForKey:(nullable NSString *)key completion:(nullable SDImageCacheQueryDataCompletionBlock)completionBlock;

/**
 * Asynchronously queries the cache with operation and call the completion when done.
 * 使用操作异步查询缓存，并在完成时调用完成
 *
 * @param key       The unique key used to store the wanted image. If you want transformed or thumbnail image, calculate the key with `SDTransformedKeyForKey`, `SDThumbnailedKeyForKey`, or generate the cache key from url with `cacheKeyForURL:context:`.
 * 如果你想要转换或缩略图图像，用' SDTransformedKeyForKey '， ' SDThumbnailedKeyForKey '计算键，或从url生成缓存键' cacheKeyForURL:context: '
 * @param doneBlock The completion block. Will not get called if the operation is cancelled
 * 完成回调，如果操作取消则不会调用
 *
 * @return a NSOperation instance containing the cache op 包含缓存操作的NSOperation实例
 */
- (nullable NSOperation *)queryCacheOperationForKey:(nullable NSString *)key done:(nullable SDImageCacheQueryCompletionBlock)doneBlock;

/**
 * Asynchronously queries the cache with operation and call the completion when done.
 * 使用操作异步查询缓存，并在完成时调用完成
 *
 * @param key       The unique key used to store the wanted image. If you want transformed or thumbnail image, calculate the key with `SDTransformedKeyForKey`, `SDThumbnailedKeyForKey`, or generate the cache key from url with `cacheKeyForURL:context:`.
 * 如果你想要转换或缩略图图像，用' SDTransformedKeyForKey '， ' SDThumbnailedKeyForKey '计算键，或从url生成缓存键' cacheKeyForURL:context: '
 * @param options   A mask to specify options to use for this cache query
 * 图像缓存选项
 * @param doneBlock The completion block. Will not get called if the operation is cancelled
 * 完成回调，如果操作取消则不会调用
 * @return a NSOperation instance containing the cache op 包含缓存操作的NSOperation实例
 */
- (nullable NSOperation *)queryCacheOperationForKey:(nullable NSString *)key options:(SDImageCacheOptions)options done:(nullable SDImageCacheQueryCompletionBlock)doneBlock;

/**
 * Asynchronously queries the cache with operation and call the completion when done.
 * 使用操作异步查询缓存并在完成时调用完成block
 *
 * @param key       The unique key used to store the wanted image. If you want transformed or thumbnail image, calculate the key with `SDTransformedKeyForKey`, `SDThumbnailedKeyForKey`, or generate the cache key from url with `cacheKeyForURL:context:`.
 * @param options   A mask to specify options to use for this cache query
 * 图像缓存选项
 * @param context   A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
 * 上下文包含不同的选项来执行指定的更改或过程，请参阅“SDWebImageContextOption”.这个保存了' options ' enum不能保存的额外对象。
 * @param doneBlock The completion block. Will not get called if the operation is cancelled
 *
 * @return a NSOperation instance containing the cache op
 */
- (nullable NSOperation *)queryCacheOperationForKey:(nullable NSString *)key options:(SDImageCacheOptions)options context:(nullable SDWebImageContext *)context done:(nullable SDImageCacheQueryCompletionBlock)doneBlock;

/**
 * Asynchronously queries the cache with operation and call the completion when done.
 * 使用操作异步查询缓存并在完成时调用block
 *
 * @param key       The unique key used to store the wanted image. If you want transformed or thumbnail image, calculate the key with `SDTransformedKeyForKey`, `SDThumbnailedKeyForKey`, or generate the cache key from url with `cacheKeyForURL:context:`.
 * 用来存储目标图像的key，如果你想要转换或缩略图图像，用' SDTransformedKeyForKey '， ' SDThumbnailedKeyForKey '计算键，或从url生成缓存键' cacheKeyForURL:context: '
 * @param options   A mask to specify options to use for this cache query
 * 图像存储选项
 * @param context   A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
 * 上下文包含不同的选项来执行指定的更改或过程，请参阅“SDWebImageContextOption”.这个保存了' options ' 枚举不能保存的额外对象。
 * @param queryCacheType Specify where to query the cache from. By default we use `.all`, which means both memory cache and disk cache. You can choose to query memory only or disk only as well. Pass `.none` is invalid and callback with nil immediately.
 * 指定从哪里查找.默认为“.all”, 即从内存和硬盘中查找。可以选择只从内存或只从硬盘中查找，传递”.none '无效，立即用nil回调
 * @param doneBlock The completion block. Will not get called if the operation is cancelled
 *
 * @return a NSOperation instance containing the cache op
 */
- (nullable NSOperation *)queryCacheOperationForKey:(nullable NSString *)key options:(SDImageCacheOptions)options context:(nullable SDWebImageContext *)context cacheType:(SDImageCacheType)queryCacheType done:(nullable SDImageCacheQueryCompletionBlock)doneBlock;

/**
 * Synchronously query the memory cache.
 * 同步从内存缓存中查找图片对象
 *
 * @param key The unique key used to store the image
 * @return The image for the given key, or nil if not found.
 */
- (nullable UIImage *)imageFromMemoryCacheForKey:(nullable NSString *)key;

/**
 * Synchronously query the disk cache.
 * 同步从硬盘缓存中查找对象
 *
 * @param key The unique key used to store the image
 * @return The image for the given key, or nil if not found.
 */
- (nullable UIImage *)imageFromDiskCacheForKey:(nullable NSString *)key;

/**
 * Synchronously query the disk cache. With the options and context which may effect the image generation. (Such as transformer, animated image, thumbnail, etc)
 * 同步查找硬盘缓存使用可能影响图像生成的选项和上下文。(如变形器、动画图像、缩略图等)
 *
 * @param key The unique key used to store the image
 * @param options   A mask to specify options to use for this cache query
 * @param context   A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
 * 上下文包含不同的选项来执行指定的更改或过程，请参阅“SDWebImageContextOption”.这个保存了' options ' 枚举不能保存的额外对象。
 * @return The image for the given key, or nil if not found.
 */
- (nullable UIImage *)imageFromDiskCacheForKey:(nullable NSString *)key options:(SDImageCacheOptions)options context:(nullable SDWebImageContext *)context;

/**
 * Synchronously query the cache (memory and or disk) after checking the memory cache.
 * 检查内存缓存后，同步查询缓存(内存和磁盘)
 *
 * @param key The unique key used to store the image
 * @return The image for the given key, or nil if not found.
 */
- (nullable UIImage *)imageFromCacheForKey:(nullable NSString *)key;

/**
 * Synchronously query the cache (memory and or disk) after checking the memory cache. With the options and context which may effect the image generation. (Such as transformer, animated image, thumbnail, etc)
 * 检查内存缓存后，同步查询缓存(内存和磁盘)。使用可能影响图像生成的选项和上下文。(如变形器、动画图像、缩略图等)
 *
 * @param key The unique key used to store the image
 * 存储图像的key
 * @param options   A mask to specify options to use for this cache query
 * 图像缓存选项
 * @param context   A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
 * 上下文包含不同的选项来执行指定的更改或过程，请参阅“SDWebImageContextOption”.这个保存了' options ' 枚举不能保存的额外对象。
 * @return The image for the given key, or nil if not found.
 */
- (nullable UIImage *)imageFromCacheForKey:(nullable NSString *)key options:(SDImageCacheOptions)options context:(nullable SDWebImageContext *)context;

#pragma mark - Remove Ops

/**
 * Asynchronously remove the image from memory and disk cache
 * 异步从内存和硬盘中删除图片
 *
 * @param key             The unique image cache key
 * @param completion      A block that should be executed after the image has been removed (optional)
 */
- (void)removeImageForKey:(nullable NSString *)key withCompletion:(nullable SDWebImageNoParamsBlock)completion;

/**
 * Asynchronously remove the image from memory and optionally disk cache
 * 异步从内存和硬盘（可选）中删除图片
 *
 * @param key             The unique image cache key
 * @param fromDisk        Also remove cache entry from disk if YES. If NO, the completion block is called synchronously
 * @param completion      A block that should be executed after the image has been removed (optional)
 */
- (void)removeImageForKey:(nullable NSString *)key fromDisk:(BOOL)fromDisk withCompletion:(nullable SDWebImageNoParamsBlock)completion;

/**
 Synchronously remove the image from memory cache.
 同步从内存缓存中删除图片
 
 @param key The unique image cache key
 */
- (void)removeImageFromMemoryForKey:(nullable NSString *)key;

/**
 Synchronously remove the image from disk cache.
 同步从硬盘缓存中删除图片
 
 @param key The unique image cache key
 */
- (void)removeImageFromDiskForKey:(nullable NSString *)key;

#pragma mark - Cache clean Ops

/**
 * Synchronously Clear all memory cached images
 * 同步清理所有内存缓存图片
 */
- (void)clearMemory;

/**
 * Asynchronously clear all disk cached images. Non-blocking method - returns immediately.
 * 异步清理所有硬盘缓存图片.
 * @param completion    A block that should be executed after cache expiration completes (optional)
 */
- (void)clearDiskOnCompletion:(nullable SDWebImageNoParamsBlock)completion;

/**
 * Asynchronously remove all expired cached image from disk. Non-blocking method - returns immediately.
 * 异步删除硬盘中所有过期的缓存图片，非阻塞方法-立即返回
 * @param completionBlock A block that should be executed after cache expiration completes (optional)
 */
- (void)deleteOldFilesWithCompletionBlock:(nullable SDWebImageNoParamsBlock)completionBlock;

#pragma mark - Cache Info

/**
 * Get the total bytes size of images in the disk cache
 * 硬盘总缓存图片大小
 */
- (NSUInteger)totalDiskSize;

/**
 * Get the number of images in the disk cache
 * 硬盘中缓存图片的数量
 */
- (NSUInteger)totalDiskCount;

/**
 * Asynchronously calculate the disk cache's size.
 * 异步计算磁盘缓存的大小
 */
- (void)calculateSizeWithCompletionBlock:(nullable SDImageCacheCalculateSizeBlock)completionBlock;

@end

/**
 * SDImageCache is the built-in image cache implementation for web image manager. It adopts `SDImageCache` protocol to provide the function for web image manager to use for image loading process.
 * SDImageCache是web图像管理器的内置图像缓存实现。它采用“SDImageCache”协议，为web图像管理器提供图像加载过程中使用的功能
 */
@interface SDImageCache (SDImageCache) <SDImageCache>

@end
