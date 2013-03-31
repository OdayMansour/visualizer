#ifndef EFFECTS_H
#define EFFECTS_H

#include "common.h"

void analyzeSpectrumSpiral(std::vector<float> specL, std::vector<float> specR);
void analyzeSpectrumVox(std::vector<float> specL, std::vector<float> specR);
void analyzeSpectrumLoop(std::vector<float> specL, std::vector<float> specR);
void analyzeSpectrumFountain(std::vector<float> specL, std::vector<float> specR);
void analyzeSpectrumShadows(std::vector<float> specL, std::vector<float> specR);
void analyzeSpectrumSphere(std::vector<float> specL, std::vector<float> specR);

#endif //EFFECTS_H