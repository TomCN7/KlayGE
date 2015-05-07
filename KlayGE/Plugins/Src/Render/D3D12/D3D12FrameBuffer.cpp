/**
 * @file D3D12FrameBuffer.cpp
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
#include <KFL/ThrowErr.hpp>
#include <KFL/Math.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/Texture.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/FrameBuffer.hpp>

#include <KlayGE/D3D12/D3D12RenderEngine.hpp>
#include <KlayGE/D3D12/D3D12RenderView.hpp>
#include <KlayGE/D3D12/D3D12FrameBuffer.hpp>

namespace KlayGE
{
	D3D12FrameBuffer::D3D12FrameBuffer()
	{
		left_ = 0;
		top_ = 0;

		viewport_->left	= left_;
		viewport_->top	= top_;

		d3d_viewport_.MinDepth = 0.0f;
		d3d_viewport_.MaxDepth = 1.0f;
	}

	D3D12FrameBuffer::~D3D12FrameBuffer()
	{
	}

	ID3D11RenderTargetViewPtr D3D12FrameBuffer::D3DRTView(uint32_t n) const
	{
		if (n < clr_views_.size())
		{
			if (clr_views_[n])
			{
				D3D12RenderTargetRenderView const & d3d_view(*checked_pointer_cast<D3D12RenderTargetRenderView>(clr_views_[n]));
				return d3d_view.D3DRenderTargetView();
			}
			else
			{
				return ID3D11RenderTargetViewPtr();
			}
		}
		else
		{
			return ID3D11RenderTargetViewPtr();
		}
	}

	ID3D11DepthStencilViewPtr D3D12FrameBuffer::D3DDSView() const
	{
		if (rs_view_)
		{
			D3D12DepthStencilRenderView const & d3d_view(*checked_pointer_cast<D3D12DepthStencilRenderView>(rs_view_));
			return d3d_view.D3DDepthStencilView();
		}
		else
		{
			return ID3D11DepthStencilViewPtr();
		}
	}

	ID3D11UnorderedAccessViewPtr D3D12FrameBuffer::D3DUAView(uint32_t n) const
	{
		if (n < ua_views_.size())
		{
			if (ua_views_[n])
			{
				D3D12UnorderedAccessView const & d3d_view(*checked_pointer_cast<D3D12UnorderedAccessView>(ua_views_[n]));
				return d3d_view.D3DUnorderedAccessView();
			}
			else
			{
				return ID3D11UnorderedAccessViewPtr();
			}
		}
		else
		{
			return ID3D11UnorderedAccessViewPtr();
		}
	}

	std::wstring const & D3D12FrameBuffer::Description() const
	{
		static std::wstring const desc(L"Direct3D12 Framebuffer");
		return desc;
	}

	void D3D12FrameBuffer::OnBind()
	{
		D3D12RenderEngine& re = *checked_cast<D3D12RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D11DeviceContextPtr const & d3d_imm_ctx = re.D3D11DeviceImmContext();

		std::vector<void*> rt_src;
		std::vector<uint32_t> rt_first_subres;
		std::vector<uint32_t> rt_num_subres;
		std::vector<ID3D11RenderTargetView*> rt_view(clr_views_.size());
		for (uint32_t i = 0; i < clr_views_.size(); ++ i)
		{
			if (clr_views_[i])
			{
				D3D12RenderTargetRenderView* p = checked_cast<D3D12RenderTargetRenderView*>(clr_views_[i].get());
				rt_src.push_back(p->RTSrc());
				rt_first_subres.push_back(p->RTFirstSubRes());
				rt_num_subres.push_back(p->RTNumSubRes());
				rt_view[i] = this->D3DRTView(i).get();
			}
			else
			{
				rt_view[i] = nullptr;
			}
		}
		if (rs_view_)
		{
			D3D12DepthStencilRenderView* p = checked_cast<D3D12DepthStencilRenderView*>(rs_view_.get());
			rt_src.push_back(p->RTSrc());
			rt_first_subres.push_back(p->RTFirstSubRes());
			rt_num_subres.push_back(p->RTNumSubRes());
		}
		std::vector<ID3D11UnorderedAccessView*> ua_view(ua_views_.size());
		std::vector<UINT> ua_init_count(ua_views_.size());
		for (uint32_t i = 0; i < ua_views_.size(); ++ i)
		{
			if (ua_views_[i])
			{
				D3D12UnorderedAccessView* p = checked_cast<D3D12UnorderedAccessView*>(ua_views_[i].get());
				rt_src.push_back(p->UASrc());
				rt_first_subres.push_back(p->UAFirstSubRes());
				rt_num_subres.push_back(p->UANumSubRes());
				ua_view[i] = this->D3DUAView(i).get();
				ua_init_count[i] = ua_views_[i]->InitCount();
			}
			else
			{
				ua_view[i] = nullptr;
				ua_init_count[i] = 0;
			}
		}

		for (size_t i = 0; i < rt_src.size(); ++ i)
		{
			re.DetachSRV(rt_src[i], rt_first_subres[i], rt_num_subres[i]);
		}

		if (ua_views_.empty())
		{
			d3d_imm_ctx->OMSetRenderTargets(static_cast<UINT>(rt_view.size()), &rt_view[0], this->D3DDSView().get());
		}
		else
		{
			ID3D11RenderTargetView** rts = rt_view.empty() ? nullptr : &rt_view[0];
			d3d_imm_ctx->OMSetRenderTargetsAndUnorderedAccessViews(static_cast<UINT>(rt_view.size()), rts, this->D3DDSView().get(),
				0, static_cast<UINT>(ua_view.size()), &ua_view[0], &ua_init_count[0]);
		}
	
		if (!rt_view.empty())
		{
			d3d_viewport_.TopLeftX = static_cast<float>(viewport_->left);
			d3d_viewport_.TopLeftY = static_cast<float>(viewport_->top);
			d3d_viewport_.Width = static_cast<float>(viewport_->width);
			d3d_viewport_.Height = static_cast<float>(viewport_->height);
			re.RSSetViewports(1, &d3d_viewport_);
		}
	}

	void D3D12FrameBuffer::OnUnbind()
	{
	}

	void D3D12FrameBuffer::Clear(uint32_t flags, Color const & clr, float depth, int32_t stencil)
	{
		if (flags & CBM_Color)
		{
			for (uint32_t i = 0; i < clr_views_.size(); ++ i)
			{
				if (clr_views_[i])
				{
					clr_views_[i]->ClearColor(clr);
				}
			}
		}
		if ((flags & CBM_Depth) && (flags & CBM_Stencil))
		{
			if (rs_view_)
			{
				rs_view_->ClearDepthStencil(depth, stencil);
			}
		}
		else
		{
			if (flags & CBM_Depth)
			{
				if (rs_view_)
				{
					rs_view_->ClearDepth(depth);
				}
			}

			if (flags & CBM_Stencil)
			{
				if (rs_view_)
				{
					rs_view_->ClearStencil(stencil);
				}
			}
		}
	}

	void D3D12FrameBuffer::Discard(uint32_t flags)
	{
		if (flags & CBM_Color)
		{
			for (uint32_t i = 0; i < clr_views_.size(); ++ i)
			{
				if (clr_views_[i])
				{
					clr_views_[i]->Discard();
				}
			}
		}
		if ((flags & CBM_Depth) || (flags & CBM_Stencil))
		{
			if (rs_view_)
			{
				rs_view_->Discard();
			}
		}
	}
}
