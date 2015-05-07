/**
 * @file D3D12RenderView.cpp
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

#include <KlayGE/KlayGE.hpp>
#include <KFL/Util.hpp>
#include <KFL/COMPtr.hpp>
#include <KFL/ThrowErr.hpp>
#include <KFL/Math.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGe/FrameBuffer.hpp>

#include <boost/assert.hpp>

#include <KlayGE/D3D12/D3D12RenderEngine.hpp>
#include <KlayGE/D3D12/D3D12Mapping.hpp>
#include <KlayGE/D3D12/D3D12Texture.hpp>
#include <KlayGE/D3D12/D3D12RenderView.hpp>

namespace KlayGE
{
	D3D12RenderView::D3D12RenderView()
	{
		D3D12RenderEngine& renderEngine(*checked_cast<D3D12RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance()));
		d3d_device_ = renderEngine.D3D11Device();
		d3d_imm_ctx_ = renderEngine.D3D11DeviceImmContext();
	}

	D3D12RenderView::~D3D12RenderView()
	{
	}


	D3D12RenderTargetRenderView::D3D12RenderTargetRenderView(Texture& texture, int first_array_index, int array_size, int level)
		: rt_src_(&texture), rt_first_subres_(first_array_index * texture.NumMipMaps() + level), rt_num_subres_(1)
	{
		rt_view_ = checked_cast<D3D12Texture*>(&texture)->RetriveD3DRenderTargetView(first_array_index, array_size, level);

		width_ = texture.Width(level);
		height_ = texture.Height(level);
		pf_ = texture.Format();
	}

	D3D12RenderTargetRenderView::D3D12RenderTargetRenderView(Texture& texture_3d, int array_index, uint32_t first_slice, uint32_t num_slices, int level)
		: rt_src_(&texture_3d), rt_first_subres_((array_index * texture_3d.Depth(level) + first_slice) * texture_3d.NumMipMaps() + level), rt_num_subres_(num_slices * texture_3d.NumMipMaps() + level)
	{
		rt_view_ = checked_cast<D3D12Texture*>(&texture_3d)->RetriveD3DRenderTargetView(array_index, first_slice, num_slices, level);

		width_ = texture_3d.Width(level);
		height_ = texture_3d.Height(level);
		pf_ = texture_3d.Format();
	}

	D3D12RenderTargetRenderView::D3D12RenderTargetRenderView(Texture& texture_cube, int array_index, Texture::CubeFaces face, int level)
		: rt_src_(&texture_cube), rt_first_subres_((array_index * 6 + face) * texture_cube.NumMipMaps() + level), rt_num_subres_(1)
	{
		rt_view_ = checked_cast<D3D12Texture*>(&texture_cube)->RetriveD3DRenderTargetView(array_index, face, level);

		width_ = texture_cube.Width(level);
		height_ = texture_cube.Width(level);
		pf_ = texture_cube.Format();
	}

	D3D12RenderTargetRenderView::D3D12RenderTargetRenderView(GraphicsBuffer& gb, uint32_t width, uint32_t height, ElementFormat pf)
		: rt_src_(&gb), rt_first_subres_(0), rt_num_subres_(1)
	{
		BOOST_ASSERT(gb.AccessHint() & EAH_GPU_Write);

		D3D11_RENDER_TARGET_VIEW_DESC desc;
		desc.Format = D3D12Mapping::MappingFormat(pf);
		desc.ViewDimension = D3D11_RTV_DIMENSION_BUFFER;
		desc.Buffer.ElementOffset = 0;
		desc.Buffer.ElementWidth = std::min(width * height, gb.Size() / NumFormatBytes(pf));

		ID3D11RenderTargetView* rt_view;
		TIF(d3d_device_->CreateRenderTargetView(checked_cast<D3D12GraphicsBuffer*>(&gb)->D3DBuffer().get(), &desc, &rt_view));
		rt_view_ = MakeCOMPtr(rt_view);

		width_ = width * height;
		height_ = 1;
		pf_ = pf;
	}

	D3D12RenderTargetRenderView::D3D12RenderTargetRenderView(ID3D11RenderTargetViewPtr const & view, uint32_t width, uint32_t height, ElementFormat pf)
		: rt_view_(view), rt_src_(nullptr), rt_first_subres_(0), rt_num_subres_(0)
	{
		width_ = width;
		height_ = height;
		pf_ = pf;
	}

	void D3D12RenderTargetRenderView::ClearColor(Color const & clr)
	{
		d3d_imm_ctx_->ClearRenderTargetView(rt_view_.get(), &clr.r());
	}

	void D3D12RenderTargetRenderView::ClearDepth(float /*depth*/)
	{
		BOOST_ASSERT(false);
	}

	void D3D12RenderTargetRenderView::ClearStencil(int32_t /*stencil*/)
	{
		BOOST_ASSERT(false);
	}

	void D3D12RenderTargetRenderView::ClearDepthStencil(float /*depth*/, int32_t /*stencil*/)
	{
		BOOST_ASSERT(false);
	}

	void D3D12RenderTargetRenderView::Discard()
	{
		ID3D11DeviceContext1Ptr const & d3d_imm_ctx_1 = static_pointer_cast<ID3D11DeviceContext1>(d3d_imm_ctx_);
		d3d_imm_ctx_1->DiscardView(rt_view_.get());
	}

	void D3D12RenderTargetRenderView::OnAttached(FrameBuffer& /*fb*/, uint32_t /*att*/)
	{
	}

	void D3D12RenderTargetRenderView::OnDetached(FrameBuffer& /*fb*/, uint32_t /*att*/)
	{
	}


	D3D12DepthStencilRenderView::D3D12DepthStencilRenderView(Texture& texture, int first_array_index, int array_size, int level)
		: rt_src_(&texture), rt_first_subres_(first_array_index * texture.NumMipMaps() + level), rt_num_subres_(1)
	{
		ds_view_ = checked_cast<D3D12Texture*>(&texture)->RetriveD3DDepthStencilView(first_array_index, array_size, level);

		width_ = texture.Width(level);
		height_ = texture.Height(level);
		pf_ = texture.Format();
	}

	D3D12DepthStencilRenderView::D3D12DepthStencilRenderView(Texture& texture_3d, int array_index, uint32_t first_slice, uint32_t num_slices, int level)
		: rt_src_(&texture_3d), rt_first_subres_((array_index * texture_3d.Depth(level) + first_slice) * texture_3d.NumMipMaps() + level), rt_num_subres_(num_slices * texture_3d.NumMipMaps() + level)
	{
		ds_view_ = checked_cast<D3D12Texture*>(&texture_3d)->RetriveD3DDepthStencilView(array_index, first_slice, num_slices, level);

		width_ = texture_3d.Width(level);
		height_ = texture_3d.Height(level);
		pf_ = texture_3d.Format();
	}

	D3D12DepthStencilRenderView::D3D12DepthStencilRenderView(Texture& texture_cube, int array_index, Texture::CubeFaces face, int level)
		: rt_src_(&texture_cube), rt_first_subres_((array_index * 6 + face) * texture_cube.NumMipMaps() + level), rt_num_subres_(1)
	{
		ds_view_ = checked_cast<D3D12Texture*>(&texture_cube)->RetriveD3DDepthStencilView(array_index, face, level);

		width_ = texture_cube.Width(level);
		height_ = texture_cube.Width(level);
		pf_ = texture_cube.Format();
	}

	D3D12DepthStencilRenderView::D3D12DepthStencilRenderView(ID3D11DepthStencilViewPtr const & view, uint32_t width, uint32_t height, ElementFormat pf)
		: ds_view_(view), rt_src_(nullptr), rt_first_subres_(0), rt_num_subres_(0)
	{
		width_ = width;
		height_ = height;
		pf_ = pf;
	}

	D3D12DepthStencilRenderView::D3D12DepthStencilRenderView(uint32_t width, uint32_t height,
											ElementFormat pf, uint32_t sample_count, uint32_t sample_quality)
		: rt_src_(nullptr), rt_first_subres_(0), rt_num_subres_(0)
	{
		BOOST_ASSERT(IsDepthFormat(pf));

		D3D11_TEXTURE2D_DESC tex_desc;
		tex_desc.Width = width;
		tex_desc.Height = height;
		tex_desc.MipLevels = 1;
		tex_desc.ArraySize = 1;
		tex_desc.Format = D3D12Mapping::MappingFormat(pf);
		tex_desc.SampleDesc.Count = sample_count;
		tex_desc.SampleDesc.Quality = sample_quality;
		tex_desc.Usage = D3D11_USAGE_DEFAULT;
		tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		tex_desc.CPUAccessFlags = 0;
		tex_desc.MiscFlags = 0;
		ID3D11Texture2D* depth_tex;
		TIF(d3d_device_->CreateTexture2D(&tex_desc, nullptr, &depth_tex));

		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		desc.Format = tex_desc.Format;
		if (sample_count > 1)
		{
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		}
		else
		{
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		}
		desc.Flags = 0;
		desc.Texture2D.MipSlice = 0;

		ID3D11DepthStencilView* ds_view;
		TIF(d3d_device_->CreateDepthStencilView(depth_tex, &desc, &ds_view));
		ds_view_ = MakeCOMPtr(ds_view);

		depth_tex->Release();

		width_ = width;
		height_ = height;
		pf_ = pf;
	}

	void D3D12DepthStencilRenderView::ClearColor(Color const & /*clr*/)
	{
		BOOST_ASSERT(false);
	}

	void D3D12DepthStencilRenderView::ClearDepth(float depth)
	{
		d3d_imm_ctx_->ClearDepthStencilView(ds_view_.get(), D3D11_CLEAR_DEPTH, depth, 0);
	}

	void D3D12DepthStencilRenderView::ClearStencil(int32_t stencil)
	{
		d3d_imm_ctx_->ClearDepthStencilView(ds_view_.get(), D3D11_CLEAR_STENCIL, 1, static_cast<uint8_t>(stencil));
	}

	void D3D12DepthStencilRenderView::ClearDepthStencil(float depth, int32_t stencil)
	{
		d3d_imm_ctx_->ClearDepthStencilView(ds_view_.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, static_cast<uint8_t>(stencil));
	}

	void D3D12DepthStencilRenderView::Discard()
	{
		ID3D11DeviceContext1Ptr const & d3d_imm_ctx_1 = static_pointer_cast<ID3D11DeviceContext1>(d3d_imm_ctx_);
		d3d_imm_ctx_1->DiscardView(ds_view_.get());
	}

	void D3D12DepthStencilRenderView::OnAttached(FrameBuffer& /*fb*/, uint32_t att)
	{
		UNREF_PARAM(att);

		BOOST_ASSERT(FrameBuffer::ATT_DepthStencil == att);
	}

	void D3D12DepthStencilRenderView::OnDetached(FrameBuffer& /*fb*/, uint32_t att)
	{
		UNREF_PARAM(att);

		BOOST_ASSERT(FrameBuffer::ATT_DepthStencil == att);
	}


	D3D12UnorderedAccessView::D3D12UnorderedAccessView(Texture& texture, int first_array_index, int array_size, int level)
		: ua_src_(&texture), ua_first_subres_(first_array_index * texture.NumMipMaps() + level), ua_num_subres_(1)
	{
		D3D12RenderEngine& renderEngine(*checked_cast<D3D12RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance()));
		d3d_device_ = renderEngine.D3D11Device();
		d3d_imm_ctx_ = renderEngine.D3D11DeviceImmContext();

		ua_view_ = checked_cast<D3D12Texture*>(&texture)->RetriveD3DUnorderedAccessView(first_array_index, array_size, level);

		width_ = texture.Width(level);
		height_ = texture.Height(level);
		pf_ = texture.Format();
	}

	D3D12UnorderedAccessView::D3D12UnorderedAccessView(Texture& texture_3d, int array_index, uint32_t first_slice, uint32_t num_slices, int level)
		: ua_src_(&texture_3d), ua_first_subres_((array_index * texture_3d.Depth(level) + first_slice) * texture_3d.NumMipMaps() + level), ua_num_subres_(num_slices * texture_3d.NumMipMaps() + level)
	{
		D3D12RenderEngine& renderEngine(*checked_cast<D3D12RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance()));
		d3d_device_ = renderEngine.D3D11Device();
		d3d_imm_ctx_ = renderEngine.D3D11DeviceImmContext();

		ua_view_ = checked_cast<D3D12Texture*>(&texture_3d)->RetriveD3DUnorderedAccessView(array_index, first_slice, num_slices, level);

		width_ = texture_3d.Width(level);
		height_ = texture_3d.Height(level);
		pf_ = texture_3d.Format();
	}

	D3D12UnorderedAccessView::D3D12UnorderedAccessView(Texture& texture_cube, int array_index, Texture::CubeFaces face, int level)
		: ua_src_(&texture_cube), ua_first_subres_((array_index * 6 + face) * texture_cube.NumMipMaps() + level), ua_num_subres_(1)
	{
		D3D12RenderEngine& renderEngine(*checked_cast<D3D12RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance()));
		d3d_device_ = renderEngine.D3D11Device();
		d3d_imm_ctx_ = renderEngine.D3D11DeviceImmContext();

		ua_view_ = checked_cast<D3D12Texture*>(&texture_cube)->RetriveD3DUnorderedAccessView(array_index, face, level);

		width_ = texture_cube.Width(level);
		height_ = texture_cube.Width(level);
		pf_ = texture_cube.Format();
	}

	D3D12UnorderedAccessView::D3D12UnorderedAccessView(GraphicsBuffer& gb, ElementFormat pf)
		: ua_src_(&gb), ua_first_subres_(0), ua_num_subres_(1)
	{
		BOOST_ASSERT(gb.AccessHint() & EAH_GPU_Write);

		D3D12RenderEngine& renderEngine(*checked_cast<D3D12RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance()));
		d3d_device_ = renderEngine.D3D11Device();
		d3d_imm_ctx_ = renderEngine.D3D11DeviceImmContext();

		ua_view_ = checked_cast<D3D12GraphicsBuffer*>(&gb)->D3DUnorderedAccessView();

		width_ = gb.Size() / NumFormatBytes(pf);
		height_ = 1;
		pf_ = pf;
	}

	D3D12UnorderedAccessView::D3D12UnorderedAccessView(ID3D11UnorderedAccessViewPtr const & view, uint32_t width, uint32_t height, ElementFormat pf)
		: ua_view_(view), ua_src_(nullptr), ua_first_subres_(0), ua_num_subres_(0)
	{
		D3D12RenderEngine& renderEngine(*checked_cast<D3D12RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance()));
		d3d_device_ = renderEngine.D3D11Device();
		d3d_imm_ctx_ = renderEngine.D3D11DeviceImmContext();

		width_ = width;
		height_ = height;
		pf_ = pf;
	}
	
	D3D12UnorderedAccessView::~D3D12UnorderedAccessView()
	{
	}

	void D3D12UnorderedAccessView::Clear(float4 const & val)
	{
		d3d_imm_ctx_->ClearUnorderedAccessViewFloat(ua_view_.get(), &val.x());
	}

	void D3D12UnorderedAccessView::Clear(uint4 const & val)
	{
		d3d_imm_ctx_->ClearUnorderedAccessViewUint(ua_view_.get(), &val.x());
	}

	void D3D12UnorderedAccessView::Discard()
	{
		ID3D11DeviceContext1Ptr const & d3d_imm_ctx_1 = static_pointer_cast<ID3D11DeviceContext1>(d3d_imm_ctx_);
		d3d_imm_ctx_1->DiscardView(ua_view_.get());
	}

	void D3D12UnorderedAccessView::OnAttached(FrameBuffer& /*fb*/, uint32_t /*att*/)
	{
	}

	void D3D12UnorderedAccessView::OnDetached(FrameBuffer& /*fb*/, uint32_t /*att*/)
	{
	}
}
