/*
 *                           0BSD 
 * 
 *                    BSD Zero Clause License
 * 
 *  Copyright (c) 2019 Hermann Meyer
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.

 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include "xcombomenubox.h"
#include "xcombomenubox_private.h"


void combomenubox_set_active_entry(Widget_t *w, int active) {
    float value = (float)active;
    if (value>w->adj->max_value) value = w->adj->max_value;
    else if (value<w->adj->min_value) value = w->adj->min_value;
    adj_set_value(w->adj, value);
}

Widget_t* add_combomenubox(Widget_t *parent, const char  * label, int x, int y, int width, int height) {

    Widget_t *wid = create_widget(parent->app, parent, x, y, width, height);
    wid->label = label;
    wid->scale.gravity = CENTER;
    wid->adj_y = add_adjustment(wid,0.0, 0.0, 0.0, -1.0,1.0, CL_ENUM);
    wid->adj = wid->adj_y;
    wid->func.adj_callback = _combomenubox_set_entry;
    wid->func.expose_callback = _draw_combomenubox;
    wid->func.enter_callback = transparent_draw;
    wid->func.leave_callback = transparent_draw;
    wid->func.button_release_callback = _combomenubox_button_released;

    Widget_t* button = add_button(wid, "", width-20, 0, 20, height);
    button->func.expose_callback = _draw_combomenubox_button;  
    button->func.button_release_callback = _button_combomenubox_released;

    Widget_t* menu = create_menu(wid, 25);
    menu->func.button_release_callback = _combomenubox_entry_released;


    return wid;
}

Widget_t *combomenubox_add_entry(Widget_t *wid, const char  * label) {
    Widget_t *menu = wid->childlist->childs[1];
    Widget_t *item = menu_add_item(menu,label);
    wid->label = label;
    float max_value = wid->adj->max_value+1.0;
    set_adjustment(wid->adj,0.0, max_value, 0.0, max_value,1.0, CL_ENUM);
    
    return item;
}

void combomenubox_add_numeric_entrys(Widget_t *wid, int imin, int imax) {
    Widget_t *menu = wid->childlist->childs[1];
    menu_add_numeric_items(menu, &imin, &imax);
    float max_value = wid->adj->max_value+(imax-imin)+1;
    set_adjustment(wid->adj,0.0, max_value, 0.0, max_value,1.0, CL_ENUM);
}