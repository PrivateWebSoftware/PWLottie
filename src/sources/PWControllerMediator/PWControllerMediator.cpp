/*
 * Copyright (C) PrivateWeb Software (https://github.com/PrivateWebSoftware) - All Rights Reserved
 *
 * Licensed under the Apache License 2.0 (the "License"). You may not use
 * this file except in compliance with the License. You can obtain a copy
 * in the file LICENSE in the source distribution
 *
 * Written by PrivateWeb Software <privatewebsoftware@protonmail.com>, July 2024
 */

#include "include/PWControllerMediator/PWControllerMediator.h"

PWControllerMediator::PWControllerMediator(QObject* parent)
    : QObject { parent }
{
    /* Register all needed controllers signals in constructor */
    connect(&m_lottieBasicController, &PWLottieBaseController::fpsChanged, this, [=](const quint16 fps, const QString& lottieUuid) {
        emit instance()->fpsChanged(fps, PWControllerMediator::BaseController, lottieUuid);
    });

    connect(&m_lottieIconController, &PWLottieIconController::fpsChanged, this, [=](const quint16 fps, const QString& lottieUuid) {
        emit instance()->fpsChanged(fps, PWControllerMediator::BaseController, lottieUuid);
    });
}

///
/// \brief PWControllerMediator::registerLottieAnimation - Registers lottie item in control system.
/// \param controllerType - Controller type, that will register lottie item in it's own system.
/// \param lottieUuid - Unique lottie item ID with that ID lottie item will be registered.
/// \return Returns current fps of registred lotti animation.
///
quint16 PWControllerMediator::registerLottieAnimation(const ControllerType controllerType, const QString& lottieUuid)
{
    /* Register lottie item in controller */
    if (controllerType == ControllerType::BaseController) {
        return m_lottieBasicController.addLottieItem(lottieUuid);
    } else if (controllerType == ControllerType::IconController) {
        return m_lottieIconController.addLottieItem(lottieUuid);
    }

    return m_standardFps;
}

///
/// \brief PWControllerMediator::unregisterLottieAnimation - Unregisters lottie item in control system.
/// \attention unregisterLottieAnimation() must be always run if "registerLottieAnimation" was used before PWLottieItem delted.
///
/// \param controllerType - Controller type, that will register lottie item in it's own system.
/// \param lottieUuid- Unique lottie item ID with that ID lottie item will be unregistered.
///
void PWControllerMediator::unregisterLottieAnimation(const ControllerType controllerType, const QString& lottieUuid)
{
    /* Remove lottie with it's UUID from from controller */
    if (controllerType == PWControllerMediator::ControllerType::BaseController) {
        m_lottieBasicController.removeLottieItem(lottieUuid);
    } else if (controllerType == PWControllerMediator::ControllerType::IconController) {
        m_lottieIconController.removeLottieItem(lottieUuid);
    }
}
