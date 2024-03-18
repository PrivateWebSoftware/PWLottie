/*
 * Copyright (C) PrivateWeb Software (https://github.com/PrivateWebSoftware) - All Rights Reserved
 *
 * Licensed under the Apache License 2.0 (the "License"). You may not use
 * this file except in compliance with the License. You can obtain a copy
 * in the file LICENSE in the source distribution
 *
 * Written by PrivateWeb Software <privatewebsoftware@protonmail.com>, July 2024
 */

#ifndef PWLOTTIEABSTRACTCONTROLLER_H
#define PWLOTTIEABSTRACTCONTROLLER_H

#include <QObject>
#include <QSet>
#include <QString>

///
/// \brief The PWLottieAbstractController class - Abstract Controller for all other controllers.
///
class PWLottieAbstractController : public QObject {
    Q_OBJECT

#define allLottiesDefiner "all"

public:
    explicit PWLottieAbstractController(QObject* parent = nullptr) { }

    /*************/
    /* Functions */
    /*************/

    ///
    /// \brief addLottieItem - Function adds lottie item to controller.
    /// \param lottieUuid - Unique lottie UUID for it's controlling.
    /// \return Returns current fps of registred lotti animation.
    ///
    virtual quint16 addLottieItem(const QString& lottieUuid) = 0;

    ///
    /// \brief removeLottieItem - Function removes lottie item from controller.
    /// \param lottieUuid - Unique lottie UUID for it's controlling.
    ///
    virtual void removeLottieItem(const QString& lottieUuid) = 0;

    ///
    /// \brief getTotalLottieCount - Function gets count of all registred lottie animations in controller.
    /// \return Returns count of all lottie aniamtions registred in lottieItemsList.
    ///
    [[nodiscard]] inline qsizetype getTotalLottieCount()
    {
        return lottieItemsList.count();
    }

protected:
    QSet<QString> lottieItemsList = {};
};

#endif // PWLOTTIEABSTRACTCONTROLLER_H
