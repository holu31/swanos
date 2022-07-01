#include <kernel.h>

#define CURRENT_YEAR 2022

int32_t century_register = 0x00;

unsigned char second;
unsigned char minute;
unsigned char hour;
unsigned char day;
unsigned char month;
uint32_t year;
unsigned char century;

unsigned char last_second;
unsigned char last_minute;
unsigned char last_hour;
unsigned char last_day;
unsigned char last_month;
unsigned char last_year;
unsigned char last_century;
unsigned char registerB;

void outb(int32_t port, int32_t value);
int32_t inb(int32_t port);

enum {
    cmos_address = 0x70,
    cmos_data    = 0x71
};

int32_t get_update_in_progress_flag() {
    outb(cmos_address, 0x0A);
    return (inb(cmos_data) & 0x80);
}

unsigned char get_RTC_register(int32_t reg) {
    outb(cmos_address, reg);
    return inb(cmos_data);
}

void read_rtc() {

    while (get_update_in_progress_flag());
    second = get_RTC_register(0x00);
    minute = get_RTC_register(0x02);
    hour = get_RTC_register(0x04);
    day = get_RTC_register(0x07);
    month = get_RTC_register(0x08);
    year = get_RTC_register(0x09);
    if(century_register != 0) {
        century = get_RTC_register(century_register);
    }

    do {
        last_second = second;
        last_minute = minute;
        last_hour = hour;
        last_day = day;
        last_month = month;
        last_year = year;
        last_century = century;

        while (get_update_in_progress_flag());
        second = get_RTC_register(0x00);
        minute = get_RTC_register(0x02);
        hour = get_RTC_register(0x04);
        day = get_RTC_register(0x07);
        month = get_RTC_register(0x08);
        year = get_RTC_register(0x09);
        if(century_register != 0) {
            century = get_RTC_register(century_register);
        }
    } while( (last_second != second) || (last_minute != minute) || (last_hour != hour) ||
           (last_day != day) || (last_month != month) || (last_year != year) ||
           (last_century != century) );

    registerB = get_RTC_register(0x0B);

    if (!(registerB & 0x04)) {
        second = (second & 0x0F) + ((second / 16) * 10);
        minute = (minute & 0x0F) + ((minute / 16) * 10);
        hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
        day = (day & 0x0F) + ((day / 16) * 10);
        month = (month & 0x0F) + ((month / 16) * 10);
        year = (year & 0x0F) + ((year / 16) * 10);
        if(century_register != 0) {
            century = (century & 0x0F) + ((century / 16) * 10);
        }
    }

    if (!(registerB & 0x02) && (hour & 0x80)) {
        hour = ((hour & 0x7F) + 12) % 24;
    }

    if(century_register != 0) {
        year += century * 100;
    } else {
        year += (CURRENT_YEAR / 100) * 100;
        if(year < CURRENT_YEAR) year += 100;
    }
}

int32_t cmos_get_second(){
    read_rtc();
    return second;
}
int32_t cmos_get_minute(){
    read_rtc();
    return minute;
}
int32_t cmos_get_hour(){
    read_rtc();
    return hour;
}
int32_t cmos_get_day(){
    read_rtc();
    return day;
}
int32_t cmos_get_month(){
    read_rtc();
    return month;
}
int32_t cmos_get_year(){
    read_rtc();
    return year;
}