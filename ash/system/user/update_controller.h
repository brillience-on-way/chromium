// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_USER_UPDATE_CONTROLLER_H_
#define ASH_SYSTEM_USER_UPDATE_CONTROLLER_H_

#include "ash/ash_export.h"

namespace ash {

class ASH_EXPORT UpdateController {
 public:
  virtual ~UpdateController() {}

  virtual void OnUpdateRecommended() = 0;
};

}  // namespace ash

#endif  //ASH_SYSTEM_USER_UPDATE_CONTROLLER_H_
