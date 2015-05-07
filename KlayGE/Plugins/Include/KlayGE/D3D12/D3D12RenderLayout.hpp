/**
 * @file D3D12RenderLayout.hpp
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

#ifndef _D3D12RENDERLAYOUT_HPP
#define _D3D12RENDERLAYOUT_HPP

#pragma once

#include <vector>

#include <KlayGE/D3D12/D3D12MinGWDefs.hpp>
#include <D3D11Shader.h>

#include <KlayGE/RenderLayout.hpp>
#include <KlayGE/D3D12/D3D12Typedefs.hpp>

namespace KlayGE
{
	class D3D12RenderLayout : public RenderLayout
	{
	public:
		D3D12RenderLayout();

		ID3D11InputLayoutPtr const & InputLayout(size_t signature, std::vector<uint8_t> const & vs_code) const;

	private:
		typedef std::vector<D3D11_INPUT_ELEMENT_DESC> input_elems_type;
		input_elems_type vertex_elems_;

		mutable std::vector<std::pair<size_t, ID3D11InputLayoutPtr> > input_layouts_;
	};
}

#endif			// _D3D12RENDERLAYOUT_HPP