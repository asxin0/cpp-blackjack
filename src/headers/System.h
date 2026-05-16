#pragma once

void LoadBlackjackFile(int& chips, int& lastDailyCheck);
void SaveBlackjackFile(int chips, int lastDailyCheck);
int CurrentTime();
void DailyBlackjackReward(int chips, int lastDailyCheck);