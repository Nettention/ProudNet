APP_STL := gnustl_static
APP_SHORT_COMMANDS := true
APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -DCOCOS2D_DEBUG=1 \
                -fexceptions -fpermissive -Wwrite-strings