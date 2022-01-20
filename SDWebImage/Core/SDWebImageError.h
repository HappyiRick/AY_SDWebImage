/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 * (c) Jamie Pinkham
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

FOUNDATION_EXPORT NSErrorDomain const _Nonnull SDWebImageErrorDomain;

/// The response instance for invalid download response (NSURLResponse *)
/// 非法下载响应实例
FOUNDATION_EXPORT NSErrorUserInfoKey const _Nonnull SDWebImageErrorDownloadResponseKey;
/// The HTTP status code for invalid download response (NSNumber *)
/// 非法下载响应HTTP状态码
FOUNDATION_EXPORT NSErrorUserInfoKey const _Nonnull SDWebImageErrorDownloadStatusCodeKey;
/// The HTTP MIME content type for invalid download response (NSString *)
/// 非法下载响应的HTTP MIME 内容类型
FOUNDATION_EXPORT NSErrorUserInfoKey const _Nonnull SDWebImageErrorDownloadContentTypeKey;

/// SDWebImage error domain and codes
/// SDWebImage错误域和代码
typedef NS_ERROR_ENUM(SDWebImageErrorDomain, SDWebImageError) {
    SDWebImageErrorInvalidURL = 1000, // The URL is invalid, such as nil URL or corrupted URL - URL无效，如nil URL或已损坏的URL
    SDWebImageErrorBadImageData = 1001, // The image data can not be decoded to image, or the image data is empty - 图像数据无法被解码为图片，或者图像数据为空
    SDWebImageErrorCacheNotModified = 1002, // The remote location specify that the cached image is not modified, such as the HTTP response 304 code. It's useful for `SDWebImageRefreshCached` - 远程位置指定缓存的映像不被修改，比如HTTP响应304代码。这对‘SDWebImageRefreshCached’很有用
    SDWebImageErrorBlackListed = 1003, // The URL is blacklisted because of unrecoverable failure marked by downloader (such as 404), you can use `.retryFailed` option to avoid this - 该URL被列入黑名单，因为下载程序标记了不可恢复的失败(如404)，你可以使用'。retryFailed '选项来避免这种情况
    SDWebImageErrorInvalidDownloadOperation = 2000, // The image download operation is invalid, such as nil operation or unexpected error occur when operation initialized - 镜像下载操作无效，如nil操作或初始化操作时发生意外错误
    SDWebImageErrorInvalidDownloadStatusCode = 2001, // The image download response a invalid status code. You can check the status code in error's userInfo under `SDWebImageErrorDownloadStatusCodeKey` - 映像下载响应无效的状态码。你可以在错误的userInfo下' SDWebImageErrorDownloadStatusCodeKey '查看状态码
    SDWebImageErrorCancelled = 2002, // The image loading operation is cancelled before finished, during either async disk cache query, or waiting before actual network request. For actual network request error, check `NSURLErrorDomain` error domain and code. - 在异步磁盘缓存查询或等待实际网络请求期间，图像加载操作在完成之前被取消。对于实际的网络请求错误，检查' NSURLErrorDomain '错误域和代码
    SDWebImageErrorInvalidDownloadResponse = 2003, // When using response modifier, the modified download response is nil and marked as failed. - 当使用响应修饰符时，修改后的下载响应为nil，并标记为失败
    SDWebImageErrorInvalidDownloadContentType = 2004, // The image download response a invalid content type. You can check the MIME content type in error's userInfo under `SDWebImageErrorDownloadContentTypeKey` - 映像下载响应的内容类型无效。你可以在错误的userInfo下' SDWebImageErrorDownloadContentTypeKey '检查MIME内容类型
};
