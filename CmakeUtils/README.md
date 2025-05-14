# CMakeUtils

Small utility functions for CMake.

**NB:** they require at least CMake version 3.20.

Example usage:

```cmake
include("CMakeUtils/files_and_folders.cmake")
Cool__target_copy_folder(${PROJECT_NAME} "my_assets")
```

## files_and_folders

### Cool__target_copy_folder(TARGET FOLDER OUT_DIR?)

Unlike the default CMake functions this one will re-copy the files of the folder whenever they change or a file is added

Copies FOLDER and all its files to the directory where the executable of your TARGET will be created (a.k.a. the output directory of your target).<br/>
FOLDER can be either an absolute or a relative path. If it is relative it will be relative to CMAKE_SOURCE_DIR (a.k.a. the top-level CMakeLists.txt).<br/>
You can optionally specify a third argument to control the output directory, relative to the output directory of your TARGET.

```cmake
Cool__target_copy_folder(${PROJECT_NAME} "my_assets") # Copies "my_assets" to "my_assets"
```

```cmake
Cool__target_copy_folder(${PROJECT_NAME} "my_assets" "out") # Copies "my_assets" to "out"
```

### Cool__target_copy_file(TARGET FILE OUT_DIR?)

Unlike the default CMake functions this one will re-copy the file whenever it changes.

Copies FILE to the directory where the executable of your TARGET will be created (a.k.a. the output directory of your target).<br/>
FILE can be either an absolute or a relative path. If it is relative it will be relative to CMAKE_SOURCE_DIR (a.k.a. the top-level CMakeLists.txt).<br/>
You can optionally specify a third argument to control the output directory and file name, relative to the output directory of your TARGET.

```cmake
Cool__target_copy_file(${PROJECT_NAME} "my_config.json")
```

```cmake
Cool__target_copy_file(${PROJECT_NAME} "my_config.json" "out/some_config.json")
```

### Cool__create_file_if_it_doesnt_exist(FILE)

Creates FILE if it doesn't already exist.

```cmake
Cool__create_file_if_it_doesnt_exist(${CMAKE_SOURCE_DIR}/imgui.ini)
```

This can be useful in some cases to make sure a file exists before trying to copy it with `Cool__target_copy_file`, because that command will crash if the file doesn't exist.

## visual_studio_helpers

Functions that are specific to Visual Studio (NB: not Visual Studio Code).

### vs_hide_all_build_and_zero_check()

CMake for Visual Studio creates a few helper targets that you don't really want to see: you can hide them with this function.

```cmake
Cool__vs_hide_all_build_and_zero_check()
```

### vs_register_files(FILES)

Recreates your folder architecture inside the Visual Studio solution.

```cmake
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS src/*)
Cool__vs_register_files(${MY_SOURCES})
```
