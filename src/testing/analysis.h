#pragma once

void analysis_start(float target);
void analysis_analyse(float& timeToMax, float& period, float& amplitude);
bool analysis_update(float time, float value);
