/*
 * Copyright (C) PrivateWeb Software (https://github.com/PrivateWebSoftware) - All Rights Reserved
 *
 * Licensed under the Apache License 2.0 (the "License"). You may not use
 * this file except in compliance with the License. You can obtain a copy
 * in the file LICENSE in the source distribution
 *
 * Written by PrivateWeb Software <privatewebsoftware@protonmail.com>, July 2024
 */

#ifndef PWLOTTIEICONCONTROLLER_H
#define PWLOTTIEICONCONTROLLER_H

#include <QObject>
#include <QPointer>
#include <QSet>

#include "include/PWLottieControllers/PWLottieAbstractController.h"

class PWLottieIconController : public PWLottieAbstractController {
    Q_OBJECT

public:
    PWLottieIconController() { }
    explicit PWLottieIconController(QObject* parent);

    ///
    /// \brief addLottieItem - Function adds lottie item to controller.
    /// \param lottieUuid - Unique lottie UUID for it's controlling.
    /// \return Returns current fps of registred lotti animation.
    ///
    quint16 addLottieItem(const QString& lottieUuid) override { return 0; }

    ///
    /// \brief removeLottieItem - Function removes lottie item from controller.
    /// \param lottieUuid - Unique lottie UUID for it's controlling.
    ///
    void removeLottieItem(const QString& lottieUuid) override { }

signals:
    void fpsChanged(const quint16 fps, const QString& lottieUuid = allLottiesDefiner);
};

#endif // PWLOTTIEICONCONTROLLER_H
