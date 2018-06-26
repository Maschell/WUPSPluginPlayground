#include <wups.h>
#include <string.h>
#include <stdlib.h>
#include <coreinit/systeminfo.h>
#include <nsysnet/socket.h>
#include <vpad/input.h>
#include <utils/logger.h>

#define TARGET_WIDTH (854)
#define TARGET_HEIGHT (480)

void printVPADButtons(VPADStatus * buffer);

WUPS_PLUGIN_NAME("Vpad input logger");
WUPS_PLUGIN_DESCRIPTION("Prints information about vpad inputs and sensors");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("GPL");

ON_APPLICATION_START(args) {
    socket_lib_init();
    log_init();
}

uint8_t counter = 0;
uint8_t old_slideVolume = 0;
uint32_t old_usingHeadphones = 0;

DECL_FUNCTION(int32_t, VPADRead, int32_t chan, VPADStatus *buffer, uint32_t buffer_size, int32_t *error) {
    int32_t result = real_VPADRead(chan, buffer, buffer_size, error);
    
    if(result > 0 && OSIsHomeButtonMenuEnabled()){
        if(counter++ > 15){ 
            log_printf("acc.acc(x,y,z): %-+.2f %-+.2f %-+.2f acc.mag: %-+.2f acc.variation: %-+.2f acc.vertical(x,y) %-+.2f %-+.2f gyro (x,y,z) %-+.2f %-+.2f %-+.2f angle (x,y,z) %-+.2f %-+.2f %-+.2f\n",buffer->accelorometer.acc.x,
                                                          buffer->accelorometer.acc.y,
                                                          buffer->accelorometer.acc.z,
                                                          buffer->accelorometer.magnitude,
                                                          buffer->accelorometer.variation,
                                                          buffer->accelorometer.vertical.x,
                                                          buffer->accelorometer.vertical.y,
                                                          buffer->gyro.x,
                                                          buffer->gyro.y,
                                                          buffer->gyro.z,
                                                          buffer->angle.x,
                                                          buffer->angle.y,
                                                          buffer->angle.z
                                                          );
            log_printf("mag(x,y,z): %-+.2f %-+.2f %-+.2f direction.x: %-+.2f %-+.2f %-+.2f direction.y: %-+.2f %-+.2f %-+.2f direction.z: %-+.2f %-+.2f %-+.2f\n",buffer->mag.x,
                                                          buffer->mag.y,
                                                          buffer->mag.z,
                                                          buffer->direction.x.x,
                                                          buffer->direction.x.y,
                                                          buffer->direction.x.z,
                                                          buffer->direction.y.x,
                                                          buffer->direction.y.y,
                                                          buffer->direction.y.z,
                                                          buffer->direction.z.x,
                                                          buffer->direction.z.y,
                                                          buffer->direction.z.z
                                                          );
                                                          
            counter = 0;
        }
                                                      
        VPADTouchData tpCalib;
        VPADGetTPCalibratedPoint(0, &tpCalib, &buffer->tpNormal);
        float tpXPos = (((float)tpCalib.x / 1280.0f) * (float)TARGET_WIDTH);
        float tpYPos = (float)TARGET_HEIGHT - (((float)tpCalib.y / 720.0f) * (float)TARGET_HEIGHT);
        BOOL tpTouched = tpCalib.touched;
        
        if(old_slideVolume != buffer->slideVolume){
            log_printf("The DRC volume has changed from %d to %d\n", old_slideVolume, buffer->slideVolume);
            old_slideVolume = buffer->slideVolume;
        }
        
        if(old_usingHeadphones != buffer->usingHeadphones){
            if(buffer->usingHeadphones){
                log_printf("Plugged in headphones\n");
            }else{
                 log_printf("Unplugged headphones\n");
            }
            old_usingHeadphones = buffer->usingHeadphones;
        }
        
        if(tpTouched){
            log_printf("Touch: x: %-+3.2f y: %-+3.2f touched %d\n",tpXPos,tpYPos,tpTouched);
        }
        printVPADButtons(buffer);


    }
    return result;
}

void printVPADButtons(VPADStatus * buffer){
    if(buffer->hold != 0x00000000){
        char output[1000];

        output[0] = 0; //null terminate it. just in case.

        if((buffer->hold & VPAD_BUTTON_A)                 ==  VPAD_BUTTON_A)                  strcat(output,"A        ");
        if((buffer->hold & VPAD_BUTTON_B)                 ==  VPAD_BUTTON_B)                  strcat(output,"B        ");
        if((buffer->hold & VPAD_BUTTON_X)                 ==  VPAD_BUTTON_X)                  strcat(output,"X        ");
        if((buffer->hold & VPAD_BUTTON_Y)                 ==  VPAD_BUTTON_Y)                  strcat(output,"Y        ");
        if((buffer->hold & VPAD_BUTTON_L)                 ==  VPAD_BUTTON_L)                  strcat(output,"L        ");
        if((buffer->hold & VPAD_BUTTON_R)                 ==  VPAD_BUTTON_R)                  strcat(output,"R        ");
        if((buffer->hold & VPAD_BUTTON_ZR)                ==  VPAD_BUTTON_ZR)                 strcat(output,"ZR       ");
        if((buffer->hold & VPAD_BUTTON_ZL)                ==  VPAD_BUTTON_ZL)                 strcat(output,"ZL       ");
        if((buffer->hold & VPAD_BUTTON_LEFT)              ==  VPAD_BUTTON_LEFT)               strcat(output,"Left     ");
        if((buffer->hold & VPAD_BUTTON_RIGHT)             ==  VPAD_BUTTON_RIGHT)              strcat(output,"Right    ");
        if((buffer->hold & VPAD_BUTTON_UP)                ==  VPAD_BUTTON_UP)                 strcat(output,"Up       ");
        if((buffer->hold & VPAD_BUTTON_DOWN)              ==  VPAD_BUTTON_DOWN)               strcat(output,"Down     ");
        if((buffer->hold & VPAD_BUTTON_PLUS)              ==  VPAD_BUTTON_PLUS)               strcat(output,"+        ");
        if((buffer->hold & VPAD_BUTTON_MINUS)             ==  VPAD_BUTTON_MINUS)              strcat(output,"-        ");
        if((buffer->hold & VPAD_BUTTON_TV)                ==  VPAD_BUTTON_TV)                 strcat(output,"TV       ");
        if((buffer->hold & VPAD_BUTTON_HOME)              ==  VPAD_BUTTON_HOME)               strcat(output,"HOME     ");
        if((buffer->hold & VPAD_BUTTON_STICK_L)           ==  VPAD_BUTTON_STICK_L)            strcat(output,"SL       ");
        if((buffer->hold & VPAD_BUTTON_STICK_R)           ==  VPAD_BUTTON_STICK_R)            strcat(output,"SR       ");
        if((buffer->hold & VPAD_STICK_R_EMULATION_LEFT)   ==  VPAD_STICK_R_EMULATION_LEFT)    strcat(output,"RE_Left  ");
        if((buffer->hold & VPAD_STICK_R_EMULATION_RIGHT)  ==  VPAD_STICK_R_EMULATION_RIGHT)   strcat(output,"RE_Right ");
        if((buffer->hold & VPAD_STICK_R_EMULATION_UP)     ==  VPAD_STICK_R_EMULATION_UP)      strcat(output,"RE_Up    ");
        if((buffer->hold & VPAD_STICK_R_EMULATION_DOWN)   ==  VPAD_STICK_R_EMULATION_DOWN)    strcat(output,"RE_Down  ");
        if((buffer->hold & VPAD_STICK_L_EMULATION_LEFT)   ==  VPAD_STICK_L_EMULATION_LEFT)    strcat(output,"LE_Left  ");
        if((buffer->hold & VPAD_STICK_L_EMULATION_RIGHT)  ==  VPAD_STICK_L_EMULATION_RIGHT)   strcat(output,"LE_Right ");
        if((buffer->hold & VPAD_STICK_L_EMULATION_UP)     ==  VPAD_STICK_L_EMULATION_UP)      strcat(output,"LE_Up    ");
        if((buffer->hold & VPAD_STICK_L_EMULATION_DOWN)   ==  VPAD_STICK_L_EMULATION_DOWN)    strcat(output,"LE_Down  ");

        log_printf("%spressed Sticks: LX %f LY %f RX %f RY %f\n",output,buffer->leftStick.x,buffer->leftStick.y,buffer->rightStick.x,buffer->rightStick.y);
    }    
}


WUPS_MUST_REPLACE(VPADRead,     WUPS_LOADER_LIBRARY_VPAD,       VPADRead);
