#include "KEngine/Interfaces/IInputController.hpp"

#include "KEngine/Graphics/Imgui.hpp"


void ke::IMouseInputController::update(ke::Time)
{
}

bool ke::IMouseInputController::isAppWantCaptureMouse() const
{
    return ImGui::GetIO().WantCaptureMouse;
}

void ke::IKeyboardInputController::update(ke::Time)
{
}

bool ke::IKeyboardInputController::isAppWantCaptureKeyboard() const
{
    return ImGui::GetIO().WantCaptureKeyboard;
}

bool ke::IKeyboardInputController::isAppWantCaptureTextInput() const
{
    return ImGui::GetIO().WantTextInput;
}

void ke::IGamepadInputController::update(ke::Time)
{
}
