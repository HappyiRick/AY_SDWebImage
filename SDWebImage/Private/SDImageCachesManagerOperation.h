/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"

/// This is used for operation management, but not for operation queue execute
/// 这用于操作管理，但不用于操作队列执行
@interface SDImageCachesManagerOperation : NSOperation
/// 等待个数
@property (nonatomic, assign, readonly) NSUInteger pendingCount;
/// 开始
- (void)beginWithTotalCount:(NSUInteger)totalCount;
/// 完成1个
- (void)completeOne;
/// 完成
- (void)done;

@end
