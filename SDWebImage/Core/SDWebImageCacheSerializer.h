/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"

typedef NSData * _Nullable(^SDWebImageCacheSerializerBlock)(UIImage * _Nonnull image, NSData * _Nullable data, NSURL * _Nullable imageURL);

/**
 This is the protocol for cache serializer. - 缓存序列化协议
 We can use a block to specify the cache serializer. But Using protocol can make this extensible, and allow Swift user to use it easily instead of using `@convention(block)` to store a block into context options.
 我们可以使用block来指定缓存序列化器。但使用协议可以使其可扩展，并允许Swift用户轻松使用它，而不是使用“@convention(block)”将一个块存储到上下文选项中
 */
@protocol SDWebImageCacheSerializer <NSObject>

/// Provide the image data associated to the image and store to disk cache - 提供与图像相关的图像数据并存储到磁盘缓存中
/// @param image The loaded image
/// @param data The original loaded image data
/// @param imageURL The image URL
- (nullable NSData *)cacheDataWithImage:(nonnull UIImage *)image originalData:(nullable NSData *)data imageURL:(nullable NSURL *)imageURL;

@end

/**
 A cache serializer class with block.
 带有block的缓存序列类
 */
@interface SDWebImageCacheSerializer : NSObject <SDWebImageCacheSerializer>

- (nonnull instancetype)initWithBlock:(nonnull SDWebImageCacheSerializerBlock)block;
+ (nonnull instancetype)cacheSerializerWithBlock:(nonnull SDWebImageCacheSerializerBlock)block;

@end
