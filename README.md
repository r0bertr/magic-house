# Magic House

A CG Project by OpenGL

## Usage

### Prerequisites

- `GLFW3` Library

- `GLM` 0.9.8-5

### Makefile

You can use `make` to build and run.

```bash
$ git clone https://github.com/HinanawiTenshi/magic-house.git
$ cd magic-house
$ make
```

Other `make` commands are:

- `clean` clean

- `build` build

- `all` clean, build and run

- `rebuild` clean and build

### Visual Studio (Beta)

Current version is only well tested in the cutting-edge version of Visual Studio 2017. Problems may occurred under various versions of Windows and Visual Studios.

Open `magic-house.sln` by Visual Studio and it may load the project.

You need to make sure that `GLFW` and `GLM` are **SYSTEM** libraries. (which means `#include <...>` but not `$include "..."`) 

## For Contributors

You **SHOULDN'T** commit a change on source codes only in order to make it compatible with your OpenGL enviroment.

If you are not sure a change is suitable or not, make a new branch and create pull requests is always a good choice.

Implementation as well as design details are avaliable in `docs`.
