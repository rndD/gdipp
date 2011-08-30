#pragma once

#include "gdipp_client/painter.h"
#include "gdipp_rpc/gdipp_rpc.h"

namespace gdipp
{

struct painter_tls
{
	HDC hdc_canvas;
	BYTE *text_bits;

	HDC last_hdc;
};

class gdi_painter : public painter
{
public:
	bool begin(const dc_context *context);
	bool paint(int x, int y, UINT options, CONST RECT *lprect, const void *text, UINT c, CONST INT *lpDx);

private:
	struct glyph_run_metrics
	{
		/*
		extent and baseline determine the bounding box before clipping
		visible rectangle is the visible part after optional clipping
		the area of visible rectangle is always less or equal to the extent
		*/
		SIZE extent;
		POINT baseline;
		RECT visible_rect;
	};

	// adjust the glyph boxes from distance array
	static void adjust_glyph_bbox(bool is_pdy, UINT count, CONST INT *lpDx, gdipp_rpc_bitmap_glyph_run *a_glyph_run);

	void set_mono_mask_bits(const FT_BitmapGlyph glyph,
		const RECT &src_rect,
		BYTE *dest_bits,
		const RECT &dest_rect,
		int dest_pitch,
		bool project_bottom) const;
	void set_gray_text_bits(const FT_BitmapGlyph glyph,
		const RECT &src_rect,
		BYTE *dest_bits,
		const RECT &dest_rect,
		int dest_pitch,
		bool project_bottom) const;
	void set_lcd_text_bits(const FT_BitmapGlyph glyph,
		const RECT &src_rect,
		BYTE *dest_bits,
		const RECT &dest_rect,
		int dest_pitch,
		bool project_bottom,
		BYTE alpha) const;

	BOOL paint_mono(UINT options, CONST RECT *lprect, const gdipp_rpc_bitmap_glyph_run *a_glyph_run, const glyph_run_metrics &grm) const;
	BOOL paint_gray(UINT options, CONST RECT *lprect, const gdipp_rpc_bitmap_glyph_run *a_glyph_run, const glyph_run_metrics &grm) const;
	BOOL paint_lcd(UINT options, CONST RECT *lprect, const gdipp_rpc_bitmap_glyph_run *a_glyph_run, const glyph_run_metrics &grm) const;
	BOOL paint_glyph_run(UINT options, CONST RECT *lprect, const gdipp_rpc_bitmap_glyph_run *a_glyph_run);

	painter_tls *_tls;
	RGBQUAD _text_rgb_gamma;
	bool _update_cp;
};

}
