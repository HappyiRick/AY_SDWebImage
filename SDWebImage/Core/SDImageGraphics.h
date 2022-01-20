/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"
#import <CoreGraphics/CoreGraphics.h>

/**
 These following graphics context method are provided to easily write cross-platform(AppKit/UIKit) code.
 For UIKit, these methods just call the same method in `UIGraphics.h`. See the documentation for usage.
 For AppKit, these methods use `NSGraphicsContext` to create image context and match the behavior like UIKit.
 @note If you don't care bitmap format (ARGB8888) and just draw image, use `SDGraphicsImageRenderer` instead. It's more performant on RAM usage.`
 */

/// Returns the current graphics context.
/// 返回当前图形上下文
FOUNDATION_EXPORT CGContextRef __nullable SDGraphicsGetCurrentContext(void) CF_RETURNS_NOT_RETAINED;
/// Creates a bitmap-based graphics context and makes it the current context.
/// 创建一个基于位图的图形上下文，并使其成为当前上下文
FOUNDATION_EXPORT void SDGraphicsBeginImageContext(CGSize size);
/// Creates a bitmap-based graphics context with the specified options.
/// 使用指定选项创建基于位图的图形上下文
FOUNDATION_EXPORT void SDGraphicsBeginImageContextWithOptions(CGSize size, BOOL opaque, CGFloat scale);
/// Removes the current bitmap-based graphics context from the top of the stack.
/// 从堆栈顶部移除当前基于位图的图形上下文
FOUNDATION_EXPORT void SDGraphicsEndImageContext(void);
/// Returns an image based on the contents of the current bitmap-based graphics context.
/// 根据当前基于位图的图形上下文的内容返回图像
FOUNDATION_EXPORT UIImage * __nullable SDGraphicsGetImageFromCurrentImageContext(void);
