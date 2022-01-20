/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"

/// A Image-Asset manager to work like UIKit/AppKit's image cache behavior
/// Apple parse the Asset Catalog compiled file(`Assets.car`) by CoreUI.framework, however it's a private framework and there are no other ways to directly get the data. So we just process the normal bundle files :)
/// 一个像UIKit/AppKit的图像缓存行为一样的图像资源管理器
/// Apple通过coreUI.framework解析Asset Catalog编译文件(' Assets.car ')，但它是一个私有框架，没有其他方式直接获取数据。所以我们只处理普通的bundle文件:)
@interface SDImageAssetManager : NSObject
/// 图像表
@property (nonatomic, strong, nonnull) NSMapTable<NSString *, UIImage *> *imageTable;
/// 共享资源管理器
+ (nonnull instancetype)sharedAssetManager;
/// 获取图像path
- (nullable NSString *)getPathForName:(nonnull NSString *)name bundle:(nonnull NSBundle *)bundle preferredScale:(nonnull CGFloat *)scale;
/// 根据name获取图像
- (nullable UIImage *)imageForName:(nonnull NSString *)name;
/// 用name存储图像
- (void)storeImage:(nonnull UIImage *)image forName:(nonnull NSString *)name;

@end
