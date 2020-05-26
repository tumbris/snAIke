#pragma once

enum class UpdatePriority
{
    FrameBegin,
    PostFrameBegin,

    ApplicationUpdate,
    ImGuiUpdate,
    GameUpdate,

    PreFrameEnd,
    FrameEnd,

    Default
};