/*
* This file is part of the SDWebImage package.
* (c) Olivier Poitrey <rs@dailymotion.com>
* (c) Fabrice Aneche
*
* For the full copyright and license information, please view the LICENSE
* file that was distributed with this source code.
*/

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"

@interface UIImage (ExtendedCacheData)

/**
 Read and Write the extended object and bind it to the image. Which can hold some extra metadata like Image's scale factor, URL rich link, date, etc.
 The extended object should conforms to NSCoding, which we use `NSKeyedArchiver` and `NSKeyedUnarchiver` to archive it to data, and write to disk cache.
 @note The disk cache preserve both of the data and extended data with the same cache key. For manual query, use the `SDDiskCache` protocol method `extendedDataForKey:` instead.
 @note You can specify arbitrary object conforms to NSCoding (NSObject protocol here is used to support object using `NS_ROOT_CLASS`, which is not NSObject subclass). If you load image from disk cache, you should check the extended object class to avoid corrupted data.
 @warning This object don't need to implements NSSecureCoding (but it's recommended),  because we allows arbitrary class.
 
 读取和写入扩展的对象，并将其绑定到图像。哪些可以保存一些额外的元数据，如图像的比例因子、URL丰富链接、日期等
 扩展的对象应该符合NSCoding，我们使用' NSKeyedArchiver '和' NSKeyedUnarchiver '来将其归档为数据，并写入磁盘缓存。
 磁盘缓存使用相同的缓存键来保存数据和扩展数据。对于手动查询，使用' SDDiskCache '协议方法' extendedDataForKey: '代替。
 你可以指定任意符合NSCoding的对象(这里的NSObject协议是用来支持对象使用' NS_ROOT_CLASS '，这不是NSObject的子类)。如果从磁盘缓存加载图像，应该检查扩展的对象类以避免损坏的数据。
 这个对象不需要实现NSSecureCoding(但推荐)，因为我们允许任意的类。
 */
@property (nonatomic, strong, nullable) id<NSObject, NSCoding> sd_extendedObject;

@end
