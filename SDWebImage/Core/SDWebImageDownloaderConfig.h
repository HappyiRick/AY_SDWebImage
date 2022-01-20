/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"

/// Operation execution order
/// 操作执行顺序
typedef NS_ENUM(NSInteger, SDWebImageDownloaderExecutionOrder) {
    /**
     * Default value. All download operations will execute in queue style (first-in-first-out).
     * 默认值，所有下载操作会按照先入先出的顺序执行 FIFO
     */
    SDWebImageDownloaderFIFOExecutionOrder,
    
    /**
     * All download operations will execute in stack style (last-in-first-out).
     * 所有的下载操作会按照 后入先出 的顺序执行 LIFO
     */
    SDWebImageDownloaderLIFOExecutionOrder
};

/**
 The class contains all the config for image downloader
 @note This class conform to NSCopying, make sure to add the property in `copyWithZone:` as well.
 该类包含了所有对于图像下载器的配置
 @note 该类遵循了NSCopying协议，确保在' copyWithZone: '中添加属性
 */
@interface SDWebImageDownloaderConfig : NSObject <NSCopying>

/**
 Gets the default downloader config used for shared instance or initialization when it does not provide any downloader config. Such as `SDWebImageDownloader.sharedDownloader`.
 @note You can modify the property on default downloader config, which can be used for later created downloader instance. The already created downloader instance does not get affected.
 获取用于共享实例或初始化的默认下载配置(当它不提供任何下载配置时)。如“SDWebImageDownloader.sharedDownloader”。
 @note 您可以修改默认的downloader配置上的属性，该属性可用于以后创建的downloader实例。已经创建的下载器实例不会受到影响。
 */
@property (nonatomic, class, readonly, nonnull) SDWebImageDownloaderConfig *defaultDownloaderConfig;

/**
 * The maximum number of concurrent downloads.
 * Defaults to 6.
 * 最大并行下载数，默认为6
 */
@property (nonatomic, assign) NSInteger maxConcurrentDownloads;

/**
 * The timeout value (in seconds) for each download operation.
 * Defaults to 15.0.
 * 下载超时时间，默认为15s
 */
@property (nonatomic, assign) NSTimeInterval downloadTimeout;

/**
 * The minimum interval about progress percent during network downloading. Which means the next progress callback and current progress callback's progress percent difference should be larger or equal to this value. However, the final finish download progress callback does not get effected.
 * The value should be 0.0-1.0.
 * @note If you're using progressive decoding feature, this will also effect the image refresh rate.
 * @note This value may enhance the performance if you don't want progress callback too frequently.
 * Defaults to 0, which means each time we receive the new data from URLSession, we callback the progressBlock immediately.
 * 网络下载过程中进度百分比的最小间隔。这意味着下一个进度回调和当前进度回调的进度百分比差应该大于或等于这个值。但是，最终完成下载进度回调不会受到影响。
 * 该值处于 0.0 ～ 1.0 之间
 * @note 如果您正在使用渐进式解码功能，这也将影响图像刷新率
 * @note 如果不频繁调用progress代码块的话，将提高性能
 * 默认为0，意味着每次收到新数据都会立即调用progress代码块
 */
@property (nonatomic, assign) double minimumProgressInterval;

/**
 * The custom session configuration in use by NSURLSession. If you don't provide one, we will use `defaultSessionConfiguration` instead.
 * Defatuls to nil.
 * @note This property does not support dynamic changes, means it's immutable after the downloader instance initialized.
 * NSURLSession使用的自定义会话配置.如果不设置的话默认为`defaultSessionConfiguration`
 * 默认为nil
 * @note 该属性不支持动态修改，意味着在下载器实例初始化之后，它是不可变的
 */
@property (nonatomic, strong, nullable) NSURLSessionConfiguration *sessionConfiguration;

/**
 * Gets/Sets a subclass of `SDWebImageDownloaderOperation` as the default
 * `NSOperation` to be used each time SDWebImage constructs a request
 * operation to download an image.
 * Defaults to nil.
 * @note Passing `NSOperation<SDWebImageDownloaderOperation>` to set as default. Passing `nil` will revert to `SDWebImageDownloaderOperation`.
 * 获取/设置' SDWebImageDownloaderOperation '的默认子类
 * 每次SDWebImage构造一个请求时使用‘NSOperation’操作来下载图片
 * 默认为nil
 * @note 传递' NSOperation<SDWebImageDownloaderOperation> '设置为默认值。传递' nil '将恢复为' SDWebImageDownloaderOperation '
 */
@property (nonatomic, assign, nullable) Class operationClass;

/**
 * Changes download operations execution order.
 * Defaults to `SDWebImageDownloaderFIFOExecutionOrder`.
 * 改变下载操作执行顺序
 * 默认为FIFO
 */
@property (nonatomic, assign) SDWebImageDownloaderExecutionOrder executionOrder;

/**
 * Set the default URL credential to be set for request operations.
 * Defaults to nil.
 * 设置为请求操作设置的默认URL凭据
 * 默认为nil
 */
@property (nonatomic, copy, nullable) NSURLCredential *urlCredential;

/**
 * Set username using for HTTP Basic authentication.
 * Defaults to nil.
 * 设置HTTP基本认证使用的用户名
 * 默认为nil
 */
@property (nonatomic, copy, nullable) NSString *username;

/**
 * Set password using for HTTP Basic authentication.
 * Defaults to nil.
 * 设置HTTP基本身份验证的密码
 * 默认为nil
 */
@property (nonatomic, copy, nullable) NSString *password;

/**
 * Set the acceptable HTTP Response status code. The status code which beyond the range will mark the download operation failed.
 * For example, if we config [200, 400) but server response is 503, the download will fail with error code `SDWebImageErrorInvalidDownloadStatusCode`.
 * Defaults to [200,400). Nil means no validation at all.
 * 设置可接受的HTTP响应状态码。超出范围的状态码将标记下载操作失败
 * 例如，如果我们配置[200,400)，但服务器的响应是503，下载将失败，错误码' SDWebImageErrorInvalidDownloadStatusCode '。
 * 默认为[200, 400)，nil 表示没有验证
 */
@property (nonatomic, copy, nullable) NSIndexSet *acceptableStatusCodes;

/**
 * Set the acceptable HTTP Response content type. The content type beyond the set will mark the download operation failed.
 * For example, if we config ["image/png"] but server response is "application/json", the download will fail with error code `SDWebImageErrorInvalidDownloadContentType`.
 * Normally you don't need this for image format detection because we use image's data file signature magic bytes: https://en.wikipedia.org/wiki/List_of_file_signatures
 * Defaults to nil. Nil means no validation at all.
 * 设置可接受的HTTP响应内容类型。超出集合的内容类型将标记下载操作失败
 * 例如，如果我们配置["image/png"]，但服务器的响应是"application/json"，下载将失败，错误代码' SDWebImageErrorInvalidDownloadContentType '。
 * 通常您不需要这个图像格式检测，因为我们使用图像的数据文件签名魔术字节:https://en.wikipedia.org/wiki/List_of_file_signatures
 * 默认为nil，标识不需要检测
 */
@property (nonatomic, copy, nullable) NSSet<NSString *> *acceptableContentTypes;

@end
