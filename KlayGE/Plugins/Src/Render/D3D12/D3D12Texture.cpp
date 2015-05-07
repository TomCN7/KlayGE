/**
 * @file D3D12Texture.cpp
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
#include <KlayGE/Context.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/Texture.hpp>

#include <cstring>
#include <boost/assert.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4100 6011 6334)
#endif
#include <boost/functional/hash.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(pop)
#endif

#include <KlayGE/D3D12/D3D12MinGWDefs.hpp>
#include <KlayGE/D3D12/D3D12RenderEngine.hpp>
#include <KlayGE/D3D12/D3D12Texture.hpp>

namespace KlayGE
{
	D3D12Texture::D3D12Texture(TextureType type, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint)
					: Texture(type, sample_count, sample_quality, access_hint)
	{
		if (access_hint & EAH_GPU_Write)
		{
			BOOST_ASSERT(!(access_hint & EAH_CPU_Read));
			BOOST_ASSERT(!(access_hint & EAH_CPU_Write));
		}

		D3D12RenderEngine& renderEngine(*checked_cast<D3D12RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance()));
		d3d_device_ = renderEngine.D3D11Device();
		d3d_imm_ctx_ = renderEngine.D3D11DeviceImmContext();
	}

	D3D12Texture::~D3D12Texture()
	{
	}

	std::wstring const & D3D12Texture::Name() const
	{
		static const std::wstring name(L"Direct3D12 Texture");
		return name;
	}

	uint32_t D3D12Texture::Width(uint32_t level) const
	{
		UNREF_PARAM(level);
		BOOST_ASSERT(level < num_mip_maps_);

		return 1;
	}

	uint32_t D3D12Texture::Height(uint32_t level) const
	{
		UNREF_PARAM(level);
		BOOST_ASSERT(level < num_mip_maps_);

		return 1;
	}

	uint32_t D3D12Texture::Depth(uint32_t level) const
	{
		UNREF_PARAM(level);
		BOOST_ASSERT(level < num_mip_maps_);

		return 1;
	}

	void D3D12Texture::CopyToSubTexture1D(Texture& /*target*/,
			uint32_t /*dst_array_index*/, uint32_t /*dst_level*/, uint32_t /*dst_x_offset*/, uint32_t /*dst_width*/,
			uint32_t /*src_array_index*/, uint32_t /*src_level*/, uint32_t /*src_x_offset*/, uint32_t /*src_width*/)
	{
		BOOST_ASSERT(false);
	}

	void D3D12Texture::CopyToSubTexture2D(Texture& /*target*/,
			uint32_t /*dst_array_index*/, uint32_t /*dst_level*/, uint32_t /*dst_x_offset*/, uint32_t /*dst_y_offset*/, uint32_t /*dst_width*/, uint32_t /*dst_height*/,
			uint32_t /*src_array_index*/, uint32_t /*src_level*/, uint32_t /*src_x_offset*/, uint32_t /*src_y_offset*/, uint32_t /*src_width*/, uint32_t /*src_height*/)
	{
		BOOST_ASSERT(false);
	}

	void D3D12Texture::CopyToSubTexture3D(Texture& /*target*/,
			uint32_t /*dst_array_index*/, uint32_t /*dst_level*/, uint32_t /*dst_x_offset*/, uint32_t /*dst_y_offset*/, uint32_t /*dst_z_offset*/, uint32_t /*dst_width*/, uint32_t /*dst_height*/, uint32_t /*dst_depth*/,
			uint32_t /*src_array_index*/, uint32_t /*src_level*/, uint32_t /*src_x_offset*/, uint32_t /*src_y_offset*/, uint32_t /*src_z_offset*/, uint32_t /*src_width*/, uint32_t /*src_height*/, uint32_t /*src_depth*/)
	{
		BOOST_ASSERT(false);
	}

	void D3D12Texture::CopyToSubTextureCube(Texture& /*target*/,
			uint32_t /*dst_array_index*/, CubeFaces /*dst_face*/, uint32_t /*dst_level*/, uint32_t /*dst_x_offset*/, uint32_t /*dst_y_offset*/, uint32_t /*dst_width*/, uint32_t /*dst_height*/,
			uint32_t /*src_array_index*/, CubeFaces /*src_face*/, uint32_t /*src_level*/, uint32_t /*src_x_offset*/, uint32_t /*src_y_offset*/, uint32_t /*src_width*/, uint32_t /*src_height*/)
	{
		BOOST_ASSERT(false);
	}

	ID3D11ShaderResourceViewPtr const & D3D12Texture::RetriveD3DShaderResourceView(uint32_t /*first_array_index*/, uint32_t /*num_items*/, uint32_t /*first_level*/, uint32_t /*num_levels*/)
	{
		BOOST_ASSERT(false);
		static ID3D11ShaderResourceViewPtr ret;
		return ret;
	}

	ID3D11UnorderedAccessViewPtr const & D3D12Texture::RetriveD3DUnorderedAccessView(uint32_t /*first_array_index*/, uint32_t /*num_items*/, uint32_t /*level*/)
	{
		BOOST_ASSERT(false);
		static ID3D11UnorderedAccessViewPtr ret;
		return ret;
	}

	ID3D11UnorderedAccessViewPtr const & D3D12Texture::RetriveD3DUnorderedAccessView(uint32_t /*array_index*/, uint32_t /*first_slice*/, uint32_t /*num_slices*/, uint32_t /*level*/)
	{
		BOOST_ASSERT(false);
		static ID3D11UnorderedAccessViewPtr ret;
		return ret;
	}

	ID3D11UnorderedAccessViewPtr const & D3D12Texture::RetriveD3DUnorderedAccessView(uint32_t /*first_array_index*/, uint32_t /*num_items*/, CubeFaces /*first_face*/, uint32_t /*num_faces*/,
		uint32_t /*level*/)
	{
		BOOST_ASSERT(false);
		static ID3D11UnorderedAccessViewPtr ret;
		return ret;
	}

	ID3D11RenderTargetViewPtr const & D3D12Texture::RetriveD3DRenderTargetView(uint32_t /*first_array_index*/, uint32_t /*array_size*/, uint32_t /*level*/)
	{
		BOOST_ASSERT(false);
		static ID3D11RenderTargetViewPtr ret;
		return ret;
	}

	ID3D11RenderTargetViewPtr const & D3D12Texture::RetriveD3DRenderTargetView(uint32_t /*array_index*/, uint32_t /*first_slice*/, uint32_t /*num_slices*/, uint32_t /*level*/)
	{
		BOOST_ASSERT(false);
		static ID3D11RenderTargetViewPtr ret;
		return ret;
	}

	ID3D11RenderTargetViewPtr const & D3D12Texture::RetriveD3DRenderTargetView(uint32_t /*array_index*/, Texture::CubeFaces /*face*/, uint32_t /*level*/)
	{
		BOOST_ASSERT(false);
		static ID3D11RenderTargetViewPtr ret;
		return ret;
	}

	ID3D11DepthStencilViewPtr const & D3D12Texture::RetriveD3DDepthStencilView(uint32_t /*first_array_index*/, uint32_t /*array_size*/, uint32_t /*level*/)
	{
		BOOST_ASSERT(false);
		static ID3D11DepthStencilViewPtr ret;
		return ret;
	}

	ID3D11DepthStencilViewPtr const & D3D12Texture::RetriveD3DDepthStencilView(uint32_t /*array_index*/, uint32_t /*first_slice*/, uint32_t /*num_slices*/, uint32_t /*level*/)
	{
		BOOST_ASSERT(false);
		static ID3D11DepthStencilViewPtr ret;
		return ret;
	}

	ID3D11DepthStencilViewPtr const & D3D12Texture::RetriveD3DDepthStencilView(uint32_t /*array_index*/, Texture::CubeFaces /*face*/, uint32_t /*level*/)
	{
		BOOST_ASSERT(false);
		static ID3D11DepthStencilViewPtr ret;
		return ret;
	}

	void D3D12Texture::GetD3DFlags(D3D11_USAGE& usage, UINT& bind_flags, UINT& cpu_access_flags, UINT& misc_flags)
	{
		if (access_hint_ & EAH_Immutable)
		{
			usage = D3D11_USAGE_IMMUTABLE;
		}
		else
		{
			if ((EAH_CPU_Write | EAH_GPU_Read) == access_hint_)
			{
				usage = D3D11_USAGE_DYNAMIC;
			}
			else
			{
				if (EAH_CPU_Write == access_hint_)
				{
					if ((num_mip_maps_ != 1) || (TT_Cube == type_))
					{
						usage = D3D11_USAGE_STAGING;
					}
					else
					{
						usage = D3D11_USAGE_DYNAMIC;
					}
				}
				else
				{
					if (!(access_hint_ & EAH_CPU_Read) && !(access_hint_ & EAH_CPU_Write))
					{
						usage = D3D11_USAGE_DEFAULT;
					}
					else
					{
						usage = D3D11_USAGE_STAGING;
					}
				}
			}
		}

		bind_flags = 0;
		if ((access_hint_ & EAH_GPU_Read) || (D3D11_USAGE_DYNAMIC == usage))
		{
			bind_flags |= D3D11_BIND_SHADER_RESOURCE;
		}
		if (access_hint_ & EAH_GPU_Write)
		{
			if (IsDepthFormat(format_))
			{
				bind_flags |= D3D11_BIND_DEPTH_STENCIL;
			}
			else
			{
				bind_flags |= D3D11_BIND_RENDER_TARGET;
			}
		}
		D3D12RenderEngine const & re = *checked_cast<D3D12RenderEngine const *>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		if (re.DeviceFeatureLevel() >= D3D_FEATURE_LEVEL_11_0)
		{
			if (access_hint_ & EAH_GPU_Unordered)
			{
				bind_flags |= D3D11_BIND_UNORDERED_ACCESS;
			}
		}

		cpu_access_flags = 0;
		if (access_hint_ & EAH_CPU_Read)
		{
			cpu_access_flags |= D3D11_CPU_ACCESS_READ;
		}
		if (access_hint_ & EAH_CPU_Write)
		{
			cpu_access_flags |= D3D11_CPU_ACCESS_WRITE;
		}

		misc_flags = 0;
		if (access_hint_ & EAH_Generate_Mips)
		{
			misc_flags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}
	}

	ID3D11ShaderResourceViewPtr const & D3D12Texture::RetriveD3DSRV(D3D11_SHADER_RESOURCE_VIEW_DESC const & desc)
	{
		char const * p = reinterpret_cast<char const *>(&desc);
		size_t hash_val = 0;
		boost::hash_range(hash_val, p, p + sizeof(desc));

		KLAYGE_AUTO(iter, d3d_sr_views_.find(hash_val));
		if (iter != d3d_sr_views_.end())
		{
			return iter->second;
		}

		ID3D11ShaderResourceView* d3d_sr_view;
		d3d_device_->CreateShaderResourceView(this->D3DResource().get(), &desc, &d3d_sr_view);
		KLAYGE_AUTO(ret, d3d_sr_views_.insert(std::make_pair(hash_val, MakeCOMPtr(d3d_sr_view))));
		return ret.first->second;
	}

	ID3D11UnorderedAccessViewPtr const & D3D12Texture::RetriveD3DUAV(D3D11_UNORDERED_ACCESS_VIEW_DESC const & desc)
	{
		char const * p = reinterpret_cast<char const *>(&desc);
		size_t hash_val = 0;
		boost::hash_range(hash_val, p, p + sizeof(desc));

		KLAYGE_AUTO(iter, d3d_ua_views_.find(hash_val));
		if (iter != d3d_ua_views_.end())
		{
			return iter->second;
		}

		ID3D11UnorderedAccessView* d3d_ua_view;
		d3d_device_->CreateUnorderedAccessView(this->D3DResource().get(), &desc, &d3d_ua_view);
		KLAYGE_AUTO(ret, d3d_ua_views_.insert(std::make_pair(hash_val, MakeCOMPtr(d3d_ua_view))));
		return ret.first->second;
	}

	ID3D11RenderTargetViewPtr const & D3D12Texture::RetriveD3DRTV(D3D11_RENDER_TARGET_VIEW_DESC const & desc)
	{
		char const * p = reinterpret_cast<char const *>(&desc);
		size_t hash_val = 0;
		boost::hash_range(hash_val, p, p + sizeof(desc));

		KLAYGE_AUTO(iter, d3d_rt_views_.find(hash_val));
		if (iter != d3d_rt_views_.end())
		{
			return iter->second;
		}

		ID3D11RenderTargetView* rt_view;
		d3d_device_->CreateRenderTargetView(this->D3DResource().get(), &desc, &rt_view);
		KLAYGE_AUTO(ret, d3d_rt_views_.insert(std::make_pair(hash_val, MakeCOMPtr(rt_view))));
		return ret.first->second;
	}

	ID3D11DepthStencilViewPtr const & D3D12Texture::RetriveD3DDSV(D3D11_DEPTH_STENCIL_VIEW_DESC const & desc)
	{
		char const * p = reinterpret_cast<char const *>(&desc);
		size_t hash_val = 0;
		boost::hash_range(hash_val, p, p + sizeof(desc));

		KLAYGE_AUTO(iter, d3d_ds_views_.find(hash_val));
		if (iter != d3d_ds_views_.end())
		{
			return iter->second;
		}

		ID3D11DepthStencilView* ds_view;
		d3d_device_->CreateDepthStencilView(this->D3DResource().get(), &desc, &ds_view);
		KLAYGE_AUTO(ret, d3d_ds_views_.insert(std::make_pair(hash_val, MakeCOMPtr(ds_view))));
		return ret.first->second;
	}

	void D3D12Texture::Map1D(uint32_t /*array_index*/, uint32_t /*level*/, TextureMapAccess /*tma*/,
			uint32_t /*x_offset*/, uint32_t /*width*/,
			void*& /*data*/)
	{
		BOOST_ASSERT(false);
	}

	void D3D12Texture::Map2D(uint32_t /*array_index*/, uint32_t /*level*/, TextureMapAccess /*tma*/,
			uint32_t /*x_offset*/, uint32_t /*y_offset*/, uint32_t /*width*/, uint32_t /*height*/,
			void*& /*data*/, uint32_t& /*row_pitch*/)
	{
		BOOST_ASSERT(false);
	}

	void D3D12Texture::Map3D(uint32_t /*array_index*/, uint32_t /*level*/, TextureMapAccess /*tma*/,
			uint32_t /*x_offset*/, uint32_t /*y_offset*/, uint32_t /*z_offset*/,
			uint32_t /*width*/, uint32_t /*height*/, uint32_t /*depth*/,
			void*& /*data*/, uint32_t& /*row_pitch*/, uint32_t& /*slice_pitch*/)
	{
		BOOST_ASSERT(false);
	}

	void D3D12Texture::MapCube(uint32_t /*array_index*/, CubeFaces /*face*/, uint32_t /*level*/, TextureMapAccess /*tma*/,
			uint32_t /*x_offset*/, uint32_t /*y_offset*/, uint32_t /*width*/, uint32_t /*height*/,
			void*& /*data*/, uint32_t& /*row_pitch*/)
	{
		BOOST_ASSERT(false);
	}

	void D3D12Texture::Unmap1D(uint32_t /*array_index*/, uint32_t /*level*/)
	{
		BOOST_ASSERT(false);
	}

	void D3D12Texture::Unmap2D(uint32_t /*array_index*/, uint32_t /*level*/)
	{
		BOOST_ASSERT(false);
	}

	void D3D12Texture::Unmap3D(uint32_t /*array_index*/, uint32_t /*level*/)
	{
		BOOST_ASSERT(false);
	}

	void D3D12Texture::UnmapCube(uint32_t /*array_index*/, CubeFaces /*face*/, uint32_t /*level*/)
	{
		BOOST_ASSERT(false);
	}
}
