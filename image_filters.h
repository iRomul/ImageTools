#pragma once
#include <stdint.h>
#include "short_bmp.h"

void filter_binary_threshold(Image*, uint8_t /*Threshold*/);

void filter_gauss_blur(Image*, double /*Radius*/);

void convert_BGR_to_grayscale(Image*);