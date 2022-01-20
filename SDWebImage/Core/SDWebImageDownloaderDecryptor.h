/*
* This file is part of the SDWebImage package.
* (c) Olivier Poitrey <rs@dailymotion.com>
*
* For the full copyright and license information, please view the LICENSE
* file that was distributed with this source code.
*/

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"

typedef NSData * _Nullable (^SDWebImageDownloaderDecryptorBlock)(NSData * _Nonnull data, NSURLResponse * _Nullable response);

/**
This is the protocol for downloader decryptor. Which decrypt the original encrypted data before decoding. Note progressive decoding is not compatible for decryptor.
We can use a block to specify the downloader decryptor. But Using protocol can make this extensible, and allow Swift user to use it easily instead of using `@convention(block)` to store a block into context options.
 下载解密器协议。在解码之前先解密加密数据，注意，解密器不兼容渐进式解码
 我们可以使用一个块来指定下载程序解密器。但使用协议可以使其可扩展，并允许Swift用户轻松使用它，而不是使用“@convention(block)”将一个块存储到上下文选项中。
*/
@protocol SDWebImageDownloaderDecryptor <NSObject>

/// Decrypt the original download data and return a new data. You can use this to decrypt the data using your preferred algorithm.
/// @param data The original download data 原始下载数据
/// @param response The URL response for data. If you modify the original URL response via response modifier, the modified version will be here. This arg is nullable. 数据的URL响应。如果您通过响应修饰符修改了原始的URL响应，修改后的版本将在这里。这个参数是空的。
/// @note If nil is returned, the image download will be marked as failed with error `SDWebImageErrorBadImageData`
///
/// 解密原始下载数据并返回新的数据，您可以通过它用喜欢的算法解密数据
/// 如果返回nil，图像下载将被标记为失败，错误' SDWebImageErrorBadImageData '
///
- (nullable NSData *)decryptedDataWithData:(nonnull NSData *)data response:(nullable NSURLResponse *)response;

@end

/**
A downloader response modifier class with block.
 一个带block的下载响应修改器类
*/
@interface SDWebImageDownloaderDecryptor : NSObject <SDWebImageDownloaderDecryptor>

/// Create the data decryptor with block
/// @param block A block to control decrypt logic
/// 创建一个带block的数据解密器
- (nonnull instancetype)initWithBlock:(nonnull SDWebImageDownloaderDecryptorBlock)block;

/// Create the data decryptor with block
/// @param block A block to control decrypt logic
/// 创建一个带block的数据解密器
+ (nonnull instancetype)decryptorWithBlock:(nonnull SDWebImageDownloaderDecryptorBlock)block;

@end

/// Convenience way to create decryptor for common data encryption.
/// 快速创建针对普通数据加密的解密器
@interface SDWebImageDownloaderDecryptor (Conveniences)

/// Base64 Encoded image data decryptor
/// Base64 编码图像数据解密器
@property (class, readonly, nonnull) SDWebImageDownloaderDecryptor *base64Decryptor;

@end
