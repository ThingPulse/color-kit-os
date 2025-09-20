#include "lvgl.h"

#include "hardware/hardware.h"
#include "hardware/powermgm.h"
#include "system/homescreen.h"
#include "system/ui/screens.h"


#if defined( NATIVE_64BIT )
    #include <stdio.h>
    /**
     * for non arduino
     */                 
    void setup( void );
    void loop( void );

    int main( void ) {
        setvbuf(stdout, NULL, _IOLBF, 0);
        setup();
        while( 1 ) { loop(); };
        return( 0 );
    }
#endif // NATIVE_64BIT

void setup() {
    /**
     * hardware setup
     */
    hardware_setup();
    /**
     * gui setup
     */
    create_home_screen();

    /**
     * apps autocall setup
     */
    //app_autocall_all_setup_functions();
    /**
     * post hardware setup
     */
    hardware_post_setup();
}

void loop(){
    powermgm_loop();
    lv_task_handler();

}
