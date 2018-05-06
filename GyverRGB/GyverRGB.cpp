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

void GRGB::setRGB(uint8_t R, uint8_t G, uint8_t B) {
	analogWrite(_r, R);
	analogWrite(_g, G);
	analogWrite(_b, B);
}

void GRGB::setHSV_fast(uint8_t h, uint8_t s, uint8_t v) {
	uint8_t r, g, b;
    uint8_t region, remainder, p, q, t;

    if (s == 0)
    {
        r = v;
        g = v;
        b = v;
		setRGB(r * 255, g * 255, b * 255);
        return;
    }

    region = h / 43;
    remainder = (h - (region * 43)) * 6; 

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            r = v; g = t; b = p;
            break;
        case 1:
            r = q; g = v; b = p;
            break;
        case 2:
            r = p; g = v; b = t;
            break;
        case 3:
            r = p; g = q; b = v;
            break;
        case 4:
            r = t; g = p; b = v;
            break;
        default:
            r = v; g = p; b = q;
            break;
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