/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

@class SDImageCacheConfig;
/**
 A protocol to allow custom memory cache used in SDImageCache.
 允许自定义内存缓存在SDImageCache中使用的协议
 */
@protocol SDMemoryCache <NSObject>

@required

/**
 Create a new memory cache instance with the specify cache config. You can check `maxMemoryCost` and `maxMemoryCount` used for memory cache.
 使用指定的缓存配置创建一个新的内存缓存实例。你可以检查用于内存缓存的“maxMemoryCost”和“maxMemoryCount”
 
 @param config The cache config to be used to create the cache. 用于创建缓存对象的缓存配置
 @return The new memory cache instance. 新的内存缓存实例
 */
- (nonnull instancetype)initWithConfig:(nonnull SDImageCacheConfig *)config;

/**
 Returns the value associated with a given key.
 返回跟给定key绑定的值

 @param key An object identifying the value. If nil, just return nil. 一个标识value的对象，如果是nil，则返回nil
 @return The value associated with key, or nil if no value is associated with key. 和key绑定的value，如果没有则为nil
 */
- (nullable id)objectForKey:(nonnull id)key;

/**
 Sets the value of the specified key in the cache (0 cost).
 设置缓存中指定键的值(0代价)

 @param object The object to be stored in the cache. If nil, it calls `removeObjectForKey:`. 被存储在缓存中的对象，如果为nil则调用`removeObjectForKey:`
 @param key    The key with which to associate the value. If nil, this method has no effect. 绑定值的键，如果为空则该方法无效
 @discussion Unlike an NSMutableDictionary object, a cache does not copy the key objects that are put into it. 与NSMutableDictionary对象不同，缓存不复制键
 放入其中的对象。
 */
- (void)setObject:(nullable id)object forKey:(nonnull id)key;

/**
 Sets the value of the specified key in the cache, and associates the key-value pair with the specified cost.
 设置缓存中指定key的值，并将key-value对与指定cost值关联

 @param object The object to store in the cache. If nil, it calls `removeObjectForKey`. 被存储在缓存中的对象，如果为nil则调用`removeObjectForKey:`
 @param key    The key with which to associate the value. If nil, this method has no effect. 绑定值的键，如果为空则该方法无效
 @param cost   The cost with which to associate the key-value pair. 关联键值对的代价
 @discussion Unlike an NSMutableDictionary object, a cache does not copy the key objects that are put into it. 与NSMutableDictionary对象不同，缓存不复制键
 放入其中的对象。
 */
- (void)setObject:(nullable id)object forKey:(nonnull id)key cost:(NSUInteger)cost;

/**
 Removes the value of the specified key in the cache.
 移除缓存中指定键的值

 @param key The key identifying the value to be removed. If nil, this method has no effect.  标识要删除的值的键。如果为nil，则此方法无效
 */
- (void)removeObjectForKey:(nonnull id)key;

/**
 Empties the cache immediately.
 立即清空缓存
 */
- (void)removeAllObjects;

@end

/**
 A memory cache which auto purge the cache on memory warning and support weak cache.
 一种内存缓存，在内存警告时自动清除缓存，并支持弱缓存
 */
@interface SDMemoryCache <KeyType, ObjectType> : NSCache <KeyType, ObjectType> <SDMemoryCache>

@property (nonatomic, strong, nonnull, readonly) SDImageCacheConfig *config;

@end
