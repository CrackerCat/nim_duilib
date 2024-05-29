#include "UiColors.h"

//包含全部基本类型数据结构，以在开发阶段测试是否有编译错误等
#include "duilib/Core/UiColor.h"
#include "duilib/Core/UiFont.h"
#include "duilib/Core/UiRect.h"
#include "duilib/Core/UiSize.h"
#include "duilib/Core/UiPoint.h"

namespace ui 
{
namespace UiColors
{
    /** 常见颜色值，对应的字符串常量
    */
    void GetUiColorsString(std::vector<std::pair<DString, int32_t>>& uiColors)
    {
        uiColors = {
                    {_T("AliceBlue"),UiColors::AliceBlue},
                    {_T("AntiqueWhite"),UiColors::AntiqueWhite},
                    {_T("Aqua"),UiColors::Aqua},
                    {_T("Aquamarine"),UiColors::Aquamarine},
                    {_T("Azure"),UiColors::Azure},
                    {_T("Beige"),UiColors::Beige},
                    {_T("Bisque"),UiColors::Bisque},
                    {_T("Black"),UiColors::Black},
                    {_T("BlanchedAlmond"),UiColors::BlanchedAlmond},
                    {_T("Blue"),UiColors::Blue},
                    {_T("BlueViolet"),UiColors::BlueViolet},
                    {_T("Brown"),UiColors::Brown},
                    {_T("BurlyWood"),UiColors::BurlyWood},
                    {_T("CadetBlue"),UiColors::CadetBlue},
                    {_T("Chartreuse"),UiColors::Chartreuse},
                    {_T("Chocolate"),UiColors::Chocolate},
                    {_T("Coral"),UiColors::Coral},
                    {_T("CornflowerBlue"),UiColors::CornflowerBlue},
                    {_T("Cornsilk"),UiColors::Cornsilk},
                    {_T("Crimson"),UiColors::Crimson},
                    {_T("Cyan"),UiColors::Cyan},
                    {_T("DarkBlue"),UiColors::DarkBlue},
                    {_T("DarkCyan"),UiColors::DarkCyan},
                    {_T("DarkGoldenrod"),UiColors::DarkGoldenrod},
                    {_T("DarkGray"),UiColors::DarkGray},
                    {_T("DarkGreen"),UiColors::DarkGreen},
                    {_T("DarkKhaki"),UiColors::DarkKhaki},
                    {_T("DarkMagenta"),UiColors::DarkMagenta},
                    {_T("DarkOliveGreen"),UiColors::DarkOliveGreen},
                    {_T("DarkOrange"),UiColors::DarkOrange},
                    {_T("DarkOrchid"),UiColors::DarkOrchid},
                    {_T("DarkRed"),UiColors::DarkRed},
                    {_T("DarkSalmon"),UiColors::DarkSalmon},
                    {_T("DarkSeaGreen"),UiColors::DarkSeaGreen},
                    {_T("DarkSlateBlue"),UiColors::DarkSlateBlue},
                    {_T("DarkSlateGray"),UiColors::DarkSlateGray},
                    {_T("DarkTurquoise"),UiColors::DarkTurquoise},
                    {_T("DarkViolet"),UiColors::DarkViolet},
                    {_T("DeepPink"),UiColors::DeepPink},
                    {_T("DeepSkyBlue"),UiColors::DeepSkyBlue},
                    {_T("DimGray"),UiColors::DimGray},
                    {_T("DodgerBlue"),UiColors::DodgerBlue},
                    {_T("Firebrick"),UiColors::Firebrick},
                    {_T("FloralWhite"),UiColors::FloralWhite},
                    {_T("ForestGreen"),UiColors::ForestGreen},
                    {_T("Fuchsia"),UiColors::Fuchsia},
                    {_T("Gainsboro"),UiColors::Gainsboro},
                    {_T("GhostWhite"),UiColors::GhostWhite},
                    {_T("Gold"),UiColors::Gold},
                    {_T("Goldenrod"),UiColors::Goldenrod},
                    {_T("Gray"),UiColors::Gray},
                    {_T("Green"),UiColors::Green},
                    {_T("GreenYellow"),UiColors::GreenYellow},
                    {_T("Honeydew"),UiColors::Honeydew},
                    {_T("HotPink"),UiColors::HotPink},
                    {_T("IndianRed"),UiColors::IndianRed},
                    {_T("Indigo"),UiColors::Indigo},
                    {_T("Ivory"),UiColors::Ivory},
                    {_T("Khaki"),UiColors::Khaki},
                    {_T("Lavender"),UiColors::Lavender},
                    {_T("LavenderBlush"),UiColors::LavenderBlush},
                    {_T("LawnGreen"),UiColors::LawnGreen},
                    {_T("LemonChiffon"),UiColors::LemonChiffon},
                    {_T("LightBlue"),UiColors::LightBlue},
                    {_T("LightCoral"),UiColors::LightCoral},
                    {_T("LightCyan"),UiColors::LightCyan},
                    {_T("LightGoldenrodYellow"),UiColors::LightGoldenrodYellow},
                    {_T("LightGray"),UiColors::LightGray},
                    {_T("LightGreen"),UiColors::LightGreen},
                    {_T("LightPink"),UiColors::LightPink},
                    {_T("LightSalmon"),UiColors::LightSalmon},
                    {_T("LightSeaGreen"),UiColors::LightSeaGreen},
                    {_T("LightSkyBlue"),UiColors::LightSkyBlue},
                    {_T("LightSlateGray"),UiColors::LightSlateGray},
                    {_T("LightSteelBlue"),UiColors::LightSteelBlue},
                    {_T("LightYellow"),UiColors::LightYellow},
                    {_T("Lime"),UiColors::Lime},
                    {_T("LimeGreen"),UiColors::LimeGreen},
                    {_T("Linen"),UiColors::Linen},
                    {_T("Magenta"),UiColors::Magenta},
                    {_T("Maroon"),UiColors::Maroon},
                    {_T("MediumAquamarine"),UiColors::MediumAquamarine},
                    {_T("MediumBlue"),UiColors::MediumBlue},
                    {_T("MediumOrchid"),UiColors::MediumOrchid},
                    {_T("MediumPurple"),UiColors::MediumPurple},
                    {_T("MediumSeaGreen"),UiColors::MediumSeaGreen},
                    {_T("MediumSlateBlue"),UiColors::MediumSlateBlue},
                    {_T("MediumSpringGreen"),UiColors::MediumSpringGreen},
                    {_T("MediumTurquoise"),UiColors::MediumTurquoise},
                    {_T("MediumVioletRed"),UiColors::MediumVioletRed},
                    {_T("MidnightBlue"),UiColors::MidnightBlue},
                    {_T("MintCream"),UiColors::MintCream},
                    {_T("MistyRose"),UiColors::MistyRose},
                    {_T("Moccasin"),UiColors::Moccasin},
                    {_T("NavajoWhite"),UiColors::NavajoWhite},
                    {_T("Navy"),UiColors::Navy},
                    {_T("OldLace"),UiColors::OldLace},
                    {_T("Olive"),UiColors::Olive},
                    {_T("OliveDrab"),UiColors::OliveDrab},
                    {_T("Orange"),UiColors::Orange},
                    {_T("OrangeRed"),UiColors::OrangeRed},
                    {_T("Orchid"),UiColors::Orchid},
                    {_T("PaleGoldenrod"),UiColors::PaleGoldenrod},
                    {_T("PaleGreen"),UiColors::PaleGreen},
                    {_T("PaleTurquoise"),UiColors::PaleTurquoise},
                    {_T("PaleVioletRed"),UiColors::PaleVioletRed},
                    {_T("PapayaWhip"),UiColors::PapayaWhip},
                    {_T("PeachPuff"),UiColors::PeachPuff},
                    {_T("Peru"),UiColors::Peru},
                    {_T("Pink"),UiColors::Pink},
                    {_T("Plum"),UiColors::Plum},
                    {_T("PowderBlue"),UiColors::PowderBlue},
                    {_T("Purple"),UiColors::Purple},
                    {_T("Red"),UiColors::Red},
                    {_T("RosyBrown"),UiColors::RosyBrown},
                    {_T("RoyalBlue"),UiColors::RoyalBlue},
                    {_T("SaddleBrown"),UiColors::SaddleBrown},
                    {_T("Salmon"),UiColors::Salmon},
                    {_T("SandyBrown"),UiColors::SandyBrown},
                    {_T("SeaGreen"),UiColors::SeaGreen},
                    {_T("SeaShell"),UiColors::SeaShell},
                    {_T("Sienna"),UiColors::Sienna},
                    {_T("Silver"),UiColors::Silver},
                    {_T("SkyBlue"),UiColors::SkyBlue},
                    {_T("SlateBlue"),UiColors::SlateBlue},
                    {_T("SlateGray"),UiColors::SlateGray},
                    {_T("Snow"),UiColors::Snow},
                    {_T("SpringGreen"),UiColors::SpringGreen},
                    {_T("SteelBlue"),UiColors::SteelBlue},
                    {_T("Tan"),UiColors::Tan},
                    {_T("Teal"),UiColors::Teal},
                    {_T("Thistle"),UiColors::Thistle},
                    {_T("Tomato"),UiColors::Tomato},
                    {_T("Transparent"),UiColors::Transparent},
                    {_T("Turquoise"),UiColors::Turquoise},
                    {_T("Violet"),UiColors::Violet},
                    {_T("Wheat"),UiColors::Wheat},
                    {_T("White"),UiColors::White},
                    {_T("WhiteSmoke"),UiColors::WhiteSmoke},
                    {_T("Yellow"),UiColors::Yellow},
                    {_T("YellowGreen"),UiColors::YellowGreen}
                    };
    }
}// namespace UiColors
} // namespace ui

