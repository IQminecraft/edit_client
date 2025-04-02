#pragma once
#include "../Hook.hpp"
#include "../../../../Utils/Memory/Game/SignatureAndOffsetManager.hpp"
#include "SDK/Client/Options/OptionsParser.hpp"
#include "../../../GUI/Engine/bgfx.hpp"

class UnderUIHooks : public Hook {
    static void ClearDepthStencilViewCallbackDX11(
    ID3D11DeviceContext* pContext,
    ID3D11DepthStencilView *pDepthStencilView,
    UINT                   ClearFlags,
    FLOAT                  Depth,
    UINT8                  Stencil);

    static void ClearDepthStencilViewCallbackDX12(
    ID3D12GraphicsCommandList* cmdList,
    D3D12_CPU_DESCRIPTOR_HANDLE pDepthStencilView,
    D3D12_CLEAR_FLAGS           ClearFlags,
    FLOAT                       Depth,
    UINT8                       Stencil,
    UINT                        NumRects,
    const D3D12_RECT            *pRects);

    static void callBackRenderContextD3D11Submit(
        bgfx::RenderContextD3D11* a1,
        void* a2,
        void* a3,
        void* a4);

public:
    typedef void (__thiscall* originalDX11)(
    ID3D11DeviceContext* pContext,
    ID3D11DepthStencilView *pDepthStencilView,
    UINT                   ClearFlags,
    FLOAT                  Depth,
    UINT8                  Stencil);

    static inline originalDX11 funcOriginalDX11 = nullptr;


    typedef void (__thiscall* originalDX12)(
    ID3D12GraphicsCommandList* cmdList,
    D3D12_CPU_DESCRIPTOR_HANDLE pDepthStencilView,
    D3D12_CLEAR_FLAGS           ClearFlags,
    FLOAT                       Depth,
    UINT8                       Stencil,
    UINT                        NumRects,
    const D3D12_RECT            *pRects);

    static inline originalDX12 funcOriginalDX12 = nullptr;

    typedef void(__thiscall* originalRenderContextD3D11Submit)(
        bgfx::RenderContextD3D11* a1,
        void* a2,
        void* a3,
        void* a4
        );

    static inline originalRenderContextD3D11Submit funcoriginalRenderContextD3D11Submit = nullptr;

    static inline int index = 0;

    UnderUIHooks() : Hook("ClearDepthStencilView", 0) {}

    void enableHook() override;

    static OptionsParser options;
};
