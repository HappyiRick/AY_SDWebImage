/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

#if SD_UIKIT || SD_MAC

#import "SDAnimatedImage.h"
#import "SDAnimatedImagePlayer.h"

/**
 A drop-in replacement for UIImageView/NSImageView, you can use this for animated image rendering.
 这是UIImageView/NSImageView的替换，你可以用它来渲染动画图像。
 Call `setImage:` with `UIImage(NSImage)` which conforms to `SDAnimatedImage` protocol will start animated image rendering. Call with normal UIImage(NSImage) will back to normal UIImageView(NSImageView) rendering
 For UIKit: use `-startAnimating`, `-stopAnimating` to control animating. `isAnimating` to check animation state.
 For AppKit: use `-setAnimates:` to control animating, `animates` to check animation state. This view is layer-backed.
 */
@interface SDAnimatedImageView : UIImageView
/**
 The internal animation player. - 内置动画播放器
 This property is only used for advanced usage, like inspecting/debugging animation status, control progressive loading, complicated animation frame index control, etc. - 此属性仅用于高级用途，如检查/调试动画状态，控制渐进加载，复杂动画帧索引控制等
 @warning Pay attention if you directly update the player's property like `totalFrameCount`, `totalLoopCount`, the same property on `SDAnimatedImageView` may not get synced. - 如果直接更新播放器的属性的话，在SDAnimatedImageView的相同属性不一定会同步
 */
@property (nonatomic, strong, readonly, nullable) SDAnimatedImagePlayer *player;

/**
 Current display frame image. This value is KVO Compliance.
 当前展示帧图像，该值为KVO合规
 */
@property (nonatomic, strong, readonly, nullable) UIImage *currentFrame;
/**
 Current frame index, zero based. This value is KVO Compliance.
 当前帧索引，从0开始，该值为KVO合规
 */
@property (nonatomic, assign, readonly) NSUInteger currentFrameIndex;
/**
 Current loop count since its latest animating. This value is KVO Compliance.
 最新动画的循环次数，该值为KVO合规
 */
@property (nonatomic, assign, readonly) NSUInteger currentLoopCount;
/**
 YES to choose `animationRepeatCount` property for animation loop count. No to use animated image's `animatedImageLoopCount` instead.
 Default is NO.
 YES则用animationRepeatCount设置动画循环次数，NO则用动图的动画循环次数代替，默认为NO
 */
@property (nonatomic, assign) BOOL shouldCustomLoopCount;
/**
 Total loop count for animated image rendering. Default is animated image's loop count.
 If you need to set custom loop count, set `shouldCustomLoopCount` to YES and change this value.
 This class override UIImageView's `animationRepeatCount` property on iOS, use this property as well.
 
 动画图像渲染的循环总数，默认为动画图像的循环数量，如果需要设置自定义循环次数，设置shouldCustomLoopCount为YES并改变该参数
 */
@property (nonatomic, assign) NSInteger animationRepeatCount;
/**
 The animation playback rate. Default is 1.0.
 `1.0` means the normal speed.
 `0.0` means stopping the animation.
 `0.0-1.0` means the slow speed.
 `> 1.0` means the fast speed.
 `< 0.0` is not supported currently and stop animation. (may support reverse playback in the future)
 
 动画播放速率， 1.0 - 常速， 0.0-1.0 慢速， >1.0 表示快速， < 0.0 目前不支持，会停止动画，将来可能会支持倒放
 */
@property (nonatomic, assign) double playbackRate;

/// Asynchronous setup animation playback mode. Default mode is SDAnimatedImagePlaybackModeNormal.
/// 异步设置动图回放模式，默认模式为SDAnimatedImagePlaybackModeNormal
@property (nonatomic, assign) SDAnimatedImagePlaybackMode playbackMode;

/**
 Provide a max buffer size by bytes. This is used to adjust frame buffer count and can be useful when the decoding cost is expensive (such as Animated WebP software decoding). Default is 0.
 `0` means automatically adjust by calculating current memory usage.
 `1` means without any buffer cache, each of frames will be decoded and then be freed after rendering. (Lowest Memory and Highest CPU)
 `NSUIntegerMax` means cache all the buffer. (Lowest CPU and Highest Memory)
 
 按字节提供最大缓冲区大小。这是用来调整帧缓冲计数，可以有用的解码成本昂贵(如动画WebP软件解码)。默认值为0。
 0 - 表示通过计算当前内存使用率自动调整
 1 - 意味着在没有任何缓冲缓存的情况下，每一帧都将被解码，然后在渲染后被释放。(最低内存和最高CPU)
 ' NSUIntegerMax '表示缓存所有的缓冲区。(最低CPU和最高内存)
 */
@property (nonatomic, assign) NSUInteger maxBufferSize;
/**
 Whether or not to enable incremental image load for animated image. This is for the animated image which `sd_isIncremental` is YES (See `UIImage+Metadata.h`). If enable, animated image rendering will stop at the last frame available currently, and continue when another `setImage:` trigger, where the new animated image's `animatedImageData` should be updated from the previous one. If the `sd_isIncremental` is NO. The incremental image load stop.
 @note If you are confused about this description, open Chrome browser to view some large GIF images with low network speed to see the animation behavior.
 @note The best practice to use incremental load is using `initWithAnimatedCoder:scale:` in `SDAnimatedImage` with animated coder which conform to `SDProgressiveImageCoder` as well. Then call incremental update and incremental decode method to produce the image.
 Default is YES. Set to NO to only render the static poster for incremental animated image.
 
 是否为动画图像启用增量图像加载。这是为' sd_isIncremental '是YES的动画图像(参见' UIImage+Metadata.h ')。如果启用，动画图像渲染将在当前可用的最后一帧停止，并继续当另一个' setImage: '触发器，其中新的动画图像的' animatedImageData '应该从上一个更新。如果' sd_isIncremental '是NO。增量图像加载停止。
 默认为YES,设置为NO只渲染增量动画图像的静态海报
 */
@property (nonatomic, assign) BOOL shouldIncrementalLoad;

/**
 Whether or not to clear the frame buffer cache when animation stopped. See `maxBufferSize`
 This is useful when you want to limit the memory usage during frequently visibility changes (such as image view inside a list view, then push and pop)
 Default is NO.
 当动画图像停止的时候是否清理帧缓存，查看'maxBufferSize'，当在可见性频繁变化时约束内存使用时时很有用的，默认为NO
 */
@property (nonatomic, assign) BOOL clearBufferWhenStopped;

/**
 Whether or not to reset the current frame index when animation stopped.
 For some of use case, you may want to reset the frame index to 0 when stop, but some other want to keep the current frame index.
 Default is NO.
 当动画停止的时候是否重置当前帧索引，对于一些使用场景，动画停止时候需要设置帧索引为0，但是有些情况需要保持为当前帧索引，默认为NO
 */
@property (nonatomic, assign) BOOL resetFrameIndexWhenStopped;

/**
 If the image which conforms to `SDAnimatedImage` protocol has more than one frame, set this value to `YES` will automatically
 play/stop the animation when the view become visible/invisible.
 Default is YES.
 如果符合“SDAnimatedImage”协议的图像有多个帧，将此值设置为“YES”，当视图变为可见/不可见时将自动播放/停止动画。默认为YES
 */
@property (nonatomic, assign) BOOL autoPlayAnimatedImage;

/**
 You can specify a runloop mode to let it rendering.
 Default is NSRunLoopCommonModes on multi-core device, NSDefaultRunLoopMode on single-core device
 @note This is useful for some cases, for example, always specify NSDefaultRunLoopMode, if you want to pause the animation when user scroll (for Mac user, drag the mouse or touchpad)
 可以指定runloop模式进行渲染，多核设备默认为NSRunLoopCommonModes, 单核设备默认为NSDefaultRunLoopMode
 */
@property (nonatomic, copy, nonnull) NSRunLoopMode runLoopMode;
@end

#endif
