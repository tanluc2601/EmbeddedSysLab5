#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_system.h"
#include "driver/gpio.h"

#include <inttypes.h>
#include <stdio.h>

#include "sys/time.h"

#define TIMER_1_MESSAGE "ahihi"
#define TIMER_2_MESSAGE "ihaha"

#define TIMER_1_TO_CALLBACK 10
#define TIMER_2_TO_CALLBACK 5

#define TIMER_1_PERIOD 2000
#define TIMER_2_PERIOD 3000

TimerHandle_t timer1;
TimerHandle_t timer2;

BaseType_t xTimer1Started;
BaseType_t xTimer2Started;

uint8_t timer1_CallbackTime = 0;
uint8_t timer2_CallbackTime = 0;

struct timeval tv_now;
static inline void print_current_time()
{
    gettimeofday(&tv_now, NULL);
    printf("t = %li\t", tv_now.tv_sec);
}

static void vTimerCallback(TimerHandle_t tim)
{
    uint32_t id = (uint32_t) pvTimerGetTimerID(tim);

    if (id == 1)
    {
        timer1_CallbackTime++;

        print_current_time();
        printf("%s\t\n", TIMER_1_MESSAGE);

        if (timer1_CallbackTime == TIMER_1_TO_CALLBACK)
        {
            if (xTimerStop(tim, 0) == pdFAIL)
            {
                printf("CANNOT stop timer 1\n");
            }
        }
    }
    else if (id == 2)
    {
        timer2_CallbackTime++;

        print_current_time();
        printf("%s\n", TIMER_2_MESSAGE);

        if (timer2_CallbackTime == TIMER_2_TO_CALLBACK)
        {
            if (xTimerStop(tim, 0) == pdFAIL)
            {
                printf("CANNOT stop timer 2\n");
            }
        }
    }
    else
    {
        printf("Error: There no timer associated with ID %d\n", id);
    }
}

void app_main(void)
{
    timer1_CallbackTime = 0;
    timer1 = xTimerCreate(
        "Timer 1",
        pdMS_TO_TICKS(TIMER_1_PERIOD),
        pdTRUE, // Auto reload
        1,
        vTimerCallback);

    timer2_CallbackTime = 0;
    timer2 = xTimerCreate(
        "Timer 2",
        pdMS_TO_TICKS(TIMER_2_PERIOD),
        pdTRUE, // Auto reload
        2,
        vTimerCallback);

    if ((timer1 != NULL) && (timer2 != NULL))
    {
        xTimer1Started = xTimerStart(timer1, 0);
        xTimer2Started = xTimerStart(timer2, 0);
    }
}