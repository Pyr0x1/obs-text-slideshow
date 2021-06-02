/*
obs-text-slideshow
Copyright (C) 2021 Joshua Wong jbwong05@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include "freetype2-source.h"

// text freetype2
#define S_FONT                         "font"
#define S_TEXT                         "text"
#define S_FROM_FILE                    "from_file"
#define S_ANTIALIASING                 "antialiasing"
#define S_LOG_MODE                     "log_mode"
#define S_LOG_LINES                    "log_lines"
#define S_TEXT_FILE                    "text_file"
#define S_COLOR_1                      "color1"
#define S_COLOR_2                      "color2"
#define S_OUTLINE                      "outline"
#define S_DROP_SHADOW                  "drop_shadow"
#define S_CUSTOM_WIDTH                 "custom_width"
#define S_WORD_WRAP                    "word_wrap"
#define S_FACE                         "face"
#define S_SIZE                         "size"
#define S_FLAGS                        "flags"
#define S_STYLE                        "style"

// text freetype2
#define T_TXT_(text) obs_module_text("Text." text)
#define T_FONT                         T_TXT_("Font")
#define T_TEXT                         T_TXT_("Text")
#define T_FROM_FILE                    T_TXT_("ReadFromFile")
#define T_ANTIALIASING                 T_TXT_("Antialiasing")
#define T_LOG_MODE                     T_TXT_("ChatLogMode")
#define T_LOG_LINES                    T_TXT_("ChatLogLines")
#define T_TEXT_FILE                    T_TXT_("TextFile")
#define T_TEXT_FILE_FILTER             T_TXT_("TextFileFilter")
#define T_COLOR_1                      T_TXT_("Color1")
#define T_COLOR_2                      T_TXT_("Color2")
#define T_OUTLINE                      T_TXT_("Outline")
#define T_DROP_SHADOW                  T_TXT_("DropShadow")
#define T_CUSTOM_WIDTH                 T_TXT_("CustomWidth")
#define T_WORD_WRAP                    T_TXT_("WordWrap")

#ifdef _WIN32
#define DEFAULT_FACE "Arial"
#elif __APPLE__
#define DEFAULT_FACE "Helvetica"
#else
#define DEFAULT_FACE "Sans Serif"
#endif

obs_source_t *freetype2_create_source(const char *text, 
		obs_data_t *text_ss_settings) {
	obs_data_t *settings = obs_data_create();
	obs_source_t *source;

	obs_data_set_obj(settings, S_FONT, obs_data_get_obj(text_ss_settings, S_FONT));
	obs_data_set_bool(settings, S_DROP_SHADOW, 
		obs_data_get_bool(text_ss_settings, S_DROP_SHADOW));
	obs_data_set_bool(settings, S_OUTLINE, 
		obs_data_get_bool(text_ss_settings, S_OUTLINE));
	obs_data_set_bool(settings, S_WORD_WRAP, 
		obs_data_get_bool(text_ss_settings, S_WORD_WRAP));
	obs_data_set_int(settings, S_COLOR_1, 
		obs_data_get_int(text_ss_settings, S_COLOR_1));
	obs_data_set_int(settings, S_COLOR_2, 
		obs_data_get_int(text_ss_settings, S_COLOR_2));
	obs_data_set_int(settings, S_CUSTOM_WIDTH, 
		obs_data_get_int(text_ss_settings, S_CUSTOM_WIDTH));
	obs_data_set_bool(settings, S_FROM_FILE, false);
	obs_data_set_bool(settings, S_LOG_MODE, 
		obs_data_get_bool(text_ss_settings, S_LOG_MODE));
	obs_data_set_int(settings, S_LOG_LINES, 
		obs_data_get_int(text_ss_settings, S_LOG_LINES));
	obs_data_set_bool(settings, S_ANTIALIASING, 
		obs_data_get_bool(text_ss_settings, S_ANTIALIASING));
	obs_data_set_string(settings, S_TEXT_FILE, "");
	obs_data_set_string(settings, S_TEXT, text);
	source = obs_source_create_private("text_ft2_source", NULL, settings);

	obs_data_release(settings);

	return source;
}

void freetype2_text_defaults(obs_data_t *settings) {
	obs_data_t *font_obj = obs_data_create();

	// Currently only supporting text freetype2
	const uint16_t font_size = 256;

	obs_data_set_default_string(font_obj, S_FACE, DEFAULT_FACE);
	obs_data_set_default_int(font_obj, S_SIZE, font_size);
	obs_data_set_default_int(font_obj, S_FLAGS, 0);
	obs_data_set_default_string(font_obj, S_STYLE, "");
	obs_data_set_default_obj(settings, S_FONT, font_obj);

	obs_data_set_default_bool(settings, S_ANTIALIASING, true);
	obs_data_set_default_bool(settings, S_WORD_WRAP, false);
	obs_data_set_default_bool(settings, S_OUTLINE, false);
	obs_data_set_default_bool(settings, S_DROP_SHADOW, false);

	obs_data_set_default_int(settings, S_LOG_LINES, 6);

	obs_data_set_default_int(settings, S_COLOR_1, 0xFFFFFFFF);
	obs_data_set_default_int(settings, S_COLOR_2, 0xFFFFFFFF);

	obs_data_release(font_obj);
}

void freetype2_text_properties(obs_properties_t *props) {
	// TODO:
	//	Scrolling. Can't think of a way to do it with the render
	//		targets currently being broken. (0.4.2)
	//	Better/pixel shader outline/drop shadow
	//	Some way to pull text files from network, I dunno

	obs_properties_add_font(props, S_FONT, T_FONT);

	obs_properties_add_bool(props, S_ANTIALIASING,
				T_ANTIALIASING);

	obs_properties_add_bool(props, S_LOG_MODE,
				T_LOG_MODE);

	obs_properties_add_int(props, S_LOG_LINES,
			       T_LOG_LINES, 1, 1000, 1);

	obs_properties_add_color(props, S_COLOR_1, T_COLOR_1);

	obs_properties_add_color(props, S_COLOR_2, T_COLOR_2);

	obs_properties_add_bool(props, S_OUTLINE, T_OUTLINE);

	obs_properties_add_bool(props, S_DROP_SHADOW,
				T_DROP_SHADOW);

	obs_properties_add_int(props, S_CUSTOM_WIDTH,
			       T_CUSTOM_WIDTH, 0, 4096, 1);

	obs_properties_add_bool(props, S_WORD_WRAP,
				T_WORD_WRAP);
}