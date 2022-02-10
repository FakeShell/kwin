/*
    KWin - the KDE window manager
    This file is part of the KDE project.

    SPDX-FileCopyrightText: 2021 Xaver Hugl <xaver.hugl@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "drm_abstract_output.h"
#include "drm_gpu.h"
#include "drm_backend.h"

namespace KWin
{

DrmAbstractOutput::DrmAbstractOutput(DrmGpu *gpu)
    : AbstractWaylandOutput(gpu->platform())
    , DrmDisplayDevice(gpu)
    , m_renderLoop(new RenderLoop(this))
{
}

RenderLoop *DrmAbstractOutput::renderLoop() const
{
    return m_renderLoop;
}

}
