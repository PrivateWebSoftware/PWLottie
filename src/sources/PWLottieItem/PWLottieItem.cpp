/*
 * Copyright (C) PrivateWeb Software (https://github.com/PrivateWebSoftware) - All Rights Reserved
 *
 * Licensed under the Apache License 2.0 (the "License"). You may not use
 * this file except in compliance with the License. You can obtain a copy
 * in the file LICENSE in the source distribution
 *
 * Written by PrivateWeb Software <privatewebsoftware@protonmail.com>, July 2024
 */

#include "include/PWLottieItem/PWLottieItem.h"

PWLottieItem::PWLottieItem()
    : m_lottieUuid(QUuid::createUuid().toString())
{
    /* Limit the number of threads so as not to take too much resources */
    m_renderThreadPool.setMaxThreadCount(maxRenderThreads);

    /* Create timer for controlling fps rate of lottie animation */
    m_renderTimer.setTimerType(Qt::PreciseTimer);
    connect(&m_renderTimer, &QTimer::timeout, this, [this]() {
        if (this->running()) {
            this->render();
        }
    });

    /* Set up framerate and start render timer */
    setFrameRate(m_frameRate);

    m_renderTimer.start();
}

///
/// \brief PWLottieItem::setSourceSize - Function sets source size of lottie animation.
/// \param sourceSize - Size that will be installed.
///
void PWLottieItem::setSourceSize(const QSizeF& sourceSize)
{
    m_sourceSize = sourceSize;
    emit sourceSizeChanged();

    /* Locate memory for frame pixels */
    m_frameBuffer.reset(new char[static_cast<qint32>(m_sourceSize.height() * m_sourceSize.width() * lottieRgbFormatSize / lottieRgbChannelSize)]);

    /* Start rendering */
    this->render();
}

///
/// \brief PWLottieItem::setController - Functions sets ControllerType for PWLottieItem.
/// \param controllerType - Controller type that will be set to PWLottieItem.
///
void PWLottieItem::setController(const PWControllerMediator::ControllerType controllerType)
{
    if (controllerType != PWControllerMediator::ControllerType::NoController) {
        /* Set controller if we haven't set it before s*/
        m_controllerType = controllerType;

        /* Register lottie item in controller */
        setFrameRate(PWControllerMediator::registerLottieAnimation(m_controllerType, m_lottieUuid));

        /* If controller changed framerate, change it in lottie item*/
        connect(PWControllerMediator::instance(), &PWControllerMediator::fpsChanged, this, [=, this](const quint16 fps, const PWControllerMediator::ControllerType controllerType, const QString& lottieUuid) {
            if (lottieUuid == allLottiesDefiner || lottieUuid == m_lottieUuid) {
                setFrameRate(fps);
            }
        });
    } else if (m_controllerType != PWControllerMediator::ControllerType::NoController) {
        /* Change controller type if we set it before */
        PWControllerMediator::unregisterLottieAnimation(m_controllerType, m_lottieUuid);

        m_controllerType = controllerType;
    }

    emit controllerChanged();
}

///
/// \brief PWLottieItem::setSource - Functions sets source size and loads rlottie::Animation and it's properties/
/// \param source - Source of image that will be applied for item.
///
void PWLottieItem::setSource(const QString& source)
{
    QFile lottieFile(source);
    if (lottieFile.open(QFile::ReadOnly)) {
        const QByteArray lottieBuffer = lottieFile.readAll();

        if (!lottieBuffer.isEmpty()) {
            /* Create lottie animation */
            m_animation = rlottie::Animation::loadFromData(lottieBuffer.constData(), QUrl(source).fileName().toStdString(), QCoreApplication::applicationDirPath().toUtf8().constData());

            if (m_animation) {
                /* Set up lottie animation properties */
                m_source = source;
                m_totalFrames = m_animation->totalFrame();
                m_duration = m_animation->duration();

                /* Emit that state of PWLottieItem was changed */
                emit sourceChanged();

                return;
            }
        }
    } else {
        qWarning() << "Couldn't open lottie file with error:" << lottieFile.errorString();
    }

    emit errorOccured();
}

///
/// \brief PWLottieItem::paint - Overrided QQuickPaintedItem function 'paint'. It paints image in qml.
/// \param painter - Rendering device.
///
void PWLottieItem::paint(QPainter* painter)
{
    if (!painter) {
        return;
    }

    if (!m_currentImage.isNull()) {
        painter->drawImage(this->boundingRect(), m_currentImage);
    }
}

///
/// \brief PWLottieItem::render - Function renders in thread Lottie Image data before it's painting.
///
void PWLottieItem::render()
{
    if (m_running && !m_source.isEmpty() && !m_sourceSize.isEmpty()) {
        QtConcurrent::run(&m_renderThreadPool, [&]() {
            /* Render lottie animation in synchronus function, because we making it asynchronus with Qt */
            rlottie::Surface surface(reinterpret_cast<uint32_t*>(m_frameBuffer.data()), qRound(m_sourceSize.width()), qRound(m_sourceSize.height()), qRound(m_sourceSize.width()) * lottieRgbFormatSize / lottieRgbChannelSize);
            m_animation->renderSync(m_currentFrame, surface);

            /* If total frames more than we have, reset */
            if (m_currentFrame >= m_totalFrames) {
                m_currentFrame = 0;
            } else {
                m_currentFrame += 1;
            }

            /* Create new image that will be painted */
            QImage image(m_sourceSize.width(), m_sourceSize.height(), QImage::Format_RGB32);

            for (qint32 i = 0; i != image.height(); ++i) {
                /* Get pixel data from buffer that was rendered with rlottie */
                const char* pixel = m_frameBuffer.data() + i * image.bytesPerLine();

                if (pixel) {
                    /* Copy pixel data to image */
                    std::memcpy(image.scanLine(i), pixel, image.bytesPerLine());
                }
            }

            /*
             * Cause we making, multi thread rendering,
             * save rendered image in buffer and read it,
             * when we need to paint it.
             */
            m_currentImage = image;
        }).then(this, [this] {
            /* Before starting new thread check for animation loops */
            if (m_loops > 0) {
                if (m_loops - 1 == m_currentLoops && m_totalFrames == m_currentFrame) {
                    /* Stop animation */
                    pause();
                } else if (m_totalFrames == m_currentFrame) {
                    /* If user haven't infinite loop, plus on more loop */
                    m_currentLoops += 1;
                }
            }

            update();
        });
    }
}
