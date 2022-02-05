/*
    KWin - the KDE window manager
    This file is part of the KDE project.

    SPDX-FileCopyrightText: 2022 Xaver Hugl <xaver.hugl@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include "drm_layer.h"

#include <QSharedPointer>
#include <QPointer>
#include <QMap>
#include <QRegion>
#include <optional>
#include <epoxy/egl.h>

namespace KWaylandServer
{
class SurfaceInterface;
class LinuxDmaBufV1ClientBuffer;
}

namespace KWin
{

class GbmSurface;
class DumbSwapchain;
class ShadowBuffer;
class DrmAbstractOutput;
class DrmBuffer;
class DrmGpu;
class SurfaceItem;
class GLTexture;

class EglGbmLayer : public DrmLayer
{
public:
    EglGbmLayer(DrmGpu *renderGpu, DrmAbstractOutput *output);
    ~EglGbmLayer();

    std::optional<QRegion> startRendering() override;
    bool endRendering(const QRegion &damagedRegion) override;
    bool scanout(SurfaceItem *surfaceItem) override;
    QSharedPointer<DrmBuffer> testBuffer() override;
    QSharedPointer<DrmBuffer> currentBuffer() const override;
    QSharedPointer<GLTexture> texture() const;

    DrmAbstractOutput *output() const override;
    int bufferAge() const;
    EGLSurface eglSurface() const;

private:
    bool createGbmSurface();
    bool doesGbmSurfaceFit(GbmSurface *surf) const;
    bool doesShadowBufferFit(ShadowBuffer *buffer) const;
    bool doesSwapchainFit(DumbSwapchain *swapchain) const;
    void sendDmabufFeedback(KWaylandServer::LinuxDmaBufV1ClientBuffer *failedBuffer);
    bool renderTestBuffer();

    QSharedPointer<DrmBuffer> importBuffer();
    QSharedPointer<DrmBuffer> importDmabuf();
    QSharedPointer<DrmBuffer> importWithCpu();

    enum class MultiGpuImportMode {
        Dmabuf,
        DumbBuffer,
        DumbBufferXrgb8888,
        Failed
    };
    MultiGpuImportMode m_importMode = MultiGpuImportMode::Dmabuf;

    struct {
        QPointer<KWaylandServer::SurfaceInterface> surface;
        QMap<uint32_t, QVector<uint64_t>> attemptedFormats;
        bool attemptedThisFrame = false;
    } m_scanoutCandidate;

    QSharedPointer<DrmBuffer> m_currentBuffer;
    QSharedPointer<GbmSurface> m_gbmSurface;
    QSharedPointer<GbmSurface> m_oldGbmSurface;
    QSharedPointer<ShadowBuffer> m_shadowBuffer;
    QSharedPointer<ShadowBuffer> m_oldShadowBuffer;
    QSharedPointer<DumbSwapchain> m_importSwapchain;
    QSharedPointer<DumbSwapchain> m_oldImportSwapchain;

    DrmAbstractOutput *const m_output;
    DrmGpu *const m_renderGpu;
};

}
