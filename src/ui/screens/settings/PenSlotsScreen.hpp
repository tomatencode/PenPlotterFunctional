#pragma once

#include "ui/framework/screen/Screen.hpp"
#include "ui/framework/router/Router.hpp"
#include "ui/components/HeaderLine.hpp"
#include "ui/components/LabeledValueSelector.hpp"
#include "ui/framework/widgets/leaves/Button.hpp"
#include "ui/framework/widgets/leaves/Label.hpp"
#include "ui/framework/widgets/layouts/LinearLayout.hpp"
#include "ui/framework/widgets/layouts/ScrollableVerticalLayout.hpp"
#include "ui/screens/ScreensContext.hpp"

namespace ui {
namespace screens {

class PenSlotsScreen : public Screen
{
public:
    PenSlotsScreen(const ScreensContext& ctx)
    : Screen(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},

            std::make_unique<components::HeaderLine>(components::HeaderLineProps{
                .textProvider = "Pen Slots",
                .wifiStatusProvider = ctx.wifiStatusProvider,
                .onBackPress = [this]() {
                    if (router()) router()->popScreen();
                }
            }),

            std::make_unique<widgets::ScrollableVerticalLayout>(
                widgets::ScrollableVerticalLayoutStyle{},
                
                std::make_unique<widgets::Label>("Pen 1:"),

                std::make_unique<components::LabeledValueSelector<float>>(components::LabeledValueSelectorProps<float>{
                    .labelText = "Stroke",
                    .valueSelectorProps = widgets::ValueSelectorProps<float>{
                        .getValue = [&rs = ctx.runtimeSettings]() { 
                            auto penSlots = rs.penSlots();
                            return penSlots.size() > 0 ? static_cast<float>(penSlots[0].stroke) : 0; 
                        },
                        .next = [&rs = ctx.runtimeSettings, &ps = ctx.settingsPersistence](float v) { 
                            auto penSlots = rs.penSlots();
                            if (penSlots.size() > 0) {
                                penSlots[0].stroke = v + 0.1; 
                                ps.setPenSlots(penSlots);
                            }
                        },
                        .prev = [&rs = ctx.runtimeSettings, &ps = ctx.settingsPersistence](float v) { 
                            auto penSlots = rs.penSlots();
                            if (penSlots.size() > 0) {
                                penSlots[0].stroke = v - 0.1; 
                                ps.setPenSlots(penSlots);
                            }
                        },
                        .toString = [](const float& value) {
                            char buf[16];
                            snprintf(buf, sizeof(buf), "%.1f mm", value);
                            return std::string(buf);
                        }
                    }
                }),

                std::make_unique<widgets::Label>("Color (RGBA):"),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = " R",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .getValue = [&rs = ctx.runtimeSettings]() { 
                            auto penSlots = rs.penSlots();
                            return penSlots.size() > 0 ? static_cast<int>(penSlots[0].color[0]) : 0; 
                        },
                        .next = [&rs = ctx.runtimeSettings, &ps = ctx.settingsPersistence](int v) { 
                            auto penSlots = rs.penSlots();
                            if (penSlots.size() > 0) {
                                penSlots[0].color[0] = static_cast<uint8_t>(v + 1); 
                                ps.setPenSlots(penSlots);
                            }
                        },
                        .prev = [&rs = ctx.runtimeSettings, &ps = ctx.settingsPersistence](int v) { 
                            auto penSlots = rs.penSlots();
                            if (penSlots.size() > 0) {
                                penSlots[0].color[0] = static_cast<uint8_t>(v - 1); 
                                ps.setPenSlots(penSlots);
                            }
                        },
                        .toString = [](const int& value) { return std::to_string(value); }
                    }  
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = " G",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .getValue = [&rs = ctx.runtimeSettings]() { 
                            auto penSlots = rs.penSlots();
                            return penSlots.size() > 0 ? static_cast<int>(penSlots[0].color[1]) : 0; 
                        },
                        .next = [&rs = ctx.runtimeSettings, &ps = ctx.settingsPersistence](int v) { 
                            auto penSlots = rs.penSlots();
                            if (penSlots.size() > 0) {
                                penSlots[0].color[1] = static_cast<uint8_t>(v + 1); 
                                ps.setPenSlots(penSlots);
                            }
                        },
                        .prev = [&rs = ctx.runtimeSettings, &ps = ctx.settingsPersistence](int v) { 
                            auto penSlots = rs.penSlots();
                            if (penSlots.size() > 0) {
                                penSlots[0].color[1] = static_cast<uint8_t>(v - 1); 
                                ps.setPenSlots(penSlots);
                            }
                        },
                        .toString = [](const int& value) { return std::to_string(value); }
                    }  
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = " B",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .getValue = [&rs = ctx.runtimeSettings]() { 
                            auto penSlots = rs.penSlots();
                            return penSlots.size() > 0 ? static_cast<int>(penSlots[0].color[2]) : 0; 
                        },
                        .next = [&rs = ctx.runtimeSettings, &ps = ctx.settingsPersistence](int v) { 
                            auto penSlots = rs.penSlots();
                            if (penSlots.size() > 0) {
                                penSlots[0].color[2] = static_cast<uint8_t>(v + 1); 
                                ps.setPenSlots(penSlots);
                            }
                        },
                        .prev = [&rs = ctx.runtimeSettings, &ps = ctx.settingsPersistence](int v) { 
                            auto penSlots = rs.penSlots();
                            if (penSlots.size() > 0) {
                                penSlots[0].color[2] = static_cast<uint8_t>(v - 1); 
                                ps.setPenSlots(penSlots);
                            }
                        },
                        .toString = [](const int& value) { return std::to_string(value); }
                    }  
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = " A",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .getValue = [&rs = ctx.runtimeSettings]() { 
                            auto penSlots = rs.penSlots();
                            return penSlots.size() > 0 ? static_cast<int>(penSlots[0].color[3]) : 0; 
                        },
                        .next = [&rs = ctx.runtimeSettings, &ps = ctx.settingsPersistence](int v) { 
                            auto penSlots = rs.penSlots();
                            if (penSlots.size() > 0) {
                                penSlots[0].color[3] = static_cast<uint8_t>(v + 1); 
                                ps.setPenSlots(penSlots);
                            }
                        },
                        .prev = [&rs = ctx.runtimeSettings, &ps = ctx.settingsPersistence](int v) { 
                            auto penSlots = rs.penSlots();
                            if (penSlots.size() > 0) {
                                penSlots[0].color[3] = static_cast<uint8_t>(v - 1); 
                                ps.setPenSlots(penSlots);
                            }
                        },
                        .toString = [](const int& value) { return std::to_string(value); }
                        }
                    }
                )
            )
        )
    )
    {}
};

} // namespace screens
} // namespace ui