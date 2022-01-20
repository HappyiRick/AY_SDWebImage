/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"
#import "SDWebImageOperation.h"
#import "SDWebImageDefine.h"

/// Image Cache Type
/// 图像缓存类型
typedef NS_ENUM(NSInteger, SDImageCacheType) {
    /**
     * For query and contains op in response, means the image isn't available in the image cache
     * For op in request, this type is not available and take no effect.
     * 对于查询，并且在响应中包含op，意味着图像在图像缓存中不可用
     * 对于请求中的op，此类型不可用，不起作用
     */
    SDImageCacheTypeNone,
    /**
     * For query and contains op in response, means the image was obtained from the disk cache.
     * For op in request, means process only disk cache.
     * 对于查询，并且响应中包含op，表示从磁盘缓存中获取图像。
     * 对于请求中的op，意味着只处理磁盘缓存
     */
    SDImageCacheTypeDisk,
    /**
     * For query and contains op in response, means the image was obtained from the memory cache.
     * For op in request, means process only memory cache.
     * 对于查询，并且响应中包含op, 表示该图像是从内存缓存中获取的
     * 对于请求中的op，意味着只处理内存缓存
     */
    SDImageCacheTypeMemory,
    /**
     * For query and contains op in response, this type is not available and take no effect.
     * For op in request, means process both memory cache and disk cache.
     * 对于查询，且响应中包含op，此类型不可用，且不起作用
     * 对于请求中的op，意味着内存缓存和硬盘缓存都能处理
     */
    SDImageCacheTypeAll
};
/// 检查block
typedef void(^SDImageCacheCheckCompletionBlock)(BOOL isInCache);
/// 查询数据完成block
typedef void(^SDImageCacheQueryDataCompletionBlock)(NSData * _Nullable data);
/// 计算缓存大小block
typedef void(^SDImageCacheCalculateSizeBlock)(NSUInteger fileCount, NSUInteger totalSize);
/// 获取key绑定的数据路径block
typedef NSString * _Nullable (^SDImageCacheAdditionalCachePathBlock)(NSString * _Nonnull key);
/// 查询结果block
typedef void(^SDImageCacheQueryCompletionBlock)(UIImage * _Nullable image, NSData * _Nullable data, SDImageCacheType cacheType);
/// 包含缓存类型
typedef void(^SDImageCacheContainsCompletionBlock)(SDImageCacheType containsCacheType);

/**
 This is the built-in decoding process for image query from cache. 这是从缓存中查询图像的内置解码过程
 @note If you want to implement your custom loader with `queryImageForKey:options:context:completion:` API, but also want to keep compatible with SDWebImage's behavior, you'd better use this to produce image.
 如果你想用' queryImageForKey:options:context:completion: ' API实现你的自定义加载器，但又想保持与SDWebImage的行为兼容，你最好使用这个来生成图像
 
 @param imageData The image data from the cache. Should not be nil 缓存中的图像数据，不应为nil
 @param cacheKey The image cache key from the input. Should not be nil 缓存key
 @param options The options arg from the input 输入选项参数
 @param context The context arg from the input 输入上下文参数
 @return The decoded image for current image data query from cache 当前从内存中查找出来的图像数据的解码图片
 */
FOUNDATION_EXPORT UIImage * _Nullable SDImageCacheDecodeImageData(NSData * _Nonnull imageData, NSString * _Nonnull cacheKey, SDWebImageOptions options, SDWebImageContext * _Nullable context);

/**
 This is the image cache protocol to provide custom image cache for `SDWebImageManager`.
 Though the best practice to custom image cache, is to write your own class which conform `SDMemoryCache` or `SDDiskCache` protocol for `SDImageCache` class (See more on `SDImageCacheConfig.memoryCacheClass & SDImageCacheConfig.diskCacheClass`).
 However, if your own cache implementation contains more advanced feature beyond `SDImageCache` itself, you can consider to provide this instead. For example, you can even use a cache manager like `SDImageCachesManager` to register multiple caches.
 这是图像缓存协议，为SDWebImageManager提供自定义的图像缓存。
 尽管自定义图像缓存的最佳实践是，为“SDImageCache”类编写符合“SDMemoryCache”或“SDDiskCache”协议的自己的类(参见“SDImageCacheConfig”。memoryCacheClass & SDImageCacheConfig.diskCacheClass”)。
 但是，如果您自己的缓存实现包含“SDImageCache”本身之外的更高级的特性，那么您可以考虑提供这个功能。例如，你甚至可以使用像“SDImageCachesManager”这样的缓存管理器来注册多个缓存。
 */
@protocol SDImageCache <NSObject>

@required
/**
 Query the cached image from image cache for given key. The operation can be used to cancel the query.
 If image is cached in memory, completion is called synchronously, else asynchronously and depends on the options arg (See `SDWebImageQueryDiskSync`)
 根据给定key从缓存中查找图片.operation实例可以用来取消本次查询
 如果图像缓存在内存中，同步调用回调，否则异步并且依赖options参数
 
 @param key The image cache key
 @param options A mask to specify options to use for this query
 @param context A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
 @param completionBlock The completion block. Will not get called if the operation is cancelled
 @return The operation for this query
 */
- (nullable id<SDWebImageOperation>)queryImageForKey:(nullable NSString *)key
                                             options:(SDWebImageOptions)options
                                             context:(nullable SDWebImageContext *)context
                                          completion:(nullable SDImageCacheQueryCompletionBlock)completionBlock;

/**
 Query the cached image from image cache for given key. The operation can be used to cancel the query.
 If image is cached in memory, completion is called synchronously, else asynchronously and depends on the options arg (See `SDWebImageQueryDiskSync`)
 根据给定key从缓存中查找图片.operation实例可以用来取消本次查询
 如果图像缓存在内存中，同步调用回调，否则异步并且依赖options参数

 @param key The image cache key
 @param options A mask to specify options to use for this query
 @param context A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
 @param cacheType Specify where to query the cache from. By default we use `.all`, which means both memory cache and disk cache. You can choose to query memory only or disk only as well. Pass `.none` is invalid and callback with nil immediately.
 @param completionBlock The completion block. Will not get called if the operation is cancelled
 @return The operation for this query
 */
- (nullable id<SDWebImageOperation>)queryImageForKey:(nullable NSString *)key
                                             options:(SDWebImageOptions)options
                                             context:(nullable SDWebImageContext *)context
                                           cacheType:(SDImageCacheType)cacheType
                                          completion:(nullable SDImageCacheQueryCompletionBlock)completionBlock;

/**
 Store the image into image cache for the given key. If cache type is memory only, completion is called synchronously, else asynchronously.
 将给定键的图像存储到图像缓存中。如果缓存类型是仅内存，完成是同步调用，否则是异步调用。
 
 @param image The image to store
 @param imageData The image data to be used for disk storage
 @param key The image cache key
 @param cacheType The image store op cache type
 @param completionBlock A block executed after the operation is finished
 */
- (void)storeImage:(nullable UIImage *)image
         imageData:(nullable NSData *)imageData
            forKey:(nullable NSString *)key
         cacheType:(SDImageCacheType)cacheType
        completion:(nullable SDWebImageNoParamsBlock)completionBlock;

/**
 Remove the image from image cache for the given key. If cache type is memory only, completion is called synchronously, else asynchronously.
 将给定键的图像从图像缓存中移除。如果缓存类型是仅内存，完成是同步调用，否则是异步调用
 
 @param key The image cache key
 @param cacheType The image remove op cache type
 @param completionBlock A block executed after the operation is finished
 */
- (void)removeImageForKey:(nullable NSString *)key
                cacheType:(SDImageCacheType)cacheType
               completion:(nullable SDWebImageNoParamsBlock)completionBlock;

/**
 Check if image cache contains the image for the given key (does not load the image). If image is cached in memory, completion is called synchronously, else asynchronously.
 检查图像缓存是否包含给定键的图像(不加载图像)。如果图像缓存在内存中，完成被同步调用，否则异步调用。

 @param key The image cache key
 @param cacheType The image contains op cache type
 @param completionBlock A block executed after the operation is finished.
 */
- (void)containsImageForKey:(nullable NSString *)key
                  cacheType:(SDImageCacheType)cacheType
                 completion:(nullable SDImageCacheContainsCompletionBlock)completionBlock;

/**
 Clear all the cached images for image cache. If cache type is memory only, completion is called synchronously, else asynchronously.
 清除所有缓存的图像用于图像缓存。如果缓存类型是仅内存，完成是同步调用，否则是异步调用。
 
 @param cacheType The image clear op cache type
 @param completionBlock A block executed after the operation is finished
 */
- (void)clearWithCacheType:(SDImageCacheType)cacheType
                completion:(nullable SDWebImageNoParamsBlock)completionBlock;

@end
