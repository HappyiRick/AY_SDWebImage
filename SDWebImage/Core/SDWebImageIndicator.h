/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

#if SD_UIKIT || SD_MAC

/**
 A protocol to custom the indicator during the image loading.
 All of these methods are called from main queue.
 自定义图片加载指示器协议，所有方法都在主线程执行
 */
@protocol SDWebImageIndicator <NSObject>

@required
/**
 The view associate to the indicator. 视图与指标相关联

 @return The indicator view
 */
@property (nonatomic, strong, readonly, nonnull) UIView *indicatorView;

/**
 Start the animating for indicator. - 启动指示器的动画
 */
- (void)startAnimatingIndicator;

/**
 Stop the animating for indicator. - 停止指示器的动画
 */
- (void)stopAnimatingIndicator;

@optional
/**
 Update the loading progress (0-1.0) for indicator. Optional - 更新指示器的进度，可选
 
 @param progress The progress, value between 0 and 1.0
 */
- (void)updateIndicatorProgress:(double)progress;

@end

#pragma mark - Activity Indicator

/**
 Activity indicator class.
 活动指示器类
 for UIKit(macOS), it use a `UIActivityIndicatorView`.
 for AppKit(macOS), it use a `NSProgressIndicator` with the spinning style.
 */
@interface SDWebImageActivityIndicator : NSObject <SDWebImageIndicator>

#if SD_UIKIT
@property (nonatomic, strong, readonly, nonnull) UIActivityIndicatorView *indicatorView;
#else
@property (nonatomic, strong, readonly, nonnull) NSProgressIndicator *indicatorView;
#endif

@end

/**
 Convenience way to use activity indicator.
 快捷使用活动指示器
 */
@interface SDWebImageActivityIndicator (Conveniences)

/// These indicator use the fixed color without dark mode support - 这些指示器使用固定颜色，不支持暗模式
/// gray-style activity indicator - 灰色样式活动指示器
@property (nonatomic, class, nonnull, readonly) SDWebImageActivityIndicator *grayIndicator;
/// large gray-style activity indicator -  大型灰色活动指示器
@property (nonatomic, class, nonnull, readonly) SDWebImageActivityIndicator *grayLargeIndicator;
/// white-style activity indicator - 白色样式活动指示器
@property (nonatomic, class, nonnull, readonly) SDWebImageActivityIndicator *whiteIndicator;
/// large white-style activity indicator - 大型白色活动指示器
@property (nonatomic, class, nonnull, readonly) SDWebImageActivityIndicator *whiteLargeIndicator;
/// These indicator use the system style, supports dark mode if available (iOS 13+/macOS 10.14+) - 这些指示器使用系统风格，支持暗模式(iOS 13+/macOS 10.14+)
/// large activity indicator - 大型活动指示器
@property (nonatomic, class, nonnull, readonly) SDWebImageActivityIndicator *largeIndicator;
/// medium activity indicator - 中型活动指示器
@property (nonatomic, class, nonnull, readonly) SDWebImageActivityIndicator *mediumIndicator;

@end

#pragma mark - Progress Indicator

/**
 Progress indicator class.
 进度指示器类
 for UIKit(macOS), it use a `UIProgressView`.
 for AppKit(macOS), it use a `NSProgressIndicator` with the bar style.
 */
@interface SDWebImageProgressIndicator : NSObject <SDWebImageIndicator>

#if SD_UIKIT
@property (nonatomic, strong, readonly, nonnull) UIProgressView *indicatorView;
#else
@property (nonatomic, strong, readonly, nonnull) NSProgressIndicator *indicatorView;
#endif

@end

/**
 Convenience way to create progress indicator. Remember to specify the indicator width or use layout constraint if need.
 方便的方法来创建进度指示器。如果需要，请记住指定指示器宽度或使用布局约束。
 */
@interface SDWebImageProgressIndicator (Conveniences)

/// default-style progress indicator
/// 默认样式的进度指示器
@property (nonatomic, class, nonnull, readonly) SDWebImageProgressIndicator *defaultIndicator;
/// bar-style progress indicator
/// 条形样式进度
@property (nonatomic, class, nonnull, readonly) SDWebImageProgressIndicator *barIndicator API_UNAVAILABLE(macos, tvos);

@end

#endif
