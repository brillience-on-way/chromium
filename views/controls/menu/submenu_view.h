// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef VIEWS_CONTROLS_MENU_SUBMENU_VIEW_H_
#define VIEWS_CONTROLS_MENU_SUBMENU_VIEW_H_
#pragma once

#include <string>

#include "views/controls/menu/menu_delegate.h"
#include "views/view.h"

namespace views {

class MenuHost;
class MenuItemView;
class MenuScrollViewContainer;

// SubmenuView is the parent of all menu items.
//
// SubmenuView has the following responsibilities:
// . It positions and sizes all child views (any type of View may be added,
//   not just MenuItemViews).
// . Forwards the appropriate events to the MenuController. This allows the
//   MenuController to update the selection as the user moves the mouse around.
// . Renders the drop indicator during a drop operation.
// . Shows and hides the window (a WidgetWin) when the menu is shown on
//   screen.
//
// SubmenuView is itself contained in a MenuScrollViewContainer.
// MenuScrollViewContainer handles showing as much of the SubmenuView as the
// screen allows. If the SubmenuView is taller than the screen, scroll buttons
// are provided that allow the user to see all the menu items.
class SubmenuView : public View {
 public:
  // The submenu's class name.
  static const char kViewClassName[];

  // Creates a SubmenuView for the specified menu item.
  explicit SubmenuView(MenuItemView* parent);
  ~SubmenuView();

  // Returns the number of child views that are MenuItemViews.
  // MenuItemViews are identified by ID.
  int GetMenuItemCount();

  // Returns the MenuItemView at the specified index.
  MenuItemView* GetMenuItemAt(int index);

  // Positions and sizes the child views. This tiles the views vertically,
  // giving each child the available width.
  virtual void Layout();
  virtual gfx::Size GetPreferredSize();

  // View method. Overridden to schedule a paint. We do this so that when
  // scrolling occurs, everything is repainted correctly.
  virtual void OnBoundsChanged();

  // Override from View.
  virtual AccessibilityTypes::Role GetAccessibleRole();

  // Painting.
  void PaintChildren(gfx::Canvas* canvas);

  // Drag and drop methods. These are forwarded to the MenuController.
  virtual bool GetDropFormats(
      int* formats,
      std::set<OSExchangeData::CustomFormat>* custom_formats);
  virtual bool AreDropTypesRequired();
  virtual bool CanDrop(const OSExchangeData& data);
  virtual void OnDragEntered(const DropTargetEvent& event);
  virtual int OnDragUpdated(const DropTargetEvent& event);
  virtual void OnDragExited();
  virtual int OnPerformDrop(const DropTargetEvent& event);

  // Scrolls on menu item boundaries.
  virtual bool OnMouseWheel(const MouseWheelEvent& e);

  // Returns true if the menu is showing.
  bool IsShowing();

  // Shows the menu at the specified location. Coordinates are in screen
  // coordinates. max_width gives the max width the view should be.
  void ShowAt(gfx::NativeWindow parent,
              const gfx::Rect& bounds,
              bool do_capture);

  // Resets the bounds of the submenu to |bounds|.
  void Reposition(const gfx::Rect& bounds);

  // Closes the menu, destroying the host.
  void Close();

  // Hides the hosting window.
  //
  // The hosting window is hidden first, then deleted (Close) when the menu is
  // done running. This is done to avoid deletion ordering dependencies. In
  // particular, during drag and drop (and when a modal dialog is shown as
  // a result of choosing a context menu) it is possible that an event is
  // being processed by the host, so that host is on the stack when we need to
  // close the window. If we closed the window immediately (and deleted it),
  // when control returned back to host we would crash as host was deleted.
  void Hide();

  // If mouse capture was grabbed, it is released. Does nothing if mouse was
  // not captured.
  void ReleaseCapture();

  // Overriden from View to prevent tab from doing anything.
  virtual bool SkipDefaultKeyEventProcessing(const KeyEvent& e);

  // Returns the parent menu item we're showing children for.
  MenuItemView* GetMenuItem() const { return parent_menu_item_; }

  // Set the drop item and position.
  void SetDropMenuItem(MenuItemView* item,
                       MenuDelegate::DropPosition position);

  // Returns whether the selection should be shown for the specified item.
  // The selection is NOT shown during drag and drop when the drop is over
  // the menu.
  bool GetShowSelection(MenuItemView* item);

  // Returns the container for the SubmenuView.
  MenuScrollViewContainer* GetScrollViewContainer();

  // Returns the NativeWindow host of the menu, or NULL if not showing.
  gfx::NativeWindow native_window() const;

  // Invoked if the menu is prematurely destroyed. This can happen if the window
  // closes while the menu is shown. If invoked the SubmenuView must drop all
  // references to the MenuHost as the MenuHost is about to be deleted.
  void MenuHostDestroyed();

  // Max width of accelerators in child menu items. This doesn't include
  // children's children, only direct children.
  int max_accelerator_width() const { return max_accelerator_width_; }

  // Padding around the edges of the submenu.
  static const int kSubmenuBorderSize;

 protected:
  virtual std::string GetClassName() const;

 private:
  // Paints the drop indicator. This is only invoked if item is non-NULL and
  // position is not DROP_NONE.
  void PaintDropIndicator(gfx::Canvas* canvas,
                          MenuItemView* item,
                          MenuDelegate::DropPosition position);

  void SchedulePaintForDropIndicator(MenuItemView* item,
                                     MenuDelegate::DropPosition position);

  // Calculates the location of th edrop indicator.
  gfx::Rect CalculateDropIndicatorBounds(MenuItemView* item,
                                         MenuDelegate::DropPosition position);

  // Parent menu item.
  MenuItemView* parent_menu_item_;

  // Widget subclass used to show the children. This is deleted when we invoke
  // |DestroyMenuHost|, or |MenuHostDestroyed| is invoked back on us.
  MenuHost* host_;

  // If non-null, indicates a drop is in progress and drop_item is the item
  // the drop is over.
  MenuItemView* drop_item_;

  // Position of the drop.
  MenuDelegate::DropPosition drop_position_;

  // Ancestor of the SubmenuView, lazily created.
  MenuScrollViewContainer* scroll_view_container_;

  // See description above getter.
  int max_accelerator_width_;

  DISALLOW_COPY_AND_ASSIGN(SubmenuView);
};

}  // namespace views

#endif  // VIEWS_CONTROLS_MENU_SUBMENU_VIEW_H_
