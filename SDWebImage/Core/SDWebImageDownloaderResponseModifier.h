/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"

typedef NSURLResponse * _Nullable (^SDWebImageDownloaderResponseModifierBlock)(NSURLResponse * _Nonnull response);

/**
 This is the protocol for downloader response modifier.
 We can use a block to specify the downloader response modifier. But Using protocol can make this extensible, and allow Swift user to use it easily instead of using `@convention(block)` to store a block into context options.
 下载器相应修改协议
 可以使用block指定下载响应修饰符。但使用协议可以使其可扩展，并允许Swift用户轻松使用它，而不是使用“@convention(block)”将一个块存储到上下文选项中
 */
@protocol SDWebImageDownloaderResponseModifier <NSObject>

/// Modify the original URL response and return a new response. You can use this to check MIME-Type, mock server response, etc.
/// @param response The original URL response, note for HTTP request it's actually a `NSHTTPURLResponse` instance
/// @note If nil is returned, the image download will marked as cancelled with error `SDWebImageErrorInvalidDownloadResponse`
/// 修改原始URL响应并返回一个新的响应。您可以使用它来检查MIME-Type、模拟服务器响应等
/// 原始的URL响应，注意对于HTTP请求它实际上是一个' NSHTTPURLResponse '实例
/// 如果返回nil，图像下载将被标记为取消，错误为'SDWebImageErrorInvalidDownloadResponse '
- (nullable NSURLResponse *)modifiedResponseWithResponse:(nonnull NSURLResponse *)response;

@end

/**
 A downloader response modifier class with block.
 一个带有block的下载响应修饰符类。
 */
@interface SDWebImageDownloaderResponseModifier : NSObject <SDWebImageDownloaderResponseModifier>

/// Create the response modifier with block
/// @param block A block to control modifier logic
/// 创建带有block的下载相应修饰符
/// 控制修改逻辑的block
- (nonnull instancetype)initWithBlock:(nonnull SDWebImageDownloaderResponseModifierBlock)block;

/// Create the response modifier with block
/// @param block A block to control modifier logic
+ (nonnull instancetype)responseModifierWithBlock:(nonnull SDWebImageDownloaderResponseModifierBlock)block;

@end

/**
A convenient response modifier to provide the HTTP response including HTTP Status Code, Version and Headers.
 提供包括HTTP状态码、版本和头的便捷响应修饰符
*/
@interface SDWebImageDownloaderResponseModifier (Conveniences)

/// Create the response modifier with HTTP Status code.
/// @param statusCode HTTP Status Code.
/// @note This is for convenience, if you need code to control the logic, use block API instead.
/// 用HTTP状态码创建一个响应修饰符
- (nonnull instancetype)initWithStatusCode:(NSInteger)statusCode;

/// Create the response modifier with HTTP Version. Status code defaults to 200.
/// @param version HTTP Version, nil means "HTTP/1.1".
/// @note This is for convenience, if you need code to control the logic, use block API instead.
/// 用HTTP版本创建一个响应修饰符，默认状态码为200, 默认版本为“HTTP/1.1”
- (nonnull instancetype)initWithVersion:(nullable NSString *)version;

/// Create the response modifier with HTTP Headers. Status code defaults to 200.
/// @param headers HTTP Headers. Case insensitive according to HTTP/1.1(HTTP/2) standard. The headers will override the same fields from original response.
/// @note This is for convenience, if you need code to control the logic, use block API instead.
/// 用HTTP头创建一个响应修饰符，默认状态码200，HTTP/1.1(HTTP/2)标准，不区分大小写。报头将覆盖来自原始响应的相同字段。
- (nonnull instancetype)initWithHeaders:(nullable NSDictionary<NSString *, NSString *> *)headers;

/// Create the response modifier with HTTP Status Code, Version and Headers.
/// @param statusCode HTTP Status Code.
/// @param version HTTP Version, nil means "HTTP/1.1".
/// @param headers HTTP Headers. Case insensitive according to HTTP/1.1(HTTP/2) standard. The headers will override the same fields from original response.
/// @note This is for convenience, if you need code to control the logic, use block API instead.
- (nonnull instancetype)initWithStatusCode:(NSInteger)statusCode version:(nullable NSString *)version headers:(nullable NSDictionary<NSString *, NSString *> *)headers;

@end
