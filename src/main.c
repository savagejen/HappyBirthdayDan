/*
This pebble watchface is a birthday present for @dan_crowley from @savagejen.
It displays the month, day, and time skinned like a Legend of Zelda style menu.
This watchface was adapted from the "Simplicity" watchface code.
The font comes from: http://www.zeldauniverse.net/typography-and-fonts/
*/
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
	
#include "resource_ids.auto.h"
	
#define MY_UUID { 0x1A, 0x9E, 0x3A, 0x03, 0x51, 0x31, 0x49, 0xD3, 0xA9, 0x6D, 0x33, 0x29, 0xF5, 0xBF, 0xEA, 0xD8 }
PBL_APP_INFO(MY_UUID, "Happy Birthday Dan", "savagejen", 0,1 /* App version */, DEFAULT_MENU_ICON, APP_INFO_WATCH_FACE);
Window window;

TextLayer text_month_layer;
TextLayer text_date_layer;
TextLayer text_time_layer;
Layer triforce_layer;

//Draws the triforce at the top of the watch face
void update_layer_callback(Layer *me, GContext* ctx) {
	(void)me;
	graphics_context_set_text_color(ctx, GColorWhite);
    GFont custom_font_big = \
	  fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TRIFORCE_38));
    graphics_text_draw(ctx,
		     "*",
		     custom_font_big,
		     GRect(0, 0, 144, 100),
		     GTextOverflowModeWordWrap,
		     GTextAlignmentCenter,
		     NULL);
}

void handle_init(AppContextRef ctx) {
    (void)ctx;
	window_init(&window,"Happy Birthday Dan");
	window_stack_push(&window, true);
	window_set_background_color(&window, GColorBlack);
	
	resource_init_current_app(&APP_RESOURCES);
	
	GFont custom_font = \
	  fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TRIFORCE_22));
	GFont custom_font_big = \
	  fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TRIFORCE_38));
	
	//Displays the day of the month like: "LEVEL-17"
	text_layer_init(&text_date_layer, window.layer.frame);
    text_layer_set_text_color(&text_date_layer, GColorWhite);
    text_layer_set_background_color(&text_date_layer, GColorClear);
    layer_set_frame(&text_date_layer.layer, GRect(0, 74, 144-0, 168-74));
    text_layer_set_font(&text_date_layer, custom_font);
	text_layer_set_text_alignment(&text_date_layer, GTextAlignmentCenter);
    layer_add_child(&window.layer, &text_date_layer.layer);
	
	//Displays the month like: -May-
	text_layer_init(&text_month_layer, window.layer.frame);
    text_layer_set_text_color(&text_month_layer, GColorWhite);
    text_layer_set_background_color(&text_month_layer, GColorClear);
    layer_set_frame(&text_month_layer.layer, GRect(0, 50, 144-0, 168-50));
    text_layer_set_font(&text_month_layer, custom_font);
	text_layer_set_text_alignment(&text_month_layer, GTextAlignmentCenter);
    layer_add_child(&window.layer, &text_month_layer.layer);

	//Displays the time like: $x1245
    text_layer_init(&text_time_layer, window.layer.frame);
    text_layer_set_text_color(&text_time_layer, GColorWhite);
    text_layer_set_background_color(&text_time_layer, GColorClear);
    layer_set_frame(&text_time_layer.layer, GRect(0, 98, 144-0, 168-98));
    text_layer_set_font(&text_time_layer, custom_font_big);
	text_layer_set_text_alignment(&text_time_layer, GTextAlignmentCenter);
    layer_add_child(&window.layer, &text_time_layer.layer);

	//Displays the triforce
    layer_init(&triforce_layer, window.layer.frame);
    triforce_layer.update_proc = update_layer_callback;
    layer_add_child(&window.layer, &triforce_layer);

    layer_mark_dirty(&triforce_layer);
}

//Draws the month, day, and time
void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {

  (void)ctx;

  static char time_text[] = "  0000";
  static char month_text[] = " XXXXXXXXX ";
  static char date_text[] = "LEVEL-00";
	
  char *time_format;

  string_format_time(month_text, sizeof(month_text), "-%b-", t->tick_time);
  text_layer_set_text(&text_month_layer, month_text);
	
  string_format_time(date_text, sizeof(date_text), "LEVEL-%e", t->tick_time);
  text_layer_set_text(&text_date_layer, date_text);

	
  if (clock_is_24h_style()) {
    time_format = "$x%R";
  } else {
    time_format = "$x%I%M";
  }

  string_format_time(time_text, sizeof(time_text), time_format, t->tick_time);

  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }

  text_layer_set_text(&text_time_layer, time_text);

}


void pbl_main(void *params) {
	PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }

  };
  app_event_loop(params, &handlers);
}
