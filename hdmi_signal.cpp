/*
 * Original Copyright (C) 2011 Samsung Electronics Co.Ltd
 * Authors:
 * Seung-Woo Kim <sw0312.kim@samsung.com>
 * Inki Dae <inki.dae@samsung.com>
 * Joonyoung Shim <jy0922.shim@samsung.com>
 *
 * Tool Copyright (C) 2018 Daniel Mehrwald
 * Based on HARDKERNEL 3.10 linux/drivers/gpu/drm/exynos/exynos_hdmi.c
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 */

#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string>
#include <sstream>

// Original PHY config buffer
unsigned char buf_orig[32] = {
    0x01, 0x51, 0x28, 0x55, 0x44, 0x40, 0x00, 0xC8,
    0x02, 0xC8, 0xF0, 0xD8, 0x45, 0xA0, 0xAC, 0x80,
    0x08, 0x80, 0x09, 0x84, 0x05, 0x02, 0x24, 0x86,
    0x54, 0x80, 0x25, 0x01, 0x00, 0x00, 0x01, 0x80,
};

// PHY config buffer
unsigned char buf[32] = {
    0x01, 0x51, 0x2D, 0x55, 0x40, 0x40, 0x00, 0xC8,
    0x02, 0xC8, 0x0E, 0xD9, 0x45, 0xA0, 0xAC, 0x80,
    0x08, 0x80, 0x09, 0x84, 0x05, 0x02, 0x24, 0x66,
    0x54, 0xAB, 0x24, 0x00, 0x00, 0x00, 0x01, 0x80,
};

unsigned int gTxAmpLevel  = 17;  // Default setup
unsigned int gTxLevelCh0  =  1;  // Default setup
unsigned int gTxLevelCh1  =  0;  // Default setup
unsigned int gTxLevelCh2  =  2;  // Default setup
unsigned int gTxEmpLevel  =  0;  // Default setup
unsigned int gClkAmpLevel = 16;  // Default setup
unsigned int gTxRes       =  0;  // Default setup

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** it = std::find(begin, end, option);
    if(it != end && ++it != end) {
        return *it;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

void hdmi_phy_tune_info(unsigned char *buffer)
{
    unsigned char  value;

    printf("\t========================================\n");
    printf("\t         HDMI PHY TUNE INFO\n");
    printf("\t========================================\n");
    value = buffer[16] & 0x0F;  value <<= 1;
    value |= (buffer[15] & 0x80) ? 0x01 : 0x00;

    printf("\tTX_AMP_LVL [%d] (760 mVdiff ~ 1380 mVdiff) = %d mVdiff\n",
        value, 760 + (unsigned short)value * 20);

    value = (buffer[4] & 0xC0) >> 6;
    printf("\tTX_LVL_CH0 [%d] (0 mVdiff ~ 60 mVdiff) = %d mVdiff\n",
        value, value * 20);

    value = buffer[19] & 0x03;
    printf("\tTX_LVL_CH1 [%d] (0 mVdiff ~ 60 mVdiff) = %d mVdiff\n",
        value, value * 20);

    value = buffer[23] & 0x03;
    printf("\tTX_LVL_CH2 [%d] (0 mVdiff ~ 60 mVdiff) = %d mVdiff\n",
        value, value * 20);

    value = (buffer[16] & 0xF0) >> 4;
    printf("\tTX_EMP_LVL [%d] (0 db ~ -7.45 db) = ", value);

    if(value == 1)  printf("-0.25 db\n");
    else    {
        if(value)   printf("-%d.%02d db\n",
            (25 + 45 * (unsigned short)(value -1)) / 100,
            (25 + 45 * (unsigned short)(value -1)) % 100);
        else        printf("0 db\n");
    }

    value = (buffer[23] & 0xF8) >> 3;
    printf("\tTX_CLK_LVL [%d] (790 mVdiff ~ 1410 mVdiff) = %d mVdiff\n",
        value, 790 + (unsigned short)value * 20);

    value = (buffer[15] & 0x30) >> 4;
    printf("\tTX_RES [%d] = ", value);
    switch(value)   {
        case    0:
        default :   printf("Source Termination OFF\n");
            break;
        case    1:  printf("200 ohm\n");
            break;
        case    2:  printf("300 ohm\n");
            break;
        case    3:  printf("120 ohm\n");
            break;
    }
    printf("\t========================================\n");
}

void hdmi_phy_tune(unsigned char *buffer)
{
    // TxAmpLevel Control
    buffer[16] &= (~0x0F);    buffer[15] &= (~0x80);

    buffer[16] |= (gTxAmpLevel >> 1) & 0x0F;
    buffer[15] |= (gTxAmpLevel & 0x01) ? 0x80 : 0x00;

    // TxLevel Control 0
    buffer[4] &= (~0xC0);
    buffer[4] |= (gTxLevelCh0 << 6) & 0xC0;

    // TxLevel Control 1
    buffer[19] &= (~0x03);
    buffer[19] |= (gTxLevelCh1 & 0x03);

    // TxLevel Control 2
    buffer[23] &= (~0x03);
    buffer[23] |= (gTxLevelCh2 & 0x03);

    // TxEmpLevel Control
    buffer[16] &= (~0xF0);
    buffer[16] |= (gTxEmpLevel << 4) & 0xF0;

    //ClkAmpLevel Control
    buffer[23] &= (~0xF8);
    buffer[23] |= (gClkAmpLevel << 3) & 0xF8;

    // TxRes Control
    buffer[15] &= (~0x30);
    buffer[15] |= (gTxRes << 4) & 0x30;
}

int main (int argc, char* argv[])
{
    const char *opt = nullptr;

    if(cmdOptionExists(argv, argv+argc, "-h") || cmdOptionExists(argv, argv+argc, "--help") || argc < 2) {
        printf("Usage: %s [-h] [--help] this screen\n", argv[0]);
        printf("\t[-A]  TMDS data TX_AMP_LVL amplitude control, default is 17\n");
        printf("\t[-C0] TMDS data TX_LVL_CH0 amplitude fine control for each channel, Default is 1\n");
        printf("\t[-C1] TMDS data TX_LVL_CH1 amplitude fine control for each channel, Default is 0\n");
        printf("\t[-C2] TMDS data TX_LVL_CH2 amplitude fine control for each channel, Default is 2\n");
        printf("\t[-E]  TMDS data TX_EMP_LVL pre-emphasis level control, default is 6\n");
        printf("\t[-CA] TMDS data TX_CLK_LVL clock amplitude control, 0 - 31, default is 12\n");
        printf("\t[-R]  TMDS data TX_RES source termination resistor control, 0 - 3, default is 0 OFF\n");
        hdmi_phy_tune_info(buf);
        exit(EXIT_SUCCESS);
    }

    if(opt = getCmdOption(argv, argv+argc, "-A")) {
        if(opt) {
            std::stringstream(std::string(opt)) >> gTxAmpLevel;
        }
    }
    if(opt = getCmdOption(argv, argv+argc, "-C0")) {
        if(opt) {
            std::stringstream(std::string(opt)) >> gTxLevelCh0;
        }
    }
    if(opt = getCmdOption(argv, argv+argc, "-C1")) {
        if(opt) {
            std::stringstream(std::string(opt)) >> gTxLevelCh1;
        }
    }
    if(opt = getCmdOption(argv, argv+argc, "-C2")) {
        if(opt) {
            std::stringstream(std::string(opt)) >> gTxLevelCh2;
        }
    }
    if(opt = getCmdOption(argv, argv+argc, "-E")) {
        if(opt) {
            std::stringstream(std::string(opt)) >> gTxEmpLevel;
        }
    }
    if(opt = getCmdOption(argv, argv+argc, "-CA")) {
        if(opt) {
            std::stringstream(std::string(opt)) >> gClkAmpLevel;
        }
    }
    if(opt = getCmdOption(argv, argv+argc, "-R")) {
        if(opt) {
            std::stringstream(std::string(opt)) >> gTxRes;
        }
    }

    hdmi_phy_tune(buf);
    hdmi_phy_tune_info(buf);
    printf("\n\tHDMI PHY config in memory:\n\t");
    for(int i = 0; i < sizeof(buf); i++) {
        if(i+1 == sizeof(buf)) {
            printf("0x%02X,\n", buf[i]);
            return 0;
        }
        if((i+1) % 8 == 0) {
            printf("0x%02X,\n\t", buf[i]);
        } else {
            printf("0x%02X, ", buf[i]);
        }
    }

    return 0;
}
