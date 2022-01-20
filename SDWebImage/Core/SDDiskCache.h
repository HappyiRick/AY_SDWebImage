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
 A protocol to allow custom disk cache used in SDImageCache.
 允许自定义硬盘缓存用于SDImageCache的协议
 */
@protocol SDDiskCache <NSObject>

// All of these method are called from the same global queue to avoid blocking on main queue and thread-safe problem. But it's also recommend to ensure thread-safe yourself using lock or other ways.
// 所有这些方法都从同一个全局队列中调用，以避免主队列阻塞和线程安全问题。但也建议您自己使用lock或其他方式来确保线程安全。
@required
/**
 Create a new disk cache based on the specified path. You can check `maxDiskSize` and `maxDiskAge` used for disk cache.
 按照指定的路径创建新的磁盘缓存。你可以检查“maxDiskSize”和“maxDiskAge”用于磁盘缓存
 
 @param cachePath Full path of a directory in which the cache will write data. 
 Once initialized you should not read and write to this directory. 缓存将在其中写数据的目录的全路径。初始化后，您不应该对该目录进行读写操作
 @param config The cache config to be used to create the cache. 用于创建缓存对象的配置项
 
 @return A new cache object, or nil if an error occurs. 一个新的缓存对象，发生错误时为nil
 */
- (nullable instancetype)initWithCachePath:(nonnull NSString *)cachePath config:(nonnull SDImageCacheConfig *)config;

/**
 Returns a boolean value that indicates whether a given key is in cache.
 This method may blocks the calling thread until file read finished.
 返回一个标识给定key绑定内容是否在缓存中的布尔值
 这个方法可能会阻塞调用线程知道文件读取完毕
 
 @param key A string identifying the data. If nil, just return NO. 一个标识数据的字符串，如果为nil则返回NO
 @return Whether the key is in cache. key是否在缓存中
 */
- (BOOL)containsDataForKey:(nonnull NSString *)key;

/**
 Returns the data associated with a given key.
 This method may blocks the calling thread until file read finished.
 返回与key绑定的数据提
 这个方法可能会阻塞调用线程知道文件读取完毕
 
 @param key A string identifying the data. If nil, just return nil.
 @return The value associated with key, or nil if no value is associated with key.
 */
- (nullable NSData *)dataForKey:(nonnull NSString *)key;

/**
 Sets the value of the specified key in the cache.
 This method may blocks the calling thread until file write finished.
 设置缓存中指定键的值
 
 @param data The data to be stored in the cache.
 @param key    The key with which to associate the value. If nil, this method has no effect.
 */
- (void)setData:(nullable NSData *)data forKey:(nonnull NSString *)key;

/**
 Returns the extended data associated with a given key.
 This method may blocks the calling thread until file read finished.
 返回跟指定key相关的扩展数据
 
 @param key A string identifying the data. If nil, just return nil.
 @return The value associated with key, or nil if no value is associated with key.
 */
- (nullable NSData *)extendedDataForKey:(nonnull NSString *)key;

/**
 Set extended data with a given key.
 设置缓存中指定键的扩展数据
 
 @discussion You can set any extended data to exist cache key. Without override the exist disk file data.
 on UNIX, the common way for this is to use the Extended file attributes (xattr)
 您可以设置任何扩展数据存在缓存键。不覆盖现有的磁盘文件数据。
 在UNIX上，常见的方法是使用扩展文件属性(xattr)。
 
 @param extendedData The extended data (pass nil to remove). 扩展数据(传nil则删除)
 @param key The key with which to associate the value. If nil, this method has no effect.
*/
- (void)setExtendedData:(nullable NSData *)extendedData forKey:(nonnull NSString *)key;

/**
 Removes the value of the specified key in the cache.
 This method may blocks the calling thread until file delete finished.
 在缓存中删掉指定key的值
 
 @param key The key identifying the value to be removed. If nil, this method has no effect.
 */
- (void)removeDataForKey:(nonnull NSString *)key;

/**
 Empties the cache.
 This method may blocks the calling thread until file delete finished.
 清空硬盘缓存
 */
- (void)removeAllData;

/**
 Removes the expired data from the cache. You can choose the data to remove base on `ageLimit`, `countLimit` and `sizeLimit` options.
 从硬盘缓存中删除过期数据.你可以根据' ageLimit '， ' countLimit '和' sizeLimit '选项选择要删除的数据
 */
- (void)removeExpiredData;

/**
 The cache path for key
 key的缓存路径

 @param key A string identifying the value
 @return The cache path for key. Or nil if the key can not associate to a path
 */
- (nullable NSString *)cachePathForKey:(nonnull NSString *)key;

/**
 Returns the number of data in this cache.
 This method may blocks the calling thread until file read finished.
 返回当前在硬盘缓存中的数据数量
 
 @return The total data count.
 */
- (NSUInteger)totalCount;

/**
 Returns the total size (in bytes) of data in this cache.
 This method may blocks the calling thread until file read finished.
 返回当前在硬盘缓存中的数据大小（单位字节）
 
 @return The total data size in bytes.
 */
- (NSUInteger)totalSize;

@end

/**
 The built-in disk cache.
 */
@interface SDDiskCache : NSObject <SDDiskCache>
/**
 Cache Config object - storing all kind of settings.
 */
/// 配置对象
@property (nonatomic, strong, readonly, nonnull) SDImageCacheConfig *config;

- (nonnull instancetype)init NS_UNAVAILABLE;

/**
 Move the cache directory from old location to new location, the old location will be removed after finish.
 If the old location does not exist, does nothing. 如果老位置不存在，什么也不做
 If the new location does not exist, only do a movement of directory. 如果新位置不存在，仅仅引动目录
 If the new location does exist, will move and merge the files from old location. 如果新位置已经存在，会从老位置移动和合并文件
 If the new location does exist, but is not a directory, will remove it and do a movement of directory. 如果新位置存在，但是不是一个目录，则会删掉该文件并移动目录

 @param srcPath old location of cache directory
 @param dstPath new location of cache directory
 */
- (void)moveCacheDirectoryFromPath:(nonnull NSString *)srcPath toPath:(nonnull NSString *)dstPath;

@end
