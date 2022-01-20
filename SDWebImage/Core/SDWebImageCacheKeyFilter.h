/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"

typedef NSString * _Nullable(^SDWebImageCacheKeyFilterBlock)(NSURL * _Nonnull url);

/**
 This is the protocol for cache key filter. - 缓存key过滤协议
 We can use a block to specify the cache key filter. But Using protocol can make this extensible, and allow Swift user to use it easily instead of using `@convention(block)` to store a block into context options.
 我们可以使用block来指定缓存键过滤器。但使用协议可以使其可扩展，并允许Swift用户轻松使用它，而不是使用“@convention(block)”将一个块存储到上下文选项中。
 */
@protocol SDWebImageCacheKeyFilter <NSObject>

- (nullable NSString *)cacheKeyForURL:(nonnull NSURL *)url;

@end

/**
 A cache key filter class with block.
 带block的缓存key过滤类
 */
@interface SDWebImageCacheKeyFilter : NSObject <SDWebImageCacheKeyFilter>

- (nonnull instancetype)initWithBlock:(nonnull SDWebImageCacheKeyFilterBlock)block;
+ (nonnull instancetype)cacheKeyFilterWithBlock:(nonnull SDWebImageCacheKeyFilterBlock)block;

@end
