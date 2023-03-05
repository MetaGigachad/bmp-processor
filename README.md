# BMP Processor

CLI tool which can perform a range of operations on [BMP](http://en.wikipedia.org/wiki/BMP_file_format) image.

> **Warning**: currently only 24bit uncompressed BMPs with no color tables are supported.

## Help message

```
Usage: bmp_processor <input_file> <output_file> [<-filter_name> [filter_params]]
Available filters:
  -crop <width> <height>          Crops image.
  -gs                             Applies grayscale filter.
  -neg                            Applies negative filter.
  -sharp                          Sharpens image.
  -edge <threshold>               Produces grayscale image with white edges.
  -blur <sigma>                   Applies gaussian blur to image.
  -pixelate <res_multiplier>      Reduces image resolution.
```

## How to build

Run following commands in the repo root directory:
```
cmake -G Ninja -S . -B build
ninja -C build
```

App and test binaries will be inside `build` directory.

## Image example

Input image:

<img src="examples/example.bmp" alt="input" width="400"/>

Blur 5 applied:

<img src="examples/blur-5-output.bmp" alt="output" width="400"/>
