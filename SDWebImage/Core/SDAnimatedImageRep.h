/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

#if SD_MAC

/**
 A subclass of `NSBitmapImageRep` to fix that GIF duration issue because `NSBitmapImageRep` will reset `NSImageCurrentFrameDuration` by using `kCGImagePropertyGIFDelayTime` but not `kCGImagePropertyGIFUnclampedDelayTime`.
 This also fix the GIF loop count issue, which will use the Netscape standard (See http://www6.uniovi.es/gifanim/gifabout.htm)  to only place once when the `kCGImagePropertyGIFLoopCount` is nil. This is what modern browser's behavior.
 Built in GIF coder use this instead of `NSBitmapImageRep` for better GIF rendering. If you do not want this, only enable `SDImageIOCoder`, which just call `NSImage` API and actually use `NSBitmapImageRep` for GIF image.
 This also support APNG format using `SDImageAPNGCoder`. Which provide full alpha-channel support and the correct duration match the `kCGImagePropertyAPNGUnclampedDelayTime`.
 
 ' NSBitmapImageRep '的子类，以修复GIF持续时间问题，因为' NSBitmapImageRep '将重置' NSImageCurrentFrameDuration '使用' kCGImagePropertyGIFDelayTime '，而不是' kCGImagePropertyGIFUnclampedDelayTime '。
 这也修复了GIF循环计数问题，这将使用Netscape标准(见http://www6.uniovi.es/gifanim/gifabout.htm)只放置一次，当' kCGImagePropertyGIFLoopCount '为nil。这就是现代浏览器的行为。
 内置的GIF编码器使用这个代替' NSBitmapImageRep '更好的GIF渲染。如果你不想这样，只启用' SDImageIOCoder '，它只是调用' NSImage ' API，并实际使用' NSBitmapImageRep '的GIF图像。
 这也支持APNG格式使用' SDImageAPNGCoder '。它提供完整的alpha通道支持和正确的持续时间匹配' kCGImagePropertyAPNGUnclampedDelayTime '。
 */
@interface SDAnimatedImageRep : NSBitmapImageRep

@end

#endif
