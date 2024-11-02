# Git User Tracker

## Overview

This program provides a visual indicator in the system tray that displays the currently active Git user. It monitors the `user.name` configuration and updates the indicator icon accordingly.

## Functionality

* Generates colored icons based on the configured colors.
* Displays the icon corresponding to the current Git user in the system tray.

## Dependencies

* GTK+ 3
* libayatana-appindicator
* jsoncpp
* cairo

## Build Instructions

This program utilizes a standard CMake build system.

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

Run the compiled executable `git-user-tracker` from the build directory.

**Configuration:**

The program reads user color configurations from a JSON file located at `~/.usercolorconfig.json`. The file should follow the following format:

```json
{
  "alice": "#FF0000",
  "bob": "#00FF00",
  "eve": "#0000FF"
}
```

## License

This program is released under the MIT license.