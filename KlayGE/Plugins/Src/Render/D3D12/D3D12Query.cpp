/**
 * @file D3D12Query.cpp
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

#include <KlayGE/D3D12/D3D12RenderEngine.hpp>
#include <KlayGE/D3D12/D3D12Query.hpp>

namespace KlayGE
{
	D3D12OcclusionQuery::D3D12OcclusionQuery()
	{
		D3D12RenderEngine const & re = *checked_cast<D3D12RenderEngine const *>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D11DevicePtr const & d3d_device = re.D3D11Device();

		D3D11_QUERY_DESC desc;
		desc.Query = D3D11_QUERY_OCCLUSION;
		desc.MiscFlags = 0;

		ID3D11Query* query;
		d3d_device->CreateQuery(&desc, &query);
		query_ = MakeCOMPtr(query);
	}

	void D3D12OcclusionQuery::Begin()
	{
		D3D12RenderEngine const & re = *checked_cast<D3D12RenderEngine const *>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D11DeviceContextPtr const & d3d_imm_ctx = re.D3D11DeviceImmContext();

		d3d_imm_ctx->Begin(query_.get());
	}

	void D3D12OcclusionQuery::End()
	{
		D3D12RenderEngine const & re = *checked_cast<D3D12RenderEngine const *>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D11DeviceContextPtr const & d3d_imm_ctx = re.D3D11DeviceImmContext();

		d3d_imm_ctx->End(query_.get());
	}

	uint64_t D3D12OcclusionQuery::SamplesPassed()
	{
		D3D12RenderEngine const & re = *checked_cast<D3D12RenderEngine const *>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D11DeviceContextPtr const & d3d_imm_ctx = re.D3D11DeviceImmContext();

		uint64_t ret;
		while (S_OK != d3d_imm_ctx->GetData(query_.get(), &ret, sizeof(ret), 0));
		return ret;
	}


	D3D12ConditionalRender::D3D12ConditionalRender()
	{
		D3D12RenderEngine const & render_eng = *checked_cast<D3D12RenderEngine const *>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D11DevicePtr const & d3d_device = render_eng.D3D11Device();

		D3D11_QUERY_DESC desc;
		desc.Query = D3D11_QUERY_OCCLUSION_PREDICATE;
		desc.MiscFlags = 0;

		ID3D11Predicate* predicate;
		d3d_device->CreatePredicate(&desc, &predicate);
		predicate_ = MakeCOMPtr(predicate);
	}

	void D3D12ConditionalRender::Begin()
	{
		D3D12RenderEngine const & re = *checked_cast<D3D12RenderEngine const *>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D11DeviceContextPtr const & d3d_imm_ctx = re.D3D11DeviceImmContext();

		d3d_imm_ctx->Begin(predicate_.get());
	}

	void D3D12ConditionalRender::End()
	{
		D3D12RenderEngine const & re = *checked_cast<D3D12RenderEngine const *>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D11DeviceContextPtr const & d3d_imm_ctx = re.D3D11DeviceImmContext();

		d3d_imm_ctx->End(predicate_.get());
	}

	void D3D12ConditionalRender::BeginConditionalRender()
	{
		D3D12RenderEngine const & re = *checked_cast<D3D12RenderEngine const *>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D11DeviceContextPtr const & d3d_imm_ctx = re.D3D11DeviceImmContext();

		d3d_imm_ctx->SetPredication(predicate_.get(), false);
	}

	void D3D12ConditionalRender::EndConditionalRender()
	{
		D3D12RenderEngine const & re = *checked_cast<D3D12RenderEngine const *>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D11DeviceContextPtr const & d3d_imm_ctx = re.D3D11DeviceImmContext();

		d3d_imm_ctx->SetPredication(nullptr, false);
	}

	bool D3D12ConditionalRender::AnySamplesPassed()
	{
		D3D12RenderEngine const & re = *checked_cast<D3D12RenderEngine const *>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D11DeviceContextPtr const & d3d_imm_ctx = re.D3D11DeviceImmContext();

		BOOL ret;
		while (S_OK != d3d_imm_ctx->GetData(predicate_.get(), &ret, sizeof(ret), 0));
		return (TRUE == ret);
	}


	D3D12TimerQuery::D3D12TimerQuery()
	{
		D3D12RenderEngine const & re = *checked_cast<D3D12RenderEngine const *>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D11DevicePtr const & d3d_device = re.D3D11Device();

		D3D11_QUERY_DESC desc;
		desc.Query = D3D11_QUERY_TIMESTAMP;
		desc.MiscFlags = 0;

		ID3D11Query* start_query;
		d3d_device->CreateQuery(&desc, &start_query);
		timestamp_start_query_ = MakeCOMPtr(start_query);

		ID3D11Query* end_query;
		d3d_device->CreateQuery(&desc, &end_query);
		timestamp_end_query_ = MakeCOMPtr(end_query);
	}

	void D3D12TimerQuery::Begin()
	{
		D3D12RenderEngine const & re = *checked_cast<D3D12RenderEngine const *>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D11DeviceContextPtr const & d3d_imm_ctx = re.D3D11DeviceImmContext();

		d3d_imm_ctx->End(timestamp_start_query_.get());
	}

	void D3D12TimerQuery::End()
	{
		D3D12RenderEngine const & re = *checked_cast<D3D12RenderEngine const *>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D11DeviceContextPtr const & d3d_imm_ctx = re.D3D11DeviceImmContext();

		d3d_imm_ctx->End(timestamp_end_query_.get());
	}

	double D3D12TimerQuery::TimeElapsed()
	{
		D3D12RenderEngine const & re = *checked_cast<D3D12RenderEngine const *>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		if (re.InvTimestampFreq() > 0)
		{
			ID3D11DeviceContextPtr const & d3d_imm_ctx = re.D3D11DeviceImmContext();

			uint64_t start_timestamp, end_timestamp;
			while (S_OK != d3d_imm_ctx->GetData(timestamp_start_query_.get(), &start_timestamp, sizeof(start_timestamp), 0));
			while (S_OK != d3d_imm_ctx->GetData(timestamp_end_query_.get(), &end_timestamp, sizeof(end_timestamp), 0));

			return (end_timestamp - start_timestamp) * re.InvTimestampFreq();
		}
		else
		{
			return -1;
		}
	}
}
