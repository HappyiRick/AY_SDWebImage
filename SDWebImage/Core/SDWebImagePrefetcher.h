/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageManager.h"

@class SDWebImagePrefetcher;

/**
 A token represents a list of URLs, can be used to cancel the download.
 表示一个url列表的一个令牌，可以用来取消下载。
 */
@interface SDWebImagePrefetchToken : NSObject <SDWebImageOperation>

/**
 * Cancel the current prefetching.
 * 取消当前的预获取
 */
- (void)cancel;

/**
 list of URLs of current prefetching.
 当前预获取的url列表
 */
@property (nonatomic, copy, readonly, nullable) NSArray<NSURL *> *urls;

@end

/**
 The prefetcher delegate protocol - 预获取代理协议
 */
@protocol SDWebImagePrefetcherDelegate <NSObject>

@optional

/**
 * Called when an image was prefetched. Which means it's called when one URL from any of prefetching finished.
 * 当图像被预获取的时候调用，这意味着它会在任何一个预取URL完成时被调用
 *
 * @param imagePrefetcher The current image prefetcher
 * @param imageURL        The image url that was prefetched
 * @param finishedCount   The total number of images that were prefetched (successful or not)
 * @param totalCount      The total number of images that were to be prefetched
 */
- (void)imagePrefetcher:(nonnull SDWebImagePrefetcher *)imagePrefetcher didPrefetchURL:(nullable NSURL *)imageURL finishedCount:(NSUInteger)finishedCount totalCount:(NSUInteger)totalCount;

/**
 * Called when all images are prefetched. Which means it's called when all URLs from all of prefetching finished.
 * 当所有图像都被预获取的时候调用
 *
 * @param imagePrefetcher The current image prefetcher
 * @param totalCount      The total number of images that were prefetched (whether successful or not)
 * @param skippedCount    The total number of images that were skipped
 */
- (void)imagePrefetcher:(nonnull SDWebImagePrefetcher *)imagePrefetcher didFinishWithTotalCount:(NSUInteger)totalCount skippedCount:(NSUInteger)skippedCount;

@end

/// 预获取进度block
typedef void(^SDWebImagePrefetcherProgressBlock)(NSUInteger noOfFinishedUrls, NSUInteger noOfTotalUrls);
/// 预获取完成block
typedef void(^SDWebImagePrefetcherCompletionBlock)(NSUInteger noOfFinishedUrls, NSUInteger noOfSkippedUrls);

/**
 * Prefetch some URLs in the cache for future use. Images are downloaded in low priority.
 * 预获取一些链接图片
 */
@interface SDWebImagePrefetcher : NSObject

/**
 * The web image manager used by prefetcher to prefetch images. - 被预处理器用来预处理图像的网络图像管理器
 * @note You can specify a standalone manager and downloader with custom configuration suitable for image prefetching. Such as `currentDownloadCount` or `downloadTimeout`.
 */
@property (strong, nonatomic, readonly, nonnull) SDWebImageManager *manager;

/**
 * Maximum number of URLs to prefetch at the same time. Defaults to 3.
 * 同时预处理的url最大数量，默认为3
 */
@property (nonatomic, assign) NSUInteger maxConcurrentPrefetchCount;

/**
 * The options for prefetcher. Defaults to SDWebImageLowPriority.
 * 预处理选项，默认为SDWebImageLowPriority
 */
@property (nonatomic, assign) SDWebImageOptions options;

/**
 * The context for prefetcher. Defaults to nil.
 * 预处理的上下文，默认为nil
 */
@property (nonatomic, copy, nullable) SDWebImageContext *context;

/**
 * Queue options for prefetcher when call the progressBlock, completionBlock and delegate methods. Defaults to Main Queue.
 * 预处理器调用progressBlock、completionBlock和代理方法的队列选项，默认为主队列
 * @note The call is asynchronously to avoid blocking target queue.
 * @note The delegate queue should be set before any prefetching start and may not be changed during prefetching to avoid thread-safe problem.
 */
@property (strong, nonatomic, nonnull) dispatch_queue_t delegateQueue;

/**
 * The delegate for the prefetcher. Defaults to nil.
 * 预处理器的代理，默认为nil
 */
@property (weak, nonatomic, nullable) id <SDWebImagePrefetcherDelegate> delegate;

/**
 * Returns the global shared image prefetcher instance. It use a standalone manager which is different from shared manager.
 * 返回全局共享的图像预处理实例，它使用独立的管理器，这与共享管理器不同
 */
@property (nonatomic, class, readonly, nonnull) SDWebImagePrefetcher *sharedImagePrefetcher;

/**
 * Allows you to instantiate a prefetcher with any arbitrary image manager.
 * 允许您用任意映像管理器实例化一个预取器
 */
- (nonnull instancetype)initWithImageManager:(nonnull SDWebImageManager *)manager NS_DESIGNATED_INITIALIZER;

/**
 * Assign list of URLs to let SDWebImagePrefetcher to queue the prefetching. It based on the image manager so the image may from the cache and network according to the `options` property. - 分配url列表，让SDWebImagePrefetcher来排队预取。它基于图像管理器，所以图像可以根据“选项”属性从缓存和网络。
 * Prefetching is separate to each other, which means the progressBlock and completionBlock you provide is bind to the prefetching for the list of urls. - 预抓取是彼此分开的，这意味着你提供的progressBlock和completionBlock被绑定到url列表的预抓取中
 * Attention that call this will not cancel previous fetched urls. You should keep the token return by this to cancel or cancel all the prefetch.
 * 注意，调用此函数不会取消先前获取的url。你应该保持此方法返回的令牌，通过这个取消或取消所有的预取
 *
 * @param urls list of URLs to prefetch
 * @return the token to cancel the current prefetching.
 */
- (nullable SDWebImagePrefetchToken *)prefetchURLs:(nullable NSArray<NSURL *> *)urls;

/**
 * Assign list of URLs to let SDWebImagePrefetcher to queue the prefetching. It based on the image manager so the image may from the cache and network according to the `options` property.
 * Prefetching is separate to each other, which means the progressBlock and completionBlock you provide is bind to the prefetching for the list of urls.
 * Attention that call this will not cancel previous fetched urls. You should keep the token return by this to cancel or cancel all the prefetch.
 *
 * 分配url列表，让SDWebImagePrefetcher来排队预取。它基于图像管理器，所以图像可以根据“选项”属性从缓存和网络。
 * 预抓取是彼此分开的，这意味着你提供的progressBlock和completionBlock被绑定到url列表的预抓取中。
 * 注意，调用此函数不会取消先前获取的url。你应该保持此方法返回的令牌，通过这个取消或取消所有的预取
 *
 * @param urls            list of URLs to prefetch
 * @param progressBlock   block to be called when progress updates; 
 *                        first parameter is the number of completed (successful or not) requests, 
 *                        second parameter is the total number of images originally requested to be prefetched
 * @param completionBlock block to be called when the current prefetching is completed
 *                        first param is the number of completed (successful or not) requests,
 *                        second parameter is the number of skipped requests
 * @return the token to cancel the current prefetching.
 */
- (nullable SDWebImagePrefetchToken *)prefetchURLs:(nullable NSArray<NSURL *> *)urls
                                          progress:(nullable SDWebImagePrefetcherProgressBlock)progressBlock
                                         completed:(nullable SDWebImagePrefetcherCompletionBlock)completionBlock;

/**
 * Remove and cancel all the prefeching for the prefetcher.
 * 上去和取消所有该预获取器正在进行中的预获取
 */
- (void)cancelPrefetching;


@end
