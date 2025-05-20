#include "Application.h"

Application::Application(std::unique_ptr<IUIFactory> factory)
    : factory_(std::move(factory)) {}

void Application::renderUI() const {
    auto button = factory_->createButton();
    auto checkbox = factory_->createCheckbox();
    auto textField = factory_->createTextField();

    button->render();
    checkbox->render();
    textField->render();
}
