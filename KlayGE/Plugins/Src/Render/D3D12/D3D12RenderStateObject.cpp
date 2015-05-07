/**
 * @file D3D12RenderStateObject.cpp
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
#include <KFL/ThrowErr.hpp>
#include <KFL/Util.hpp>
#include <KFL/COMPtr.hpp>
#include <KFL/Math.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/RenderFactory.hpp>

#include <limits>

#include <KlayGE/D3D12/D3D12RenderEngine.hpp>
#include <KlayGE/D3D12/D3D12Mapping.hpp>
#include <KlayGE/D3D12/D3D12RenderStateObject.hpp>

namespace KlayGE
{
	D3D12RasterizerStateObject::D3D12RasterizerStateObject(RasterizerStateDesc const & desc)
		: RasterizerStateObject(desc)
	{
		D3D12RenderEngine& re = *checked_cast<D3D12RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D11DevicePtr const & d3d_device = re.D3D11Device();

		ID3D11Device1Ptr const & d3d_device_1 = static_pointer_cast<ID3D11Device1>(d3d_device);
		D3D11_RASTERIZER_DESC1 d3d_desc1;
		d3d_desc1.FillMode = D3D12Mapping::Mapping(desc.polygon_mode);
		d3d_desc1.CullMode = D3D12Mapping::Mapping(desc.cull_mode);
		d3d_desc1.FrontCounterClockwise = D3D12Mapping::Mapping(desc.front_face_ccw);
		d3d_desc1.DepthBias = static_cast<int>(desc.polygon_offset_units);
		d3d_desc1.DepthBiasClamp = desc.polygon_offset_units;
		d3d_desc1.SlopeScaledDepthBias = desc.polygon_offset_factor;
		d3d_desc1.DepthClipEnable = re.DeviceFeatureLevel() >= D3D_FEATURE_LEVEL_10_0 ? desc.depth_clip_enable : true;
		d3d_desc1.ScissorEnable = desc.scissor_enable;
		d3d_desc1.MultisampleEnable = desc.multisample_enable;
		d3d_desc1.AntialiasedLineEnable = false;
		d3d_desc1.ForcedSampleCount = 0;

		ID3D11RasterizerState1* rasterizer_state;
		TIF(d3d_device_1->CreateRasterizerState1(&d3d_desc1, &rasterizer_state));
		rasterizer_state_ = MakeCOMPtr(rasterizer_state);
	}

	void D3D12RasterizerStateObject::Active()
	{
		D3D12RenderEngine& re = *checked_cast<D3D12RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		re.RSSetState(rasterizer_state_);
	}

	D3D12DepthStencilStateObject::D3D12DepthStencilStateObject(DepthStencilStateDesc const & desc)
		: DepthStencilStateObject(desc)
	{
		D3D11_DEPTH_STENCIL_DESC d3d_desc;
		d3d_desc.DepthEnable = desc.depth_enable;
		d3d_desc.DepthWriteMask = D3D12Mapping::Mapping(desc.depth_write_mask);
		d3d_desc.DepthFunc = D3D12Mapping::Mapping(desc.depth_func);
		d3d_desc.StencilEnable = desc.front_stencil_enable;
		d3d_desc.StencilReadMask = static_cast<uint8_t>(desc.front_stencil_read_mask);
		d3d_desc.StencilWriteMask = static_cast<uint8_t>(desc.front_stencil_write_mask);
		d3d_desc.FrontFace.StencilFailOp = D3D12Mapping::Mapping(desc.front_stencil_fail);
		d3d_desc.FrontFace.StencilDepthFailOp = D3D12Mapping::Mapping(desc.front_stencil_depth_fail);
		d3d_desc.FrontFace.StencilPassOp = D3D12Mapping::Mapping(desc.front_stencil_pass);
		d3d_desc.FrontFace.StencilFunc = D3D12Mapping::Mapping(desc.front_stencil_func);
		d3d_desc.BackFace.StencilFailOp = D3D12Mapping::Mapping(desc.back_stencil_fail);
		d3d_desc.BackFace.StencilDepthFailOp = D3D12Mapping::Mapping(desc.back_stencil_depth_fail);
		d3d_desc.BackFace.StencilPassOp = D3D12Mapping::Mapping(desc.back_stencil_pass);
		d3d_desc.BackFace.StencilFunc = D3D12Mapping::Mapping(desc.back_stencil_func);

		ID3D11DevicePtr const & d3d_device = checked_cast<D3D12RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance())->D3D11Device();

		ID3D11DepthStencilState* ds_state;
		TIF(d3d_device->CreateDepthStencilState(&d3d_desc, &ds_state));
		depth_stencil_state_ = MakeCOMPtr(ds_state);
	}

	void D3D12DepthStencilStateObject::Active(uint16_t front_stencil_ref, uint16_t /*back_stencil_ref*/)
	{
		D3D12RenderEngine& re = *checked_cast<D3D12RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		re.OMSetDepthStencilState(depth_stencil_state_, front_stencil_ref);
	}

	D3D12BlendStateObject::D3D12BlendStateObject(BlendStateDesc const & desc)
		: BlendStateObject(desc)
	{
		D3D12RenderEngine& re = *checked_cast<D3D12RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D11DevicePtr const & d3d_device = re.D3D11Device();

		ID3D11Device1Ptr const & d3d_device_1 = static_pointer_cast<ID3D11Device1>(d3d_device);

		D3D11_BLEND_DESC1 d3d_desc1;
		d3d_desc1.AlphaToCoverageEnable = desc.alpha_to_coverage_enable;
		d3d_desc1.IndependentBlendEnable = desc.independent_blend_enable;
		for (int i = 0; i < 8; ++ i)
		{
			d3d_desc1.RenderTarget[i].BlendEnable = desc.blend_enable[i];
			d3d_desc1.RenderTarget[i].LogicOpEnable = desc.logic_op_enable[i];
			d3d_desc1.RenderTarget[i].SrcBlend = D3D12Mapping::Mapping(desc.src_blend[i]);
			d3d_desc1.RenderTarget[i].DestBlend = D3D12Mapping::Mapping(desc.dest_blend[i]);
			d3d_desc1.RenderTarget[i].BlendOp = D3D12Mapping::Mapping(desc.blend_op[i]);
			d3d_desc1.RenderTarget[i].SrcBlendAlpha = D3D12Mapping::Mapping(desc.src_blend_alpha[i]);
			d3d_desc1.RenderTarget[i].DestBlendAlpha = D3D12Mapping::Mapping(desc.dest_blend_alpha[i]);
			d3d_desc1.RenderTarget[i].BlendOpAlpha = D3D12Mapping::Mapping(desc.blend_op_alpha[i]);
			d3d_desc1.RenderTarget[i].LogicOp = D3D12Mapping::Mapping(desc.logic_op[i]);
			d3d_desc1.RenderTarget[i].RenderTargetWriteMask = static_cast<UINT8>(D3D12Mapping::MappingColorMask(desc.color_write_mask[i]));
		}

		ID3D11BlendState1* blend_state;
		TIF(d3d_device_1->CreateBlendState1(&d3d_desc1, &blend_state));
		blend_state_ = MakeCOMPtr(blend_state);
	}

	void D3D12BlendStateObject::Active(Color const & blend_factor, uint32_t sample_mask)
	{
		D3D12RenderEngine& re = *checked_cast<D3D12RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		re.OMSetBlendState(blend_state_, blend_factor, sample_mask);
	}

	D3D12SamplerStateObject::D3D12SamplerStateObject(SamplerStateDesc const & desc)
		: SamplerStateObject(desc)
	{
		D3D12RenderEngine const & render_eng = *checked_cast<D3D12RenderEngine const *>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D11DevicePtr const & d3d_device = render_eng.D3D11Device();
		D3D_FEATURE_LEVEL feature_level = render_eng.DeviceFeatureLevel();

		D3D11_SAMPLER_DESC d3d_desc;
		d3d_desc.Filter = D3D12Mapping::Mapping(desc.filter);
		TexAddressingMode addr_mode_u = desc.addr_mode_u;
		if ((feature_level <= D3D_FEATURE_LEVEL_9_2) && (TAM_Border == desc.addr_mode_u))
		{
			addr_mode_u = TAM_Clamp;
		}
		d3d_desc.AddressU = D3D12Mapping::Mapping(addr_mode_u);
		TexAddressingMode addr_mode_v = desc.addr_mode_v;
		if ((feature_level <= D3D_FEATURE_LEVEL_9_2) && (TAM_Border == desc.addr_mode_u))
		{
			addr_mode_v = TAM_Clamp;
		}
		d3d_desc.AddressV = D3D12Mapping::Mapping(addr_mode_v);
		TexAddressingMode addr_mode_w = desc.addr_mode_u;
		if ((feature_level <= D3D_FEATURE_LEVEL_9_2) && (TAM_Border == desc.addr_mode_u))
		{
			addr_mode_w = TAM_Clamp;
		}
		d3d_desc.AddressW = D3D12Mapping::Mapping(addr_mode_w);
		d3d_desc.MipLODBias = desc.mip_map_lod_bias;
		d3d_desc.MaxAnisotropy = (feature_level <= D3D_FEATURE_LEVEL_9_1) ? 1 : desc.max_anisotropy;
		d3d_desc.ComparisonFunc = D3D12Mapping::Mapping(desc.cmp_func);
		d3d_desc.BorderColor[0] = desc.border_clr.r();
		d3d_desc.BorderColor[1] = desc.border_clr.g();
		d3d_desc.BorderColor[2] = desc.border_clr.b();
		d3d_desc.BorderColor[3] = desc.border_clr.a();
		if (feature_level <= D3D_FEATURE_LEVEL_9_3)
		{
			d3d_desc.MinLOD = floor(desc.min_lod);
			d3d_desc.MaxLOD = std::numeric_limits<float>::max();
		}
		else
		{
			d3d_desc.MinLOD = desc.min_lod;
			d3d_desc.MaxLOD = desc.max_lod;
		}

		ID3D11SamplerState* sampler_state;
		TIF(d3d_device->CreateSamplerState(&d3d_desc, &sampler_state));
		sampler_state_ = MakeCOMPtr(sampler_state);
	}
}
