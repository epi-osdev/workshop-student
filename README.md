# OS workshops

## Cross Compiler

To compile the OS for the CPU, we need a cross compiler, a cross linker, etc.. It allows us to compile for another target than the OS.

We have made a Docker image, to build and run it, run the next instructions:

#### Docker
```bash
docker build -t osdev .
```

If you want to run qemu in graphical mode from the container (Forwarding X socket),

```
docker run -it -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -v $PWD:/osdev osdev
```

if you just want to compile,

```bash
docker run -it -v $PWD:/osdev osdev
```

#### From sources
You can build it by hand from this source https://wiki.osdev.org/GCC_Cross-Compiler.

## Documentation

To read the documentaion [**Doc**](doc/DOCUMENTATION.md).