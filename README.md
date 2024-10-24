# AudioTool

This is a simple tool to inspect and change the volume of audio devices (speakers and microphones) on Windows.

It was developed and tested on Windows 11 but the APIs used are for Windows Vista or newer, so it should work for Windows Vista and versions after that.

# Usage

## Listing Devices with their Volumes

```cmd
> audiotool show-volumes
```

It will show the friendly name of the devices followed by their current volume level.

## Changing the Volume of a Device


```cmd
> audiotool set-volume <device-name-fragment> <level>
```

`device-name-fragment` is a part of the name of the device. If there are multiple devices found the program will set the volume of the first one.

`level` is the desired volume level. It's a decimal number in the range [0, 1], 0 being mute and 1 being 100% volume.

For example, if you have a Logitech C920 Webcam in your system you can call
```cmd
> audiotool set-volume C920 0.5
```
to set the volume of the webcam microphone to 50% the max level.

Use the `show-volumes` command to see the names of the devices.

# Building

## Dependencies

- Visual C++ 2022 (You can install the community edition for free).

### Configuring your Terminal

- Open Visual Studio 2022 and create a console or windows application (C++ or C#).
- If the integrated terminal is showing as "Developer Powershell" create a new one choosing "Developer Command Prompt"
- In the Developer command prompt type in:
    - `echo %PATH%`. Then take all entries related to Visual C++ (they will contain MSVC or VC in the path) and copy to your system's PATH variable. (I'm assuming you know what you're doing when dealing with the PATH variable.)
    - Also echo the variables `%INCLUDE%`, `%LIB%` and `%LIBPATH%`. And create these variables in your system path with the contents obtained from the respective echo. If you already have any of these variables take care to merge the contents properly.

If you need more information on the developer command prompt check here: https://learn.microsoft.com/en-us/visualstudio/ide/reference/command-prompt-powershell?view=vs-2022

After the steps above you can open a new terminal (cmd, not powershell) and check if you can build a simple hello world program written in C++ (`cl /EHsc helloworld.cpp`).

## Building

Just run `build.bat`

## Installing

The project comes with a deploy script (`deploy.bat`) which will copy the generated executable to `%HOME%\bin`. It is assuming that `%HOME%\bin` is in your path. So if it is not, please go ahead and add it.