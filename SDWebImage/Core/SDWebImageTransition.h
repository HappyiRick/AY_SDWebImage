/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

#if SD_UIKIT || SD_MAC
#import "SDImageCache.h"

#if SD_UIKIT
typedef UIViewAnimationOptions SDWebImageAnimationOptions;
#else
typedef NS_OPTIONS(NSUInteger, SDWebImageAnimationOptions) {
    SDWebImageAnimationOptionAllowsImplicitAnimation   = 1 << 0, // specify `allowsImplicitAnimation` for the `NSAnimationContext`
    
    SDWebImageAnimationOptionCurveEaseInOut            = 0 << 16, // default
    SDWebImageAnimationOptionCurveEaseIn               = 1 << 16,
    SDWebImageAnimationOptionCurveEaseOut              = 2 << 16,
    SDWebImageAnimationOptionCurveLinear               = 3 << 16,
    
    SDWebImageAnimationOptionTransitionNone            = 0 << 20, // default
    SDWebImageAnimationOptionTransitionFlipFromLeft    = 1 << 20,
    SDWebImageAnimationOptionTransitionFlipFromRight   = 2 << 20,
    SDWebImageAnimationOptionTransitionCurlUp          = 3 << 20,
    SDWebImageAnimationOptionTransitionCurlDown        = 4 << 20,
    SDWebImageAnimationOptionTransitionCrossDissolve   = 5 << 20,
    SDWebImageAnimationOptionTransitionFlipFromTop     = 6 << 20,
    SDWebImageAnimationOptionTransitionFlipFromBottom  = 7 << 20,
};
#endif

typedef void (^SDWebImageTransitionPreparesBlock)(__kindof UIView * _Nonnull view, UIImage * _Nullable image, NSData * _Nullable imageData, SDImageCacheType cacheType, NSURL * _Nullable imageURL);
typedef void (^SDWebImageTransitionAnimationsBlock)(__kindof UIView * _Nonnull view, UIImage * _Nullable image);
typedef void (^SDWebImageTransitionCompletionBlock)(BOOL finished);

/**
 This class is used to provide a transition animation after the view category load image finished. Use this on `sd_imageTransition` in UIView+WebCache.h
 for UIKit(iOS & tvOS), we use `+[UIView transitionWithView:duration:options:animations:completion]` for transition animation.
 for AppKit(macOS), we use `+[NSAnimationContext runAnimationGroup:completionHandler:]` for transition animation. You can call `+[NSAnimationContext currentContext]` to grab the context during animations block.
 @note These transition are provided for basic usage. If you need complicated animation, consider to directly use Core Animation or use `SDWebImageAvoidAutoSetImage` and implement your own after image load finished.
 
 这个类用于在视图类别加载图像完成后提供过渡动画。使用这个在' sd_imageTransition '在UIView+WebCache.h
 对于UIKit(iOS和tvOS)，我们使用' +[UIView transitionWithView:duration:options:animations:completion] '来转换动画。
 对于AppKit(macOS)，我们使用' +[NSAnimationContext runAnimationGroup:completionHandler:] '用于过渡动画。你可以调用' +[NSAnimationContext currentContext]
 '来抓取动画块中的上下文。
 这些转换是为基本用法提供的。如果你需要复杂的动画，考虑直接使用核心动画或使用“SDWebImageAvoidAutoSetImage”，并实现自己的图像加载完成后。
 */
@interface SDWebImageTransition : NSObject

/**
 By default, we set the image to the view at the beginning of the animations. You can disable this and provide custom set image process
 避免自动设置图像
 */
@property (nonatomic, assign) BOOL avoidAutoSetImage;
/**
 The duration of the transition animation, measured in seconds. Defaults to 0.5.
 转场动画时间，默认为0.5s
 */
@property (nonatomic, assign) NSTimeInterval duration;
/**
 The timing function used for all animations within this transition animation (macOS).
 这个过渡动画(macOS)中用于所有动画的计时函数。
 */
@property (nonatomic, strong, nullable) CAMediaTimingFunction *timingFunction API_UNAVAILABLE(ios, tvos, watchos) API_DEPRECATED("Use SDWebImageAnimationOptions instead, or grab NSAnimationContext.currentContext and modify the timingFunction", macos(10.10, 10.10));
/**
 A mask of options indicating how you want to perform the animations.
 指示如何执行动画的选项的掩码
 */
@property (nonatomic, assign) SDWebImageAnimationOptions animationOptions;
/**
 A block object to be executed before the animation sequence starts.
 在动画序列开始之前执行的块对象
 */
@property (nonatomic, copy, nullable) SDWebImageTransitionPreparesBlock prepares;
/**
 A block object that contains the changes you want to make to the specified view.
 一个块对象，它包含您想要对指定视图进行的更改。
 */
@property (nonatomic, copy, nullable) SDWebImageTransitionAnimationsBlock animations;
/**
 A block object to be executed when the animation sequence ends.
 动画序列结束时要执行的块对象。
 */
@property (nonatomic, copy, nullable) SDWebImageTransitionCompletionBlock completion;

@end

/**
 Convenience way to create transition. Remember to specify the duration if needed.
 for UIKit, these transition just use the correspond `animationOptions`. By default we enable `UIViewAnimationOptionAllowUserInteraction` to allow user interaction during transition.
 for AppKit, these transition use Core Animation in `animations`. So your view must be layer-backed. Set `wantsLayer = YES` before you apply it.
 
 方便的方式来创建过渡。如果需要，请记住指定持续时间
 对于UIKit，这些过渡只使用对应的' animationOptions '。默认情况下，我们启用' UIViewAnimationOptionAllowUserInteraction '来允许用户在转换过程中进行交互。
 对于AppKit，这些转换在“动画”中使用Core Animation。所以你的视图必须是分层的。在应用它之前设置‘wantsLayer = YES’
 */
@interface SDWebImageTransition (Conveniences)

/// Fade-in transition.
/// 淡入专场
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *fadeTransition;
/// Flip from left transition.
/// 从左侧翻转
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *flipFromLeftTransition;
/// Flip from right transition.
/// 从右侧翻转
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *flipFromRightTransition;
/// Flip from top transition.
/// 从顶部翻转
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *flipFromTopTransition;
/// Flip from bottom transition.
/// 从底部翻转
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *flipFromBottomTransition;
/// Curl up transition.
/// 卷起过渡
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *curlUpTransition;
/// Curl down transition.
/// 落下过渡
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *curlDownTransition;

/// Fade-in transition with duration.
/// @param duration transition duration, use ease-in-out
/// 淡入转场时间
+ (nonnull instancetype)fadeTransitionWithDuration:(NSTimeInterval)duration NS_SWIFT_NAME(fade(duration:));

/// Flip from left  transition with duration.
/// @param duration transition duration, use ease-in-out
/// 带持续时间的从左侧翻转
+ (nonnull instancetype)flipFromLeftTransitionWithDuration:(NSTimeInterval)duration NS_SWIFT_NAME(flipFromLeft(duration:));

/// Flip from right transition with duration.
/// @param duration transition duration, use ease-in-out
/// 带持续时间的从右侧翻转
+ (nonnull instancetype)flipFromRightTransitionWithDuration:(NSTimeInterval)duration NS_SWIFT_NAME(flipFromRight(duration:));

/// Flip from top transition with duration.
/// @param duration transition duration, use ease-in-out
/// 带持续时间的从顶部翻转
+ (nonnull instancetype)flipFromTopTransitionWithDuration:(NSTimeInterval)duration NS_SWIFT_NAME(flipFromTop(duration:));

/// Flip from bottom transition with duration.
/// @param duration transition duration, use ease-in-out
/// 带持续时间的从底部翻转
+ (nonnull instancetype)flipFromBottomTransitionWithDuration:(NSTimeInterval)duration NS_SWIFT_NAME(flipFromBottom(duration:));

///  Curl up transition with duration.
/// @param duration transition duration, use ease-in-out
/// 卷起转场
+ (nonnull instancetype)curlUpTransitionWithDuration:(NSTimeInterval)duration NS_SWIFT_NAME(curlUp(duration:));

/// Curl down transition with duration.
/// @param duration transition duration, use ease-in-out
/// 落下转场
+ (nonnull instancetype)curlDownTransitionWithDuration:(NSTimeInterval)duration NS_SWIFT_NAME(curlDown(duration:));

@end

#endif
