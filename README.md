# WindRider-Fitted Firmware

Coming soon...

To learn more about project structure check out this [article](https://stansotn.com/embedded_environment/).

## Build
Make sure you have [arm-none-eabi](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) installed.
```shell
git clone --recursive https://github.com/stansotn/windrider.git
cd windrider
```

Specify arm-none-eabi toolchain path in [here](https://github.com/stansotn/windrider/blob/b504cc1aba64b46fe6b5661b652663123e7398de/yaccs-user-config.cmake#L4).
```
set(yaccs_compiler_paths /Applications/ARM/bin/)
```

Build.
```
mkdir build
cd build
cmake ..
make
```
