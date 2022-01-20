/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"

/// Image Cache Expire Type
/// 图像缓存过期类型
typedef NS_ENUM(NSUInteger, SDImageCacheConfigExpireType) {
    /**
     * When the image cache is accessed it will update this value
     * 当图像缓存被访问时，它将更新这个值
     */
    SDImageCacheConfigExpireTypeAccessDate,
    /**
     * When the image cache is created or modified it will update this value (Default)
     * 当图像缓存被创建或修改时，它将更新这个值(默认值)
     */
    SDImageCacheConfigExpireTypeModificationDate,
    /**
     * When the image cache is created it will update this value
     * 当图像缓存被创建时，它将更新这个值
     */
    SDImageCacheConfigExpireTypeCreationDate,
    /**
     * When the image cache is created, modified, renamed, file attribute updated (like permission, xattr)  it will update this value
     * 当图像缓存被创建、修改、重命名、文件属性更新(如权限、xattr)时，它将更新这个值
     */
    SDImageCacheConfigExpireTypeChangeDate,
};

/**
 The class contains all the config for image cache
 这个类包括所有对于图像缓存的配置
 @note This class conform to NSCopying, make sure to add the property in `copyWithZone:` as well.
 @note 此类遵循NSCopying协议, 确保在' copyWithZone: '中添加属性
 */
@interface SDImageCacheConfig : NSObject <NSCopying>

/**
 Gets the default cache config used for shared instance or initialization when it does not provide any cache config. Such as `SDImageCache.sharedImageCache`.
 获取共享实例或初始化时不提供任何缓存配置时使用的默认缓存配置。如“SDImageCache.sharedImageCache”。
 @note You can modify the property on default cache config, which can be used for later created cache instance. The already created cache instance does not get affected.
 @note 可以修改默认缓存配置上的属性，该属性可用于以后创建的缓存实例。已经创建的缓存实例不会受到影响
 */
@property (nonatomic, class, readonly, nonnull) SDImageCacheConfig *defaultCacheConfig;

/**
 * Whether or not to disable iCloud backup
 * Defaults to YES.
 * 是否不允许iClound备份, 默认为YES
 */
@property (assign, nonatomic) BOOL shouldDisableiCloud;

/**
 * Whether or not to use memory cache
 * @note When the memory cache is disabled, the weak memory cache will also be disabled.
 * Defaults to YES.
 * 是否使用内存缓存， 默认为YES
 * @note 当内存缓存不可用时，弱内存缓存也将被禁用
 */
@property (assign, nonatomic) BOOL shouldCacheImagesInMemory;

/*
 * The option to control weak memory cache for images. When enable, `SDImageCache`'s memory cache will use a weak maptable to store the image at the same time when it stored to memory, and get removed at the same time.
 * However when memory warning is triggered, since the weak maptable does not hold a strong reference to image instance, even when the memory cache itself is purged, some images which are held strongly by UIImageViews or other live instances can be recovered again, to avoid later re-query from disk cache or network. This may be helpful for the case, for example, when app enter background and memory is purged, cause cell flashing after re-enter foreground.
 * When enabling this option, we will sync back the image from weak maptable to strong cache during next time top level `sd_setImage` function call.
 * Defaults to NO (YES before 5.12.0 version). You can change this option dynamically.
 * 控制图像弱内存缓存的选项。当启用时，' SDImageCache '的内存缓存将使用一个弱映射表来存储图像，当它存储到内存时，同时被删除
 * 然而当触发内存警告,由于弱maptable不举行一个强引用图像实例,即使内存缓存本身净化,一些图像由uiimageview持有强烈或其他生活实例可以再次恢复,避免后重新查询缓存从磁盘或网络。这可能是有帮助的情况下，例如，当应用程序进入后台和内存被清除，导致单元格闪烁后重新进入前台。
 * 当启用这个选项时，我们将在下一次调用顶级' sd_setImage '函数时将图像从弱映射表同步回强缓存
 * 默认为NO(5.12.0版本之前为YES)。可以动态地更改此选项
 */
@property (assign, nonatomic) BOOL shouldUseWeakMemoryCache;

/**
 * Whether or not to remove the expired disk data when application entering the background. (Not works for macOS)
 * Defaults to YES.
 * 当app进入后台时是否删除过期硬盘数据， 默认为yes
 */
@property (assign, nonatomic) BOOL shouldRemoveExpiredDataWhenEnterBackground;

/**
 * Whether or not to remove the expired disk data when application been terminated. This operation is processed in sync to ensure clean up.
 * Defaults to YES.
 * 当应用程序被中止的时候是否删除过期硬盘数据,该操作是同步处理的，以确保清理, 默认为YES
 */
@property (assign, nonatomic) BOOL shouldRemoveExpiredDataWhenTerminate;

/**
 * The reading options while reading cache from disk.
 * Defaults to 0. You can set this to `NSDataReadingMappedIfSafe` to improve performance.
 * 从硬盘读取缓存时的读取选项
 * 默认为0.可以把它设置为' NSDataReadingMappedIfSafe '来提高性能
 */
@property (assign, nonatomic) NSDataReadingOptions diskCacheReadingOptions;

/**
 * The writing options while writing cache to disk.
 * Defaults to `NSDataWritingAtomic`. You can set this to `NSDataWritingWithoutOverwriting` to prevent overwriting an existing file.
 * 向硬盘中写缓存时的选项
 * 默认为`NSDataWritingAtomic`，可以把它设置为' NSDataWritingWithoutOverwriting '来防止覆盖现有文件
 */
@property (assign, nonatomic) NSDataWritingOptions diskCacheWritingOptions;

/**
 * The maximum length of time to keep an image in the disk cache, in seconds.
 * Setting this to a negative value means no expiring.
 * Setting this to zero means that all cached files would be removed when do expiration check.
 * Defaults to 1 week.
 * 在磁盘缓存中保存映像的最大时间长度，以秒为单位。
 * 将此设置为负值意味着不会过期。
 * 将此设置为零意味着在执行过期检查时将删除所有缓存文件。
 * 默认为1周
 */
@property (assign, nonatomic) NSTimeInterval maxDiskAge;

/**
 * The maximum size of the disk cache, in bytes.
 * Defaults to 0. Which means there is no cache size limit.
 * 硬盘缓存的最大值，以字节为单位。默认为0，意味着没有大小约束
 */
@property (assign, nonatomic) NSUInteger maxDiskSize;

/**
 * The maximum "total cost" of the in-memory image cache. The cost function is the bytes size held in memory.
 * @note The memory cost is bytes size in memory, but not simple pixels count. For common ARGB8888 image, one pixel is 4 bytes (32 bits).
 * Defaults to 0. Which means there is no memory cost limit.
 * 内存中图像缓存的最大“总量”。cost函数是存储在内存中的字节大小。
 * @note 内存成本是内存中的字节大小，而不是简单的像素计数。对于普通ARGB8888图像，一个像素是4字节(32位)
 * 默认为0，意味着没有内存使用约束
 */
@property (assign, nonatomic) NSUInteger maxMemoryCost;

/**
 * The maximum number of objects in-memory image cache should hold.
 * Defaults to 0. Which means there is no memory count limit.
 * 内存图像缓存中应该保存的对象的最大数量
 * 默认为0，意味着没有内存数量限制
 */
@property (assign, nonatomic) NSUInteger maxMemoryCount;

/**
 * The attribute which the clear cache will be checked against when clearing the disk cache
 * Default is Modified Date
 * 清除缓存时检查的属性
 * 默认为更改日期
 */
@property (assign, nonatomic) SDImageCacheConfigExpireType diskCacheExpireType;

/**
 * The custom file manager for disk cache. Pass nil to let disk cache choose the proper file manager.
 * Defaults to nil.
 * @note This value does not support dynamic changes. Which means further modification on this value after cache initialized has no effect.
 * @note Since `NSFileManager` does not support `NSCopying`. We just pass this by reference during copying. So it's not recommend to set this value on `defaultCacheConfig`.
 * 磁盘缓存的自定义文件管理器。传递nil让磁盘缓存选择合适的文件管理器，默认为nil
 * @note 此值不支持动态更改。这意味着缓存初始化后对这个值的进一步修改没有效果
 * @note 因为' NSFileManager '不支持' NSCopying '。我们只是在复制过程中通过引用传递它。所以不建议在' defaultacheconfig '上设置这个值。
 */
@property (strong, nonatomic, nullable) NSFileManager *fileManager;

/**
 * The custom memory cache class. Provided class instance must conform to `SDMemoryCache` protocol to allow usage.
 * Defaults to built-in `SDMemoryCache` class.
 * @note This value does not support dynamic changes. Which means further modification on this value after cache initialized has no effect.
 * 自定义内存缓存类。所提供的类实例必须符合“SDMemoryCache”协议来允许使用，默认为内置的“SDMemoryCache”类
 * @note 此值不支持动态更改。这意味着缓存初始化后对这个值的进一步修改没有效果
 */
@property (assign, nonatomic, nonnull) Class memoryCacheClass;

/**
 * The custom disk cache class. Provided class instance must conform to `SDDiskCache` protocol to allow usage.
 * Defaults to built-in `SDDiskCache` class.
 * @note This value does not support dynamic changes. Which means further modification on this value after cache initialized has no effect.
 * 自定义硬盘缓存类。所提供的类实例必须符合“SDDiskCache”协议来允许使用，默认为内置的“SDDiskCache”类
 * @note 此值不支持动态更改。这意味着缓存初始化后对这个值的进一步修改没有效果
 */
@property (assign ,nonatomic, nonnull) Class diskCacheClass;

@end
