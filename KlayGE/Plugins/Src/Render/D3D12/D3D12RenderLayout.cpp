/**
 * @file D3D12RenderLayout.cpp
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
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/Context.hpp>

#include <algorithm>
#include <cstring>

#include <KlayGE/D3D12/D3D12RenderEngine.hpp>
#include <KlayGE/D3D12/D3D12Mapping.hpp>
#include <KlayGE/D3D12/D3D12GraphicsBuffer.hpp>
#include <KlayGE/D3D12/D3D12RenderLayout.hpp>

namespace KlayGE
{
	D3D12RenderLayout::D3D12RenderLayout()
	{
	}

	ID3D11InputLayoutPtr const & D3D12RenderLayout::InputLayout(size_t signature, std::vector<uint8_t> const & vs_code) const
	{
		for (size_t i = 0; i < input_layouts_.size(); ++ i)
		{
			if (input_layouts_[i].first == signature)
			{
				return input_layouts_[i].second;
			}
		}

		input_elems_type elems;
		elems.reserve(vertex_streams_.size());

		for (uint32_t i = 0; i < this->NumVertexStreams(); ++ i)
		{
			input_elems_type stream_elems;
			D3D12Mapping::Mapping(stream_elems, i, this->VertexStreamFormat(i), vertex_streams_[i].type, vertex_streams_[i].freq);
			elems.insert(elems.end(), stream_elems.begin(), stream_elems.end());
		}
		if (instance_stream_.stream)
		{
			input_elems_type stream_elems;
			D3D12Mapping::Mapping(stream_elems, this->NumVertexStreams(), this->InstanceStreamFormat(), instance_stream_.type, instance_stream_.freq);
			elems.insert(elems.end(), stream_elems.begin(), stream_elems.end());
		}

		D3D12RenderEngine& re = *checked_cast<D3D12RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D11InputLayoutPtr const & ret = re.CreateD3D11InputLayout(elems, signature, vs_code);
		input_layouts_.push_back(std::make_pair(signature, ret));

		return input_layouts_.back().second;
	}
}
