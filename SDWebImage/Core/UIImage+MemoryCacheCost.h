/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

/**
 UIImage category for memory cache cost.
 内存缓存消耗分类
 */
@interface UIImage (MemoryCacheCost)

/**
 The memory cache cost for specify image used by image cache. The cost function is the bytes size held in memory.
 If you set some associated object to `UIImage`, you can set the custom value to indicate the memory cost.
 
 指定图像缓存所使用的图像的内存缓存开销。cost函数是存储在内存中的字节大小。
 如果你设置了一些关联对象到UIImage，你可以设置自定义值来表示内存开销。
 
 For `UIImage`, this method return the single frame bytes size when `image.images` is nil for static image. Return full frame bytes size when `image.images` is not nil for animated image.
 For `NSImage`, this method return the single frame bytes size because `NSImage` does not store all frames in memory.
 @note Note that because of the limitations of category this property can get out of sync if you create another instance with CGImage or other methods.
 @note For custom animated class conforms to `SDAnimatedImage`, you can override this getter method in your subclass to return a more proper value instead, which representing the current frame's total bytes.
 
 对于‘UIImage’，静态图像’image.images‘为空时返回单个帧的大小，动画图像‘image.images’不为空时返回所有帧的字节大小
 对于‘NSImage’, 因为‘NSImage’不在内存中存储所有的帧,
 注意，由于category的限制，如果你用CGImage或其他方法创建另一个实例，这个属性可能会不同步。
 对于符合SDAnimatedImage的自定义动画类，你可以在子类中重写这个getter方法来返回一个更合适的值，它代表当前帧的总字节数。
 */
@property (assign, nonatomic) NSUInteger sd_memoryCost;

@end
