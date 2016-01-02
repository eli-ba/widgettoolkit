#include <AppKit/resources.h>
#include <SDL_image.h>
#include <stdio.h>
#include <Windows.h>
#include <string.h>

#define APPKIT_VISUAL_STYLE_PATH "appkit_data/resource/window/default/"
#define APPKIT_FONTS_PATH "appkit_data/resource/fonts/"
#define APPKIT_BUTTON_RESOURCES_PATH "appkit_data/resource/button/"
#define APPKIT_LISTBOX_RESOURCES_PATH "appkit_data/resource/ListBox/"

SDL_Surface* Resources::Images[IMAGE_RESOURCE_COUNT];
TTF_Font* Resources::Fonts[FONT_RESOURCE_COUNT];
bool Resources::ImageResourcesLoaded = false;
bool Resources::FontResourcesLoaded = false;

void Resources::Load()
{
	LoadImageResources();
	LoadFontResources();
}

void Resources::LoadImageResources()
{
	if (Resources::ImageResourcesLoaded) return;

	Images[WIN_TITLE_BAR_RIGHT] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"win_title_bar_right_2.png");
	Images[WIN_TITLE_BAR_LEFT] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"win_title_bar_left_2.png");
	Images[WIN_TITLE_BAR_CENTER] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"win_title_bar_center_2.png");
	Images[TOOLWIN_WIN_TITLE_BAR_RIGHT] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"toolwin_win_title_bar_right.png");
	Images[TOOLWIN_WIN_TITLE_BAR_LEFT] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"toolwin_win_title_bar_left.png");
	Images[TOOLWIN_WIN_TITLE_BAR_CENTER] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"toolwin_win_title_bar_center.png");
	Images[TOOLWIN_CLS_BTN_PRESSED] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"toolwin_cls_btn_pressed.png");
	Images[TOOLWIN_CLS_BTN_INACTIVE] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"toolwin_cls_btn_inactive.png");
	Images[TOOLWIN_CLS_BTN] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"toolwin_cls_btn.png");
	Images[RIGHT_CORNER] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"right_corner.bmp");
	Images[RIGHT_BORDER] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"right_border.bmp");
	Images[RDC_BTN_PRESSED] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"rdc_btn_pressed.png");
	Images[RDC_BTN] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"rdc_btn.png");
	Images[MX_BTN_PRESSED] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"mx_btn_pressed.png");
	Images[MX_BTN] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"mx_btn.png");
	Images[MX_BTN_PRESSED_2] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"mx_btn_pressed_2.png");
	Images[MX_BTN_2] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"mx_btn_2.png");
	Images[MX_BTN_2_INACTIVE] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"mx_btn_2_inactive.png");
	Images[LEFT_CORNER] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"left_corner.bmp");
	Images[LEFT_BORDER] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"left_border.bmp");
	Images[CLS_BTN_PRESSED] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"cls_btn_pressed.png");
	Images[CLS_BTN] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"cls_btn.png");
	Images[BOTTOM_BORDER] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"bottom_border.bmp");
	Images[RESIZE_RECT] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"resize_rect.png");

	//Inactive elements
	Images[WIN_TITLE_BAR_RIGHT_INACTIVE] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"win_title_bar_right_inactive.png");
	Images[WIN_TITLE_BAR_LEFT_INACTIVE] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"win_title_bar_left_inactive.png");
	Images[WIN_TITLE_BAR_CENTER_INACTIVE] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"win_title_bar_center_inactive.png");

	Images[TOOLWIN_WIN_TITLE_BAR_RIGHT_INACTIVE] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"toolwin_win_title_bar_right_inactive.png");
	Images[TOOLWIN_WIN_TITLE_BAR_LEFT_INACTIVE] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"toolwin_win_title_bar_left_inactive.png");
	Images[TOOLWIN_WIN_TITLE_BAR_CENTER_INACTIVE] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"toolwin_win_title_bar_center_inactive.png");
	
	Images[RIGHT_BORDER_INACTIVE] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"right_border_inactive.bmp");
	Images[LEFT_BORDER_INACTIVE] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"left_border_inactive.bmp");
	Images[BOTTOM_BORDER_INACTIVE] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"bottom_border_inactive.bmp");
	
	Images[RIGHT_CORNER_INACTIVE] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"right_corner_inactive.bmp");
	Images[LEFT_CORNER_INACTIVE] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"left_corner_inactive.bmp");

	Images[CLS_BTN_INACTIVE] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"cls_btn_inactive.png");
	Images[MX_BTN_INACTIVE] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"mx_btn_inactive.png");
	Images[RDC_BTN_INACTIVE] = IMG_Load(APPKIT_VISUAL_STYLE_PATH"rdc_btn_inactive.png");

	/* PushButton */
	Images[PUSHBUTTON_NORMAL] = IMG_Load(APPKIT_BUTTON_RESOURCES_PATH"pushbutton_normal.PNG");
	Images[PUSHBUTTON_PRESSED] = IMG_Load(APPKIT_BUTTON_RESOURCES_PATH"pushbutton_pressed.PNG");

	/* ListBox */
	Images[SCROLLBUTTON_UP] = IMG_Load(APPKIT_LISTBOX_RESOURCES_PATH"scrollbutton_up.png");
	Images[SCROLLBUTTON_UP_PRESSED] = IMG_Load(APPKIT_LISTBOX_RESOURCES_PATH"scrollbutton_up_pressed.png");
	Images[SCROLLBUTTON_DOWN] = IMG_Load(APPKIT_LISTBOX_RESOURCES_PATH"scrollbutton_down.png");
	Images[SCROLLBUTTON_DOWN_PRESSED] = IMG_Load(APPKIT_LISTBOX_RESOURCES_PATH"scrollbutton_down_pressed.png");
	Images[SCROLLSURFACE] = IMG_Load(APPKIT_LISTBOX_RESOURCES_PATH"scrollsurface.png");

	/* RadioButton */
	Images[RADIOBUTTON_STATE1] = IMG_Load(APPKIT_BUTTON_RESOURCES_PATH"radiobutton_state1.PNG");
	Images[RADIOBUTTON_STATE2] = IMG_Load(APPKIT_BUTTON_RESOURCES_PATH"radiobutton_state2.PNG");
	Images[RADIOBUTTON_STATE3] = IMG_Load(APPKIT_BUTTON_RESOURCES_PATH"radiobutton_state3.PNG");

	// Check
	bool error = false;
	for (int i=0; i<IMAGE_RESOURCE_COUNT; i++) {
		if (!Images[i]) {
			char *msg = new char[80];
			sprintf_s(msg, strlen(msg), "Unable to load image resource %d", i);
			MessageBox(NULL, msg, "Error", MB_OK);
			error = true;
		}
	}
	if (error) exit(EXIT_FAILURE);

	Resources::ImageResourcesLoaded = true;
}

void Resources::LoadFontResources()
{
	if (Resources::FontResourcesLoaded) return;

	Fonts[DEFAULT_FONT] = TTF_OpenFont(APPKIT_FONTS_PATH"segoeui.ttf", 13);

	// Check
	bool error = false;
	for (int i=0; i<FONT_RESOURCE_COUNT; i++) {
		if (!Fonts[i]) {
			char *msg = new char[80];
			sprintf_s(msg, strlen(msg), "Unable to load font resource %d", i);
			MessageBox(NULL, msg, "Error", MB_OK);
			error = true;
		}
	}
	if (error) exit(EXIT_FAILURE);

	Resources::FontResourcesLoaded = true;
}

SDL_Surface* Resources::GetImageResource(int id)
{
	return Resources::Images[id];
}

TTF_Font* Resources::GetFontResource(int id)
{
	return Resources::Fonts[id];
}