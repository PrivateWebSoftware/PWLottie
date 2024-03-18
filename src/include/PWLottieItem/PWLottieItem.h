/*
 * Copyright (C) PrivateWeb Software (https://github.com/PrivateWebSoftware) - All Rights Reserved
 *
 * Licensed under the Apache License 2.0 (the "License"). You may not use
 * this file except in compliance with the License. You can obtain a copy
 * in the file LICENSE in the source distribution
 *
 * Written by PrivateWeb Software <privatewebsoftware@protonmail.com>, July 2024
 */

#ifndef LOTTIEITEM_H
#define LOTTIEITEM_H

#include <QDebug>
#include <QFile>
#include <QImage>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QPainter>
#include <QQuickPaintedItem>
#include <QScopedArrayPointer>
#include <QScopedPointer>
#include <QThreadPool>
#include <QTimer>
#include <QUuid>
#include <QtConcurrent>

#include <rlottie.h>
#include <rlottie_capi.h>
#include <rlottiecommon.h>

#include "include/PWControllerMediator/PWControllerMediator.h"

///
/// \brief The PWLottieItem class - QQuickItem, that paints images rendered by rlottie engine.
///
class PWLottieItem : public QQuickPaintedItem {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(qint32 frameRate READ frameRate WRITE setFrameRate NOTIFY frameRateChanged)
    Q_PROPERTY(qint32 loops READ loops WRITE setLoops NOTIFY loopsChanged)
    Q_PROPERTY(qreal duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(QSizeF sourceSize READ sourceSize WRITE setSourceSize NOTIFY sourceSizeChanged)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(PWControllerMediator::ControllerType controller READ controller WRITE setController NOTIFY controllerChanged)

#define lottieRgbFormatSize 32
#define lottieRgbChannelSize 8
#define maxRenderThreads 1

#define initializePWLottieControllers qmlRegisterUncreatableType<PWControllerMediator>("PrivateWeb.PWLottie.Controllers", 2, 0, "ControllerType", "Cannot initialize PWLottie Controllers in QML");

public:
    PWLottieItem();
    ~PWLottieItem()
    {
        /* Stop animation */
        this->pause();

        /* Unregister Lottie Animation in controllers */
        if (m_controllerType != PWControllerMediator::ControllerType::NoController) {
            PWControllerMediator::unregisterLottieAnimation(m_controllerType, m_lottieUuid);
        }
    }

    /*****************/
    /* Running state */
    /*****************/

    [[nodiscard]] inline bool running() const
    {
        return m_running;
    }

    inline void setRunning(const bool running)
    {
        m_running = running;

        emit runningChanged();
    }

    /**************/
    /* Frame Rate */
    /**************/

    [[nodiscard]] inline quint16 frameRate() const
    {
        return m_frameRate;
    }

    inline void setFrameRate(const qint32 frameRate)
    {
        m_frameRate = frameRate;

        /*
         * Set up needed framerate render changes per one second
         * 1000 - One Second
         */
        m_renderTimer.setInterval(qRound(qreal(1000) / m_frameRate));

        emit frameRateChanged(m_frameRate);
    }

    /*********/
    /* Loops */
    /*********/

    [[nodiscard]] inline qint32 loops() const
    {
        return m_loops;
    }

    inline void setLoops(const qint32 loops)
    {
        m_loops = loops;

        emit loopsChanged();
    }

    /************/
    /* Duration */
    /************/

    [[nodiscard]] inline qreal duration() const
    {
        return m_duration;
    }

    /***************/
    /* Source Size */
    /***************/

    [[nodiscard]] inline QSizeF sourceSize() const
    {
        return m_sourceSize;
    }

    ///
    /// \brief setSourceSize - Function sets source size of lottie animation.
    /// \param sourceSize - Size that will be installed.
    ///
    void setSourceSize(const QSizeF& sourceSize);

    /**********/
    /* Source */
    /**********/

    [[nodiscard]] inline QString source() const
    {
        return m_source;
    }

    /**************/
    /* Controller */
    /**************/

    [[nodiscard]] inline PWControllerMediator::ControllerType controller() const
    {
        return m_controllerType;
    }

    ///
    /// \brief setController - Functions sets ControllerType for PWLottieItem.
    /// \param controllerType - Controller type that will be set to PWLottieItem.
    ///
    void setController(const PWControllerMediator::ControllerType controllerType);

    ///
    /// \brief setSource -Functions sets source size and loads rlottie::Animation and it's properties/
    /// \param source - Source of image that will be applied for item.
    ///
    void setSource(const QString& source);

    /*********/
    /* Paint */
    /*********/

    ///
    /// \brief paint - Overrided QQuickPaintedItem function 'paint'. It paints image in qml.
    /// \param painter - Painting device.
    ///
    void paint(QPainter* painter) override;

    ///
    /// \brief render - Function renders in thread Lottie Image data before it's painting.
    ///
    void render();

public slots:
    ///
    /// \brief resume - Function resumes rendering of lottie animation.
    ///
    void resume()
    {
        if (!m_running) {
            m_running = true;
            this->render();
        }
    }

    ///
    /// \brief pause - Function pauses rendering of lottie animation.
    ///
    void pause()
    {
        m_running = false;
    }

signals:
    void runningChanged();
    void errorOccured();
    void frameRateChanged(const qint32 frameRate);
    void loopsChanged();
    void durationChanged();
    void sourceSizeChanged();
    void sourceChanged();
    void controllerChanged();

private:
    /******************/
    /* QML properties */
    /******************/

    bool m_running = true;
    qint32 m_currentFrame = 0;
    qint32 m_totalFrames = 0;
    qint32 m_loops = 0;
    qint32 m_frameRate = 60;
    qreal m_duration = 0.0;
    QSizeF m_sourceSize = { 0, 0 };
    QString m_source;
    PWControllerMediator::ControllerType m_controllerType = PWControllerMediator::ControllerType::NoController;

    /*******************/
    /* Lottie privates */
    /*******************/

    QString m_lottieUuid;
    qint32 m_currentLoops = 0;

    std::unique_ptr<rlottie::Animation> m_animation = nullptr;
    QScopedArrayPointer<char> m_frameBuffer;
    QImage m_currentImage;

    QTimer m_renderTimer;
    QThreadPool m_renderThreadPool;
};

#endif // LOTTIEITEM_H
