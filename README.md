# Video mosaic in C99 - LINUX

This is just a simple project for reading raw RGB24 pixel data from ffmpeg and displaying it in the screen, arranged in set layouts (1x1, 2x2 or 3x3) given the information in the configuration file(config.toml), see config.toml.template for reference.

To run the application without optimazation;

```make
mkdir bin && make
```

Optimized, you'll need to build;

```make
mkdir bin && make release && ./bin/mosaic
```

Make sure to have ffmpeg avaliable in your shell, ether by installing it or in your "envirioment". GCC also.

Any other dependency will be from Raylib, since I'm using it to display the pixels on screen. But raylib is compiled statically here.

# Screeshot of running application in a 3x3 layout

![3x3 mosaic layout](/mosaic_view.png?raw=true '3x3 mosaic layout')
