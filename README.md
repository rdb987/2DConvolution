# 2DConvolution
2D Parallel Convolution with MPI, Openmp and Hybrid strategy

Convolution is a common operation in image processing. An image can be considered as a
bidimensional matrix that associates to each pixel (x,y) a corresponding luminosity value for
the original image. Convolution consists in modifying an original input matrix by means of a
kernel, generally of smaller dimensions. Given a pixel (x,y), so called central pixel, of the image
matrix, the convolution consists in centering the kernel matrix on this value. Its value and the
neighbors values are multiplied for the corresponding values in the kernel. Results are sum up
and the nal sum is saved in the original (x,y) initial coordinates.
