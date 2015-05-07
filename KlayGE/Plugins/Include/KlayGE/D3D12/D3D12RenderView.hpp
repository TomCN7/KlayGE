/**
 * @file D3D12RenderView.hpp
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

#ifndef _D3D12RENDERVIEW_HPP
#define _D3D12RENDERVIEW_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderView.hpp>
#include <KlayGE/Texture.hpp>

#include <boost/noncopyable.hpp>

#include <KlayGE/D3D12/D3D12Typedefs.hpp>

namespace KlayGE
{
	class D3D12Texture1D;
	class D3D12Texture2D;
	class D3D12Texture3D;
	class D3D12TextureCube;
	class D3D12GraphicsBuffer;

	class D3D12RenderView : public RenderView
	{
	public:
		D3D12RenderView();
		virtual ~D3D12RenderView();

	protected:
		ID3D11DevicePtr d3d_device_;
		ID3D11DeviceContextPtr d3d_imm_ctx_;
	};
	typedef shared_ptr<D3D12RenderView> D3D12RenderViewPtr;

	class D3D12RenderTargetRenderView : public D3D12RenderView
	{
	public:
		D3D12RenderTargetRenderView(Texture& texture_1d_2d_cube, int first_array_index, int array_size, int level);
		D3D12RenderTargetRenderView(Texture& texture_3d, int array_index, uint32_t first_slice, uint32_t num_slices, int level);
		D3D12RenderTargetRenderView(Texture& texture_cube, int array_index, Texture::CubeFaces face, int level);
		D3D12RenderTargetRenderView(GraphicsBuffer& gb, uint32_t width, uint32_t height, ElementFormat pf);
		D3D12RenderTargetRenderView(ID3D11RenderTargetViewPtr const & view, uint32_t width, uint32_t height, ElementFormat pf);

		void ClearColor(Color const & clr);
		void ClearDepth(float depth);
		void ClearStencil(int32_t stencil);
		void ClearDepthStencil(float depth, int32_t stencil);

		virtual void Discard() KLAYGE_OVERRIDE;

		void OnAttached(FrameBuffer& fb, uint32_t att);
		void OnDetached(FrameBuffer& fb, uint32_t att);

		ID3D11RenderTargetViewPtr const & D3DRenderTargetView() const
		{
			return rt_view_;
		}

		void* RTSrc() const
		{
			return rt_src_;
		}
		uint32_t RTFirstSubRes() const
		{
			return rt_first_subres_;
		}
		uint32_t RTNumSubRes() const
		{
			return rt_num_subres_;
		}

	private:
		ID3D11RenderTargetViewPtr rt_view_;
		void* rt_src_;
		uint32_t rt_first_subres_;
		uint32_t rt_num_subres_;
	};
	typedef shared_ptr<D3D12RenderTargetRenderView> D3D12RenderTargetRenderViewPtr;

	class D3D12DepthStencilRenderView : public D3D12RenderView
	{
	public:
		D3D12DepthStencilRenderView(Texture& texture_1d_2d_cube, int first_array_index, int array_size, int level);
		D3D12DepthStencilRenderView(Texture& texture_3d, int array_index, uint32_t first_slice, uint32_t num_slices, int level);
		D3D12DepthStencilRenderView(Texture& texture_cube, int array_index, Texture::CubeFaces face, int level);
		D3D12DepthStencilRenderView(ID3D11DepthStencilViewPtr const & view, uint32_t width, uint32_t height, ElementFormat pf);
		D3D12DepthStencilRenderView(uint32_t width, uint32_t height, ElementFormat pf, uint32_t sample_count, uint32_t sample_quality);

		void ClearColor(Color const & clr);
		void ClearDepth(float depth);
		void ClearStencil(int32_t stencil);
		void ClearDepthStencil(float depth, int32_t stencil);

		virtual void Discard() KLAYGE_OVERRIDE;

		void OnAttached(FrameBuffer& fb, uint32_t att);
		void OnDetached(FrameBuffer& fb, uint32_t att);

		ID3D11DepthStencilViewPtr const & D3DDepthStencilView() const
		{
			return ds_view_;
		}

		void* RTSrc() const
		{
			return rt_src_;
		}
		uint32_t RTFirstSubRes() const
		{
			return rt_first_subres_;
		}
		uint32_t RTNumSubRes() const
		{
			return rt_num_subres_;
		}

	private:
		ID3D11DepthStencilViewPtr ds_view_;
		void* rt_src_;
		uint32_t rt_first_subres_;
		uint32_t rt_num_subres_;
	};
	typedef shared_ptr<D3D12DepthStencilRenderView> D3D12DepthStencilRenderViewPtr;


	class D3D12UnorderedAccessView : public UnorderedAccessView
	{
	public:
		D3D12UnorderedAccessView(Texture& texture_1d_2d_cube, int first_array_index, int array_size, int level);
		D3D12UnorderedAccessView(Texture& texture_3d, int array_index, uint32_t first_slice, uint32_t num_slices, int level);
		D3D12UnorderedAccessView(Texture& texture_cube, int array_index, Texture::CubeFaces face, int level);
		D3D12UnorderedAccessView(GraphicsBuffer& gb, ElementFormat pf);
		D3D12UnorderedAccessView(ID3D11UnorderedAccessViewPtr const & view, uint32_t width, uint32_t height, ElementFormat pf);
		virtual ~D3D12UnorderedAccessView();

		void Clear(float4 const & val);
		void Clear(uint4 const & val);

		virtual void Discard() KLAYGE_OVERRIDE;

		void OnAttached(FrameBuffer& fb, uint32_t att);
		void OnDetached(FrameBuffer& fb, uint32_t att);

		ID3D11UnorderedAccessViewPtr const & D3DUnorderedAccessView() const
		{
			return ua_view_;
		}

		void* UASrc() const
		{
			return ua_src_;
		}
		uint32_t UAFirstSubRes() const
		{
			return ua_first_subres_;
		}
		uint32_t UANumSubRes() const
		{
			return ua_num_subres_;
		}

	private:
		ID3D11DevicePtr d3d_device_;
		ID3D11DeviceContextPtr d3d_imm_ctx_;

		ID3D11UnorderedAccessViewPtr ua_view_;
		void* ua_src_;
		uint32_t ua_first_subres_;
		uint32_t ua_num_subres_;
	};
	typedef shared_ptr<D3D12UnorderedAccessView> D3D12UnorderedAccessViewPtr;
}

#endif			// _D3D11RENDERVIEW_HPP
