# PWLottie

PWLottie is a QML Lottie animation player based on Samsung [rlottie](https://github.com/Samsung/rlottie) library and [Qt](https://www.qt.io/) framework.


PWLottie provides additional functional for creating controllers to support optimiziation of a big amount of lottie animations loaded at the same time. This feature can support optimization in application but it is extremely important to remember to optimize the user interface in other aspects.

The project presents two examples, the first example is a basic example of a controller that stores the number of registered lottie animations and sets their FPS depending on their number and the second just empty example that inherited from abstract controller.


https://github.com/PrivateWebSoftware/PWLottie/assets/66485293/882834d1-bc6b-4425-b7a5-3edff8774946

https://github.com/PrivateWebSoftware/PWLottie/assets/66485293/dc9370bc-21e5-47f5-837b-adbc41cf9282 


In the last example we can see how laggy the interface is without controllers. 

This happens because the application tries to render a lot of animations at once at 60 fps, which is extremely problematic for the interface. 

To fix this, we enable a controller that registers the currently rendering animations and reduces their FPS to the lower fps that we set in controller, after which we can see the smoothness of the interface.

We can use controllers, for example, to improve basic interface optimization, improve optimization on weaker machines when the user has low computing resources, or when the user has low battery power and e.t.c.

## Building PWLottie

Create a build directory for out of source `build`

```sh
mkdir build
```

Run cmake command inside build directory to configure PWLottie.

```sh
cd build

cmake ..
```

Run make to build and install PWLottie libs:

```sh
make -j4
make install
```

## Using PWLottie in QML Project 

To use PWLottie in your QML project you will need to add PWLottie as `subdirectory` in your `CMakeLists.txt`:

```CMake
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/PrivateWeb/PWLottie/include/")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/PrivateWeb/PWLottie)
```

And add PWLottie to `target_link_libraries` in in your `CMakeLists.txt`:

```CMake
target_link_libraries(appPWLottieExample PRIVATE
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Concurrent
    PWLottie
)
```

## Example of using PWLottieItem in qml

To use PWLottieItem you will need to import it in qml:
```qml
import PrivateWeb.PWLottie
```

Example of accessing controller in qml:

```
PWLottieItem {
    id: pwLottieItem

    width: 512
    height: 512

    sourceSize.width: width
    sourceSize.height: height

    source: ":/path/to/lottie.json"

    frameRate: 60 
    loops: 0 
}
```

**Properties that can be accessed with PWLottieItem:**

```
running - Property determines whether the lottie is running.
frameRate - Current framerate of lottie animation. Not recommended to set it after initializing value when using controllers. Default: '60'. 
loops - Loops of lottie animation. '0' value for infinite loop. Default: '0'.
duration - Duration of lottie animation that rlottie sets.
sourceSize - Source size of lottie animation. Important to set it with the default values: 'width', 'height'. Property determines in wich resolution will the image be rendered in.
source - Source image. Avoid 'qrc' and 'file:/', when setting this value.
controller - Controller that will be used for controlling animation. By default: 'NoController'.
```

## Using Controllers in QML Project

To use controllers in QML Project you will need to enable in `main.cpp`.

Firstly, include `PWLottieItem.h` in your project:
```cpp
#include <PWLottieItem.h>
```

Secondly, initialize controllers with macros:
```cpp
/* Initialize PWLottieControllers if we need them */
initializePWLottieControllers;
```

Then in `main.qml` import PWLottieControllers with:
```qml
import PrivateWeb.PWLottie.Controllers
```

Example of accessing controller in qml:

```
PWLottieItem {
    id: pwLottieItem

    width: 512
    height: 512

    sourceSize.width: width
    sourceSize.height: height

    antialiasing: true
    smooth: true

    source: ":/path/to/lottie.json"
    controller: ControllerType.NoController /// NOTE: Here write needed controller.

    frameRate: 60
    loops: 0
}
```

## Writing own Controllers 

PWLottie provides only examples of controllers, if you want to create more complex controllers you will have to write them yourself:

1. You need to create class that inherits from `PWLottieAbstractController`;
2. You need to override `addLottieItem()` and `removeLottieItem()` functions. In this functions you will calculate fps of lottie animation.
3. Write needed functional for your controller, for example, turning off aniamtion if user have low battery perstange or low down fps if controller have too many fps registred.
4. Now we need to intialize functional in `PWControllerMediator`:
    1. In `PWControllerMediator` class you need to add your controller name in `ControllerType` enum.
    2. In `private` section of class add your controller class with `inline static`: `inline static MyController m_myController;`
    3. Now we need to add registration and unregistration in `PWControllerMediator`. In functions `registerLottieAnimation` and `unregisterLottieAnimation` just add:
   ```cpp
   /* For registration */
   else if (controllerType == ControllerType::MyController) {
        return m_myController.addLottieItem(lottieUuid);
   }

   /* For unregistration */
   else if (controllerType == ControllerType::MyController) {
        m_myController.removeLottieItem(lottieUuid);
   }
   ```
   4. If you need to invoke signal in `PWLottieItem` connect your controller signal in `PWControllerMediator` constructor.
   ```cpp
    connect(&m_myController, &MyController::fpsChanged, this, [=](const quint16 fps, const QString& lottieUuid) {
        emit instance()->fpsChanged(fps, PWControllerMediator::MyController, lottieUuid);
    });
   ```
5. And for the finale we need to make some changes to `PWLottieItem`:
    1. In `setController` function just connect signal that we connected in 4.4:
    ```cpp
    connect(PWControllerMediator::instance(), &PWControllerMediator::fpsChanged, this, [=, this](const quint16 fps, const PWControllerMediator::ControllerType controllerType, const QString& lottieUuid) {
        if (lottieUuid == allLottiesDefiner || lottieUuid == m_lottieUuid) {
            setFrameRate(fps);
        }
    });
    ```
6. In the QML item, specify the name of the controller that you specified in enum:
```qml
import PrivateWeb.PWLottie
import PrivateWeb.PWLottie.Controllers

PWLottieItem {
...
    controller: ControllerType.MyController
...
}
``` 
