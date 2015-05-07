/**
 * @file D3D12RenderWindow.hpp
 * @author Minmin Gong
 *
 * @section DESCRIPTION
 *
 * This source file is part of KlayGE
 * For the latest info, see http://www.klayge.org
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * You may alternatively use this source under the terms of
 * the KlayGE Proprietary License (KPL). You can obtained such a license
 * from http://www.klayge.org/licensing/.
 */

#ifndef _D3D12RENDERWINDOW_HPP
#define _D3D12RENDERWINDOW_HPP

#pragma once

#include <KlayGE/D3D12/D3D12FrameBuffer.hpp>
#include <KlayGE/D3D12/D3D12Adapter.hpp>

#if defined KLAYGE_PLATFORM_WINDOWS_RUNTIME
#include <agile.h>
#endif

#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4100 4512 4702 4913 6011)
#endif
#include <boost/signals2.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(pop)
#endif

class IAmdDxExtQuadBufferStereo;
typedef KlayGE::shared_ptr<IAmdDxExtQuadBufferStereo> IAmdDxExtQuadBufferStereoPtr;

namespace KlayGE
{
#if defined KLAYGE_PLATFORM_WINDOWS_RUNTIME
	ref class MetroD3D11RenderWindow;
#endif

	struct RenderSettings;

	class D3D12RenderWindow : public D3D12FrameBuffer
	{
	public:
		D3D12RenderWindow(IDXGIFactory4Ptr const & gi_factory, D3D12AdapterPtr const & adapter,
			std::string const & name, RenderSettings const & settings);
		~D3D12RenderWindow();

		void Destroy();

		void SwapBuffers();

		std::wstring const & Description() const;

		void Resize(uint32_t width, uint32_t height);
		void Reposition(uint32_t left, uint32_t top);

		bool FullScreen() const;
		void FullScreen(bool fs);

		D3D12Adapter const & Adapter() const
		{
			return *adapter_;
		}
		ID3D11Texture2DPtr const & D3DBackBuffer() const
		{
			return back_buffer_;
		}
		ID3D11Texture2DPtr const & D3DDepthStencilBuffer() const
		{
			return depth_stencil_;
		}
		ID3D11RenderTargetViewPtr const & D3DBackBufferRTV() const
		{
			return render_target_view_;
		}
		ID3D11DepthStencilViewPtr const & D3DDepthStencilBufferDSV() const
		{
			return depth_stencil_view_;
		}
		ID3D11RenderTargetViewPtr const & D3DBackBufferRightEyeRTV() const
		{
			return render_target_view_right_eye_;
		}
		uint32_t StereoRightEyeHeight() const
		{
			return stereo_amd_right_eye_height_;
		}

		ID3D11DepthStencilViewPtr const & D3DDepthStencilBufferRightEyeDSV() const
		{
			return depth_stencil_view_right_eye_;
		}

		// Method for dealing with resize / move & 3d library
		void WindowMovedOrResized();

	private:
		void OnPaint(Window const & win);
		void OnExitSizeMove(Window const & win);
		void OnSize(Window const & win, bool active);
		void OnSetCursor(Window const & win);

	private:
		void UpdateSurfacesPtrs();
		void ResetDevice();

	private:
		std::string	name_;

#ifdef KLAYGE_PLATFORM_WINDOWS_DESKTOP
		HWND	hWnd_;				// Win32 Window handle
#else
		Platform::Agile<Windows::UI::Core::CoreWindow> wnd_;

		ref class MetroD3D11RenderWindow
		{
			friend class D3D11RenderWindow;

		public:
			void OnStereoEnabledChanged(Windows::Graphics::Display::DisplayInformation^ sender,
				Platform::Object^ args);

		private:
			void BindD3D11RenderWindow(D3D11RenderWindow* win);

			D3D11RenderWindow* win_;
		};

		MetroD3D11RenderWindow^ metro_d3d_render_win_;
#endif
		bool	isFullScreen_;
		uint32_t sync_interval_;

		D3D12AdapterPtr			adapter_;

		IDXGIFactory4Ptr gi_factory_;
		bool dxgi_stereo_support_;

		DXGI_SWAP_CHAIN_DESC sc_desc_;
#ifdef KLAYGE_PLATFORM_WINDOWS_DESKTOP
		DWORD stereo_cookie_;
#endif
		IDXGISwapChainPtr		swap_chain_;
		bool					main_wnd_;

		IAmdDxExtQuadBufferStereoPtr stereo_amd_qb_ext_;
		uint32_t stereo_amd_right_eye_height_;

		ID3D11Texture2DPtr			back_buffer_;
		ID3D11Texture2DPtr			depth_stencil_;
		ID3D11RenderTargetViewPtr	render_target_view_;
		ID3D11DepthStencilViewPtr	depth_stencil_view_;
		ID3D11RenderTargetViewPtr	render_target_view_right_eye_;
		ID3D11DepthStencilViewPtr	depth_stencil_view_right_eye_;

		DXGI_FORMAT					back_buffer_format_;
		DXGI_FORMAT					depth_stencil_format_;

		std::wstring			description_;

		boost::signals2::connection on_paint_connect_;
		boost::signals2::connection on_exit_size_move_connect_;
		boost::signals2::connection on_size_connect_;
		boost::signals2::connection on_set_cursor_connect_;
	};

	typedef shared_ptr<D3D12RenderWindow> D3D12RenderWindowPtr;
}

#endif			// _D3D12RENDERWINDOW_HPP
