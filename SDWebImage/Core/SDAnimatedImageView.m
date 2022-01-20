/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDAnimatedImageView.h"

#if SD_UIKIT || SD_MAC

#import "UIImage+Metadata.h"
#import "NSImage+Compatibility.h"
#import "SDInternalMacros.h"
#import "objc/runtime.h"

@interface UIImageView () <CALayerDelegate>
@end

@interface SDAnimatedImageView () {
    /// 初始化完成标识
    BOOL _initFinished; // Extra flag to mark the `commonInit` is called
    /// 运行循环模式
    NSRunLoopMode _runLoopMode;
    /// 最大缓存size
    NSUInteger _maxBufferSize;
    /// 回放速率
    double _playbackRate;
    /// 回放模式
    SDAnimatedImagePlaybackMode _playbackMode;
}
/// 播放器
@property (nonatomic, strong, readwrite) SDAnimatedImagePlayer *player;
/// 当前帧
@property (nonatomic, strong, readwrite) UIImage *currentFrame;
/// 当前帧索引
@property (nonatomic, assign, readwrite) NSUInteger currentFrameIndex;
/// 当前循环数
@property (nonatomic, assign, readwrite) NSUInteger currentLoopCount;
/// 是否动画
@property (nonatomic, assign) BOOL shouldAnimate;
/// 是否过程中
@property (nonatomic, assign) BOOL isProgressive;
/// 实际渲染层
@property (nonatomic) CALayer *imageViewLayer; // The actual rendering layer.

@end

@implementation SDAnimatedImageView
#if SD_UIKIT
@dynamic animationRepeatCount; // we re-use this property from `UIImageView` super class on iOS. - 在iOS上重用了父类`UIImageView`的这个属性
#endif

#pragma mark - Initializers

#if SD_MAC
+ (instancetype)imageViewWithImage:(NSImage *)image
{
    NSRect frame = NSMakeRect(0, 0, image.size.width, image.size.height);
    SDAnimatedImageView *imageView = [[SDAnimatedImageView alloc] initWithFrame:frame];
    [imageView setImage:image];
    return imageView;
}
#else
// -initWithImage: isn't documented as a designated initializer of UIImageView, but it actually seems to be.
// Using -initWithImage: doesn't call any of the other designated initializers.
- (instancetype)initWithImage:(UIImage *)image
{
    self = [super initWithImage:image];
    if (self) {
        [self commonInit];
    }
    return self;
}

// -initWithImage:highlightedImage: also isn't documented as a designated initializer of UIImageView, but it doesn't call any other designated initializers.
- (instancetype)initWithImage:(UIImage *)image highlightedImage:(UIImage *)highlightedImage
{
    self = [super initWithImage:image highlightedImage:highlightedImage];
    if (self) {
        [self commonInit];
    }
    return self;
}
#endif

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self commonInit];
    }
    return self;
}

- (instancetype)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self) {
        [self commonInit];
    }
    return self;
}

- (void)commonInit
{
    // Pay attention that UIKit's `initWithImage:` will trigger a `setImage:` during initialization before this `commonInit`.
//            注意UIKit的initWithImage:会在commonInit之前的初始化过程中触发一个setImage:
    // So the properties which rely on this order, should using lazy-evaluation or do extra check in `setImage:`.
        //    所以依赖于这个顺序的属性，应该使用惰性计算或者在' setImage: '中做额外的检查。
    self.autoPlayAnimatedImage = YES;
    self.shouldCustomLoopCount = NO;
    self.shouldIncrementalLoad = YES;
    self.playbackRate = 1.0;
#if SD_MAC
    self.wantsLayer = YES;
#endif
    // Mark commonInit finished
    _initFinished = YES;
}

#pragma mark - Accessors
#pragma mark Public

- (void)setImage:(UIImage *)image
{
    if (self.image == image) {
        return;
    }
    
    // Check Progressive rendering - 检查渐进渲染
    [self updateIsProgressiveWithImage:image];
    
    if (!self.isProgressive) {
        // Stop animating - 停止动画
        self.player = nil;
        self.currentFrame = nil;
        self.currentFrameIndex = 0;
        self.currentLoopCount = 0;
    }
    
    // We need call super method to keep function. This will impliedly call `setNeedsDisplay`. But we have no way to avoid this when using animated image. So we call `setNeedsDisplay` again at the end.
    // 我们需要调用super方法来保持功能。这将隐式调用' setNeedsDisplay '。但在使用动画图像时，我们没有办法避免这种情况。我们在最后再次调用setNeedsDisplay。
    super.image = image;
    if ([image.class conformsToProtocol:@protocol(SDAnimatedImage)]) {
        if (!self.player) {
            id<SDAnimatedImageProvider> provider;
            // Check progressive loading - 检查渐进加载
            if (self.isProgressive) {
                provider = [self progressiveAnimatedCoderForImage:image];
            } else {
                provider = (id<SDAnimatedImage>)image;
            }
            // Create animated player - 创建动画播放器
            self.player = [SDAnimatedImagePlayer playerWithProvider:provider];
        } else {
            // Update Frame Count - 更新帧数
            self.player.totalFrameCount = [(id<SDAnimatedImage>)image animatedImageFrameCount];
        }
        
        if (!self.player) {
            // animated player nil means the image format is not supported, or frame count <= 1
            // 动画播放器为空意味着图像格式不支持，或者帧数<=1
            return;
        }
        
        // Custom Loop Count - 自定义循环数
        if (self.shouldCustomLoopCount) {
            self.player.totalLoopCount = self.animationRepeatCount;
        }
        // RunLoop Mode - 运行循环模式
        self.player.runLoopMode = self.runLoopMode;
        // Max Buffer Size - 文件最大缓存大小
        self.player.maxBufferSize = self.maxBufferSize;
        // Play Rate - 播放速率
        self.player.playbackRate = self.playbackRate;
        // Play Mode - 播放模式
        self.player.playbackMode = self.playbackMode;
        // Setup handler - 设置处理者
        @weakify(self);
        self.player.animationFrameHandler = ^(NSUInteger index, UIImage * frame) {
            @strongify(self);
            self.currentFrameIndex = index;
            self.currentFrame = frame;
            [self.imageViewLayer setNeedsDisplay];
        };
        self.player.animationLoopHandler = ^(NSUInteger loopCount) {
            @strongify(self);
            // Progressive image reach the current last frame index. Keep the state and pause animating. Wait for later restart
            //递进图像达到当前最后一帧索引。保持状态并暂停动画。等待稍后重启
            if (self.isProgressive) {
                NSUInteger lastFrameIndex = self.player.totalFrameCount - 1;
                [self.player seekToFrameAtIndex:lastFrameIndex loopCount:0];
                [self.player pausePlaying];
            } else {
                self.currentLoopCount = loopCount;
            }
        };
        
        // Ensure disabled highlighting; it's not supported (see `-setHighlighted:`).
        // 确保残疾人高亮显示;不支持(见' - sethighlit: ')
        super.highlighted = NO;
        
        [self stopAnimating];
        [self checkPlay];

        [self.imageViewLayer setNeedsDisplay];
    }
}

#pragma mark - Configuration

- (void)setRunLoopMode:(NSRunLoopMode)runLoopMode
{
    _runLoopMode = [runLoopMode copy];
    self.player.runLoopMode = runLoopMode;
}

- (NSRunLoopMode)runLoopMode
{
    if (!_runLoopMode) {
        _runLoopMode = [[self class] defaultRunLoopMode];
    }
    return _runLoopMode;
}

+ (NSString *)defaultRunLoopMode {
    // Key off `activeProcessorCount` (as opposed to `processorCount`) since the system could shut down cores in certain situations.
    return [NSProcessInfo processInfo].activeProcessorCount > 1 ? NSRunLoopCommonModes : NSDefaultRunLoopMode;
}

- (void)setMaxBufferSize:(NSUInteger)maxBufferSize
{
    _maxBufferSize = maxBufferSize;
    self.player.maxBufferSize = maxBufferSize;
}

- (NSUInteger)maxBufferSize {
    return _maxBufferSize; // Defaults to 0
}

- (void)setPlaybackRate:(double)playbackRate
{
    _playbackRate = playbackRate;
    self.player.playbackRate = playbackRate;
}

- (double)playbackRate
{
    if (!_initFinished) {
        return 1.0; // Defaults to 1.0
    }
    return _playbackRate;
}

- (void)setPlaybackMode:(SDAnimatedImagePlaybackMode)playbackMode {
    _playbackMode = playbackMode;
    self.player.playbackMode = playbackMode;
}

- (SDAnimatedImagePlaybackMode)playbackMode {
    if (!_initFinished) {
        return SDAnimatedImagePlaybackModeNormal; // Default mode is normal
    }
    return _playbackMode;
}


- (BOOL)shouldIncrementalLoad
{
    if (!_initFinished) {
        return YES; // Defaults to YES
    }
    return _initFinished;
}

#pragma mark - UIView Method Overrides
#pragma mark Observing View-Related Changes

#if SD_MAC
- (void)viewDidMoveToSuperview
#else
- (void)didMoveToSuperview
#endif
{
#if SD_MAC
    [super viewDidMoveToSuperview];
#else
    [super didMoveToSuperview];
#endif
    
    [self checkPlay];
}

#if SD_MAC
- (void)viewDidMoveToWindow
#else
- (void)didMoveToWindow
#endif
{
#if SD_MAC
    [super viewDidMoveToWindow];
#else
    [super didMoveToWindow];
#endif
    
    [self checkPlay];
}

#if SD_MAC
- (void)setAlphaValue:(CGFloat)alphaValue
#else
- (void)setAlpha:(CGFloat)alpha
#endif
{
#if SD_MAC
    [super setAlphaValue:alphaValue];
#else
    [super setAlpha:alpha];
#endif
    
    [self checkPlay];
}

- (void)setHidden:(BOOL)hidden
{
    [super setHidden:hidden];
    
    [self checkPlay];
}

#pragma mark - UIImageView Method Overrides
#pragma mark Image Data

- (void)setAnimationRepeatCount:(NSInteger)animationRepeatCount
{
#if SD_UIKIT
    [super setAnimationRepeatCount:animationRepeatCount];
#else
    _animationRepeatCount = animationRepeatCount;
#endif
    
    if (self.shouldCustomLoopCount) {
        self.player.totalLoopCount = animationRepeatCount;
    }
}

- (void)startAnimating
{
    if (self.player) {
        [self updateShouldAnimate];
        if (self.shouldAnimate) {
            [self.player startPlaying];
        }
    } else {
#if SD_UIKIT
        [super startAnimating];
#else
        [super setAnimates:YES];
#endif
    }
}

- (void)stopAnimating
{
    if (self.player) {
        if (self.resetFrameIndexWhenStopped) {
            [self.player stopPlaying];
        } else {
            [self.player pausePlaying];
        }
        if (self.clearBufferWhenStopped) {
            [self.player clearFrameBuffer];
        }
    } else {
#if SD_UIKIT
        [super stopAnimating];
#else
        [super setAnimates:NO];
#endif
    }
}

#if SD_UIKIT
- (BOOL)isAnimating
{
    if (self.player) {
        return self.player.isPlaying;
    } else {
        return [super isAnimating];
    }
}
#endif

#if SD_MAC
- (BOOL)animates
{
    if (self.player) {
        return self.player.isPlaying;
    } else {
        return [super animates];
    }
}

- (void)setAnimates:(BOOL)animates
{
    if (animates) {
        [self startAnimating];
    } else {
        [self stopAnimating];
    }
}
#endif

#pragma mark Highlighted Image Unsupport

- (void)setHighlighted:(BOOL)highlighted
{
    // Highlighted image is unsupported for animated images, but implementing it breaks the image view when embedded in a UICollectionViewCell.
    /// 高亮显示的图像不支持动画图像，但嵌入在UICollectionViewCell实现它时打破了图像视图
    if (!self.player) {
        [super setHighlighted:highlighted];
    }
}


#pragma mark - Private Methods
#pragma mark Animation

/// Check if it should be played - 检查imageView是否应该播放
- (void)checkPlay
{
    // Only handle for SDAnimatedImage, leave UIAnimatedImage or animationImages for super implementation control
    if (self.player && self.autoPlayAnimatedImage) {
        [self updateShouldAnimate];
        if (self.shouldAnimate) {
            [self startAnimating];
        } else {
            [self stopAnimating];
        }
    }
}

// Don't repeatedly check our window & superview in `-displayDidRefresh:` for performance reasons.
// 出于性能原因，不要在' -displayDidRefresh: '中反复检查我们的窗口和父视图。
// Just update our cached value whenever the animated image or visibility (window, superview, hidden, alpha) is changed.
// 只要更新我们的缓存值时，动画图像或可见性(窗口，父视图，隐藏，alpha)被改变
- (void)updateShouldAnimate
{
#if SD_MAC
    BOOL isVisible = self.window && self.superview && ![self isHidden] && self.alphaValue > 0.0;
#else
    BOOL isVisible = self.window && self.superview && ![self isHidden] && self.alpha > 0.0;
#endif
    self.shouldAnimate = self.player && isVisible;
}

// Update progressive status only after `setImage:` call.
/// 在‘setImage:’调用后更新渐进状态
- (void)updateIsProgressiveWithImage:(UIImage *)image
{
    self.isProgressive = NO;
    if (!self.shouldIncrementalLoad) {
        // Early return
        return;
    }
    // We must use `image.class conformsToProtocol:` instead of `image conformsToProtocol:` here
    // Because UIKit on macOS, using internal hard-coded override method, which returns NO
    /// 我们必须在这里使用' image.class conformsToProtocol: '而不是' image conformsToProtocol: '
    /// 因为UIKit在macOS上，使用内部硬编码的override方法，它返回NO
    id<SDAnimatedImageCoder> currentAnimatedCoder = [self progressiveAnimatedCoderForImage:image];
    if (currentAnimatedCoder) {
        UIImage *previousImage = self.image;
        if (!previousImage) {
            // If current animated coder supports progressive, and no previous image to check, start progressive loading
            /// 如果当前的动画编码器支持渐进式，并没有以前的图像检查，开始渐进式加载
            self.isProgressive = YES;
        } else {
            id<SDAnimatedImageCoder> previousAnimatedCoder = [self progressiveAnimatedCoderForImage:previousImage];
            if (previousAnimatedCoder == currentAnimatedCoder) {
                // If current animated coder is the same as previous, start progressive loading
                /// 如果当前动画解码器和之前的一样，则开始渐进式加载
                self.isProgressive = YES;
            }
        }
    }
}

// Check if image can represent a `Progressive Animated Image` during loading
/// 检查图像是否可以在加载过程中表示一个“渐进动画图像”
- (id<SDAnimatedImageCoder, SDProgressiveImageCoder>)progressiveAnimatedCoderForImage:(UIImage *)image
{
    if ([image.class conformsToProtocol:@protocol(SDAnimatedImage)] && image.sd_isIncremental && [image respondsToSelector:@selector(animatedCoder)]) {
        id<SDAnimatedImageCoder> animatedCoder = [(id<SDAnimatedImage>)image animatedCoder];
        if ([animatedCoder conformsToProtocol:@protocol(SDProgressiveImageCoder)]) {
            return (id<SDAnimatedImageCoder, SDProgressiveImageCoder>)animatedCoder;
        }
    }
    return nil;
}


#pragma mark Providing the Layer's Content
#pragma mark - CALayerDelegate

- (void)displayLayer:(CALayer *)layer
{
    UIImage *currentFrame = self.currentFrame;
    if (currentFrame) {
        layer.contentsScale = currentFrame.scale;
        layer.contents = (__bridge id)currentFrame.CGImage;
    } else {
        // If we have no animation frames, call super implementation. iOS 14+ UIImageView use this delegate method for rendering.
        if ([UIImageView instancesRespondToSelector:@selector(displayLayer:)]) {
            [super displayLayer:layer];
        }
    }
}

#if SD_MAC
// NSImageView use a subview. We need this subview's layer for actual rendering.
// Why using this design may because of properties like `imageAlignment` and `imageScaling`, which it's not available for UIImageView.contentMode (it's impossible to align left and keep aspect ratio at the same time)
- (NSView *)imageView {
    NSImageView *imageView = objc_getAssociatedObject(self, SD_SEL_SPI(imageView));
    if (!imageView) {
        // macOS 10.14
        imageView = objc_getAssociatedObject(self, SD_SEL_SPI(imageSubview));
    }
    return imageView;
}

// on macOS, it's the imageView subview's layer (we use layer-hosting view to let CALayerDelegate works)
- (CALayer *)imageViewLayer {
    NSView *imageView = self.imageView;
    if (!imageView) {
        return nil;
    }
    if (!_imageViewLayer) {
        _imageViewLayer = [CALayer new];
        _imageViewLayer.delegate = self;
        imageView.layer = _imageViewLayer;
        imageView.wantsLayer = YES;
    }
    return _imageViewLayer;
}
#else
// on iOS, it's the imageView itself's layer
- (CALayer *)imageViewLayer {
    return self.layer;
}

#endif

@end

#endif
