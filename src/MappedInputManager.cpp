#include "MappedInputManager.h"

#include "core/SagaTaluSettings.h"

decltype(InputManager::BTN_BACK) MappedInputManager::mapButton(const Button button) const {
  const auto frontLayout = settings_ ? static_cast<sagatalu::Settings::FrontButtonLayout>(settings_->frontButtonLayout)
                                     : sagatalu::Settings::FrontBCLR;
  const auto sideLayout = settings_ ? static_cast<sagatalu::Settings::SideButtonLayout>(settings_->sideButtonLayout)
                                    : sagatalu::Settings::PrevNext;

  switch (button) {
    case Button::Back:
      switch (frontLayout) {
        case sagatalu::Settings::FrontLRBC:
          return InputManager::BTN_LEFT;
        case sagatalu::Settings::FrontBCLR:
        default:
          return InputManager::BTN_BACK;
      }
    case Button::Confirm:
      switch (frontLayout) {
        case sagatalu::Settings::FrontLRBC:
          return InputManager::BTN_RIGHT;
        case sagatalu::Settings::FrontBCLR:
        default:
          return InputManager::BTN_CONFIRM;
      }
    case Button::Left:
      switch (frontLayout) {
        case sagatalu::Settings::FrontLRBC:
          return InputManager::BTN_BACK;
        case sagatalu::Settings::FrontBCLR:
        default:
          return InputManager::BTN_LEFT;
      }
    case Button::Right:
      switch (frontLayout) {
        case sagatalu::Settings::FrontLRBC:
          return InputManager::BTN_CONFIRM;
        case sagatalu::Settings::FrontBCLR:
        default:
          return InputManager::BTN_RIGHT;
      }
    case Button::Up:
      switch (sideLayout) {
        case sagatalu::Settings::NextPrev:
          return InputManager::BTN_DOWN;
        case sagatalu::Settings::PrevNext:
        default:
          return InputManager::BTN_UP;
      }
    case Button::Down:
      switch (sideLayout) {
        case sagatalu::Settings::NextPrev:
          return InputManager::BTN_UP;
        case sagatalu::Settings::PrevNext:
        default:
          return InputManager::BTN_DOWN;
      }
    case Button::Power:
      return InputManager::BTN_POWER;
    case Button::PageBack:
      switch (sideLayout) {
        case sagatalu::Settings::NextPrev:
          return InputManager::BTN_DOWN;
        case sagatalu::Settings::PrevNext:
        default:
          return InputManager::BTN_UP;
      }
    case Button::PageForward:
      switch (sideLayout) {
        case sagatalu::Settings::NextPrev:
          return InputManager::BTN_UP;
        case sagatalu::Settings::PrevNext:
        default:
          return InputManager::BTN_DOWN;
      }
  }

  return InputManager::BTN_BACK;
}

bool MappedInputManager::wasPressed(const Button button) const { return inputManager.wasPressed(mapButton(button)); }

bool MappedInputManager::wasReleased(const Button button) const { return inputManager.wasReleased(mapButton(button)); }

bool MappedInputManager::isPressed(const Button button) const { return inputManager.isPressed(mapButton(button)); }

bool MappedInputManager::wasAnyPressed() const { return inputManager.wasAnyPressed(); }

bool MappedInputManager::wasAnyReleased() const { return inputManager.wasAnyReleased(); }

unsigned long MappedInputManager::getHeldTime() const { return inputManager.getHeldTime(); }
