#include "WifiStatusView.h"

// extern ESP8266React esp8266React;

static View view;
// static font_t DejaVuSans10, DejaVuSans12, DejaVuSans12Bold, DejaVuSans16, DejaVuSans20, DejaVuSans24, DejaVuSans32, Fixed_5x8, Fixed_7x14;
// static coord_t DisplayWidth, DisplayHeight, DisplayWidthMidpoint, DisplayHeightMidpoint;

static void draw_centered_string(const char *font, const char* display_string, const int x_offset, const int y_offset){
	coord_t width = gdispGetWidth();
    coord_t height = gdispGetHeight();
	font_t display_font = gdispOpenFont(font);	
	int string_width = gdispGetStringWidth(display_string, display_font) + 3;
	int string_height = gdispGetFontMetric(display_font, fontHeight) + 2;
	gdispDrawStringBox((width / 2) - (string_width / 2)+x_offset+8,
					(height/2)- (string_height/2)+y_offset,
					string_width,
					string_height,
					display_string,
					display_font,
					White,
					justifyCenter);
}
static void render()
{
	// esp8266React.getWiFiSettingsService()->read([&](WiFiSettings& wifiSettings) {
        gdispGSetBacklight(gdispGetDisplay(0), 100);
        gdispGSetOrientation(gdispGetDisplay(0), GDISP_ROTATE_270);
        gdispClear(Black);
        draw_centered_string("DejaVuSans16", "Connecting:", 0, -8);
        draw_centered_string("DejaVuSans10", SystemState.ssid, 0, 8);
        draw_centered_string("DejaVuSans10", SystemState.ip, 0, 24);

        gdispGFlush(gdispGetDisplay(0));
    // });	
}

static View* init()
{
	view.render = render;

	// DejaVuSans10 =  gdispOpenFont("DejaVuSans10");
	// DejaVuSans12 =  gdispOpenFont("DejaVuSans12");
	// DejaVuSans12Bold =  gdispOpenFont("DejaVuSansBold12");
	// DejaVuSans16 =  gdispOpenFont("DejaVuSans16");

	// DejaVuSans20 = gdispOpenFont("DejaVuSans20");
	// DejaVuSans24 = gdispOpenFont("DejaVuSans24");
	
	// DejaVuSans32 = gdispOpenFont("DejaVuSans32");
	// Fixed_5x8 = gdispOpenFont("fixed_5x8");
	// Fixed_7x14 = gdispOpenFont("fixed_7x14");

	// DisplayWidth = gdispGetWidth();
	// DisplayWidthMidpoint = DisplayWidth / 2;
	// DisplayHeight = gdispGetHeight();
	// DisplayHeightMidpoint = DisplayHeight / 2;
	
	return &view;
}
const struct wifistatusview WifiStatusView = { 
	.init = init,
};