/*
 * Copyright (C) PrivateWeb Software (https://github.com/PrivateWebSoftware) - All Rights Reserved
 *
 * Licensed under the Apache License 2.0 (the "License"). You may not use
 * this file except in compliance with the License. You can obtain a copy
 * in the file LICENSE in the source distribution
 *
 * Written by PrivateWeb Software <privatewebsoftware@protonmail.com>, July 2024
 */

#ifndef PWLOTTIEBASECONTROLLER_H
#define PWLOTTIEBASECONTROLLER_H

#include <QObject>
#include <QPointer>
#include <QSet>

#include "include/PWLottieControllers/PWLottieAbstractController.h"

///
/// \brief The PWLottieBaseController class - Base controllers that gives base logic for lotties.
///
class PWLottieBaseController : public PWLottieAbstractController {
    Q_OBJECT

#define allLottiesDefiner "all"

public:
    PWLottieBaseController() { }
    explicit PWLottieBaseController(QObject* parent);

    /*************/
    /* Functions */
    /*************/

    ///
    /// \brief addLottieItem - Function adds lottie item to controller.
    /// \param lottieUuid - Unique lottie UUID for it's controlling.
    /// \return Returns current fps of registred lotti animation.
    ///
    quint16 addLottieItem(const QString& lottieUuid) override;

    ///
    /// \brief removeLottieItem - Function removes lottie item from controller.
    /// \param lottieUuid - Unique lottie UUID for it's controlling.
    ///
    void removeLottieItem(const QString& lottieUuid) override;

signals:
    void fpsChanged(const quint16 fps, const QString& lottieUuid = allLottiesDefiner);

protected:
    /*************/
    /* Functions */
    /*************/

    ///
    /// \brief getRecommendedFps - Function returns recommended fps for all lottie items in controller.
    /// \return Returns recommended frame rate.
    ///
    [[nodiscard]] quint16 getRecommendedFrameRate()
    {
        if (getTotalLottieCount() >= largeAmountOfLottieItems) {
            return largeAmountOfLottieItemsFrameRate;
        } else if (getTotalLottieCount() >= mediumAmountOfLottieItems) {
            return mediumAmountOfLottieItemsFrameRate;
        } else if (getTotalLottieCount() >= smallAmountOfLottieItems) {
            return smallAmountOfLottieItemsFrameRate;
        }

        return recommendedLottieItemsFrameRate;
    }

    ///
    /// \brief setLottieItemFps - Set ups and changes needed framerate.
    ///
    void setLottieItemFrameRate();

    /*************/
    /* Variables */
    /*************/

    ///
    /// NOTE: For other platforms that do not have much processing power,
    ///       such as Android, IOS and e.t.c, we must set lower values here
    ///       or write more complex logic to calculate the required FPS.
    ///
    /// It all depends on the specific tasks that the controller must perform.
    ///

    const quint16 smallAmountOfLottieItems = 20;
    const quint16 mediumAmountOfLottieItems = 35;
    const quint16 largeAmountOfLottieItems = 50;

    const quint16 smallAmountOfLottieItemsFrameRate = 45;
    const quint16 mediumAmountOfLottieItemsFrameRate = 30;
    const quint16 largeAmountOfLottieItemsFrameRate = 15;

    const quint16 recommendedLottieItemsFrameRate = 60;

private:
    quint16 m_currentFps = recommendedLottieItemsFrameRate;
};

#endif // PWLOTTIEBASECONTROLLER_H
