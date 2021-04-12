EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 7
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L EDN-symbol:ZVN3306A Q12
U 1 1 5FE6A1FF
P 9450 5400
F 0 "Q12" H 9250 5300 50  0000 L CNN
F 1 "ZVN3306A" V 9700 5200 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92Flat" H 10250 5400 50  0001 C CIN
F 3 "" H 9350 6300 50  0001 C CNN
	1    9450 5400
	1    0    0    -1  
$EndComp
$Comp
L EDN-symbol:ZVN3306A Q11
U 1 1 5F8D9375
P 9150 4900
F 0 "Q11" H 8950 4800 50  0000 L CNN
F 1 "ZVN3306A" V 9400 4700 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92Flat" H 9950 4900 50  0001 C CIN
F 3 "" H 9050 5800 50  0001 C CNN
	1    9150 4900
	1    0    0    -1  
$EndComp
NoConn ~ 9750 4500
NoConn ~ 9750 4700
NoConn ~ 9750 4900
$Comp
L power:GND #PWR0359
U 1 1 5FA90BDA
P 9750 5050
F 0 "#PWR0359" H 9750 4800 50  0001 C CNN
F 1 "GND" H 9755 4877 50  0001 C CNN
F 2 "" H 9750 5050 50  0001 C CNN
F 3 "" H 9750 5050 50  0001 C CNN
	1    9750 5050
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0360
U 1 1 5FE6A209
P 9250 5150
F 0 "#PWR0360" H 9250 4900 50  0001 C CNN
F 1 "GND" H 9255 4977 50  0001 C CNN
F 2 "" H 9250 5150 50  0001 C CNN
F 3 "" H 9250 5150 50  0001 C CNN
	1    9250 5150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0361
U 1 1 5FD660C4
P 9550 5650
F 0 "#PWR0361" H 9550 5400 50  0001 C CNN
F 1 "GND" H 9555 5477 50  0001 C CNN
F 2 "" H 9550 5650 50  0001 C CNN
F 3 "" H 9550 5650 50  0001 C CNN
	1    9550 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 5650 9550 5600
Wire Wire Line
	9250 5150 9250 5100
Wire Wire Line
	9750 5050 9750 5000
Text Label 5000 4800 0    50   ~ 0
Sig_Relay8
Wire Wire Line
	9250 4050 9250 4150
$Comp
L power:GND #PWR0362
U 1 1 5FE6A1E0
P 9250 4150
F 0 "#PWR0362" H 9250 3900 50  0001 C CNN
F 1 "GND" H 9255 3977 50  0001 C CNN
F 2 "" H 9250 4150 50  0001 C CNN
F 3 "" H 9250 4150 50  0001 C CNN
	1    9250 4150
	1    0    0    -1  
$EndComp
$Comp
L EDN-symbol:ZVN3306A Q8
U 1 1 5FE6A1E1
P 9150 3850
F 0 "Q8" H 9000 3750 50  0000 L CNN
F 1 "ZVN3306A" V 9400 3650 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92Flat" H 9950 3850 50  0001 C CIN
F 3 "" H 9050 4750 50  0001 C CNN
	1    9150 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 3650 9250 3600
$Comp
L power:+5VP #PWR0363
U 1 1 5FE6A1E2
P 9250 2850
F 0 "#PWR0363" H 9250 2700 50  0001 C CNN
F 1 "+5VP" H 9265 3023 50  0000 C CNN
F 2 "" H 9250 2850 50  0001 C CNN
F 3 "" H 9250 2850 50  0001 C CNN
	1    9250 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 2950 9250 2900
$Comp
L Diode:1N4148 D8
U 1 1 5FE6A1E3
P 8950 3250
F 0 "D8" V 8850 3300 50  0000 L CNN
F 1 "1N4148" H 8800 3150 50  0000 L CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 8950 3075 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 8950 3250 50  0001 C CNN
	1    8950 3250
	0    1    1    0   
$EndComp
Wire Wire Line
	8950 3400 8950 3600
Wire Wire Line
	8950 3600 9250 3600
Connection ~ 9250 3600
Wire Wire Line
	9250 3600 9250 3550
Wire Wire Line
	8950 3100 8950 2900
Wire Wire Line
	8950 2900 9250 2900
Connection ~ 9250 2900
Wire Wire Line
	9250 2900 9250 2850
$Comp
L EDN-symbol:D2n_V23105 RL8
U 2 1 5FE6A1E4
P 9250 3250
F 0 "RL8" H 9300 3500 50  0000 L CNN
F 1 "D2n_V23105" V 9100 3000 50  0001 L CNN
F 2 "" H 9850 4100 50  0001 C CNN
F 3 "" H 9850 4100 50  0001 C CNN
	2    9250 3250
	1    0    0    -1  
$EndComp
Text Label 5000 4700 0    50   ~ 0
Sig_Relay7
Wire Wire Line
	8350 4050 8350 4150
$Comp
L power:GND #PWR0364
U 1 1 5FE6A1E5
P 8350 4150
F 0 "#PWR0364" H 8350 3900 50  0001 C CNN
F 1 "GND" H 8355 3977 50  0001 C CNN
F 2 "" H 8350 4150 50  0001 C CNN
F 3 "" H 8350 4150 50  0001 C CNN
	1    8350 4150
	1    0    0    -1  
$EndComp
$Comp
L EDN-symbol:ZVN3306A Q7
U 1 1 5FE6A1E6
P 8250 3850
F 0 "Q7" H 8100 3750 50  0000 L CNN
F 1 "ZVN3306A" V 8500 3650 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92Flat" H 9050 3850 50  0001 C CIN
F 3 "" H 8150 4750 50  0001 C CNN
	1    8250 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 3650 8350 3600
$Comp
L power:+5VP #PWR0365
U 1 1 5EAE9EBA
P 8350 2850
F 0 "#PWR0365" H 8350 2700 50  0001 C CNN
F 1 "+5VP" H 8365 3023 50  0000 C CNN
F 2 "" H 8350 2850 50  0001 C CNN
F 3 "" H 8350 2850 50  0001 C CNN
	1    8350 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 2950 8350 2900
$Comp
L Diode:1N4148 D7
U 1 1 5EAE9EC1
P 8050 3250
F 0 "D7" V 7950 3300 50  0000 L CNN
F 1 "1N4148" H 7900 3150 50  0000 L CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 8050 3075 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 8050 3250 50  0001 C CNN
	1    8050 3250
	0    1    1    0   
$EndComp
Wire Wire Line
	8050 3400 8050 3600
Wire Wire Line
	8050 3600 8350 3600
Connection ~ 8350 3600
Wire Wire Line
	8350 3600 8350 3550
Wire Wire Line
	8050 3100 8050 2900
Wire Wire Line
	8050 2900 8350 2900
Connection ~ 8350 2900
Wire Wire Line
	8350 2900 8350 2850
$Comp
L EDN-symbol:D2n_V23105 RL7
U 2 1 5FE6A1E9
P 8350 3250
F 0 "RL7" H 8400 3500 50  0000 L CNN
F 1 "D2n_V23105" V 8200 3000 50  0001 L CNN
F 2 "" H 8950 4100 50  0001 C CNN
F 3 "" H 8950 4100 50  0001 C CNN
	2    8350 3250
	1    0    0    -1  
$EndComp
Text Label 5000 4600 0    50   ~ 0
Sig_Relay6
Wire Wire Line
	7450 4050 7450 4150
$Comp
L power:GND #PWR0366
U 1 1 5EAE9ED8
P 7450 4150
F 0 "#PWR0366" H 7450 3900 50  0001 C CNN
F 1 "GND" H 7455 3977 50  0001 C CNN
F 2 "" H 7450 4150 50  0001 C CNN
F 3 "" H 7450 4150 50  0001 C CNN
	1    7450 4150
	1    0    0    -1  
$EndComp
$Comp
L EDN-symbol:ZVN3306A Q6
U 1 1 5EAE9EDE
P 7350 3850
F 0 "Q6" H 7200 3750 50  0000 L CNN
F 1 "ZVN3306A" V 7600 3650 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92Flat" H 8150 3850 50  0001 C CIN
F 3 "" H 7250 4750 50  0001 C CNN
	1    7350 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 3650 7450 3600
$Comp
L power:+5VP #PWR0367
U 1 1 5FE6A1EC
P 7450 2850
F 0 "#PWR0367" H 7450 2700 50  0001 C CNN
F 1 "+5VP" H 7465 3023 50  0000 C CNN
F 2 "" H 7450 2850 50  0001 C CNN
F 3 "" H 7450 2850 50  0001 C CNN
	1    7450 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 2950 7450 2900
$Comp
L Diode:1N4148 D6
U 1 1 5FE6A1ED
P 7150 3250
F 0 "D6" V 7050 3300 50  0000 L CNN
F 1 "1N4148" H 7000 3150 50  0000 L CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 7150 3075 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 7150 3250 50  0001 C CNN
	1    7150 3250
	0    1    1    0   
$EndComp
Wire Wire Line
	7150 3400 7150 3600
Wire Wire Line
	7150 3600 7450 3600
Connection ~ 7450 3600
Wire Wire Line
	7450 3600 7450 3550
Wire Wire Line
	7150 3100 7150 2900
Wire Wire Line
	7150 2900 7450 2900
Connection ~ 7450 2900
Wire Wire Line
	7450 2900 7450 2850
$Comp
L EDN-symbol:D2n_V23105 RL6
U 2 1 5FE6A1EE
P 7450 3250
F 0 "RL6" H 7500 3500 50  0000 L CNN
F 1 "D2n_V23105" V 7300 3000 50  0001 L CNN
F 2 "" H 8050 4100 50  0001 C CNN
F 3 "" H 8050 4100 50  0001 C CNN
	2    7450 3250
	1    0    0    -1  
$EndComp
Text Label 5000 4500 0    50   ~ 0
Sig_Relay5
Wire Wire Line
	6550 4050 6550 4150
$Comp
L power:GND #PWR0368
U 1 1 5FE6A1EF
P 6550 4150
F 0 "#PWR0368" H 6550 3900 50  0001 C CNN
F 1 "GND" H 6555 3977 50  0001 C CNN
F 2 "" H 6550 4150 50  0001 C CNN
F 3 "" H 6550 4150 50  0001 C CNN
	1    6550 4150
	1    0    0    -1  
$EndComp
$Comp
L EDN-symbol:ZVN3306A Q5
U 1 1 5FE6A1F0
P 6450 3850
F 0 "Q5" H 6300 3750 50  0000 L CNN
F 1 "ZVN3306A" V 6700 3650 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92Flat" H 7250 3850 50  0001 C CIN
F 3 "" H 6350 4750 50  0001 C CNN
	1    6450 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 3650 6550 3600
$Comp
L power:+5VP #PWR0369
U 1 1 5FE6A1F1
P 6550 2850
F 0 "#PWR0369" H 6550 2700 50  0001 C CNN
F 1 "+5VP" H 6565 3023 50  0000 C CNN
F 2 "" H 6550 2850 50  0001 C CNN
F 3 "" H 6550 2850 50  0001 C CNN
	1    6550 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 2950 6550 2900
$Comp
L Diode:1N4148 D5
U 1 1 5FE6A1F2
P 6250 3250
F 0 "D5" V 6150 3300 50  0000 L CNN
F 1 "1N4148" H 6100 3150 50  0000 L CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 6250 3075 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 6250 3250 50  0001 C CNN
	1    6250 3250
	0    1    1    0   
$EndComp
Wire Wire Line
	6250 3400 6250 3600
Wire Wire Line
	6250 3600 6550 3600
Connection ~ 6550 3600
Wire Wire Line
	6550 3600 6550 3550
Wire Wire Line
	6250 3100 6250 2900
Wire Wire Line
	6250 2900 6550 2900
Connection ~ 6550 2900
Wire Wire Line
	6550 2900 6550 2850
$Comp
L EDN-symbol:D2n_V23105 RL5
U 2 1 5FE6A1F3
P 6550 3250
F 0 "RL5" H 6600 3500 50  0000 L CNN
F 1 "D2n_V23105" V 6400 3000 50  0001 L CNN
F 2 "" H 7150 4100 50  0001 C CNN
F 3 "" H 7150 4100 50  0001 C CNN
	2    6550 3250
	1    0    0    -1  
$EndComp
NoConn ~ 5500 1250
NoConn ~ 6400 1250
Wire Wire Line
	5500 1150 5400 1150
$Comp
L EDN-symbol:D2n_V23105 RL5
U 1 1 5F7F1809
P 5950 2000
F 0 "RL5" H 6100 1750 50  0000 L CNN
F 1 "D2n_V23105" H 6150 2250 50  0001 L CNN
F 2 "" H 6550 2850 50  0001 C CNN
F 3 "" H 6550 2850 50  0001 C CNN
	1    5950 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 1450 6400 1600
Wire Wire Line
	5500 1450 5500 1600
Wire Wire Line
	5500 1600 6400 1600
Wire Wire Line
	6400 1350 6500 1350
Wire Wire Line
	6500 1350 6500 1700
Wire Wire Line
	6500 1700 6050 1700
Wire Wire Line
	5500 1350 5400 1350
Wire Wire Line
	5400 1350 5400 1700
Wire Wire Line
	5400 1700 5850 1700
NoConn ~ 6700 1250
NoConn ~ 7600 1250
$Comp
L EDN-symbol:D2n_V23105 RL6
U 1 1 5F7F182F
P 7150 2000
F 0 "RL6" H 7300 1750 50  0000 L CNN
F 1 "D2n_V23105" H 7350 2250 50  0001 L CNN
F 2 "" H 7750 2850 50  0001 C CNN
F 3 "" H 7750 2850 50  0001 C CNN
	1    7150 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 1450 7600 1600
Wire Wire Line
	6700 1450 6700 1600
Wire Wire Line
	6700 1600 7600 1600
Wire Wire Line
	7600 1350 7700 1350
Wire Wire Line
	7700 1350 7700 1700
Wire Wire Line
	7700 1700 7250 1700
Wire Wire Line
	6700 1350 6600 1350
Wire Wire Line
	6600 1350 6600 1700
Wire Wire Line
	6600 1700 7050 1700
Wire Wire Line
	6700 2000 6400 2000
NoConn ~ 7900 1250
NoConn ~ 8800 1250
$Comp
L EDN-symbol:D2n_V23105 RL7
U 1 1 5F7F1856
P 8350 2000
F 0 "RL7" H 8500 1750 50  0000 L CNN
F 1 "D2n_V23105" H 8550 2250 50  0001 L CNN
F 2 "" H 8950 2850 50  0001 C CNN
F 3 "" H 8950 2850 50  0001 C CNN
	1    8350 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8800 1450 8800 1600
Wire Wire Line
	7900 1450 7900 1600
Wire Wire Line
	7900 1600 8800 1600
Wire Wire Line
	8800 1350 8900 1350
Wire Wire Line
	8900 1350 8900 1700
Wire Wire Line
	8900 1700 8450 1700
Wire Wire Line
	7900 1350 7800 1350
Wire Wire Line
	7800 1350 7800 1700
Wire Wire Line
	7800 1700 8250 1700
NoConn ~ 9100 1250
NoConn ~ 10000 1250
$Comp
L EDN-symbol:D2n_V23105 RL8
U 1 1 5F7F187C
P 9550 2000
F 0 "RL8" H 9700 1750 50  0000 L CNN
F 1 "D2n_V23105" H 9750 2250 50  0001 L CNN
F 2 "" H 10150 2850 50  0001 C CNN
F 3 "" H 10150 2850 50  0001 C CNN
	1    9550 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	10000 1450 10000 1600
Wire Wire Line
	9100 1450 9100 1600
Wire Wire Line
	9100 1600 10000 1600
Wire Wire Line
	10000 1350 10100 1350
Wire Wire Line
	10100 1350 10100 1700
Wire Wire Line
	10100 1700 9650 1700
Wire Wire Line
	9100 1350 9000 1350
Wire Wire Line
	9000 1350 9000 1700
Wire Wire Line
	9000 1700 9450 1700
Wire Wire Line
	9100 2000 8800 2000
Wire Wire Line
	7600 2000 7900 2000
Wire Wire Line
	5000 2100 4950 2100
NoConn ~ 4950 1900
Wire Wire Line
	10000 1150 10100 1150
Text Label 5000 4900 0    50   ~ 0
Sig_CTRL_SW3
Text Label 5000 5000 0    50   ~ 0
Sig_CTRL_SW4
$Comp
L Connector:AudioJack2_Switch J5
U 1 1 6017D68C
P 5700 1350
F 0 "J5" H 5750 1650 50  0000 R CNN
F 1 "AudioJack2_Switch" H 5732 1684 50  0001 C CNN
F 2 "" H 5700 1550 50  0001 C CNN
F 3 "~" H 5700 1550 50  0001 C CNN
	1    5700 1350
	-1   0    0    -1  
$EndComp
$Comp
L Connector:AudioJack2_Switch J15
U 1 1 6017D692
P 6200 1350
F 0 "J15" H 6250 1650 50  0000 R CNN
F 1 "AudioJack2_Switch" H 6232 1684 50  0001 C CNN
F 2 "" H 6200 1550 50  0001 C CNN
F 3 "~" H 6200 1550 50  0001 C CNN
	1    6200 1350
	1    0    0    -1  
$EndComp
$Comp
L Connector:AudioJack2_Switch J6
U 1 1 60223A04
P 6900 1350
F 0 "J6" H 6950 1650 50  0000 R CNN
F 1 "AudioJack2_Switch" H 6932 1684 50  0001 C CNN
F 2 "" H 6900 1550 50  0001 C CNN
F 3 "~" H 6900 1550 50  0001 C CNN
	1    6900 1350
	-1   0    0    -1  
$EndComp
$Comp
L Connector:AudioJack2_Switch J16
U 1 1 5FE6A212
P 7400 1350
F 0 "J16" H 7450 1650 50  0000 R CNN
F 1 "AudioJack2_Switch" H 7432 1684 50  0001 C CNN
F 2 "" H 7400 1550 50  0001 C CNN
F 3 "~" H 7400 1550 50  0001 C CNN
	1    7400 1350
	1    0    0    -1  
$EndComp
$Comp
L Connector:AudioJack2_Switch J7
U 1 1 602C9AB2
P 8100 1350
F 0 "J7" H 8150 1650 50  0000 R CNN
F 1 "AudioJack2_Switch" H 8132 1684 50  0001 C CNN
F 2 "" H 8100 1550 50  0001 C CNN
F 3 "~" H 8100 1550 50  0001 C CNN
	1    8100 1350
	-1   0    0    -1  
$EndComp
$Comp
L Connector:AudioJack2_Switch J17
U 1 1 602C9AB8
P 8600 1350
F 0 "J17" H 8650 1650 50  0000 R CNN
F 1 "AudioJack2_Switch" H 8632 1684 50  0001 C CNN
F 2 "" H 8600 1550 50  0001 C CNN
F 3 "~" H 8600 1550 50  0001 C CNN
	1    8600 1350
	1    0    0    -1  
$EndComp
$Comp
L Connector:AudioJack2_Switch J8
U 1 1 5FE6A215
P 9300 1350
F 0 "J8" H 9350 1650 50  0000 R CNN
F 1 "AudioJack2_Switch" H 9332 1684 50  0001 C CNN
F 2 "" H 9300 1550 50  0001 C CNN
F 3 "~" H 9300 1550 50  0001 C CNN
	1    9300 1350
	-1   0    0    -1  
$EndComp
$Comp
L Connector:AudioJack2_Switch J18
U 1 1 5FE6A216
P 9800 1350
F 0 "J18" H 9850 1650 50  0000 R CNN
F 1 "AudioJack2_Switch" H 9832 1684 50  0001 C CNN
F 2 "" H 9800 1550 50  0001 C CNN
F 3 "~" H 9800 1550 50  0001 C CNN
	1    9800 1350
	1    0    0    -1  
$EndComp
$Comp
L Connector:AudioJack3_Switch J20
U 1 1 60741781
P 9950 4800
F 0 "J20" H 9900 4400 50  0000 R CNN
F 1 "SW_CTRL2" H 10050 5100 50  0000 R CNN
F 2 "" H 9950 4800 50  0001 C CNN
F 3 "~" H 9950 4800 50  0001 C CNN
	1    9950 4800
	-1   0    0    1   
$EndComp
$Comp
L Connector:AudioJack2_Switch J19
U 1 1 5FE6A21C
P 4750 2000
F 0 "J19" H 4850 2300 50  0000 R CNN
F 1 "From FX Send" H 4700 1800 50  0000 C CNN
F 2 "" H 4750 2200 50  0001 C CNN
F 3 "~" H 4750 2200 50  0001 C CNN
	1    4750 2000
	1    0    0    -1  
$EndComp
$Comp
L EDN-symbol:302-S201 J41
U 1 1 5FCA31DE
P 4700 4950
F 0 "J41" H 4700 5533 50  0000 C CNN
F 1 "302-S201" H 4700 5534 50  0001 C CNN
F 2 "" H 4900 5400 60  0001 L CNN
F 3 "http://www.on-shore.com/wp-content/uploads/2018/04/302-SXX1.pdf" V 4850 5950 60  0001 L CNN
F 4 "ED10524-ND" H 4850 5750 60  0001 L CNN "Digi-Key_PN"
F 5 "302-S201" H 4850 5850 60  0001 L CNN "MPN"
F 6 "Connectors, Interconnects" H 4850 6000 60  0001 L CNN "Category"
F 7 "Rectangular Connectors - Headers, Male Pins" H 4850 6100 60  0001 L CNN "Family"
F 8 "http://www.on-shore.com/wp-content/uploads/2018/04/302-SXX1.pdf" H 4850 6250 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/da/on-shore-technology-inc/302-S201/ED10524-ND/2794235" H 4850 6350 60  0001 L CNN "DK_Detail_Page"
F 10 "CONN HEADER VERT 20POS 2.54MM" H 4850 6450 60  0001 L CNN "Description"
F 11 "On Shore Technology Inc." H 4850 6550 60  0001 L CNN "Manufacturer"
F 12 "Active" H 4850 6650 60  0001 L CNN "Status"
	1    4700 4950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0370
U 1 1 5FCA5E62
P 5000 5500
F 0 "#PWR0370" H 5000 5250 50  0001 C CNN
F 1 "GND" H 5005 5327 50  0001 C CNN
F 2 "" H 5000 5500 50  0001 C CNN
F 3 "" H 5000 5500 50  0001 C CNN
	1    5000 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 5500 5000 5400
Wire Wire Line
	5000 5400 4900 5400
$Comp
L power:+5VP #PWR0371
U 1 1 5FCB1922
P 4400 4400
F 0 "#PWR0371" H 4400 4250 50  0001 C CNN
F 1 "+5VP" H 4415 4573 50  0000 C CNN
F 2 "" H 4400 4400 50  0001 C CNN
F 3 "" H 4400 4400 50  0001 C CNN
	1    4400 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 4500 4400 4500
Wire Wire Line
	4400 4500 4400 4400
Wire Wire Line
	4500 4600 4400 4600
Wire Wire Line
	4400 4600 4400 4500
Connection ~ 4400 4500
Wire Wire Line
	4900 5300 5000 5300
Wire Wire Line
	5000 5300 5000 5400
Connection ~ 5000 5400
Wire Wire Line
	5650 3850 5650 4500
Wire Wire Line
	5650 4500 4900 4500
Wire Wire Line
	5650 3850 6300 3850
Wire Wire Line
	7000 3850 7000 4600
Wire Wire Line
	7000 4600 4900 4600
Wire Wire Line
	7000 3850 7200 3850
Wire Wire Line
	7900 3850 7900 4700
Wire Wire Line
	7900 4700 4900 4700
Wire Wire Line
	7900 3850 8100 3850
Wire Wire Line
	8800 3850 8800 4800
Wire Wire Line
	8800 4800 4900 4800
Wire Wire Line
	8800 3850 9000 3850
NoConn ~ 4900 5200
NoConn ~ 4900 5100
NoConn ~ 4500 5200
NoConn ~ 4500 5100
NoConn ~ 4500 4800
NoConn ~ 4500 4700
Text Label 5100 2550 0    50   ~ 0
Loop_4_5_Bypass
Text Label 4400 5400 2    50   ~ 0
RX_MIDI
Wire Wire Line
	1450 2300 1350 2300
Wire Wire Line
	1450 2700 1350 2700
$Comp
L power:GND #PWR?
U 1 1 60CA6805
P 1200 3150
AR Path="/5F987406/5F987547/60CA6805" Ref="#PWR?"  Part="1" 
AR Path="/5F987406/5F98763F/60CA6805" Ref="#PWR0372"  Part="1" 
F 0 "#PWR0372" H 1200 2900 50  0001 C CNN
F 1 "GND" H 1205 2977 50  0001 C CNN
F 2 "" H 1200 3150 50  0001 C CNN
F 3 "" H 1200 3150 50  0001 C CNN
	1    1200 3150
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60CA680C
P 1350 3150
AR Path="/5F987406/5F987547/60CA680C" Ref="#PWR?"  Part="1" 
AR Path="/5F987406/5F98763F/60CA680C" Ref="#PWR0373"  Part="1" 
F 0 "#PWR0373" H 1350 2900 50  0001 C CNN
F 1 "GND" H 1355 2977 50  0001 C CNN
F 2 "" H 1350 3150 50  0001 C CNN
F 3 "" H 1350 3150 50  0001 C CNN
	1    1350 3150
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 60CA6814
P 1200 2900
AR Path="/5F987406/5F987547/60CA6814" Ref="R?"  Part="1" 
AR Path="/5F987406/5F98763F/60CA6814" Ref="R135"  Part="1" 
F 0 "R135" H 800 2950 50  0000 L CNN
F 1 "4,7k" H 850 2850 50  0000 L CNN
F 2 "" V 1130 2900 50  0001 C CNN
F 3 "~" H 1200 2900 50  0001 C CNN
	1    1200 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 60CA681A
P 1050 2350
AR Path="/5F987406/5F987547/60CA681A" Ref="R?"  Part="1" 
AR Path="/5F987406/5F98763F/60CA681A" Ref="R134"  Part="1" 
F 0 "R134" H 1100 2400 50  0000 L CNN
F 1 "220" H 1120 2305 50  0000 L CNN
F 2 "" V 980 2350 50  0001 C CNN
F 3 "~" H 1050 2350 50  0001 C CNN
	1    1050 2350
	-1   0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 60CA6820
P 1050 2000
AR Path="/5F987406/5F987547/60CA6820" Ref="#PWR?"  Part="1" 
AR Path="/5F987406/5F98763F/60CA6820" Ref="#PWR0374"  Part="1" 
F 0 "#PWR0374" H 1050 1850 50  0001 C CNN
F 1 "+5V" H 1065 2173 50  0000 C CNN
F 2 "" H 1050 2000 50  0001 C CNN
F 3 "" H 1050 2000 50  0001 C CNN
	1    1050 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 2850 3450 2850
Wire Wire Line
	2300 2750 2300 2850
Wire Wire Line
	2300 2400 2400 2400
Wire Wire Line
	2300 2450 2300 2400
Connection ~ 2300 2850
Wire Wire Line
	2100 2850 2300 2850
Wire Wire Line
	2700 2400 2750 2400
NoConn ~ 3350 2500
NoConn ~ 3050 2200
NoConn ~ 2750 2500
Wire Wire Line
	2100 2600 2100 2850
Wire Wire Line
	2050 2600 2100 2600
$Comp
L Diode:1N4148 D?
U 1 1 60CA6832
P 2300 2600
AR Path="/5F987406/5F987547/60CA6832" Ref="D?"  Part="1" 
AR Path="/5F987406/5F98763F/60CA6832" Ref="D36"  Part="1" 
F 0 "D36" V 2400 2650 50  0000 L CNN
F 1 "1N4148" H 2150 2500 50  0000 L CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 2300 2425 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 2300 2600 50  0001 C CNN
	1    2300 2600
	0    1    1    0   
$EndComp
Connection ~ 2300 2400
Wire Wire Line
	2050 2400 2300 2400
$Comp
L Device:R R?
U 1 1 60CA683A
P 2550 2400
AR Path="/5F987406/5F987547/60CA683A" Ref="R?"  Part="1" 
AR Path="/5F987406/5F98763F/60CA683A" Ref="R137"  Part="1" 
F 0 "R137" V 2450 2300 50  0000 L CNN
F 1 "220" V 2650 2350 50  0000 L CNN
F 2 "" V 2480 2400 50  0001 C CNN
F 3 "~" H 2550 2400 50  0001 C CNN
	1    2550 2400
	0    1    1    0   
$EndComp
Wire Wire Line
	3450 2400 3450 2850
Wire Wire Line
	3350 2400 3450 2400
Wire Wire Line
	3050 3050 3050 3150
Wire Wire Line
	3450 3050 3050 3050
Wire Wire Line
	3450 3150 3450 3050
$Comp
L power:GND #PWR?
U 1 1 60CA6845
P 3450 3150
AR Path="/5F987406/5F987547/60CA6845" Ref="#PWR?"  Part="1" 
AR Path="/5F987406/5F98763F/60CA6845" Ref="#PWR0375"  Part="1" 
F 0 "#PWR0375" H 3450 2900 50  0001 C CNN
F 1 "GND" H 3455 2977 50  0001 C CNN
F 2 "" H 3450 3150 50  0001 C CNN
F 3 "" H 3450 3150 50  0001 C CNN
	1    3450 3150
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3450 3350 3350 3350
Text Label 4400 5300 2    50   ~ 0
TX_MIDI
NoConn ~ 2750 3450
NoConn ~ 3350 3450
$Comp
L power:+5V #PWR?
U 1 1 60CA6853
P 2250 3250
AR Path="/5F987406/5F987547/60CA6853" Ref="#PWR?"  Part="1" 
AR Path="/5F987406/5F98763F/60CA6853" Ref="#PWR0376"  Part="1" 
F 0 "#PWR0376" H 2250 3100 50  0001 C CNN
F 1 "+5V" H 2265 3423 50  0000 C CNN
F 2 "" H 2250 3250 50  0001 C CNN
F 3 "" H 2250 3250 50  0001 C CNN
	1    2250 3250
	1    0    0    -1  
$EndComp
$Comp
L Connector:DIN-5_180degree J?
U 1 1 60CA6859
P 3050 3450
AR Path="/5F987406/5F987547/60CA6859" Ref="J?"  Part="1" 
AR Path="/5F987406/5F98763F/60CA6859" Ref="J39"  Part="1" 
F 0 "J39" H 3200 3700 50  0000 C CNN
F 1 "MIDI OUT" H 3050 3200 50  0000 C CNN
F 2 "" H 3050 3450 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/18/40_c091_abd_e-75918.pdf" H 3050 3450 50  0001 C CNN
	1    3050 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 3350 2750 3350
$Comp
L Device:R R?
U 1 1 60CA6860
P 2500 3350
AR Path="/5F987406/5F987547/60CA6860" Ref="R?"  Part="1" 
AR Path="/5F987406/5F98763F/60CA6860" Ref="R136"  Part="1" 
F 0 "R136" V 2400 3250 50  0000 L CNN
F 1 "220" V 2600 3250 50  0000 L CNN
F 2 "" V 2430 3350 50  0001 C CNN
F 3 "~" H 2500 3350 50  0001 C CNN
	1    2500 3350
	0    1    1    0   
$EndComp
$Comp
L Connector:DIN-5_180degree J?
U 1 1 60CA6866
P 3050 2500
AR Path="/5F987406/5F987547/60CA6866" Ref="J?"  Part="1" 
AR Path="/5F987406/5F98763F/60CA6866" Ref="J38"  Part="1" 
F 0 "J38" H 3200 2750 50  0000 C CNN
F 1 "MIDI IN" H 3050 2250 50  0000 C CNN
F 2 "" H 3050 2500 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/18/40_c091_abd_e-75918.pdf" H 3050 2500 50  0001 C CNN
	1    3050 2500
	1    0    0    -1  
$EndComp
$Comp
L Isolator:6N138 U?
U 1 1 60CA686C
P 1750 2500
AR Path="/5F987406/5F987547/60CA686C" Ref="U?"  Part="1" 
AR Path="/5F987406/5F98763F/60CA686C" Ref="U28"  Part="1" 
F 0 "U28" H 1600 2850 50  0000 C CNN
F 1 "6N138" H 1650 2150 50  0000 C CNN
F 2 "" H 2040 2200 50  0001 C CNN
F 3 "http://www.onsemi.com/pub/Collateral/HCPL2731-D.pdf" H 2040 2200 50  0001 C CNN
	1    1750 2500
	-1   0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 60D09256
P 4050 4400
AR Path="/5F987406/5F987547/60D09256" Ref="#PWR?"  Part="1" 
AR Path="/5F987406/5F98763F/60D09256" Ref="#PWR0377"  Part="1" 
F 0 "#PWR0377" H 4050 4250 50  0001 C CNN
F 1 "+5V" H 4065 4573 50  0000 C CNN
F 2 "" H 4050 4400 50  0001 C CNN
F 3 "" H 4050 4400 50  0001 C CNN
	1    4050 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 5000 4400 5000
Wire Wire Line
	4500 4900 4400 4900
Wire Wire Line
	4400 4900 4400 5000
Wire Wire Line
	4400 4900 4050 4900
Connection ~ 4400 4900
Wire Wire Line
	4050 4900 4050 4400
Wire Wire Line
	3450 3350 3450 5300
Wire Wire Line
	3450 5300 4500 5300
Wire Wire Line
	1050 5400 1050 2600
Wire Wire Line
	1050 5400 4500 5400
Wire Wire Line
	9750 4600 9250 4600
Wire Wire Line
	9250 4600 9250 4700
Wire Wire Line
	9550 4800 9750 4800
Wire Wire Line
	9550 4800 9550 5200
Wire Wire Line
	4900 4900 9000 4900
Wire Wire Line
	8550 5000 8550 5400
Wire Wire Line
	4900 5000 8550 5000
Wire Wire Line
	8550 5400 9300 5400
Wire Wire Line
	4950 1800 5100 1800
Wire Wire Line
	4950 2000 5500 2000
Wire Wire Line
	5000 2100 5000 2550
Wire Wire Line
	5850 2300 5850 2350
Wire Wire Line
	5850 2350 6050 2350
Wire Wire Line
	6050 2350 6050 2300
Wire Wire Line
	7050 2300 7050 2350
Wire Wire Line
	7050 2350 7250 2350
Wire Wire Line
	7250 2350 7250 2300
Wire Wire Line
	8450 2300 8450 2350
Wire Wire Line
	8450 2350 8250 2350
Wire Wire Line
	8250 2350 8250 2300
Wire Wire Line
	9650 2300 9650 2350
Wire Wire Line
	9650 2350 9450 2350
Wire Wire Line
	9450 2350 9450 2300
Wire Wire Line
	1050 2600 1450 2600
Wire Wire Line
	1050 2500 1050 2600
Connection ~ 1050 2600
Wire Wire Line
	1050 2200 1050 2100
Wire Wire Line
	1350 2100 1050 2100
Wire Wire Line
	1350 2100 1350 2300
Connection ~ 1050 2100
Wire Wire Line
	1050 2100 1050 2000
Wire Wire Line
	1350 2700 1350 3150
Wire Wire Line
	1200 2400 1450 2400
Wire Wire Line
	1200 3050 1200 3150
Wire Wire Line
	1200 2750 1200 2400
Wire Wire Line
	5100 1850 5100 1800
$Comp
L power:GND #PWR0378
U 1 1 5FAA176C
P 5100 1850
F 0 "#PWR0378" H 5100 1600 50  0001 C CNN
F 1 "GND" H 5105 1677 50  0001 C CNN
F 2 "" H 5100 1850 50  0001 C CNN
F 3 "" H 5100 1850 50  0001 C CNN
	1    5100 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	10300 1800 10450 1800
$Comp
L Connector:AudioJack2_Switch J23
U 1 1 60419AD2
P 10650 2000
F 0 "J23" H 10700 2300 50  0000 R CNN
F 1 "To FX Return" H 10650 1775 50  0000 C CNN
F 2 "" H 10650 2200 50  0001 C CNN
F 3 "~" H 10650 2200 50  0001 C CNN
	1    10650 2000
	-1   0    0    -1  
$EndComp
NoConn ~ 10450 1900
NoConn ~ 10450 2100
Wire Wire Line
	10000 2000 10450 2000
Wire Wire Line
	10300 1850 10300 1800
$Comp
L power:GND #PWR0379
U 1 1 5FC580B9
P 10300 1850
F 0 "#PWR0379" H 10300 1600 50  0001 C CNN
F 1 "GND" H 10305 1677 50  0001 C CNN
F 2 "" H 10300 1850 50  0001 C CNN
F 3 "" H 10300 1850 50  0001 C CNN
	1    10300 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 2550 4900 2550
Wire Wire Line
	5400 1150 5400 1200
$Comp
L power:GND #PWR0380
U 1 1 5F7F17F6
P 5400 1200
F 0 "#PWR0380" H 5400 950 50  0001 C CNN
F 1 "GND" H 5405 1027 50  0001 C CNN
F 2 "" H 5400 1200 50  0001 C CNN
F 3 "" H 5400 1200 50  0001 C CNN
	1    5400 1200
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6550 1150 6700 1150
Wire Wire Line
	6400 1150 6550 1150
Connection ~ 6550 1150
Wire Wire Line
	6550 1200 6550 1150
$Comp
L power:GND #PWR0381
U 1 1 5F7F17EE
P 6550 1200
F 0 "#PWR0381" H 6550 950 50  0001 C CNN
F 1 "GND" H 6555 1027 50  0001 C CNN
F 2 "" H 6550 1200 50  0001 C CNN
F 3 "" H 6550 1200 50  0001 C CNN
	1    6550 1200
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7750 1150 7900 1150
Wire Wire Line
	7600 1150 7750 1150
Connection ~ 7750 1150
Wire Wire Line
	7750 1200 7750 1150
$Comp
L power:GND #PWR0382
U 1 1 5F7F181C
P 7750 1200
F 0 "#PWR0382" H 7750 950 50  0001 C CNN
F 1 "GND" H 7755 1027 50  0001 C CNN
F 2 "" H 7750 1200 50  0001 C CNN
F 3 "" H 7750 1200 50  0001 C CNN
	1    7750 1200
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8950 1150 9100 1150
Wire Wire Line
	8800 1150 8950 1150
Connection ~ 8950 1150
Wire Wire Line
	8950 1200 8950 1150
$Comp
L power:GND #PWR0383
U 1 1 5F7F1843
P 8950 1200
F 0 "#PWR0383" H 8950 950 50  0001 C CNN
F 1 "GND" H 8955 1027 50  0001 C CNN
F 2 "" H 8950 1200 50  0001 C CNN
F 3 "" H 8950 1200 50  0001 C CNN
	1    8950 1200
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10100 1150 10100 1200
$Comp
L power:GND #PWR0384
U 1 1 5F7F1869
P 10100 1200
F 0 "#PWR0384" H 10100 950 50  0001 C CNN
F 1 "GND" H 10105 1027 50  0001 C CNN
F 2 "" H 10100 1200 50  0001 C CNN
F 3 "" H 10100 1200 50  0001 C CNN
	1    10100 1200
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J40
U 1 1 6242BBBA
P 4700 2550
F 0 "J40" H 4618 2675 50  0000 C CNN
F 1 "Screw_Terminal_01x02" H 4618 2676 50  0001 C CNN
F 2 "" H 4700 2550 50  0001 C CNN
F 3 "~" H 4700 2550 50  0001 C CNN
	1    4700 2550
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0385
U 1 1 6243D068
P 5000 2750
F 0 "#PWR0385" H 5000 2500 50  0001 C CNN
F 1 "GND" H 5005 2577 50  0001 C CNN
F 2 "" H 5000 2750 50  0001 C CNN
F 3 "" H 5000 2750 50  0001 C CNN
	1    5000 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 2750 5000 2650
Wire Wire Line
	5000 2650 4900 2650
Wire Wire Line
	2250 3350 2250 3250
Wire Wire Line
	2250 3350 2350 3350
Wire Wire Line
	5100 2550 5000 2550
Connection ~ 5000 2550
$EndSCHEMATC
