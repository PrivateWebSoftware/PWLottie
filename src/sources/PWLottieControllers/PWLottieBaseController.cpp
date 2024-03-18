/*
 * Copyright (C) PrivateWeb Software (https://github.com/PrivateWebSoftware) - All Rights Reserved
 *
 * Licensed under the Apache License 2.0 (the "License"). You may not use
 * this file except in compliance with the License. You can obtain a copy
 * in the file LICENSE in the source distribution
 *
 * Written by PrivateWeb Software <privatewebsoftware@protonmail.com>, July 2024
 */

#include "include/PWLottieControllers/PWLottieBaseController.h"

#include <QDebug>

PWLottieBaseController::PWLottieBaseController(QObject* parent)
    : PWLottieAbstractController { parent }
{
}

///
/// \brief PWLottieBaseController::addLottieItem - Function adds lottie item to controller.
/// \param lottieUuid - Unique lottie UUID for it's controlling.
/// \return Returns current fps of registred lotti animation.
///
quint16 PWLottieBaseController::addLottieItem(const QString& lottieUuid)
{
    lottieItemsList.insert(lottieUuid);

    /* Set up needed frame rate */
    if (m_currentFps != getRecommendedFrameRate()) {
        setLottieItemFrameRate();
    }

    return getRecommendedFrameRate();
}

///
/// \brief PWLottieBaseController::removeLottieItem - Function removes lottie item from controller.
/// \param lottieUuid - Unique lottie UUID for it's controlling.
///
void PWLottieBaseController::removeLottieItem(const QString& lottieUuid)
{
    lottieItemsList.remove(lottieUuid);

    /* Set up needed frame rate */
    if (m_currentFps != getRecommendedFrameRate()) {
        setLottieItemFrameRate();
    }
}

///
/// \brief setLottieItemFps - Set ups and changes needed framerate.
///
void PWLottieBaseController::setLottieItemFrameRate()
{
    m_currentFps = getRecommendedFrameRate();

    /* Change Frame Rate in Lottie items */
    emit fpsChanged(m_currentFps);
}
