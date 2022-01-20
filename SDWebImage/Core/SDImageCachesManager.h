/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDImageCacheDefine.h"

/// Policy for cache operation
/// 缓存操作策略
typedef NS_ENUM(NSUInteger, SDImageCachesManagerOperationPolicy) {
    SDImageCachesManagerOperationPolicySerial, // process all caches serially (from the highest priority to the lowest priority cache by order) 按顺序(从最高优先级到最低优先级)处理所有缓存
    SDImageCachesManagerOperationPolicyConcurrent, // process all caches concurrently  并发处理所有缓存
    SDImageCachesManagerOperationPolicyHighestOnly, // process the highest priority cache only 只处理优先级最高的缓存
    SDImageCachesManagerOperationPolicyLowestOnly // process the lowest priority cache only 只处理最低优先级的缓存
};

/**
 A caches manager to manage multiple caches.
 一个缓存管理器来管理多个缓存
 */
@interface SDImageCachesManager : NSObject <SDImageCache>

/**
 Returns the global shared caches manager instance. By default we will set [`SDImageCache.sharedImageCache`] into the caches array.
 返回全局共享缓存管理器实例。默认情况下，我们将设置[' SDImageCache.sharedImageCache ']到缓存数组
 */
@property (nonatomic, class, readonly, nonnull) SDImageCachesManager *sharedManager;

// These are op policy for cache manager.
// 这些是缓存管理器的操作策略
/**
 Operation policy for query op. 查询操作策略
 Defaults to `Serial`, means query all caches serially (one completion called then next begin) until one cache query success (`image` != nil).
 默认值为' Serial '，表示连续查询所有缓存(一次完成，然后下一个开始)，直到一次缓存查询成功(' image ' != nil)
 */
@property (nonatomic, assign) SDImageCachesManagerOperationPolicy queryOperationPolicy;

/**
 Operation policy for store op. 存储操作策略
 Defaults to `HighestOnly`, means store to the highest priority cache only.
 默认为' HighestOnly '，表示只存储到最高优先级的缓存
 */
@property (nonatomic, assign) SDImageCachesManagerOperationPolicy storeOperationPolicy;

/**
 Operation policy for remove op. 删除操作策略
 Defaults to `Concurrent`, means remove all caches concurrently.
 默认为' Concurrent '，表示同时删除所有缓存
 */
@property (nonatomic, assign) SDImageCachesManagerOperationPolicy removeOperationPolicy;

/**
 Operation policy for contains op. 包含操作策略
 Defaults to `Serial`, means check all caches serially (one completion called then next begin) until one cache check success (`containsCacheType` != None).
 默认为' Serial '，表示连续检查所有缓存(一个完成调用然后下一个开始)，直到一次缓存检查成功(' containsCacheType ' != None)
 */
@property (nonatomic, assign) SDImageCachesManagerOperationPolicy containsOperationPolicy;

/**
 Operation policy for clear op. 清除操作策略
 Defaults to `Concurrent`, means clear all caches concurrently.
 默认为' Concurrent '，表示同时清除所有缓存
 */
@property (nonatomic, assign) SDImageCachesManagerOperationPolicy clearOperationPolicy;

/**
 All caches in caches manager. The caches array is a priority queue, which means the later added cache will have the highest priority
 所有缓存在缓存管理器。缓存数组是一个优先级队列，这意味着后面添加的缓存将具有最高的优先级
 */
@property (nonatomic, copy, nullable) NSArray<id<SDImageCache>> *caches;

/**
 Add a new cache to the end of caches array. Which has the highest priority.
 将一个新的优先级最高的缓存添加到缓存数组的末尾。
 @param cache cache
 */
- (void)addCache:(nonnull id<SDImageCache>)cache;

/**
 Remove a cache in the caches array.
 在缓存数组中删除一个缓存
 @param cache cache
 */
- (void)removeCache:(nonnull id<SDImageCache>)cache;

@end
