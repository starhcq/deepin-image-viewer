// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "globalcontrol.h"
#include "types.h"
#include "imagedata/imagesourcemodel.h"

GlobalControl::GlobalControl(QObject *parent)
    : QObject(parent)
{
}

/**
   @brief 设置全局使用的数据模型为 \a model
 */
void GlobalControl::setGlobalModel(ImageSourceModel *model)
{
    sourceModel = model;
    Q_EMIT globalModelChanged();
}

/**
   @return 返回全局的数据模型
 */
ImageSourceModel *GlobalControl::globalModel() const
{
    return sourceModel;
}

/**
   @return 返回当前设置的图片url地址
 */
QUrl GlobalControl::currentSource() const
{
    return currentImage.source();
}

/**
   @brief 设置当前展示的图片索引为 \a index
 */
void GlobalControl::setCurrentIndex(int index)
{
    if (this->index != index) {
        // 更新图像信息，无论变更均更新
        QUrl image = sourceModel->data(sourceModel->index(index), Types::ImageUrlRole).toUrl();
        currentImage.setSource(image);
        Q_EMIT currentSourceChanged();

        this->index = index;
        Q_EMIT currentIndexChanged();

        checkSwitchEnable();
    }
}

/**
   @return 返回当前展示的图片索引
 */
int GlobalControl::currentIndex() const
{
    return index;
}

/**
   @brief 设置当前展示的多页图图片索引为 \a frameIndex
 */
void GlobalControl::setCurrentFrameIndex(int frameIndex)
{
    if (this->frameIndex != frameIndex) {
        this->frameIndex = frameIndex;
        Q_EMIT currentFrameIndexChanged();

        checkSwitchEnable();
    }
}

/**
   @return 返回当前展示的多页图索引
 */
int GlobalControl::currentFrameIndex() const
{
    return frameIndex;
}

/**
   @return 返回是否可切换到前一张图片
 */
bool GlobalControl::hasPreviousImage() const
{
    return hasPrevious;
}

/**
   @return 返回是否可切换到后一张图片
 */
bool GlobalControl::hasNextImage() const
{
    return hasNext;
}

/**
   @return 切换到前一张图片并返回是否切换成功
 */
bool GlobalControl::previousImage()
{
    if (hasPreviousImage()) {
        Q_ASSERT(sourceModel);
        if (Types::MultiImage == currentImage.type()) {
            if (frameIndex > 0) {
                setCurrentFrameIndex(frameIndex - 1);
                return true;
            }
        }

        if (index > 0) {
            setCurrentIndex(index - 1);

            if (Types::MultiImage == currentImage.type()) {
                setCurrentFrameIndex(currentImage.frameCount() - 1);
            } else {
                setCurrentFrameIndex(0);
            }
            return true;
        }
    }

    return false;
}

/**
   @return 切换到后一张图片并返回是否切换成功
 */
bool GlobalControl::nextImage()
{
    if (hasNextImage()) {
        Q_ASSERT(sourceModel);
        if (Types::MultiImage == currentImage.type()) {
            if (frameIndex < currentImage.frameCount() - 1) {
                setCurrentFrameIndex(frameIndex + 1);
                return true;
            }
        }

        if (index < sourceModel->rowCount() - 1) {
            setCurrentIndex(index + 1);

            // 无论是否为多页图，均设置为0
            setCurrentFrameIndex(0);
            return true;
        }
    }

    return false;
}

/**
   @return 切换到首张图片并返回是否切换成功
 */
bool GlobalControl::firstImage()
{
    Q_ASSERT(sourceModel);
    if (sourceModel->rowCount()) {
        setCurrentFrameIndex(0);
        setCurrentIndex(0);
        return true;
    }
    return false;
}

/**
   @return 切换到最后图片并返回是否切换成功
 */
bool GlobalControl::lastImage()
{
    Q_ASSERT(sourceModel);
    int count = sourceModel->rowCount();
    if (count) {
        setCurrentIndex(count - 1);
        if (Types::MultiImage == currentImage.type()) {
            setCurrentFrameIndex(currentImage.frameCount() - 1);
        }

        return true;
    }
    return false;
}

/**
   @brief 设置打开图片列表 \a filePaths ， 其中 \a openFile 是首个展示的图片路径，
    将更新全局数据源并发送状态变更信号
 */
void GlobalControl::setImageFiles(const QStringList &filePaths, const QString &openFile)
{
    Q_ASSERT(sourceModel);
    // 优先更新数据源
    sourceModel->setImageFiles(filePaths);
    int index = filePaths.indexOf(openFile);
    if (-1 == index || filePaths.isEmpty()) {
        index = 0;
    }

    if (this->index != index) {
        this->index = index;
        Q_EMIT currentIndexChanged();
    }
    if (0 != this->frameIndex) {
        this->frameIndex = 0;
        Q_EMIT currentFrameIndexChanged();
    }

    // 更新图像信息，无论变更均更新
    if (currentImage.source() != openFile) {
        currentImage.setSource(openFile);
    }
    Q_EMIT currentSourceChanged();

    checkSwitchEnable();
}

int GlobalControl::thumbnailViewBottom() const
{
    return ThumbnailViewHeight;
}

/**
   @brief 根据当前展示图片索引判断是否允许切换前后图片
 */
void GlobalControl::checkSwitchEnable()
{
    Q_ASSERT(sourceModel);
    const bool previous = (index > 0 || frameIndex > 0);
    const bool next = index < (sourceModel->rowCount() - 1) || frameIndex < (currentImage.frameCount() - 1);

    if (previous != hasPrevious) {
        hasPrevious = previous;
        Q_EMIT hasPreviousImageChanged();
    }
    if (next != hasNext) {
        hasNext = next;
        Q_EMIT hasNextImageChanged();
    }
}