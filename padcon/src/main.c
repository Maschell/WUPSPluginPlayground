#include <wups.h>
#include <string.h>

#include <nsysnet/socket.h>
#include <vpad/input.h>
#include <utils/logger.h>

WUPS_PLUGIN_NAME("Padcon");
WUPS_PLUGIN_DESCRIPTION("Turns the gamepad screen on/off when pressing the right stick.");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("GPL");

ON_APPLICATION_START(args){
   socket_lib_init();   
   log_init();

   DEBUG_FUNCTION_LINE("Init of padcon!\n");
} 

DECL_FUNCTION(int32_t, VPADRead, int32_t chan, VPADStatus *buffer, uint32_t buffer_size, int32_t *error) {
    int32_t result = real_VPADRead(chan, buffer, buffer_size, error);
     if(buffer->release&VPAD_BUTTON_STICK_R) {
            int32_t mode;
            VPADGetLcdMode(0, (int32_t*)&mode);       // Get current display mode
            if(mode != 1) {
                VPADSetLcdMode(0, 1);       // Turn it off
            }
            else {
                VPADSetLcdMode(0, 0xFF);    // Turn it on
            }
    }
    return result;
}

WUPS_MUST_REPLACE(VPADRead ,WUPS_LOADER_LIBRARY_VPAD, VPADRead);
