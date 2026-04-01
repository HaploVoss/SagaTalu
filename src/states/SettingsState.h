#pragma once

#include <cstdint>

#include "../config.h"
#include "../ui/views/SettingsViews.h"
#include "State.h"

#include "../wifi/WifiTransfer.h"

#include "PluginHostState.h"



class GfxRenderer;

namespace sumi {

enum class SettingsScreen : uint8_t {
  Menu,
  HomeArt,
  BleTransfer,  // Wireless file transfer (BLE Legacy)
  WifiTransfer, // Wireless file transfer (WiFi)
  WifiSetup,    // WiFi network configuration
  Reader,
  Device,
  Cleanup,
  SystemInfo,
  ConfirmDialog,
#if FEATURE_PLUGINS
  AppVisibility,
#endif
#if FEATURE_BLUETOOTH
  Bluetooth,
#endif
};

class SettingsState : public State {
 public:
  explicit SettingsState(GfxRenderer& renderer);
  ~SettingsState() override;

  void enter(Core& core) override;
  void exit(Core& core) override;
  StateTransition update(Core& core) override;
  void render(Core& core) override;
  StateId id() const override { return StateId::Settings; }

 private:
  GfxRenderer& renderer_;
  Core* core_;  // Stored for helper methods that don't receive Core&
  SettingsScreen currentScreen_;
  bool needsRender_;
  bool goHome_;
  bool goApps_ = false;
  bool goNotes_ = false;
  bool themeWasChanged_;

  // Pending action for confirmation dialog
  // 0=none, 10=Clear Book Cache, 11=Clear Device Storage, 12=Factory Reset
  uint8_t pendingAction_;

  // WiFi Transfer state
  WifiTransfer* wifiTransfer_ = nullptr;
  unsigned long wifiTransferLastRender_ = 0;
  
  // BLE Transfer state
  bool bleTransferEnabled_ = false;
  unsigned long lastBleUpdate_ = 0;
  int lastBleProgress_ = -1;
  bool bleCallbackRegistered_ = false;
  bool bleShowResult_ = false;       // Show result screen after transfer
  bool bleTransferDirty_ = false;    // Files were received, need refresh on exit
  bool bleQueueComplete_ = false;    // Full queue finished

  // Views (all small structs)
  ui::SettingsMenuView menuView_;
  ui::HomeArtSettingsView homeArtView_;
  ui::ReaderSettingsView readerView_;
  ui::DeviceSettingsView deviceView_;
  ui::CleanupMenuView cleanupView_;
  ui::SystemInfoView infoView_;
  ui::ConfirmDialogView confirmView_;
#if FEATURE_PLUGINS
  ui::AppVisibilityView appVisibilityView_;
#endif

  // Navigation helpers
  void openSelected();
  void goBack(Core& core);
  void handleConfirm(Core& core);
  void handleLeftRight(int delta);

  // Settings binding
  void loadReaderSettings();
  void saveReaderSettings();
  void loadDeviceSettings();
  void saveDeviceSettings();
  void loadHomeArtSettings();
  void saveHomeArtSettings();
  void populateSystemInfo();

  // App visibility
#if FEATURE_PLUGINS
  void loadAppVisibility();
  void saveAppVisibility();
#endif

  // Actions
  void clearCache(int type, Core& core);
  
  // BLE File Transfer
  void enterBleTransfer();
  void renderBleTransfer();
  void updateBleTransfer();
  // WiFi File Transfer
  void enterWifiTransfer();
  void renderWifiTransfer();
  void exitWifiTransfer();

  // WiFi Setup
  void enterWifiSetup();
  void updateWifiSetup();
  void renderWifiSetup();
  void exitWifiSetup();

  // WiFi setup state
  enum class WifiSetupScreen : uint8_t {
    Scanning,
    NetworkList,
    PasswordEntry,
    Connecting,
    Connected,
    Failed,
  };
  WifiSetupScreen wifiSetupScreen_ = WifiSetupScreen::Scanning;
  static constexpr int MAX_WIFI_NETWORKS = 10;
  struct WifiNetwork {
    char ssid[33];
    int32_t rssi;
    bool encrypted;
  };
  WifiNetwork wifiNetworks_[MAX_WIFI_NETWORKS];
  int wifiNetworkCount_ = 0;
  int wifiNetworkSelected_ = 0;
  char wifiPasswordBuf_[64] = {0};
  int wifiPasswordLen_ = 0;
  // On-screen keyboard state
  int kbRow_ = 0;
  int kbCol_ = 0;
  bool kbShift_ = false;
  bool kbNumbers_ = false;
  int kbRowLen(int row) const;
  bool wifiSetupNeedsRender_ = false;

#if FEATURE_BLUETOOTH
  // Bluetooth screen state
  int8_t btSelected_ = 0;
  bool btScanned_ = false;
  bool btConnecting_ = false;

  // Saved devices appended after scan results (not found in scan)
  struct SavedDevice {
    char name[32];   // "Page Turner" or "Keyboard"
    char addr[18];   // MAC address
  };
  SavedDevice btSaved_[2];  // Max 2 saved devices (page turner + keyboard)
  int8_t btSavedCount_ = 0;
  int btTotalCount() const;   // scanResultCount + btSavedCount_
  void populateSavedDevices();  // Fill btSaved_ with addresses not in scan

  void enterBluetooth();
  void renderBluetooth();
#endif
};

}  // namespace sumi
