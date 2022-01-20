/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"
#import "SDWebImageDefine.h"
#import "SDWebImageOperation.h"
#import "SDImageCoder.h"
/// 图像加载进度回调
typedef void(^SDImageLoaderProgressBlock)(NSInteger receivedSize, NSInteger expectedSize, NSURL * _Nullable targetURL);
/// 图像完成回调
typedef void(^SDImageLoaderCompletedBlock)(UIImage * _Nullable image, NSData * _Nullable data, NSError * _Nullable error, BOOL finished);

#pragma mark - Context Options

/**
 A `UIImage` instance from `SDWebImageManager` when you specify `SDWebImageRefreshCached` and image cache hit.
 This can be a hint for image loader to load the image from network and refresh the image from remote location if needed. If the image from remote location does not change, you should call the completion with `SDWebImageErrorCacheNotModified` error. (UIImage)
 @note If you don't implement `SDWebImageRefreshCached` support, you do not need to care about this context option.
 当你指定' SDWebImageRefreshCached '和图像缓存命中时，' SDWebImageManager '中的' UIImage '实例
 这可以提示图像加载器从网络加载图像，并在需要时从远程位置刷新图像。如果来自远程位置的图像没有改变，你应该调用' SDWebImageErrorCacheNotModified '错误完成。(用户界面图像)
 注意，如果你没有实现“SDWebImageRefreshCached”支持，你不需要关心这个上下文选项
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextLoaderCachedImage;

#pragma mark - Helper method

/**
 This is the built-in decoding process for image download from network or local file.
 @note If you want to implement your custom loader with `requestImageWithURL:options:context:progress:completed:` API, but also want to keep compatible with SDWebImage's behavior, you'd better use this to produce image.

 @param imageData The image data from the network. Should not be nil 来自网络的图像数据，不能为nil
 @param imageURL The image URL from the input. Should not be nil 输入的图像url，不能为nil
 @param options The options arg from the input 输入选项
 @param context The context arg from the input 输入上下文
 @return The decoded image for current image data load from the network 当前网络图像数据的解码图片
 
 这是从网络或本地文件下载图像的内置解码过程
 注意：如果你想用' requestImageWithURL:options:context:progress:completed: ' API实现你的自定义加载器，但又想保持与SDWebImage的行为兼容，你最好使用这个来生成图像
 */
FOUNDATION_EXPORT UIImage * _Nullable SDImageLoaderDecodeImageData(NSData * _Nonnull imageData, NSURL * _Nonnull imageURL, SDWebImageOptions options, SDWebImageContext * _Nullable context);

/**
 This is the built-in decoding process for image progressive download from network. It's used when `SDWebImageProgressiveLoad` option is set. (It's not required when your loader does not support progressive image loading)
 @note If you want to implement your custom loader with `requestImageWithURL:options:context:progress:completed:` API, but also want to keep compatible with SDWebImage's behavior, you'd better use this to produce image.
 
 这是内置的解码过程，图像渐进式下载从网络。当“SDWebImageProgressiveLoad”选项被设置时使用。(当你的加载器不支持渐进图像加载时，它是不需要的)
 如果你想用' requestImageWithURL:options:context:progress:completed: ' API实现你的自定义加载器，但又想保持与SDWebImage的行为兼容，你最好使用这个来生成图像。

 @param imageData The image data from the network so far. Should not be nil  目前网络上的图像数据，不应为nil
 @param imageURL The image URL from the input. Should not be nil  输入的图像url,不应为nil
 @param finished Pass NO to specify the download process has not finished. Pass YES when all image data has finished. NO标识下载进度还未完成，YES表示完成
 @param operation The loader operation associated with current progressive download. Why to provide this is because progressive decoding need to store the partial decoded context for each operation to avoid conflict. You should provide the operation from `loadImageWithURL:` method return value. 与当前进度下载相关联的加载程序操作。之所以要提供这一点，是因为渐进解码需要为每个操作存储部分解码的上下文，以避免冲突。你应该提供从' loadImageWithURL: '方法返回值的操作。
 @param options The options arg from the input 输入选项参数
 @param context The context arg from the input 输入上下文参数
 @return The decoded progressive image for current image data load from the network 当前网络图像数据的渐进解码图像
 */
FOUNDATION_EXPORT UIImage * _Nullable SDImageLoaderDecodeProgressiveImageData(NSData * _Nonnull imageData, NSURL * _Nonnull imageURL, BOOL finished,  id<SDWebImageOperation> _Nonnull operation, SDWebImageOptions options, SDWebImageContext * _Nullable context);

/**
 This function get the progressive decoder for current loading operation. If no progressive decoding is happended or decoder is not able to construct, return nil.
 @return The progressive decoder associated with the loading operation.
 这个函数获取当前加载操作的递进解码器。如果没有发生递进解码或解码器无法构造，则返回nil。
 @return 与加载操作相关联的递进解码器
 */
FOUNDATION_EXPORT id<SDProgressiveImageCoder> _Nullable SDImageLoaderGetProgressiveCoder(id<SDWebImageOperation> _Nonnull operation);

/**
 This function set the progressive decoder for current loading operation. If no progressive decoding is happended, pass nil.
 @param operation The loading operation to associate the progerssive decoder.
 此函数设置当前加载操作的递进解码器。如果没有发生递进解码，则传递nil值
 跟渐进解码器关联的加载操作
 */
FOUNDATION_EXPORT void SDImageLoaderSetProgressiveCoder(id<SDWebImageOperation> _Nonnull operation, id<SDProgressiveImageCoder> _Nullable progressiveCoder);

#pragma mark - SDImageLoader

/**
 This is the protocol to specify custom image load process. You can create your own class to conform this protocol and use as a image loader to load image from network or any available remote resources defined by yourself.
 If you want to implement custom loader for image download from network or local file, you just need to concentrate on image data download only. After the download finish, call `SDImageLoaderDecodeImageData` or `SDImageLoaderDecodeProgressiveImageData` to use the built-in decoding process and produce image (Remember to call in the global queue). And finally callback the completion block.
 If you directly get the image instance using some third-party SDKs, such as image directly from Photos framework. You can process the image data and image instance by yourself without that built-in decoding process. And finally callback the completion block.
 @note It's your responsibility to load the image in the desired global queue(to avoid block main queue). We do not dispatch these method call in a global queue but just from the call queue (For `SDWebImageManager`, it typically call from the main queue).
 
 这是用来指定自定义图像加载过程的协议。您可以创建自己的类来遵循这个协议，并使用它作为图像加载器从网络或任何自己定义的可用远程资源加载图像
 如果您想实现从网络或本地文件下载图像的自定义加载器，您只需要集中于图像数据的下载。下载完成后，调用' SDImageLoaderDecodeImageData '或' SDImageLoaderDecodeProgressiveImageData '来使用内置的解码过程并生成图像(记住调用全局队列)。最后回调完成块。
 如果您使用一些第三方sdk直接获取图像实例，例如直接从Photos框架获取图像。您可以自己处理图像数据和图像实例，而不需要内置的解码过程。最后回调完成块。
 你的职责是将图像加载到所需的全局队列中(以避免阻塞主队列)。我们不是在全局队列中调度这些方法调用，而是从调用队列中调度(对于' SDWebImageManager '，它通常从主队列中调用)。
*/
@protocol SDImageLoader <NSObject>

@required
/**
 Whether current image loader supports to load the provide image URL.
 This will be checked every time a new image request come for loader. If this return NO, we will mark this image load as failed. If return YES, we will start to call `requestImageWithURL:options:context:progress:completed:`.

 当前图像加载器是否支持加载提供的图像URL
 这将在每次加载器的新图像请求时被检查。如果这个返回NO，我们将把这个图像加载标记为失败。如果返回YES，我们将开始调用' requestImageWithURL:options:context:progress:completed: '。
 
 @param url The image URL to be loaded. 加载的图像url
 @return YES to continue download, NO to stop download. YES则继续下载，NO的话停止下载
 */
- (BOOL)canRequestImageForURL:(nullable NSURL *)url API_DEPRECATED("Use canRequestImageForURL:options:context: instead", macos(10.10, API_TO_BE_DEPRECATED), ios(8.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED));

@optional
/**
 Whether current image loader supports to load the provide image URL, with associated options and context.
 This will be checked every time a new image request come for loader. If this return NO, we will mark this image load as failed. If return YES, we will start to call `requestImageWithURL:options:context:progress:completed:`.
 
 当前图像加载器是否支持加载提供的图像URL，以及相关的选项和上下文。
 这将在每次加载器的新图像请求时被检查。如果这个返回NO，我们将把这个图像加载标记为失败。如果返回YES，我们将开始调用' requestImageWithURL:options:context:progress:completed: '。

 @param url The image URL to be loaded. 加载的图像url
 @param options A mask to specify options to use for this request  指定此请求使用的选项的掩码
 @param context A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold. 上下文包含不同的选项来执行指定的更改或过程，请参阅“SDWebImageContextOption”。这个保存了' options ' 枚举不能保存的额外对象。
 @return YES to continue download, NO to stop download. YES继续下载，NO停止下载
 */
- (BOOL)canRequestImageForURL:(nullable NSURL *)url
                      options:(SDWebImageOptions)options
                      context:(nullable SDWebImageContext *)context;

@required
/**
 Load the image and image data with the given URL and return the image data. You're responsible for producing the image instance.
 用给定的URL加载图像和图像数据，并返回图像数据。负责生成映像实例

 @param url The URL represent the image. Note this may not be a HTTP URL 表示图像的URL，注意该URL不一定是HTTP的
 @param options A mask to specify options to use for this request 指定此请求使用的选项的掩码
 @param context A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold. 上下文包含不同的选项来执行指定的更改或过程，请参阅“SDWebImageContextOption”。这个保存了' options ' 枚举不能保存的额外对象
 @param progressBlock A block called while image is downloading  下载进度block，后台运行
 *                    @note the progress block is executed on a background queue
 @param completedBlock A block called when operation has been completed. 下载完成block
 @return An operation which allow the user to cancel the current request. 一个允许用户取消当前请求的操作实例
 */
- (nullable id<SDWebImageOperation>)requestImageWithURL:(nullable NSURL *)url
                                                options:(SDWebImageOptions)options
                                                context:(nullable SDWebImageContext *)context
                                               progress:(nullable SDImageLoaderProgressBlock)progressBlock
                                              completed:(nullable SDImageLoaderCompletedBlock)completedBlock;


/**
 Whether the error from image loader should be marked indeed un-recoverable or not.
 If this return YES, failed URL which does not using `SDWebImageRetryFailed` will be blocked into black list. Else not.
 
 来自图像加载器的错误是否应该被标记为确实不可恢复。
 如果这个返回YES，失败的URL不使用' SDWebImageRetryFailed '将被阻塞到黑名单。其他的没有。

 @param url The URL represent the image. Note this may not be a HTTP URL 表示图像的URL，注意该URL不一定是HTTP的
 @param error The URL's loading error, from previous `requestImageWithURL:options:context:progress:completed:` completedBlock's error. URL加载错误，即前置`requestImageWithURL:options:context:progress:completed:`方法完成回调中的错误
 @return Whether to block this url or not. Return YES to mark this URL as failed. 是否阻塞该url,返回YES则标记该url失败
 */
- (BOOL)shouldBlockFailedURLWithURL:(nonnull NSURL *)url
                              error:(nonnull NSError *)error API_DEPRECATED("Use shouldBlockFailedURLWithURL:error:options:context: instead", macos(10.10, API_TO_BE_DEPRECATED), ios(8.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED));

@optional
/**
 Whether the error from image loader should be marked indeed un-recoverable or not, with associated options and context.
 If this return YES, failed URL which does not using `SDWebImageRetryFailed` will be blocked into black list. Else not.
 
 是否应该用相关的选项和上下文将来自图像加载器的错误标记为确实不可恢复

 @param url The URL represent the image. Note this may not be a HTTP URL
 @param error The URL's loading error, from previous `requestImageWithURL:options:context:progress:completed:` completedBlock's error.
 @param options A mask to specify options to use for this request
 @param context A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
 @return Whether to block this url or not. Return YES to mark this URL as failed.
 */
- (BOOL)shouldBlockFailedURLWithURL:(nonnull NSURL *)url
                              error:(nonnull NSError *)error
                            options:(SDWebImageOptions)options
                            context:(nullable SDWebImageContext *)context;

@end
