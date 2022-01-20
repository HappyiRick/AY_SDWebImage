/*
* This file is part of the SDWebImage package.
* (c) Olivier Poitrey <rs@dailymotion.com>
*
* For the full copyright and license information, please view the LICENSE
* file that was distributed with this source code.
*/

#import "SDWebImageCompat.h"

/// Copy the associated object from source image to target image. The associated object including all the category read/write properties.
/// 将相关对象从源图像复制到目标图像。包含所有类别读/写属性的关联对象
///
/// @param source source
/// @param target target
FOUNDATION_EXPORT void SDImageCopyAssociatedObject(UIImage * _Nullable source, UIImage * _Nullable target);
