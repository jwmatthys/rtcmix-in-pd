# this script makes it so that the cords are hidden when not in edit mode

proc change_background_color_in_editmode {mytoplevel eventname} {
    if {$mytoplevel eq ".pdwindow"} {return}
    set tkcanvas [tkcanvas_name $mytoplevel]
        if { ! [winfo exists $mytoplevel] } {return}
    if {$::editmode($mytoplevel) == 1} {
        $tkcanvas configure -background "lavender"
    } else {
        $tkcanvas configure -background white
    }
}

bind PatchWindow <<EditMode>> {+change_background_color_in_editmode %W editmode}
bind PatchWindow <<Loaded>> {+change_background_color_in_editmode %W loaded}
