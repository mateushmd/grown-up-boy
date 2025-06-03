# Grown Up Boy

*A Game Boy emulator built in `C++` for fun and learning.*

This emulator is a personal project focused on learning and experimentation. It is not guaranteed to be the most efficient or the most accurate implementation.


## Building
The project requires `CMake` and `SDL3` to compile.

For faster compilation, consider using `Ninja` as the build system.

### Linux

Run the `build.sh` script an automated build with `Ninja`:

```
./build.sh
```

If you are using the `Nix` package manager, run: 

```
nix-shell
```

in the project's root directory to resolve all dependencies automatically.
