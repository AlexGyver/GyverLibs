#include "GyverRGB.h"
#include <Arduino.h>

GRGB::GRGB(uint8_t r, uint8_t g, uint8_t b) {
	_r = r;
	_g = g;
	_b = b;
}

void GRGB::init() {
	pinMode(_r, OUTPUT);
	pinMode(_g, OUTPUT);
	pinMode(_b, OUTPUT);
	
	analogWrite(_r, 0);
	analogWrite(_g, 0);
	analogWrite(_b, 0);
}

void GRGB::reverse(boolean rev) {
	_reverse_flag = rev;
}

void GRGB::setColor(uint32_t color) {
	byte r = (color >> 16) & 0xff;
	byte g = (color >> 8) & 0xff;
	byte b = color & 0xff;
	setRGB(r, g, b);
}

void GRGB::setRGB(uint8_t R, uint8_t G, uint8_t B) {
	if (_reverse_flag) {
		analogWrite(_r, 255-R);
		analogWrite(_g, 255-G);
		analogWrite(_b, 255-B);
	} else {
		analogWrite(_r, R);
		analogWrite(_g, G);
		analogWrite(_b, B);
	}	
}

void GRGB::setHSV_fast(uint8_t hue, uint8_t sat, uint8_t val) {
	byte h = ((24 * hue / 17) / 60) % 6;
	byte vmin = (long)val - val * sat / 255;
	byte a = (long)val * sat / 255 * (hue * 24 / 17 % 60) / 60;
	byte vinc = vmin + a;
	byte vdec = val - a;
	byte r, g, b;
	switch (h) {
		case 0: r = val; g = vinc; b = vmin; break;
		case 1: r = vdec; g = val; b = vmin; break;
		case 2: r = vmin; g = val; b = vinc; break;
		case 3: r = vmin; g = vdec; b = val; break;
		case 4: r = vinc; g = vmin; b = val; break;
		case 5: r = val; g = vmin; b = vdec; break;
	}	
	setRGB(r, g, b);
}


void GRGB::setHSV(uint8_t h, uint8_t s, uint8_t v) {
	float r, g, b;
	
	float H = (float)h / 255;
	float S = (float)s / 255;
	float V = (float)v / 255;
	
    int i = int(H * 6);
    float f = H * 6 - i;
    float p = V * (1 - S);
    float q = V * (1 - f * S);
    float t = V * (1 - (1 - f) * S);

    switch(i % 6){
        case 0: r = V, g = t, b = p; break;
        case 1: r = q, g = V, b = p; break;
        case 2: r = p, g = V, b = t; break;
        case 3: r = p, g = q, b = V; break;
        case 4: r = t, g = p, b = V; break;
        case 5: r = V, g = p, b = q; break;
    }	
	setRGB(r * 255, g * 255, b * 255);
}