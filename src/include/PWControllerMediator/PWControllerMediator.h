/*
 * Copyright (C) PrivateWeb Software (https://github.com/PrivateWebSoftware) - All Rights Reserved
 *
 * Licensed under the Apache License 2.0 (the "License"). You may not use
 * this file except in compliance with the License. You can obtain a copy
 * in the file LICENSE in the source distribution
 *
 * Written by PrivateWeb Software <privatewebsoftware@protonmail.com>, July 2024
 */

#ifndef PWCONTROLLERMEDIATOR_H
#define PWCONTROLLERMEDIATOR_H

#include <QObject>
#include <QString>

#include "include/PWLottieControllers/PWLottieBaseController.h"
#include "include/PWLottieControllers/PWLottieIconController.h"

///
/// \brief The PWControllerMediator class - A class whose task is to reduce coupling between controllers and QML Item. Class provides functional for controllers.
///
class PWControllerMediator : public QObject {
    Q_OBJECT

public:
    explicit PWControllerMediator(QObject* parent = nullptr);

    /*********/
    /* Enums */
    /*********/

    ///
    /// \brief The ControllerType enum - Controller types that can be used.
    ///
    enum ControllerType {
        NoController = 0,
        BaseController = 1,
        IconController = 2
    };
    Q_ENUM(ControllerType)

    /*************/
    /* Functions */
    /*************/

    ///
    /// \brief instance - Singleton instance funtion, cause we need only one copy of object for hole application.
    /// \return Instance to PWLottieBaseController class.
    ///
    static inline QPointer<PWControllerMediator> instance()
    {
        if (!m_instance) {
            m_instance = QPointer<PWControllerMediator>(new PWControllerMediator);
        }

        return m_instance;
    }

    ///
    /// \brief registerLottieAnimation - Registers lottie item in control system.
    /// \param controllerType - Controller type, that will register lottie item in it's own system.
    /// \param lottieUuid - Unique lottie item ID with that ID lottie item will be registered.
    /// \return Returns current fps of registred lotti animation.
    ///
    static quint16 registerLottieAnimation(const ControllerType controllerType, const QString& lottieUuid);

    ///
    /// \brief unregisterLottieAnimation - Unregisters lottie item in control system.
    /// \attention unregisterLottieAnimation() must be always run if "registerLottieAnimation" was used before PWLottieItem delted.
    ///
    /// \param controllerType - Controller type, that will register lottie item in it's own system.
    /// \param lottieUuid- Unique lottie item ID with that ID lottie item will be unregistered.
    ///
    static void unregisterLottieAnimation(const ControllerType controllerType, const QString& lottieUuid);

signals:
    ///
    /// \brief fpsChanged - Signal that supports functional of PWLottieBaseController and PWLottieIconController
    ///
    void fpsChanged(const qint16 fps, const ControllerType controllerType, const QString& lottieUuid);

private:
    inline static quint16 m_standardFps = 30;

    inline static PWLottieBaseController m_lottieBasicController;
    inline static PWLottieIconController m_lottieIconController;

    inline static QPointer<PWControllerMediator> m_instance;
};

#endif // PWCONTROLLERMEDIATOR_H
