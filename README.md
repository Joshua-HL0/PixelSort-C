# PixelSort-C
A program to sort the pixels of an image by a given function (e.g. luminance) written in C. \
\
Currently the luminance comparison is given to quicksort, but there is also a plain hex comparison and you can implement your own. \
I use the [stb image libraries](https://github.com/nothings/stb) to load and save the images using arrays.\
\
An example of image sorted by luminance is given as the (copyright free) flag of Mozambique.\
\
Finally, remember to -lm when compiling, e.g. `gcc -o out main.c -lm`
