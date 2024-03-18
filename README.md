# PWLottie

PWLottie is a QML Lottie animation player based on Samsung [rlottie](https://github.com/Samsung/rlottie) library and [Qt](https://www.qt.io/) framework.


PWLottie provides additional functional for creating controllers to support optimiziation of a big amount of lottie animations loaded at the same time. This feature can support optimization in application but it is extremely important to remember to optimize the user interface in other aspects.

The project presents two examples, the first example is a basic example of a controller that stores the number of registered lottie animations and sets their FPS depending on their number and the second just empty example that inherited from abstract controller.

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

Properties that can be accessed with PWLottieItem:

`running` - Property determines whether the lottie is running.
`frameRate` - Current framerate of lottie animation. Not recommended to set it after initializing value when using controllers. Default: `60`. 
`loops` - Loops of lottie animation. `0` value for infinite loop. Default: `0`
`duration` - Duration of lottie animation that rlottie sets.
`sourceSize` - Source size of lottie animation. Important to set it with the default values: `width`, `height`. Property determines in wich resolution will the image be rendered in.
`source` - Source image. Avoid `qrc` and `file:/`, when setting this value.
`controller` - Controller that will be used for controlling animation. By default: `NoController`.

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
