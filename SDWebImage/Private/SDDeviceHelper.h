/*
* This file is part of the SDWebImage package.
* (c) Olivier Poitrey <rs@dailymotion.com>
*
* For the full copyright and license information, please view the LICENSE
* file that was distributed with this source code.
*/

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"

/// Device information helper methods - 硬件信息方法
@interface SDDeviceHelper : NSObject
/// 总内存
+ (NSUInteger)totalMemory;
/// 闲置内存
+ (NSUInteger)freeMemory;

@end
