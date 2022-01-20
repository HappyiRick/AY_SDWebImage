/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

#if SD_MAC

#import "SDWebImageManager.h"

/**
 * Integrates SDWebImage async downloading and caching of remote images with NSButton.
 * 通过NSButton集成了SDWebImage异步下载和缓存远程图像
 */
@interface NSButton (WebCache)

#pragma mark - Image

/**
 * Get the current image URL.
 * 获取当前图像URL
 */
@property (nonatomic, strong, readonly, nullable) NSURL *sd_currentImageURL;

/**
 * Set the button `image` with an `url`.  通过url设置按钮图像
 *
 * The download is asynchronous and cached. 下载异步且缓存
 *
 * @param url The url for the image.
 */
- (void)sd_setImageWithURL:(nullable NSURL *)url NS_REFINED_FOR_SWIFT;

/**
 * Set the button `image` with an `url` and a placeholder. 通过url和占位图设置按钮图像
 *
 * The download is asynchronous and cached. 异步下载并缓存
 *
 * @param url         The url for the image.
 * @param placeholder The image to be set initially, until the image request finishes.
 * @see sd_setImageWithURL:placeholderImage:options:
 */
- (void)sd_setImageWithURL:(nullable NSURL *)url
          placeholderImage:(nullable UIImage *)placeholder NS_REFINED_FOR_SWIFT;

/**
 * Set the button `image` with an `url`, placeholder and custom options. 通过url、占位图和普通选项设置按钮图像
 *
 * The download is asynchronous and cached. 异步下载且缓存
 *
 * @param url         The url for the image.
 * @param placeholder The image to be set initially, until the image request finishes.
 * @param options     The options to use when downloading the image. @see SDWebImageOptions for the possible values.
 */
- (void)sd_setImageWithURL:(nullable NSURL *)url
          placeholderImage:(nullable UIImage *)placeholder
                   options:(SDWebImageOptions)options NS_REFINED_FOR_SWIFT;

/**
 * Set the button `image` with an `url`, placeholder and custom options. 通过url、占位图和普通选项设置按钮图像
 *
 * The download is asynchronous and cached. 一步下载且缓存
 *
 * @param url         The url for the image.
 * @param placeholder The image to be set initially, until the image request finishes.
 * @param options     The options to use when downloading the image. @see SDWebImageOptions for the possible values.
 * @param context     A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
 */
- (void)sd_setImageWithURL:(nullable NSURL *)url
          placeholderImage:(nullable UIImage *)placeholder
                   options:(SDWebImageOptions)options
                   context:(nullable SDWebImageContext *)context;

/**
 * Set the button `image` with an `url`.  通过url设置图像
 *
 * The download is asynchronous and cached. 异步下载且缓存
 *
 * @param url            The url for the image.
 * @param completedBlock A block called when operation has been completed. This block has no return value
 *                       and takes the requested UIImage as first parameter. In case of error the image parameter
 *                       is nil and the second parameter may contain an NSError. The third parameter is a Boolean
 *                       indicating if the image was retrieved from the local cache or from the network.
 *                       The fourth parameter is the original image url.
 */
- (void)sd_setImageWithURL:(nullable NSURL *)url
                 completed:(nullable SDExternalCompletionBlock)completedBlock;

/**
 * Set the button `image` with an `url`, placeholder. 通过url、占位图设置
 *
 * The download is asynchronous and cached. 异步下载且缓存
 *
 * @param url            The url for the image.
 * @param placeholder    The image to be set initially, until the image request finishes.
 * @param completedBlock A block called when operation has been completed. This block has no return value
 *                       and takes the requested UIImage as first parameter. In case of error the image parameter
 *                       is nil and the second parameter may contain an NSError. The third parameter is a Boolean
 *                       indicating if the image was retrieved from the local cache or from the network.
 *                       The fourth parameter is the original image url.
 */
- (void)sd_setImageWithURL:(nullable NSURL *)url
          placeholderImage:(nullable UIImage *)placeholder
                 completed:(nullable SDExternalCompletionBlock)completedBlock NS_REFINED_FOR_SWIFT;

/**
 * Set the button `image` with an `url`, placeholder and custom options. - 通过url、占位图和选项设置按钮图像
 *
 * The download is asynchronous and cached. - 异步下载并缓存
 *
 * @param url            The url for the image.
 * @param placeholder    The image to be set initially, until the image request finishes.
 * @param options        The options to use when downloading the image. @see SDWebImageOptions for the possible values.
 * @param completedBlock A block called when operation has been completed. This block has no return value
 *                       and takes the requested UIImage as first parameter. In case of error the image parameter
 *                       is nil and the second parameter may contain an NSError. The third parameter is a Boolean
 *                       indicating if the image was retrieved from the local cache or from the network.
 *                       The fourth parameter is the original image url.
 */
- (void)sd_setImageWithURL:(nullable NSURL *)url
          placeholderImage:(nullable UIImage *)placeholder
                   options:(SDWebImageOptions)options
                 completed:(nullable SDExternalCompletionBlock)completedBlock;

/**
 * Set the button `image` with an `url`, placeholder and custom options. - 通过url、占位图和选项设置按钮图像
 *
 * The download is asynchronous and cached. - 异步下载并缓存
 *
 * @param url            The url for the image.
 * @param placeholder    The image to be set initially, until the image request finishes.
 * @param options        The options to use when downloading the image. @see SDWebImageOptions for the possible values.
 * @param progressBlock  A block called while image is downloading
 *                       @note the progress block is executed on a background queue
 * @param completedBlock A block called when operation has been completed. This block has no return value
 *                       and takes the requested UIImage as first parameter. In case of error the image parameter
 *                       is nil and the second parameter may contain an NSError. The third parameter is a Boolean
 *                       indicating if the image was retrieved from the local cache or from the network.
 *                       The fourth parameter is the original image url.
 */
- (void)sd_setImageWithURL:(nullable NSURL *)url
          placeholderImage:(nullable UIImage *)placeholder
                   options:(SDWebImageOptions)options
                  progress:(nullable SDImageLoaderProgressBlock)progressBlock
                 completed:(nullable SDExternalCompletionBlock)completedBlock;

/**
 * Set the button `image` with an `url`, placeholder and custom options. - 通过url、占位图和选项配置设置按钮图像
 *
 * The download is asynchronous and cached. - 异步下载并缓存
 *
 * @param url            The url for the image.
 * @param placeholder    The image to be set initially, until the image request finishes.
 * @param options        The options to use when downloading the image. @see SDWebImageOptions for the possible values.
 * @param context        A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
 * @param progressBlock  A block called while image is downloading
 *                       @note the progress block is executed on a background queue
 * @param completedBlock A block called when operation has been completed. This block has no return value
 *                       and takes the requested UIImage as first parameter. In case of error the image parameter
 *                       is nil and the second parameter may contain an NSError. The third parameter is a Boolean
 *                       indicating if the image was retrieved from the local cache or from the network.
 *                       The fourth parameter is the original image url.
 */
- (void)sd_setImageWithURL:(nullable NSURL *)url
          placeholderImage:(nullable UIImage *)placeholder
                   options:(SDWebImageOptions)options
                   context:(nullable SDWebImageContext *)context
                  progress:(nullable SDImageLoaderProgressBlock)progressBlock
                 completed:(nullable SDExternalCompletionBlock)completedBlock;

#pragma mark - Alternate Image

/**
 * Get the current alternateImage URL. - 获取当前替代图像URL
 */
@property (nonatomic, strong, readonly, nullable) NSURL *sd_currentAlternateImageURL;

/**
 * Set the button `alternateImage` with an `url`. - 通过url设置按钮替代图像
 *
 * The download is asynchronous and cached. - 异步下载并缓存
 *
 * @param url The url for the alternateImage.
 */
- (void)sd_setAlternateImageWithURL:(nullable NSURL *)url NS_REFINED_FOR_SWIFT;

/**
 * Set the button `alternateImage` with an `url` and a placeholder. - 通过url和占位图设置按钮替换图像
 *
 * The download is asynchronous and cached. - 异步下载并缓存
 *
 * @param url         The url for the alternateImage.
 * @param placeholder The alternateImage to be set initially, until the alternateImage request finishes.
 * @see sd_setAlternateImageWithURL:placeholderImage:options:
 */
- (void)sd_setAlternateImageWithURL:(nullable NSURL *)url
                   placeholderImage:(nullable UIImage *)placeholder NS_REFINED_FOR_SWIFT;

/**
 * Set the button `alternateImage` with an `url`, placeholder and custom options. - 通过url、占位图和常用选项设置按钮替代图
 *
 * The download is asynchronous and cached. - 异步下载并缓存
 *
 * @param url         The url for the alternateImage.
 * @param placeholder The alternateImage to be set initially, until the alternateImage request finishes.
 * @param options     The options to use when downloading the alternateImage. @see SDWebImageOptions for the possible values.
 */
- (void)sd_setAlternateImageWithURL:(nullable NSURL *)url
                   placeholderImage:(nullable UIImage *)placeholder
                            options:(SDWebImageOptions)options NS_REFINED_FOR_SWIFT;

/**
 * Set the button `alternateImage` with an `url`, placeholder, custom options and context. - 通过URL、占位图、常用选项和上下文设置按钮替代图
 *
 * The download is asynchronous and cached. - 异步下载并缓存
 *
 * @param url         The url for the alternateImage.
 * @param placeholder The alternateImage to be set initially, until the alternateImage request finishes.
 * @param options     The options to use when downloading the alternateImage. @see SDWebImageOptions for the possible values.
 * @param context     A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
 */
- (void)sd_setAlternateImageWithURL:(nullable NSURL *)url
                   placeholderImage:(nullable UIImage *)placeholder
                            options:(SDWebImageOptions)options
                            context:(nullable SDWebImageContext *)context;

/**
 * Set the button `alternateImage` with an `url`. - 通过url设置按钮替代图
 *
 * The download is asynchronous and cached. - 异步下载并缓存
 *
 * @param url            The url for the alternateImage.
 * @param completedBlock A block called when operation has been completed. This block has no return value
 *                       and takes the requested UIImage as first parameter. In case of error the alternateImage parameter
 *                       is nil and the second parameter may contain an NSError. The third parameter is a Boolean
 *                       indicating if the alternateImage was retrieved from the local cache or from the network.
 *                       The fourth parameter is the original alternateImage url.
 */
- (void)sd_setAlternateImageWithURL:(nullable NSURL *)url
                          completed:(nullable SDExternalCompletionBlock)completedBlock;

/**
 * Set the button `alternateImage` with an `url`, placeholder. - 通过url、占位图设置按钮替代图
 *
 * The download is asynchronous and cached. - 异步下载并缓存
 *
 * @param url            The url for the alternateImage.
 * @param placeholder    The alternateImage to be set initially, until the alternateImage request finishes.
 * @param completedBlock A block called when operation has been completed. This block has no return value
 *                       and takes the requested UIImage as first parameter. In case of error the alternateImage parameter
 *                       is nil and the second parameter may contain an NSError. The third parameter is a Boolean
 *                       indicating if the alternateImage was retrieved from the local cache or from the network.
 *                       The fourth parameter is the original alternateImage url.
 */
- (void)sd_setAlternateImageWithURL:(nullable NSURL *)url
                   placeholderImage:(nullable UIImage *)placeholder
                          completed:(nullable SDExternalCompletionBlock)completedBlock NS_REFINED_FOR_SWIFT;

/**
 * Set the button `alternateImage` with an `url`, placeholder and custom options. - 通过url、占位图和常用选项设置按钮替代图
 *
 * The download is asynchronous and cached. - 异步下载并缓存
 *
 * @param url            The url for the alternateImage.
 * @param placeholder    The alternateImage to be set initially, until the alternateImage request finishes.
 * @param options        The options to use when downloading the alternateImage. @see SDWebImageOptions for the possible values.
 * @param completedBlock A block called when operation has been completed. This block has no return value
 *                       and takes the requested UIImage as first parameter. In case of error the alternateImage parameter
 *                       is nil and the second parameter may contain an NSError. The third parameter is a Boolean
 *                       indicating if the alternateImage was retrieved from the local cache or from the network.
 *                       The fourth parameter is the original alternateImage url.
 */
- (void)sd_setAlternateImageWithURL:(nullable NSURL *)url
                   placeholderImage:(nullable UIImage *)placeholder
                            options:(SDWebImageOptions)options
                          completed:(nullable SDExternalCompletionBlock)completedBlock;

/**
 * Set the button `alternateImage` with an `url`, placeholder and custom options. - 通过url、占位图和常用选项设置按钮替代图
 *
 * The download is asynchronous and cached. - 异步下载并缓存
 *
 * @param url            The url for the alternateImage.
 * @param placeholder    The alternateImage to be set initially, until the alternateImage request finishes.
 * @param options        The options to use when downloading the alternateImage. @see SDWebImageOptions for the possible values.
 * @param progressBlock  A block called while alternateImage is downloading
 *                       @note the progress block is executed on a background queue
 * @param completedBlock A block called when operation has been completed. This block has no return value
 *                       and takes the requested UIImage as first parameter. In case of error the alternateImage parameter
 *                       is nil and the second parameter may contain an NSError. The third parameter is a Boolean
 *                       indicating if the alternateImage was retrieved from the local cache or from the network.
 *                       The fourth parameter is the original alternateImage url.
 */
- (void)sd_setAlternateImageWithURL:(nullable NSURL *)url
                   placeholderImage:(nullable UIImage *)placeholder
                            options:(SDWebImageOptions)options
                           progress:(nullable SDImageLoaderProgressBlock)progressBlock
                          completed:(nullable SDExternalCompletionBlock)completedBlock;

/**
 * Set the button `alternateImage` with an `url`, placeholder, custom options and context. - 通过url、占位图、常用选项和上下文设置按钮替代图像
 *
 * The download is asynchronous and cached. - 异步下载并缓存
 *
 * @param url            The url for the alternateImage.
 * @param placeholder    The alternateImage to be set initially, until the alternateImage request finishes.
 * @param options        The options to use when downloading the alternateImage. @see SDWebImageOptions for the possible values.
 * @param context        A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
 * @param progressBlock  A block called while alternateImage is downloading
 *                       @note the progress block is executed on a background queue
 * @param completedBlock A block called when operation has been completed. This block has no return value
 *                       and takes the requested UIImage as first parameter. In case of error the alternateImage parameter
 *                       is nil and the second parameter may contain an NSError. The third parameter is a Boolean
 *                       indicating if the alternateImage was retrieved from the local cache or from the network.
 *                       The fourth parameter is the original alternateImage url.
 */
- (void)sd_setAlternateImageWithURL:(nullable NSURL *)url
                   placeholderImage:(nullable UIImage *)placeholder
                            options:(SDWebImageOptions)options
                            context:(nullable SDWebImageContext *)context
                           progress:(nullable SDImageLoaderProgressBlock)progressBlock
                          completed:(nullable SDExternalCompletionBlock)completedBlock;

#pragma mark - Cancel

/**
 * Cancel the current image download - 取消当前图像下载
 */
- (void)sd_cancelCurrentImageLoad;

/**
 * Cancel the current alternateImage download - 取消当前替代图像下载
 */
- (void)sd_cancelCurrentAlternateImageLoad;

@end

#endif
