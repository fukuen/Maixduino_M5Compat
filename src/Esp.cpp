/*
 Esp.cpp - ESP31B-specific APIs
 Copyright (c) 2015 Ivan Grokhotkov. All rights reserved.
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Arduino.h"
#include "Esp.h"

/**
 * User-defined Literals
 *  usage:
 *
 *   uint32_t = test = 10_MHz; // --> 10000000
 */

unsigned long long operator"" _kHz(unsigned long long x)
{
    return x * 1000;
}

unsigned long long operator"" _MHz(unsigned long long x)
{
    return x * 1000 * 1000;
}

unsigned long long operator"" _GHz(unsigned long long x)
{
    return x * 1000 * 1000 * 1000;
}

unsigned long long operator"" _kBit(unsigned long long x)
{
    return x * 1024;
}

unsigned long long operator"" _MBit(unsigned long long x)
{
    return x * 1024 * 1024;
}

unsigned long long operator"" _GBit(unsigned long long x)
{
    return x * 1024 * 1024 * 1024;
}

unsigned long long operator"" _kB(unsigned long long x)
{
    return x * 1024;
}

unsigned long long operator"" _MB(unsigned long long x)
{
    return x * 1024 * 1024;
}

unsigned long long operator"" _GB(unsigned long long x)
{
    return x * 1024 * 1024 * 1024;
}


EspClass ESP;

void EspClass::deepSleep(uint32_t time_us)
{
//    esp_deep_sleep(time_us);
}

void EspClass::restart(void)
{
//    esp_restart();
    sysctl_reset(SYSCTL_RESET_SOC);
}

uint32_t EspClass::getHeapSize(void)
{
//    multi_heap_info_t info;
//    heap_caps_get_info(&info, MALLOC_CAP_INTERNAL);
//    return info.total_free_bytes + info.total_allocated_bytes;
    return (uint32_t) get_free_heap_size();
}
