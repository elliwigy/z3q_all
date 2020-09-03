/* Copyright (c) 2015, The Linux Foundataion. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above
*       copyright notice, this list of conditions and the following
*       disclaimer in the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of The Linux Foundation nor the names of its
*       contributors may be used to endorse or promote products derived
*       from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
* ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#ifndef __HWC_COLOR_MANAGER_H__
#define __HWC_COLOR_MANAGER_H__

#include <stdlib.h>
#include <core/sdm_types.h>
#include <utils/locker.h>
#include <binder/Parcel.h>
#include <powermanager/IPowerManager.h>
#include <binder/BinderService.h>

namespace sdm {

// This macro defines name for display APIs interface wrapper library.
// This macro shall be used to load library using dlopen().
#define DISPLAY_API_INTERFACE_LIBRARY_NAME "libsdm-display-apis.so"

// This macro defines variable name of display color APIs function tables
// This macro shall be used to specify name of the variable in dlsym().
#define DISPLAY_API_FUNC_TABLES "display_color_apis_ftables"
#define QDCM_DIAG_CLIENT_LIBRARY_NAME "libsdm-diag.so"
#define INIT_QDCM_DIAG_CLIENT_NAME "QDCMDiagInit"
#define DEINIT_QDCM_DIAG_CLIENT_NAME "QDCMDiagDeInit"

typedef int (*QDCMDiagInit)(void *ftables);

typedef int (*QDCMDiagDeInit)(void);

// Class to encapsulte all details of managing QDCM operating mode.
class HWCQDCMModeManager {
 public:
  static const uint32_t kSocketCMDMaxLength = 4096;
  static const uint32_t kFullWakeLock = 0x0000001a;
  static const uint32_t kAcquireCauseWakeup = 0x10000000;
  static const uint32_t kONAfterRelease = 0x20000000;
  enum ActiveFeatureID {
    kCABLFeature,
    kADFeature,
    kSVIFeature,
    kMaxNumActiveFeature,
  };

  struct ActiveFeatureCMD {
    const char *cmd_on;
    const char *cmd_off;
    const char *cmd_query_status;
    const char *running;
    ActiveFeatureCMD(const char *arg1, const char *arg2, const char *arg3, const char *arg4)
        : cmd_on(arg1), cmd_off(arg2), cmd_query_status(arg3), running(arg4) {}
    ActiveFeatureCMD() : cmd_on(NULL), cmd_off(NULL), cmd_query_status(NULL), running(NULL) {}
  };

  static const ActiveFeatureCMD kActiveFeatureCMD[kMaxNumActiveFeature];

 public:
  static HWCQDCMModeManager *CreateQDCMModeMgr();
  ~HWCQDCMModeManager();
  int EnableQDCMMode(bool enable);

 protected:
  HWCQDCMModeManager()
      : cabl_was_running_(false), socket_fd_(-1), wakelock_token_(NULL), power_mgr_(NULL) {}
  bool SendSocketCmd();
  int AcquireAndroidWakeLock(bool enable);
  int EnableActiveFeatures(bool enable);
  int EnableActiveFeatures(bool enable, const ActiveFeatureCMD &cmds, bool *was_running);

 private:
  bool cabl_was_running_;
  int socket_fd_;
  android::sp<android::IBinder> wakelock_token_;
  android::sp<android::IPowerManager> power_mgr_;
  static const char *const kSocketName;
  static const char *const kTagName;
  static const char *const kPackageName;
};

// Class to encapsulte all HWC/OS specific behaviours for ColorManager.
class HWCColorManager {
 public:
  static HWCColorManager *CreateColorManager();
  static int CreatePayloadFromParcel(const android::Parcel &in, uint32_t *disp_id,
                                     PPDisplayAPIPayload *sink);
  static void MarshallStructIntoParcel(const PPDisplayAPIPayload &data,
                                       android::Parcel &out_parcel);

  ~HWCColorManager();
  void DestroyColorManager();
  int EnableQDCMMode(bool enable);

 protected:
  HWCColorManager()
      : color_apis_lib_(NULL), diag_client_lib_(NULL), color_apis_(NULL), qdcm_mode_mgr_(NULL) {}

 private:
  void *color_apis_lib_, *diag_client_lib_;
  void *color_apis_;
  QDCMDiagInit qdcm_diag_init_;
  QDCMDiagDeInit qdcm_diag_deinit_;
  HWCQDCMModeManager *qdcm_mode_mgr_;
};

}  // namespace sdm

#endif  // __HWC_COLOR_MANAGER_H__
