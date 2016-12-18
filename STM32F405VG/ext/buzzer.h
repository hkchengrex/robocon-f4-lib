#ifndef	__BUZZER_H
#define	__BUZZER_H

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "lcd_main.h"

#define BUZZER_GPIO				GPIOB
#define BUZZER_GPIO_PIN		GPIO_Pin_9
#define BUZZER_GPIO_RCC 	RCC_AHB1Periph_GPIOB
#define BUZZER_GPIO_PINSOURCE GPIO_PinSource9

#define BUZZER_TIM				TIM11
#define BUZZER_TIM_RCC		RCC_APB2Periph_TIM11
#define BUZZER_AF_TIM 		GPIO_AF_TIM11

#define BUZZER_COUNT_PER_SECOND 1000000	/*!< Buzzer timer period, used for prescaling and relavant calculation */
#define BUZZER_TIM_OC_INIT			TIM_OC1Init
#define	BUZZER_TIM_SETCOMPARE		TIM_SetCompare1
#define BUZZER_TIM_OCPreloadConfig TIM_OC1PreloadConfig

#define	C0_PERIOD			                61158   
#define TWELFTH_ROOT_OF_TWOx10000     10595
#define BUZZER_QUEUE_SIZE 						12

/* Basic Buzzer Control Function */
void buzzer_init(void);

/**
  * @brief Turning on the buzzer (keeping the original frequency)
  * @param None
  * @retval None
  */
void buzzer_on(void);

/**
  * @brief Turning off the buzzer (keeping the original frequency)
  * @param None
  * @retval None
  */
void buzzer_off(void);

/**
  * @brief  Generate specific pattern of buzzer
  * @param  count: number of buzz to be generated
  * @param  period: time (in millisecond) for each buzz and each break in between each buzz
  * @retval None
  */
void buzzer_control(u8 count, u16 period);

/* Musical Note Control */
// Note frequency (enharmonic equivalent notes have the same value)

typedef enum {
	NOTE_END		= -1,
	NOTE_REST		=	0,
	NOTE_C			= 1,    // C
	NOTE_Cs			=	2,    // C# 
	NOTE_Db			=	2,    // Db
	NOTE_D			=	3,    // D
	NOTE_Ds			=	4,    // D#
	NOTE_Eb			=	4,    // Eb
	NOTE_E			=	5,    // E
	NOTE_Fb			=	5,    // Fb
	NOTE_F			=	6,    // F
	NOTE_Fs			=	7,    // Fs
	NOTE_Gb			=	7,    // Gb
	NOTE_G			=	8,    // G
	NOTE_Gs			=	9,    // G#
	NOTE_Ab			=	9,    // Ab
	NOTE_A			=	10,   // A
	NOTE_As			=	11,   // A#
	NOTE_Bb			=	11,   // Bb
	NOTE_B			=	12,   // B
	NOTE_Bs			=	1     // B#
} MUSIC_NOTE_LETTER;

typedef struct {
	MUSIC_NOTE_LETTER note;
	u8 octave;
} MUSIC_NOTE;

/**
  * @brief  Buzzer check for handling "buzzer_control" and "buzzer_play_song" (to be called per 1 ms)
  * @param  None
  * @retval None
  */
void buzzer_check(void);

/**
  * @brief Set the buzzer musical note period (in microseconds)
  * @param The musical note period (in microseconds), e.g., 1/440 for note A4
  */
void buzzer_set_note_period(u16 p);

/**
  * @brief Set the volume of the buzzer (Output compare of the timer)
  * @param vol: Volume of timer (0-100)
  */
void buzzer_set_volume(u8 vol);	// 0 - 100 (0: muted, 100: full)

/**
  * @brief Calculate the musical note period 
  * @param note: The musical note enumator
  * @param octave: The selected octave number
  */
u16 get_note_period(MUSIC_NOTE_LETTER note, u8 octave);

/**
  * @brief Start playing song
  * @param Song (an array of MUSIC_NOTE)
  * @param Note length of each note (in millisecond)
  * @param Note length of each break (in millisecond)
  */
void buzzer_play_song(const MUSIC_NOTE* song, u16 note_length, u16 note_break);

/**
  * @brief Stop playing a song
  */
void buzzer_stop_song(void);

#endif
