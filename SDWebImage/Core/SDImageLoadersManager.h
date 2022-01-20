/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDImageLoader.h"

/**
 A loaders manager to manage multiple loaders
 用于管理多个加载器的加载器管理器
 */
@interface SDImageLoadersManager : NSObject <SDImageLoader>

/**
 Returns the global shared loaders manager instance. By default we will set [`SDWebImageDownloader.sharedDownloader`] into the loaders array.
 返回全局共享加载器管理器实例。默认情况下，我们将设置[' SDWebImageDownloader.sharedDownloader ']到加载器数组
 */
@property (nonatomic, class, readonly, nonnull) SDImageLoadersManager *sharedManager;

/**
 All image loaders in manager. The loaders array is a priority queue, which means the later added loader will have the highest priority
 所有图像加载器在管理器。加载器数组是一个优先级队列，这意味着后面添加的加载器将具有最高的优先级
 */
@property (nonatomic, copy, nullable) NSArray<id<SDImageLoader>>* loaders;

/**
 Add a new image loader to the end of loaders array. Which has the highest priority.
 在加载器数组的末尾添加一个新的图像加载器，其优先级最高
 
 @param loader loader 加载器
 */
- (void)addLoader:(nonnull id<SDImageLoader>)loader;

/**
 Remove an image loader in the loaders array.
 在加载器数组中删除一个图像加载器
 
 @param loader loader
 */
- (void)removeLoader:(nonnull id<SDImageLoader>)loader;

@end
