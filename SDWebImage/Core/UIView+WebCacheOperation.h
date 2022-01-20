/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"
#import "SDWebImageOperation.h"

/**
 These methods are used to support canceling for UIView image loading, it's designed to be used internal but not external.
 All the stored operations are weak, so it will be dealloced after image loading finished. If you need to store operations, use your own class to keep a strong reference for them.
 
 这些方法是用来支持取消UIView图像加载的，它被设计为内部使用而不是外部
 所有的存储操作都是弱的，所以在加载完图像后会被释放。如果需要存储操作，请使用自己的类来为它们保留强引用。
 */
@interface UIView (WebCacheOperation)

/**
 *  Get the image load operation for key
 *  为键获取图像加载操作
 *
 *  @param key key for identifying the operations - 用于标识操作的键
 *  @return the image load operation - 图像加载操作
 */
- (nullable id<SDWebImageOperation>)sd_imageLoadOperationForKey:(nullable NSString *)key;

/**
 *  Set the image load operation (storage in a UIView based weak map table)
 *  设置图像加载操作(存储在基于UIView的弱映射表中)
 *
 *  @param operation the operation - 操作
 *  @param key       key for storing the operation - 存储操作的key
 */
- (void)sd_setImageLoadOperation:(nullable id<SDWebImageOperation>)operation forKey:(nullable NSString *)key;

/**
 *  Cancel all operations for the current UIView and key
 *  取消当前UIView和key的所有操作
 *
 *  @param key key for identifying the operations - 标识操作的key
 */
- (void)sd_cancelImageLoadOperationWithKey:(nullable NSString *)key;

/**
 *  Just remove the operations corresponding to the current UIView and key without cancelling them
 *  只需删除对应于当前UIView和key的操作，而不取消它们
 *
 *  @param key key for identifying the operations - 标识操作的key
 */
- (void)sd_removeImageLoadOperationWithKey:(nullable NSString *)key;

@end
