#include <avr/io.h>
#include <stdint-gcc.h>
#include <stdlib.h>
#include "Tuner.h"
#include "InputCapture.h"
#include "util.h"

/* Defines */
#define CLK_PERIOD (1/F_CPU)

#define MUTE_DDR				DDRD
#define MUTE_PORT				PORTD
#define MUTE_PORT_PIN           PORTD5

#define NOTES      12
#define OCTAVES     8
#define LIMIT_SHIFT_HARMONIC    7
#define LIMIT_SHIFT_RIGHT_NOTE  7

#define HARMONICS   4
#define WORK_ARRAY_SIZE  (HARMONICS + 1)

/* Size of the circular result buffer, must be power of 2 */
#define RESULT_ARRAY_SIZE   8
/* Mask for calculating result buffer head/tail */
#define RESULT_ARRAY_MASK	( RESULT_ARRAY_SIZE - 1)

/* Number of steps for red LEDs in tuner display */
#define NUMBER_OF_LED_STEPS 8

/* Global variables */
volatile uint8_t Tuner_LED_Data[2] = {0};
volatile uint8_t Tuner_7seg_Data[4] = {' ', ' ', ' ', ' '};
volatile uint8_t Tuner_TransmitTimerOvfCnt = 0;
volatile bool Tuner_TransmitTimerFlag = false;

/* Local variables */
volatile uint32_t CountArray[WORK_ARRAY_SIZE] = {0};
volatile uint32_t SumArray[WORK_ARRAY_SIZE] = {0};
volatile uint32_t DiffArray[WORK_ARRAY_SIZE] = {0};
volatile uint32_t CountResultArray[RESULT_ARRAY_SIZE] = {0};
volatile uint8_t NoteResultArray[RESULT_ARRAY_SIZE] = {0};
volatile uint8_t FundamentalNoteResultArray[RESULT_ARRAY_SIZE] = {0};

volatile uint8_t CntArrayHead = 0;
volatile uint8_t ResultArrayHead = 0;
volatile uint8_t ResultArrayTail = 0;

volatile uint8_t Harmonic_Result[HARMONICS] = {0};
const uint8_t Harmonic_Mask[HARMONICS] = 
{
    0b00011111, // Mask for 1st harmonic (root frequency)
    0b00010101, // Mask for 2nd harmonic
    0b00001001, // Mask for 3rd harmonic
    0b00010001, // Mask for 4th harmonic
};

const char Tuner_NoteCharacter[NOTES][2] =
{
    {'C',' '},
    {'D','b'},
    {'D',' '},
    {'E','b'},
    {'E',' '},
    {'F',' '},
    {'G','b'},
    {'G',' '},
    {'A','b'},
    {'A',' '},
    {'B','b'},
    {'B',' '}
};

const uint8_t Tuner_RedLEDStates[NUMBER_OF_LED_STEPS * 2] =
{
    0b00010000,
    0b00110000,
    0b00100000,
    0b01100000,
    0b01000000,
    0b11000000,
    0b10000000,
    0b00000000,
    0b00001000,
    0b00001100,
    0b00000100,
    0b00000110,
    0b00000010,
    0b00000011,
    0b00000001,
    0b00000000
};

const uint32_t Tuner_NoteCycleArray[NOTES*OCTAVES] =
{
    1223122,    // C0	[20MHz / 16.35160Hz]    [0]
    1154474,    // C#0	[20MHz / 17.32391Hz]
    1089678,    // D0	[20MHz / 18.35405Hz]
    1028519,    // D#0	[20MHz / 19.44544Hz]
     970793,    // E0	[20MHz / 20.60172Hz]
     916306,    // F0	[20MHz / 21.82676Hz]
     864878,    // F#0	[20MHz / 23.12465Hz]
     816336,    // G0	[20MHz / 24.49971Hz]
     770519,    // G#0	[20MHz / 25.95654Hz]
     727273,    // A0	[20MHz / 27.50000Hz]
     686454,    // A#0	[20MHz / 29.13524Hz]
     647926,    // B0	[20MHz / 30.86771Hz]

     611561,    // C1	[20MHz / 32.70320Hz]    [12]
     577237,    // C#1	[20MHz / 34.64783Hz]
     544839,    // D1	[20MHz / 36.70810Hz]
     514260,    // D#1	[20MHz / 38.89087Hz]
     485396,    // E1	[20MHz / 41.20344Hz]
     458153,    // F1	[20MHz / 43.65353Hz]
     432439,    // F#1	[20MHz / 46.24930Hz]
     408168,    // G1	[20MHz / 48.99943Hz]
     385259,    // G#1	[20MHz / 51.91309Hz]
     363636,    // A1	[20MHz / 55.00000Hz]
     343227,    // A#1	[20MHz / 58.27047Hz]
     323963,    // B1	[20MHz / 61.73541Hz]

     305781,    // C2	[20MHz / 65.40639Hz]    [24]
     288618,    // C#2	[20MHz / 69.29566Hz]
     272419,    // D2	[20MHz / 73.41619Hz]
     257130,    // D#2	[20MHz / 77.78175Hz]
     242698,    // E2	[20MHz / 82.40689Hz]
     229077,    // F2	[20MHz / 87.30706Hz]
     216219,    // F#2	[20MHz / 92.49861Hz]
     204084,    // G2	[20MHz / 97.99886Hz]
     192630,    // G#2	[20MHz / 103.8262Hz]
     181818,    // A2	[20MHz / 110.0000Hz]
     171614,    // A#2	[20MHz / 116.5409Hz]
     161982,    // B2	[20MHz / 123.4708Hz]

     152890,    // C3	[20MHz / 130.8128Hz]    [36]
     144309,    // C#3	[20MHz / 138.5913Hz]
     136210,    // D3	[20MHz / 146.8324Hz]
     128565,    // D#3	[20MHz / 155.5635Hz]
     121349,    // E3	[20MHz / 164.8138Hz]
     114538,    // F3	[20MHz / 174.6141Hz]
     108110,    // F#3	[20MHz / 184.9972Hz]
     102042,    // G3	[20MHz / 195.9977Hz]
      96315,    // G#3	[20MHz / 207.6523Hz]
      90909,    // A3	[20MHz / 220.0000Hz]
      85807,    // A#3	[20MHz / 233.0819Hz]
      80991,    // B3	[20MHz / 246.9417Hz]

      76445,    // C4	[20MHz / 261.6256Hz]    [48]
      72155,    // C#4	[20MHz / 277.1826Hz]
      68105,    // D4	[20MHz / 293.6648Hz]
      64282,    // D#4	[20MHz / 311.1270Hz]
      60675,    // E4	[20MHz / 329.6276Hz]
      57269,    // F4	[20MHz / 349.2282Hz]
      54055,    // F#4	[20MHz / 369.9944Hz]
      51021,    // G4	[20MHz / 391.9954Hz]
      48157,    // G#4	[20MHz / 415.3047Hz]
      45455,    // A4	[20MHz / 440.0000Hz]
      42903,    // A#4	[20MHz / 466.1638Hz]
      40495,    // B4	[20MHz / 493.8833Hz]

      38223,    // C5	[20MHz / 523.2511Hz]    [60]
      36077,    // C#5	[20MHz / 554.3653Hz]
      34052,    // D5	[20MHz / 587.3295Hz]
      32141,    // D#5	[20MHz / 622.2540Hz]
      30337,    // E5	[20MHz / 659.2551Hz]
      28635,    // F5	[20MHz / 698.4565Hz]
      27027,    // F#5	[20MHz / 739.9888Hz]
      25511,    // G5	[20MHz / 783.9909Hz]
      24079,    // G#5	[20MHz / 830.6094Hz]
      22727,    // A5	[20MHz / 880.0000Hz]
      21452,    // A#5	[20MHz / 932.3275Hz]
      20248,    // B5	[20MHz / 987.7666Hz]

      19111,    // C6	[20MHz / 1046.502Hz]    [72]
      18039,    // C#6	[20MHz / 1108.731Hz]
      17026,    // D6	[20MHz / 1174.659Hz]
      16071,    // D#6	[20MHz / 1244.508Hz]
      15169,    // E6	[20MHz / 1318.510Hz]
      14317,    // F6	[20MHz / 1396.913Hz]
      13514,    // F#6	[20MHz / 1479.978Hz]
      12755,    // G6	[20MHz / 1567.982Hz]
      12039,    // G#6	[20MHz / 1661.219Hz]
      11364,    // A6	[20MHz / 1760.000Hz]
      10726,    // A#6	[20MHz / 1864.655Hz]
      10124,    // B6	[20MHz / 1975.533Hz]
      
       9556,    // C7	[20MHz / 2093.005Hz]    [84]
       9019,    // C#7	[20MHz / 2217.461Hz]
       8513,    // D7	[20MHz / 2349.318Hz]
       8035,    // D#7	[20MHz / 2489.016Hz]
       7584,    // E7	[20MHz / 2637.020Hz]
       7159,    // F7	[20MHz / 2793.826Hz]
       6757,    // F#7	[20MHz / 2959.955Hz]
       6378,    // G7	[20MHz / 3135.963Hz]
       6020,    // G#7	[20MHz / 3322.438Hz]
       5682,    // A7	[20MHz / 3520.000Hz]
       5363,    // A#7	[20MHz / 3729.310Hz]
       5062     // B7	[20MHz / 3951.066Hz]
};


/* Function implementations */

void Tuner_Init()
{
    /* Set Mute signal GPIO to output */
    MUTE_DDR |= (1 << MUTE_PORT_PIN);
}

void Tuner_Enable()
{
    /* Mute loop and thru outputs, turn on tuner signal */
    MUTE_PORT |= (1 << MUTE_PORT_PIN);

    /* Start transmit timer */
    Tuner_TransmitTimerOvfCnt = 1;
	
    InputCapture_Enable();
}

void Tuner_Disable()
{
    InputCapture_Disable();

    /* Stop transmit timer and clear flag */
    Tuner_TransmitTimerOvfCnt = 0;
    Tuner_TransmitTimerFlag = false;

    /* Clear Mute signal */
    MUTE_PORT &= ~(1 << MUTE_PORT_PIN);
}

bool Tuner_ResultAvailable()
{
    return (ResultArrayHead != ResultArrayTail);
}

void Tuner_FetchCounts()
{
    /* Get new InputCapture counts */
    while (InputCapture_Available())
    {
        /* Get next count */
        CountArray[CntArrayHead] = InputCapture_GetCount();

        /* calculate new array head index (ignore overflow) */
	    if (++CntArrayHead >= WORK_ARRAY_SIZE)
            CntArrayHead = 0;
    }
}

void Tuner_DetectFrequency()
{
    /* Calculate sums and differences */
    uint32_t accum = 0;
    uint8_t accIdx = CntArrayHead;
    for (uint8_t a = 0; a < WORK_ARRAY_SIZE; a++)
    {
        if (accIdx-- == 0)
            accIdx = WORK_ARRAY_SIZE - 1;
        
        /* Find accumulated sums */
        accum += CountArray[accIdx];
        SumArray[a] = accum;

        /* Find absolute difference values */
        DiffArray[a] = (uint32_t)abs((int32_t)(SumArray[0] - CountArray[accIdx]));
    }

    /* Find harmonics */
    uint32_t limit = 0;
    for (uint8_t h = 0; h < HARMONICS; h++)
    {
        Harmonic_Result[h] = 0;
        limit = (SumArray[h] >> LIMIT_SHIFT_HARMONIC);   // Fraction of presumed period

        for (uint8_t d = 0; d < WORK_ARRAY_SIZE; d++)
        {
            if (DiffArray[d] <= limit)
            {
                Harmonic_Result[h] |= (1 << d);
            }
        }

        /* Evaluate resulting count - First encounter of correct harmonic pattern is result*/
        if (Harmonic_Result[h] == Harmonic_Mask[h])
        {
            /* Update result arrays */
            CountResultArray[ResultArrayHead] = SumArray[h];
            NoteResultArray[ResultArrayHead] = FindClosestArrayIndexDecending(Tuner_NoteCycleArray, NOTES*OCTAVES, SumArray[h]);
            FundamentalNoteResultArray[ResultArrayHead] = NoteResultArray[ResultArrayHead] % NOTES;
            
            /* Calculate new result buffer head index */
            ResultArrayHead = (ResultArrayHead + 1) & RESULT_ARRAY_MASK;

            /* Check for buffer overflow */
            if ( ResultArrayHead == ResultArrayTail )
            {
                /* Calculate new result buffer tail index (ignore overflow)*/
                ResultArrayTail = (ResultArrayTail + 1) & RESULT_ARRAY_MASK;
            }

            /* Break for-loop */
            h = HARMONICS;
        }
    }
}

void Tuner_GenerateDisplayData()
{
    /* Find most numerous fundamental note */
    uint8_t most_common = GetMostCommon((uint8_t*)FundamentalNoteResultArray, RESULT_ARRAY_SIZE);

    /* Update note name */
    Tuner_7seg_Data[1] = Tuner_NoteCharacter[most_common][0];
    Tuner_7seg_Data[2] = Tuner_NoteCharacter[most_common][1];

    /* Find array index of newest instance of that note */
    uint8_t arrIdx = ResultArrayHead;
    for (uint8_t i = 0; i < RESULT_ARRAY_SIZE; i++)
    {
        if (arrIdx-- == 0)
        {
            arrIdx = (RESULT_ARRAY_SIZE - 1);
        }

        if (FundamentalNoteResultArray[arrIdx] == most_common)
        {
            /* Break for-loop */
            i = RESULT_ARRAY_SIZE;
        }
    }

    /* Count difference negative if played note is lower than closest not */
    int32_t count_diff = Tuner_NoteCycleArray[NoteResultArray[arrIdx]] - CountResultArray[arrIdx];

    /* Calculate the number corresponding to one step change for red LEDs */
    uint32_t span = 0;
    uint32_t res = 0;
    
    if (count_diff < 0)
    {
        span = Tuner_NoteCycleArray[NoteResultArray[arrIdx] - 1] - Tuner_NoteCycleArray[NoteResultArray[arrIdx]];
    }
    else
    {
        span = Tuner_NoteCycleArray[NoteResultArray[arrIdx]] - Tuner_NoteCycleArray[NoteResultArray[arrIdx] + 1];
    }

    span /= 2;
    res = span / NUMBER_OF_LED_STEPS;

    if (abs(count_diff) < (span >> LIMIT_SHIFT_RIGHT_NOTE))
    {
        Tuner_LED_Data[0] = 0x00;   // Clear all red LEDs
        Tuner_LED_Data[1] = 0x01;   // Set green LED
        Tuner_7seg_Data[0] = ' ';
        Tuner_7seg_Data[3] = ' ';
    }
    else
    {
        uint8_t i = 0;
        for (i = 0; i <= NUMBER_OF_LED_STEPS; i++)
        {
            if (abs(count_diff) < (res * (i+1)))
            {
                break;
            }
        }

        if (count_diff < 0)
        {
            Tuner_7seg_Data[0] = '-';
            Tuner_7seg_Data[3] = ' ';
        }
        else
        {
            i += NUMBER_OF_LED_STEPS;
            Tuner_7seg_Data[0] = ' ';
            Tuner_7seg_Data[3] = '-';
        }

        Tuner_LED_Data[0] = Tuner_RedLEDStates[i];  // Set red LEDs
        Tuner_LED_Data[1] = 0x00;                   // Clear green LED
    }
}
