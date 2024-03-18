/*
 * Copyright (C) PrivateWeb Software (https://github.com/PrivateWebSoftware) - All Rights Reserved
 *
 * Licensed under the Apache License 2.0 (the "License"). You may not use
 * this file except in compliance with the License. You can obtain a copy
 * in the file LICENSE in the source distribution
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "include/PWLottieItem/PWLottieItem.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    /* Initialize PWLottieControllers if we need them */
    initializePWLottieControllers;

    engine.loadFromModule("PWLottieExample", "Main");

    return app.exec();
}
