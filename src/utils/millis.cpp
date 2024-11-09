#ifdef __APPLE__
    #include <mach/mach_time.h>
    #include <stdint.h>
    #include "millis.h"

    long millis( void ) {
        static mach_timebase_info_data_t sTimebaseInfo;
        if ( sTimebaseInfo.denom == 0 ) {
            (void) mach_timebase_info( &sTimebaseInfo );
        }
        uint64_t machTime = mach_absolute_time();
        uint64_t nanos = machTime * sTimebaseInfo.numer / sTimebaseInfo.denom;
        return nanos / 1000000;
    }

#elif NATIVE_64BIT
        #include <stdio.h>
        #include <stdlib.h>
        #include <errno.h>
        #include <linux/unistd.h>       /* for _syscallX macros/related stuff */
        #include <linux/kernel.h>       /* for struct sysinfo */
        #include <sys/sysinfo.h>
        #include "millis.h"

        long millis( void ) {
            struct sysinfo s_info;
            int error = sysinfo(&s_info);
            if(error != 0) {
                printf("code error = %d\n", error);
            }
            return s_info.uptime;        
        }
#endif