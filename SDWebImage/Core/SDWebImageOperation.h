/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>

/// A protocol represents cancelable operation.
/// 标识操作可取消的协议
@protocol SDWebImageOperation <NSObject>

- (void)cancel;

@end

/// NSOperation conform to `SDWebImageOperation`.
/// 遵守`SDWebImageOperation`协议的NSOperation
@interface NSOperation (SDWebImageOperation) <SDWebImageOperation>

@end
