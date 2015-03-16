Asteroids
=========
Asteroids clone written in C11 in so far as MSVC 2013 supports C11

Dependencies
------------

- SDL2
- SDL2_image
- SDL2_ttf

Building
-------

### Windows

On windows there are two options:

**Visual Studio 2013**

Although MSVC has not supported most C99/11 features more recently due to the need to support some of these features for C++11/14 compliance we have a lot more to work with.  Theoretically all the external dependencies are included and setup so all
you should have to do is open up asteroids.sln and it'll compile and run.

**mingw-w64**

Setting up the mingw-w64 toolchain is a bit more annoying.

mingw-w64 versions of the dependencies will have to be installed on your system.  Currently the Makefile expects them to be in C:/dev/lib/x86.  The include directory is expected to be C:/dev/include.

This should probably change so that the mingw-w64 libs are in the External/ directory in the project.

Assuming git and the mingw-w64 binaries are in your PATH the whole process should look like this once you get the dependencies setup.

```
git clone https://github.com/tedajax/asteroids.git
cd asteroids
make
./asteroids
```

The make file should automatically copy necessary .dlls into the runtime directory but in case it doesn't just run copy_dll_32.bat.

### Linux

Install dependencies with your distro's package manager.

This has only been tested on Arch so far.

```
sudo pacman -S sdl2 sdl2_image sdl2_ttf sdl2_gfx
```

But I'm sure you can figure out how to install those packages on your distro.

Cloning, building, and running is then all very simple.

```
git clone https://github.com/tedajax/asteroids.git
cd asteroids
make
./asteroids
```

### OSX

I haven't tested this at all but it should work.  Same instructions as the **Linux** section but you'll have to use something like brew to install the dependencies

```
brew install sdl2 sdl2_image sdl2_ttf sdl2_gfx
```

License
-------

Currently licensed under the MIT license (see LICENSE.md)
