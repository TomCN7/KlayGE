/**
 * @file D3D12Typedefs.hpp
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

#ifndef _D3D12TYPEDEFS_HPP
#define _D3D12TYPEDEFS_HPP

#pragma once

#include <KlayGE/D3D12/D3D12MinGWDefs.hpp>
#include <dxgi1_4.h>
#include <d3d11_2.h>
#include <d3d11on12.h>

namespace KlayGE
{
	typedef shared_ptr<IDXGIFactory4>				IDXGIFactory4Ptr;
	typedef shared_ptr<IDXGIAdapter1>				IDXGIAdapter1Ptr;
	typedef shared_ptr<IDXGISwapChain>				IDXGISwapChainPtr;
	typedef shared_ptr<ID3D12Device>				ID3D12DevicePtr;
	typedef shared_ptr<ID3D12CommandQueue>			ID3D12CommandQueuePtr;
	typedef shared_ptr<ID3D12Resource>				ID3D12ResourcePtr;
	typedef shared_ptr<ID3D11On12Device>			ID3D11On12DevicePtr;
	typedef shared_ptr<ID3D11Device>				ID3D11DevicePtr;
	typedef shared_ptr<ID3D11DeviceContext>			ID3D11DeviceContextPtr;
	typedef shared_ptr<ID3D11Device1>				ID3D11Device1Ptr;
	typedef shared_ptr<ID3D11DeviceContext1>		ID3D11DeviceContext1Ptr;
	typedef shared_ptr<ID3D11Device2>				ID3D11Device2Ptr;
	typedef shared_ptr<ID3D11DeviceContext2>		ID3D11DeviceContext2Ptr;
	typedef shared_ptr<ID3D11Resource>				ID3D11ResourcePtr;
	typedef shared_ptr<ID3D11Texture1D>				ID3D11Texture1DPtr;
	typedef shared_ptr<ID3D11Texture2D>				ID3D11Texture2DPtr;
	typedef shared_ptr<ID3D11Texture3D>				ID3D11Texture3DPtr;
	typedef shared_ptr<ID3D11Texture2D>				ID3D11TextureCubePtr;
	typedef shared_ptr<ID3D11Buffer>				ID3D11BufferPtr;
	typedef shared_ptr<ID3D11InputLayout>			ID3D11InputLayoutPtr;
	typedef shared_ptr<ID3D11Query>					ID3D11QueryPtr;
	typedef shared_ptr<ID3D11Predicate>				ID3D11PredicatePtr;
	typedef shared_ptr<ID3D11VertexShader>			ID3D11VertexShaderPtr;
	typedef shared_ptr<ID3D11PixelShader>			ID3D11PixelShaderPtr;
	typedef shared_ptr<ID3D11GeometryShader>		ID3D11GeometryShaderPtr;
	typedef shared_ptr<ID3D11ComputeShader>			ID3D11ComputeShaderPtr;
	typedef shared_ptr<ID3D11HullShader>			ID3D11HullShaderPtr;
	typedef shared_ptr<ID3D11DomainShader>			ID3D11DomainShaderPtr;
	typedef shared_ptr<ID3D11RenderTargetView>		ID3D11RenderTargetViewPtr;
	typedef shared_ptr<ID3D11DepthStencilView>		ID3D11DepthStencilViewPtr;
	typedef shared_ptr<ID3D11UnorderedAccessView>	ID3D11UnorderedAccessViewPtr;
	typedef shared_ptr<ID3D11RasterizerState>		ID3D11RasterizerStatePtr;
	typedef shared_ptr<ID3D11RasterizerState1>		ID3D11RasterizerState1Ptr;
	typedef shared_ptr<ID3D11DepthStencilState>		ID3D11DepthStencilStatePtr;
	typedef shared_ptr<ID3D11BlendState>			ID3D11BlendStatePtr;
	typedef shared_ptr<ID3D11BlendState1>			ID3D11BlendState1Ptr;
	typedef shared_ptr<ID3D11SamplerState>			ID3D11SamplerStatePtr;
	typedef shared_ptr<ID3D11ShaderResourceView>	ID3D11ShaderResourceViewPtr;
}

#endif		// _D3D12TYPEDEFS_HPP
