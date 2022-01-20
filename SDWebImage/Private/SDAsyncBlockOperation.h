/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

@class SDAsyncBlockOperation;
typedef void (^SDAsyncBlock)(SDAsyncBlockOperation * __nonnull asyncOperation);

/// A async block operation, success after you call `completer` (not like `NSBlockOperation` which is for sync block, success on return)
/// 一个异步块操作，你调用completer后会成功(不像NSBlockOperation，它是同步block，成功返回)
@interface SDAsyncBlockOperation : NSOperation

- (nonnull instancetype)initWithBlock:(nonnull SDAsyncBlock)block;
+ (nonnull instancetype)blockOperationWithBlock:(nonnull SDAsyncBlock)block;
- (void)complete;

@end
