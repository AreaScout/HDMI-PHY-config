# HDMI-PHY-config
HDMI signal strength control parameter tool

![VU5A](https://www.hardkernel.com/main/_Files/prdt/2018/201805/201805080534386792.jpg)

```
odroid@odroid:~$ ./hdmi_signal -h
Usage: ./hdmi_signal [-h] [--help] this screen
        [-A]  TMDS data TX_AMP_LVL amplitude control, default is 17
        [-C0] TMDS data TX_LVL_CH0 amplitude fine control for each channel, Default is 1
        [-C1] TMDS data TX_LVL_CH1 amplitude fine control for each channel, Default is 0
        [-C2] TMDS data TX_LVL_CH2 amplitude fine control for each channel, Default is 2
        [-E]  TMDS data TX_EMP_LVL pre-emphasis level control, default is 6
        [-CA] TMDS data TX_CLK_LVL clock amplitude control, 0 - 31, default is 12
        [-R]  TMDS data TX_RES source termination resistor control, 0 - 3, default is 0 OFF
        ========================================
                 HDMI PHY TUNE INFO
        ========================================
        TX_AMP_LVL [17] (760 mVdiff ~ 1380 mVdiff) = 1100 mVdiff
        TX_LVL_CH0 [1] (0 mVdiff ~ 60 mVdiff) = 20 mVdiff
        TX_LVL_CH1 [0] (0 mVdiff ~ 60 mVdiff) = 0 mVdiff
        TX_LVL_CH2 [2] (0 mVdiff ~ 60 mVdiff) = 40 mVdiff
        TX_EMP_LVL [0] (0 db ~ -7.45 db) = 0 db
        TX_CLK_LVL [12] (790 mVdiff ~ 1410 mVdiff) = 1030 mVdiff
        TX_RES [0] = Source Termination OFF
        ========================================
```
