/****************************************************************************
 * Copyright (C) 2018 Maschell
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include <wups.h>
#include "main.h"
#include <utils/logger.h>
#include <utils/StringTools.h>
#include <vpad/input.h>
#include <coreinit/screen.h>

void SplashScreen_callback(wups_overlay_options_type_t screen){
    int32_t i=0;

    while(i<100){
        WUPS_Overlay_OSScreenClear(screen);
        
        WUPS_Overlay_PrintTextOnScreen(screen, 0,0,"This could be something cool.");
        WUPS_Overlay_PrintTextOnScreen(screen, 0,5,"Testing changing text: %d",i);

        WUPS_Overlay_FlipBuffers(screen);
        
        i++;
    }
}


uint8_t gCallbackCooldown __attribute__((section(".data"))) = 0;

DECL_FUNCTION(int32_t, VPADRead, int32_t chan, VPADStatus *buffer, uint32_t buffer_size, int32_t *error) {
    int32_t result = real_VPADRead(chan, buffer, buffer_size, error);
    uint32_t btns = VPAD_BUTTON_Y | VPAD_BUTTON_X | VPAD_BUTTON_A | VPAD_BUTTON_B;
    if(result > 0 && ((buffer[0].hold & (btns)) == btns) && gCallbackCooldown == 0 ){
        gCallbackCooldown = 0x3C;
        
        //WUPS_OpenOverlay(WUPS_OVERLAY_DRC_ONLY                  ,SplashScreen_callback);
        //WUPS_OpenOverlay(WUPS_OVERLAY_DRC_ONLY                  ,SplashScreen_callback);
        //WUPS_OpenOverlay(WUPS_OVERLAY_TV_ONLY                   ,SplashScreen_callback);
        WUPS_OpenOverlay(WUPS_OVERLAY_DRC_AND_TV                ,SplashScreen_callback);
        //WUPS_OpenOverlay(WUPS_OVERLAY_DRC_AND_TV_WITH_DRC_PRIO  ,SplashScreen_callback);
    }
    if(gCallbackCooldown > 0){
        gCallbackCooldown--;
    }

    return result;
}

WUPS_MUST_REPLACE(VPADRead,          WUPS_LOADER_LIBRARY_VPAD,       VPADRead);