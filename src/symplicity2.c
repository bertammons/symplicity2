#include "pebble.h"
#include "time.h"

#define TIME_ZONE_OFFSET -5

Window *window;
TextLayer *text_date_layer;
TextLayer *text_day_layer;
TextLayer *text_time_layer;
TextLayer *text_unix_layer;
TextLayer *text_time_left_layer;
TextLayer *text_minutes_left_layer;
TextLayer *text_hours_left_layer;
TextLayer *text_days_left_layer;
Layer *line_layer;

/* static bool last_bt_state = false;
static BitmapLayer *bt_layer = NULL;
*/



void line_layer_update_callback(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
}


void handle_seccond_tick(struct tm *tick_time, TimeUnits units_changed) {
  // Need to be static because they're used by the system later.
  static char unix_text[] = "Xxxxxxxxxx";
  static char date_text[] = "Xxxxxxxxx 00";  
  static char day_text[] = "Xxxxxxxxx";
  static char time_text[] = "00:00";

  static char timel_text[] = "Xxxxxxxxxx";
  static char minsl_text[] = "Xxxxxxxxxx";
  static char hoursl_text[] = "Xxxxxxxxxx";
  static char daysl_text[] = "Xxxxxxxxxx";

  //Unix Time
  time_t unix_time = time(NULL);

  snprintf(unix_text, sizeof(unix_text), "%d", (int)unix_time); //convert int to string
  text_layer_set_text(text_unix_layer, unix_text);

  //Dead Time
  int born_time = 491796000; //birth seccond in unix time
  int dead_time = (born_time + 2680560000); // 85 years later

  int time_left = (dead_time - unix_time); // remaining secconds in life
  float minutes_left = (time_left / 60);
  float hours_left = (minutes_left /60);
  float days_left = (hours_left /24);

  /*
  snprintf(timel_text, sizeof(timel_text), "%d", (int)time_left);
  text_layer_set_text(text_time_left_layer, timel_text);
  */

  snprintf(minsl_text, sizeof(minsl_text), "%d", (int)minutes_left);
  text_layer_set_text(text_minutes_left_layer, minsl_text);

  snprintf(hoursl_text, sizeof(hoursl_text), "%d", (int)hours_left);
  text_layer_set_text(text_hours_left_layer, hoursl_text);

  snprintf(daysl_text, sizeof(daysl_text), "%d", (int)days_left);
  text_layer_set_text(text_days_left_layer, daysl_text);

  // Date Text
  strftime(date_text, sizeof(date_text), "%B %e", tick_time);
  text_layer_set_text(text_date_layer, date_text);

  // Day Text
  strftime(day_text, sizeof(day_text), "%A", tick_time);
  text_layer_set_text(text_day_layer, day_text);

  // Time Text
  char *time_format;

  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }
  
  strftime(time_text, sizeof(time_text), time_format, tick_time);

  // NOTE: Kludge to handle lack of non-padded hour format string
  // for twelve hour clock. "Stupid Zer0"
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }

  text_layer_set_text(text_time_layer, time_text);
}

/*
void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  // Need to be static because they're used by the system later.
*/

void handle_deinit(void) {
  tick_timer_service_unsubscribe();
}

/*
static void handle_bt(bool bt_state)
{
        if (last_bt_state && !bt_state)
                vibes_double_pulse();

        last_bt_state = bt_state;
        layer_set_hidden(bitmap_layer_get_layer(bt_layer), bt_state);
}
*/


void handle_init(void) {
  window = window_create();
  window_stack_push(window, true /* Animated */);
  window_set_background_color(window, GColorBlack);

  Layer *window_layer = window_get_root_layer(window);

  //Date Text Layer
  text_date_layer = text_layer_create(GRect(8, 140, 144-8, 168-68));
  text_layer_set_text_alignment(text_date_layer, GTextAlignmentLeft);
  text_layer_set_text_color(text_date_layer, GColorWhite);
  text_layer_set_background_color(text_date_layer, GColorClear);
  text_layer_set_font(text_date_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LEANDER_SMALL_21)));
  layer_add_child(window_layer, text_layer_get_layer(text_date_layer));

  // Unix text layer
  text_unix_layer = text_layer_create(GRect(0, 0, 144-0, 168-0));
  text_layer_set_text_alignment(text_unix_layer, GTextAlignmentLeft);
  text_layer_set_text_color(text_unix_layer, GColorWhite);
  text_layer_set_background_color(text_unix_layer, GColorClear);
  text_layer_set_font(text_unix_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LEANDER_SMALL_21)));
  layer_add_child(window_layer, text_layer_get_layer(text_unix_layer));

  // Time Left text layer
  /*
  text_time_left_layer = text_layer_create(GRect(0, 20, 144-0, 168-20));
  text_layer_set_text_alignment(text_time_left_layer, GTextAlignmentLeft);
  text_layer_set_text_color(text_time_left_layer, GColorWhite);
  text_layer_set_background_color(text_time_left_layer, GColorClear);
  text_layer_set_font(text_time_left_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LEANDER_SMALL_21)));
  layer_add_child(window_layer, text_layer_get_layer(text_time_left_layer));
  */

  // minutes left text layer
  text_minutes_left_layer = text_layer_create(GRect(0, 20, 144-0, 168-20));
  text_layer_set_text_alignment(text_minutes_left_layer, GTextAlignmentLeft);
  text_layer_set_text_color(text_minutes_left_layer, GColorWhite);
  text_layer_set_background_color(text_minutes_left_layer, GColorClear);
  text_layer_set_font(text_minutes_left_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LEANDER_SMALL_21)));
  layer_add_child(window_layer, text_layer_get_layer(text_minutes_left_layer));

  // hours left text layer
  text_hours_left_layer = text_layer_create(GRect(0, 40, 144-0, 168-40));
  text_layer_set_text_alignment(text_hours_left_layer, GTextAlignmentLeft);
  text_layer_set_text_color(text_hours_left_layer, GColorWhite);
  text_layer_set_background_color(text_hours_left_layer, GColorClear);
  text_layer_set_font(text_hours_left_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LEANDER_SMALL_21)));
  layer_add_child(window_layer, text_layer_get_layer(text_hours_left_layer));

  // hours left text layer
  text_days_left_layer = text_layer_create(GRect(0, 60, 144-0, 168-60));
  text_layer_set_text_alignment(text_days_left_layer, GTextAlignmentLeft);
  text_layer_set_text_color(text_days_left_layer, GColorWhite);
  text_layer_set_background_color(text_days_left_layer, GColorClear);
  text_layer_set_font(text_days_left_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LEANDER_SMALL_21)));
  layer_add_child(window_layer, text_layer_get_layer(text_days_left_layer));


  // Day text layer
  text_day_layer = text_layer_create(GRect(0, 81, 144, 168-81));
  text_layer_set_text_alignment(text_day_layer, GTextAlignmentRight);
  text_layer_set_text_color(text_day_layer, GColorWhite);
  text_layer_set_background_color(text_day_layer, GColorClear);
  text_layer_set_font(text_day_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LEANDER_SMALL_21)));
  layer_add_child(window_layer, text_layer_get_layer(text_day_layer));

  // Time text layer
  text_time_layer = text_layer_create(GRect(7, 99, 144-7, 168-99));
  text_layer_set_text_alignment(text_time_layer, GTextAlignmentCenter);
  text_layer_set_text_color(text_time_layer, GColorWhite);
  text_layer_set_background_color(text_time_layer, GColorClear);
  text_layer_set_font(text_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_COZUMIX_BIG_52)));
  layer_add_child(window_layer, text_layer_get_layer(text_time_layer));
/*  
  //BT connection status layer
  bt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_PHONE);
  bt_layer = bitmap_layer_create(GRect (0, 20, 144, 168-20));
  bitmap_layer_set_bitmap(bt_layer, bt_bitmap);
*/
  // Horisontal dividing line
  GRect line_frame = GRect(1, 106, 139, 2);
  line_layer = layer_create(line_frame);
  layer_set_update_proc(line_layer, line_layer_update_callback);
  layer_add_child(window_layer, line_layer);



  //tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  tick_timer_service_subscribe(SECOND_UNIT, handle_seccond_tick);
  // TODO: Update display here to avoid blank display on launch?
}


int main(void) {
  handle_init();

  app_event_loop();
  
  handle_deinit();
}
