/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"
#import "SDWebImageDefine.h"

@class SDWebImageOptionsResult;

typedef SDWebImageOptionsResult * _Nullable(^SDWebImageOptionsProcessorBlock)(NSURL * _Nullable url, SDWebImageOptions options, SDWebImageContext * _Nullable context);

/**
 The options result contains both options and context.
 选项结果包括选项和上下文
 */
@interface SDWebImageOptionsResult : NSObject

/**
 WebCache options. - 网络缓存选项
 */
@property (nonatomic, assign, readonly) SDWebImageOptions options;

/**
 Context options. - 上下文选项
 */
@property (nonatomic, copy, readonly, nullable) SDWebImageContext *context;

/**
 Create a new options result. - 创建一个新的选项结果

 @param options options - 选项
 @param context context - 上下文
 @return The options result contains both options and context.
 */
- (nonnull instancetype)initWithOptions:(SDWebImageOptions)options context:(nullable SDWebImageContext *)context;

@end

/**
 This is the protocol for options processor. - 这是选项处理者协议
 Options processor can be used, to control the final result for individual image request's `SDWebImageOptions` and `SDWebImageContext`
 选项处理器可以用来控制单个图像请求的“SDWebImageOptions”和“SDWebImageContext”的最终结果。
 Implements the protocol to have a global control for each indivadual image request's option.
 实现协议，为每个单独的图像请求的选项拥有全局控制。
 */
@protocol SDWebImageOptionsProcessor <NSObject>

/**
 Return the processed options result for specify image URL, with its options and context
 返回指定图像URL的处理过的选项结果及其选项和上下文
 
 @param url The URL to the image
 @param options A mask to specify options to use for this request
 @param context A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
 @return The processed result, contains both options and context
 */
- (nullable SDWebImageOptionsResult *)processedResultForURL:(nullable NSURL *)url
                                                    options:(SDWebImageOptions)options
                                                    context:(nullable SDWebImageContext *)context;

@end

/**
 A options processor class with block. - 带有块的选项处理器类
 */
@interface SDWebImageOptionsProcessor : NSObject<SDWebImageOptionsProcessor>

- (nonnull instancetype)initWithBlock:(nonnull SDWebImageOptionsProcessorBlock)block;
+ (nonnull instancetype)optionsProcessorWithBlock:(nonnull SDWebImageOptionsProcessorBlock)block;

@end
