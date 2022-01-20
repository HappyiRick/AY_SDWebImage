/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"

typedef NSURLRequest * _Nullable (^SDWebImageDownloaderRequestModifierBlock)(NSURLRequest * _Nonnull request);

/**
 This is the protocol for downloader request modifier.
 We can use a block to specify the downloader request modifier. But Using protocol can make this extensible, and allow Swift user to use it easily instead of using `@convention(block)` to store a block into context options.
 下载器请求修饰符协议
 我们可以使用block来指定下载请求修饰符。但使用协议可以使其可扩展，并允许Swift用户轻松使用它，而不是使用“@convention(block)”将一个块存储到上下文选项中。
 */
@protocol SDWebImageDownloaderRequestModifier <NSObject>

/// Modify the original URL request and return a new one instead. You can modify the HTTP header, cachePolicy, etc for this URL.
/// @param request The original URL request for image loading 图片加载的原始URL请求
/// @note If return nil, the URL request will be cancelled. 如果返回nil，则该URL请求将被取消
/// 修改原始的URL请求并返回一个新的代替。可以修改URL请求的HTTP头、缓存策略等
- (nullable NSURLRequest *)modifiedRequestWithRequest:(nonnull NSURLRequest *)request;

@end

/**
 A downloader request modifier class with block.
 带有代码块的下载器请求修饰符类
 */
@interface SDWebImageDownloaderRequestModifier : NSObject <SDWebImageDownloaderRequestModifier>

/// Create the request modifier with block
/// @param block A block to control modifier logic 控制修改逻辑的代码块
/// 创建一个带有block的请求修改器
- (nonnull instancetype)initWithBlock:(nonnull SDWebImageDownloaderRequestModifierBlock)block;

/// Create the request modifier with block
/// @param block A block to control modifier logic
+ (nonnull instancetype)requestModifierWithBlock:(nonnull SDWebImageDownloaderRequestModifierBlock)block;

@end

/**
A convenient request modifier to provide the HTTP request including HTTP Method, Headers and Body.
一个提供包括HTTP方法、头部和主干的便捷请求修改器
*/
@interface SDWebImageDownloaderRequestModifier (Conveniences)

/// Create the request modifier with HTTP Method.
/// @param method HTTP Method, nil means to GET. HTTP方法, 传空则为GET
/// @note This is for convenience, if you need code to control the logic, use block API instead.
/// 用HTTP方法创建请求修改器
/// 这是为了方便，如果你需要代码来控制逻辑，使用块API代替
- (nonnull instancetype)initWithMethod:(nullable NSString *)method;

/// Create the request modifier with HTTP Headers.
/// @param headers HTTP Headers. Case insensitive according to HTTP/1.1(HTTP/2) standard. The headers will override the same fields from original request.
/// @note This is for convenience, if you need code to control the logic, use block API instead.
/// 用HTTP头来创建请求修改器
/// HTTP/1.1(HTTP/2)标准，不区分大小写。报头将覆盖来自原始请求的相同字段。
- (nonnull instancetype)initWithHeaders:(nullable NSDictionary<NSString *, NSString *> *)headers;

/// Create the request modifier with HTTP Body.
/// @param body HTTP Body.
/// @note This is for convenience, if you need code to control the logic, use block API instead.
/// 用HTTP主干来创建请求修改器
- (nonnull instancetype)initWithBody:(nullable NSData *)body;

/// Create the request modifier with HTTP Method, Headers and Body.
/// @param method HTTP Method, nil means to GET.
/// @param headers HTTP Headers. Case insensitive according to HTTP/1.1(HTTP/2) standard. The headers will override the same fields from original request.
/// @param body HTTP Body.
/// @note This is for convenience, if you need code to control the logic, use block API instead.
- (nonnull instancetype)initWithMethod:(nullable NSString *)method headers:(nullable NSDictionary<NSString *, NSString *> *)headers body:(nullable NSData *)body;

@end
