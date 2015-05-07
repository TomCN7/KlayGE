/**
 * @file D3D12RenderStateObject.hpp
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

#ifndef _D3D12RENDERSTATEOBJECT_HPP
#define _D3D12RENDERSTATEOBJECT_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderStateObject.hpp>

namespace KlayGE
{
	class D3D12RasterizerStateObject : public RasterizerStateObject
	{
	public:
		explicit D3D12RasterizerStateObject(RasterizerStateDesc const & desc);

		void Active();

		ID3D11RasterizerStatePtr const & D3DRasterizerState() const
		{
			return rasterizer_state_;
		}

	private:
		ID3D11RasterizerStatePtr rasterizer_state_;
	};

	class D3D12DepthStencilStateObject : public DepthStencilStateObject
	{
	public:
		explicit D3D12DepthStencilStateObject(DepthStencilStateDesc const & desc);

		void Active(uint16_t front_stencil_ref, uint16_t back_stencil_ref);

		ID3D11DepthStencilStatePtr const & D3DDepthStencilState() const
		{
			return depth_stencil_state_;
		}

	private:
		ID3D11DepthStencilStatePtr depth_stencil_state_;
	};

	class D3D12BlendStateObject : public BlendStateObject
	{
	public:
		explicit D3D12BlendStateObject(BlendStateDesc const & desc);

		void Active(Color const & blend_factor, uint32_t sample_mask);

		ID3D11BlendStatePtr const & D3DBlendState() const
		{
			return blend_state_;
		}

	private:
		ID3D11BlendStatePtr blend_state_;
	};

	class D3D12SamplerStateObject : public SamplerStateObject
	{
	public:
		explicit D3D12SamplerStateObject(SamplerStateDesc const & desc);

		ID3D11SamplerStatePtr const & D3DSamplerState() const
		{
			return sampler_state_;
		}

	private:
		ID3D11SamplerStatePtr sampler_state_;
	};
}

#endif			// _D3D12RENDERSTATEOBJECT_HPP
