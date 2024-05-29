SET(SOURCES screens/ui_Screen1.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    images/ui_img_background_oog_png.c
    images/ui_img_oog_gekozen_8_gesplitst_png.c
    images/ui_img_vraagteken_png.c
    images/ui_img_bovenkant_ooglid_png.c
    images/ui_img_onderkant_ooglid_png.c)

add_library(ui ${SOURCES})
