// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/gtk/nine_box.h"

#include "app/gfx/gtk_util.h"
#include "app/resource_bundle.h"
#include "app/theme_provider.h"
#include "base/gfx/gtk_util.h"
#include "base/gfx/point.h"
#include "base/logging.h"

namespace {

// Draw pixbuf |src| into |dst| at position (x, y).
void DrawPixbuf(cairo_t* cr, GdkPixbuf* src, int x, int y) {
  gdk_cairo_set_source_pixbuf(cr, src, x, y);
  cairo_paint(cr);
}

// Tile pixbuf |src| across |cr| at |x|, |y| for |width| and |height|.
void TileImage(cairo_t* cr, GdkPixbuf* src,
               int x, int y, int width, int height) {
  gdk_cairo_set_source_pixbuf(cr, src, x, y);
  cairo_pattern_set_extend(cairo_get_source(cr), CAIRO_EXTEND_REPEAT);
  cairo_rectangle(cr, x, y, width, height);
  cairo_fill(cr);
}

GdkPixbuf* GetPixbufNamed(ThemeProvider* theme_provider, int name) {
  return gfx::GdkPixbufFromSkBitmap(theme_provider->GetBitmapNamed(name));
}

}  // namespace

NineBox::NineBox(int top_left, int top, int top_right, int left, int center,
                 int right, int bottom_left, int bottom, int bottom_right) {
  ResourceBundle& rb = ResourceBundle::GetSharedInstance();
  images_[0] = top_left ? rb.GetPixbufNamed(top_left) : NULL;
  images_[1] = top ? rb.GetPixbufNamed(top) : NULL;
  images_[2] = top_right ? rb.GetPixbufNamed(top_right) : NULL;
  images_[3] = left ? rb.GetPixbufNamed(left) : NULL;
  images_[4] = center ? rb.GetPixbufNamed(center) : NULL;
  images_[5] = right ? rb.GetPixbufNamed(right) : NULL;
  images_[6] = bottom_left ? rb.GetPixbufNamed(bottom_left) : NULL;
  images_[7] = bottom ? rb.GetPixbufNamed(bottom) : NULL;
  images_[8] = bottom_right ? rb.GetPixbufNamed(bottom_right) : NULL;
}

NineBox::NineBox(ThemeProvider* theme_provider,
                 int top_left, int top, int top_right, int left, int center,
                 int right, int bottom_left, int bottom, int bottom_right) {
  images_[0] = top_left ?
               GetPixbufNamed(theme_provider, top_left) : NULL;
  images_[1] = top ?
               GetPixbufNamed(theme_provider, top) : NULL;
  images_[2] = top_right ?
               GetPixbufNamed(theme_provider, top_right) : NULL;
  images_[3] = left ?
               GetPixbufNamed(theme_provider, left) : NULL;
  images_[4] = center ?
               GetPixbufNamed(theme_provider, center) : NULL;
  images_[5] = right ?
               GetPixbufNamed(theme_provider, right) : NULL;
  images_[6] = bottom_left ?
               GetPixbufNamed(theme_provider, bottom_left) : NULL;
  images_[7] = bottom ?
               GetPixbufNamed(theme_provider, bottom) : NULL;
  images_[8] = bottom_right ?
               GetPixbufNamed(theme_provider, bottom_right) : NULL;
}

NineBox::~NineBox() {
}

void NineBox::RenderToWidget(GtkWidget* dst) const {
  int dst_width = dst->allocation.width;
  int dst_height = dst->allocation.height;

  cairo_t* cr = gdk_cairo_create(GDK_DRAWABLE(dst->window));
  // For widgets that have their own window, the allocation (x,y) coordinates
  // are GdkWindow relative. For other widgets, the coordinates are relative
  // to their container.
  if (GTK_WIDGET_NO_WINDOW(dst)) {
    // Transform our cairo from window to widget coordinates.
    cairo_translate(cr, dst->allocation.x, dst->allocation.y);
  }

  // The upper-left and lower-right corners of the center square in the
  // rendering of the ninebox.
  int x1 = gdk_pixbuf_get_width(images_[0]);
  int y1 = gdk_pixbuf_get_height(images_[0]);
  int x2 = images_[2] ? dst_width - gdk_pixbuf_get_width(images_[2]) : x1;
  int y2 = images_[6] ? dst_height - gdk_pixbuf_get_height(images_[6]) : y1;
  DCHECK_GE(x2, x1);
  DCHECK_GE(y2, y1);

  // Top row, center image is horizontally tiled.
  if (images_[0])
    DrawPixbuf(cr, images_[0], 0, 0);
  if (images_[1])
    RenderTopCenterStrip(cr, x1, 0, x2 - x1);
  if (images_[2])
    DrawPixbuf(cr, images_[2], x2, 0);

  // Center row, all images are vertically tiled, center is horizontally tiled.
  if (images_[3])
    TileImage(cr, images_[3], 0, y1, x1, y2 - y1);
  if (images_[4])
    TileImage(cr, images_[4], x1, y1, x2 - x1, y2 - y1);
  if (images_[5])
    TileImage(cr, images_[5], x2, y1, dst_width - x2, y2 - y1);

  // Bottom row, center image is horizontally tiled.
  if (images_[6])
    DrawPixbuf(cr, images_[6], 0, y2);
  if (images_[7])
    TileImage(cr, images_[7], x1, y2, x2 - x1, dst_height - y2);
  if (images_[8])
    DrawPixbuf(cr, images_[8], x2, y2);

  cairo_destroy(cr);
}

void NineBox::RenderTopCenterStrip(cairo_t* cr,
                                   int x, int y, int width) const {
  const int height = gdk_pixbuf_get_height(images_[1]);
  TileImage(cr, images_[1], x, y, width, height);
}

void NineBox::ChangeWhiteToTransparent() {
  for (int image_idx = 0; image_idx < 9; ++image_idx) {
    GdkPixbuf* pixbuf = images_[image_idx];
    if (!pixbuf)
      continue;

    guchar* pixels = gdk_pixbuf_get_pixels(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);

    for (int i = 0; i < gdk_pixbuf_get_height(pixbuf); ++i) {
      for (int j = 0; j < gdk_pixbuf_get_width(pixbuf); ++j) {
         guchar* pixel = &pixels[i * rowstride + j * 4];
         if (pixel[0] == 0xff && pixel[1] == 0xff && pixel[2] == 0xff) {
           pixel[3] = 0;
         }
      }
    }
  }
}

void NineBox::ContourWidget(GtkWidget* widget) const {
  int x1 = gdk_pixbuf_get_width(images_[0]);
  int x2 = widget->allocation.width - gdk_pixbuf_get_width(images_[2]);

  // Paint the left and right sides.
  GdkBitmap* mask = gdk_pixmap_new(NULL, widget->allocation.width,
                                   widget->allocation.height, 1);
  gdk_pixbuf_render_threshold_alpha(images_[0], mask,
                                    0, 0,
                                    0, 0, -1, -1,
                                    1);
  gdk_pixbuf_render_threshold_alpha(images_[2], mask,
                                    0, 0,
                                    x2, 0, -1, -1,
                                    1);

  // Assume no transparency in the middle rectangle.
  cairo_t* cr = gdk_cairo_create(mask);
  cairo_rectangle(cr, x1, 0, x2 - x1, widget->allocation.height);
  cairo_fill(cr);

  // Mask the widget's window's shape.
  gtk_widget_shape_combine_mask(widget, mask, 0, 0);

  g_object_unref(mask);
  cairo_destroy(cr);
}
