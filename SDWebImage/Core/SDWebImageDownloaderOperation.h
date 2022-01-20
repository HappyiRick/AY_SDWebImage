/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageDownloader.h"
#import "SDWebImageOperation.h"

/**
 Describes a downloader operation. If one wants to use a custom downloader op, it needs to inherit from `NSOperation` and conform to this protocol
 For the description about these methods, see `SDWebImageDownloaderOperation`
 描述下载程序操作。如果你想使用一个自定义的下载操作，它需要从“NSOperation”继承并遵循这个协议
 关于这些方法的描述，请参见“SDWebImageDownloaderOperation”。
 @note If your custom operation class does not use `NSURLSession` at all, do not implement the optional methods and session delegate methods.
 如果你的自定义操作类根本不使用' NSURLSession '，不要实现可选方法和会话委托方法
 */
@protocol SDWebImageDownloaderOperation <NSURLSessionTaskDelegate, NSURLSessionDataDelegate>
@required
- (nonnull instancetype)initWithRequest:(nullable NSURLRequest *)request
                              inSession:(nullable NSURLSession *)session
                                options:(SDWebImageDownloaderOptions)options;

- (nonnull instancetype)initWithRequest:(nullable NSURLRequest *)request
                              inSession:(nullable NSURLSession *)session
                                options:(SDWebImageDownloaderOptions)options
                                context:(nullable SDWebImageContext *)context;

- (nullable id)addHandlersForProgress:(nullable SDWebImageDownloaderProgressBlock)progressBlock
                            completed:(nullable SDWebImageDownloaderCompletedBlock)completedBlock;

- (BOOL)cancel:(nullable id)token;
/// 请求
@property (strong, nonatomic, readonly, nullable) NSURLRequest *request;
/// 响应
@property (strong, nonatomic, readonly, nullable) NSURLResponse *response;

@optional
/// 数据绘画任务
@property (strong, nonatomic, readonly, nullable) NSURLSessionTask *dataTask;
/// 封装会话任务度量的对象
@property (strong, nonatomic, readonly, nullable) NSURLSessionTaskMetrics *metrics API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

// These operation-level config was inherited from downloader. See `SDWebImageDownloaderConfig` for documentation.
/// 这些操作级配置是从下载器继承的。请参阅“SDWebImageDownloaderConfig”文档
@property (strong, nonatomic, nullable) NSURLCredential *credential;
@property (assign, nonatomic) double minimumProgressInterval;
@property (copy, nonatomic, nullable) NSIndexSet *acceptableStatusCodes;
@property (copy, nonatomic, nullable) NSSet<NSString *> *acceptableContentTypes;

@end


/**
 The download operation class for SDWebImageDownloader.
 SDWebImageDownloader的下载操作类
 */
@interface SDWebImageDownloaderOperation : NSOperation <SDWebImageDownloaderOperation>

/**
 * The request used by the operation's task.
 * 操作任务使用的请求
 */
@property (strong, nonatomic, readonly, nullable) NSURLRequest *request;

/**
 * The response returned by the operation's task.
 * 操作的任务返回的响应
 */
@property (strong, nonatomic, readonly, nullable) NSURLResponse *response;

/**
 * The operation's task
 * 操作任务
 */
@property (strong, nonatomic, readonly, nullable) NSURLSessionTask *dataTask;

/**
 * The collected metrics from `-URLSession:task:didFinishCollectingMetrics:`.
 * This can be used to collect the network metrics like download duration, DNS lookup duration, SSL handshake duration, etc. See Apple's documentation: https://developer.apple.com/documentation/foundation/urlsessiontaskmetrics
 * 从' -URLSession:task:didFinishCollectingMetrics: '中收集的度量
 * 这可以用来收集网络指标，如下载持续时间、DNS查找持续时间、SSL握手持续时间等。
 * 参见苹果的文档:https://developer.apple.com/documentation/foundation/urlsessiontaskmetrics
 */
@property (strong, nonatomic, readonly, nullable) NSURLSessionTaskMetrics *metrics API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

/**
 * The credential used for authentication challenges in `-URLSession:task:didReceiveChallenge:completionHandler:`.
 * 在' -URLSession:task: didreceivecchallenge: completionHandler: '中用于认证挑战的凭据
 *
 * This will be overridden by any shared credentials that exist for the username or password of the request URL, if present.
 * 这将被存在于请求URL的用户名或密码的任何共享凭据覆盖(如果存在的话)
 */
@property (strong, nonatomic, nullable) NSURLCredential *credential;

/**
 * The minimum interval about progress percent during network downloading. Which means the next progress callback and current progress callback's progress percent difference should be larger or equal to this value. However, the final finish download progress callback does not get effected.
 * The value should be 0.0-1.0.
 * @note If you're using progressive decoding feature, this will also effect the image refresh rate.
 * @note This value may enhance the performance if you don't want progress callback too frequently.
 * Defaults to 0, which means each time we receive the new data from URLSession, we callback the progressBlock immediately.
 * 网络下载过程中进度百分比的最小间隔。这意味着下一个进度回调和当前进度回调的进度百分比差应该大于或等于这个值。但是，最终完成下载进度回调不会受到影响
 * 该值为0.0 - 1.0之间
 * @note 如果您正在使用渐进式解码功能，这也将影响图像刷新率
 * @note 该值会提高性能如果不频繁调用progress回调的话
 * 默认为0, 意味着每次收到新的数据时，都会立即调用progressBlock
 *
 */
@property (assign, nonatomic) double minimumProgressInterval;

/**
 * Set the acceptable HTTP Response status code. The status code which beyond the range will mark the download operation failed.
 * For example, if we config [200, 400) but server response is 503, the download will fail with error code `SDWebImageErrorInvalidDownloadStatusCode`.
 * Defaults to [200,400). Nil means no validation at all.
 * 设置可接受的HTTP响应状态码。超出范围的状态码将标记下载操作失败。
 * 例如，如果我们配置[200,400)，但服务器响应是503，下载将失败，错误码' SDWebImageErrorInvalidDownloadStatusCode '
 * 默认为[200, 400). Nil 意味着不验证
 */
@property (copy, nonatomic, nullable) NSIndexSet *acceptableStatusCodes;

/**
 * Set the acceptable HTTP Response content type. The content type beyond the set will mark the download operation failed.
 * For example, if we config ["image/png"] but server response is "application/json", the download will fail with error code `SDWebImageErrorInvalidDownloadContentType`.
 * Normally you don't need this for image format detection because we use image's data file signature magic bytes: https://en.wikipedia.org/wiki/List_of_file_signatures
 * Defaults to nil. Nil means no validation at all.
 *
 * 设置可接受的HTTP响应内容类型。超出集合的内容类型将标记下载操作失败
 * 例如，如果我们配置["image/png"]，但服务器的响应是"application/json"，下载将失败，错误代码' SDWebImageErrorInvalidDownloadContentType '。
 * 通常您不需要这个图像格式检测，因为我们使用图像的数据文件签名魔术字节:https://en.wikipedia.org/wiki/List_of_file_signatures
 * 默认为nil， 意味着无验证
 */
@property (copy, nonatomic, nullable) NSSet<NSString *> *acceptableContentTypes;

/**
 * The options for the receiver.
 * 接收方的选项
 */
@property (assign, nonatomic, readonly) SDWebImageDownloaderOptions options;

/**
 * The context for the receiver.
 * 接收方的上下文
 */
@property (copy, nonatomic, readonly, nullable) SDWebImageContext *context;

/**
 *  Initializes a `SDWebImageDownloaderOperation` object
 *  初始化一个`SDWebImageDownloaderOperation`对象
 *
 *  @see SDWebImageDownloaderOperation
 *
 *  @param request        the URL request - URL请求
 *  @param session        the URL session in which this operation will run - 运行该操作的URL会话
 *  @param options        downloader options - 下载器选项
 *
 *  @return the initialized instance 经过初始化的实例
 */
- (nonnull instancetype)initWithRequest:(nullable NSURLRequest *)request
                              inSession:(nullable NSURLSession *)session
                                options:(SDWebImageDownloaderOptions)options;

/**
 *  Initializes a `SDWebImageDownloaderOperation` object
 *  初始化一个`SDWebImageDownloaderOperation`对象
 *
 *  @see SDWebImageDownloaderOperation
 *
 *  @param request        the URL request - URL请求
 *  @param session        the URL session in which this operation will run - 运行该操作的URL会话
 *  @param options        downloader options - 下载器选项
 *  @param context        A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold. -  上下文包含不同的选项来执行指定的更改或过程，请参阅“SDWebImageContextOption”。这个保存了' options ' 枚举不能保存的额外对象
 *
 *  @return the initialized instance
 */
- (nonnull instancetype)initWithRequest:(nullable NSURLRequest *)request
                              inSession:(nullable NSURLSession *)session
                                options:(SDWebImageDownloaderOptions)options
                                context:(nullable SDWebImageContext *)context NS_DESIGNATED_INITIALIZER;

/**
 *  Adds handlers for progress and completion. Returns a token that can be passed to -cancel: to cancel this set of callbacks.
 *  为进度和完成添加处理程序。返回一个可以传递给-cancel:取消这组回调的令牌
 *
 *  @param progressBlock  the block executed when a new chunk of data arrives. - 当新数据到达时执行，默认在后台队列执行
 *                        @note the progress block is executed on a background queue
 *  @param completedBlock the block executed when the download is done. - 下载完成时执行，完成的块在主队列上执行以获得成功。如果发现错误，则该块有可能在后台队列上执行
 *                        @note the completed block is executed on the main queue for success. If errors are found, there is a chance the block will be executed on a background queue
 *
 *  @return the token to use to cancel this set of handlers - 用来取消这组处理程序的token
 */
- (nullable id)addHandlersForProgress:(nullable SDWebImageDownloaderProgressBlock)progressBlock
                            completed:(nullable SDWebImageDownloaderCompletedBlock)completedBlock;

/**
 *  Cancels a set of callbacks. Once all callbacks are canceled, the operation is cancelled.
 *  取消一组回调。一旦所有回调被取消，操作也就被取消了
 *
 *  @param token the token representing a set of callbacks to cancel - 表示要取消的一组回调的令牌
 *
 *  @return YES if the operation was stopped because this was the last token to be canceled. NO otherwise. - 如果操作因为这是最后一个要取消的令牌而停止为YES，否则为NO.
 */
- (BOOL)cancel:(nullable id)token;

@end
