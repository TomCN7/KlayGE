/**
 * @file D3D12FrameBuffer.hpp
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

#ifndef _D3D12FRAMEBUFFER_HPP
#define _D3D12FRAMEBUFFER_HPP

#pragma once

#include <KlayGE/FrameBuffer.hpp>
#include <KlayGE/D3D12/D3D12Typedefs.hpp>

namespace KlayGE
{
	class D3D12FrameBuffer : public FrameBuffer
	{
	public:
		D3D12FrameBuffer();
		virtual ~D3D12FrameBuffer();

		ID3D11RenderTargetViewPtr D3DRTView(uint32_t n) const;
		ID3D11DepthStencilViewPtr D3DDSView() const;
		ID3D11UnorderedAccessViewPtr D3DUAView(uint32_t n) const;

		virtual std::wstring const & Description() const;

		virtual void OnBind();
		virtual void OnUnbind();

		void Clear(uint32_t flags, Color const & clr, float depth, int32_t stencil);
		virtual void Discard(uint32_t flags) KLAYGE_OVERRIDE;

	private:
		D3D11_VIEWPORT d3d_viewport_;
	};

	typedef shared_ptr<D3D12FrameBuffer> D3D12FrameBufferPtr;
}

#endif			// _D3D12FRAMEBUFFER_HPP
