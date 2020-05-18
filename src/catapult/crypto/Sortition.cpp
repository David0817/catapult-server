/**
*** Copyright (c) 2016-present,
*** Jaguar0625, gimre, BloodyRookie, Tech Bureau, Corp. All rights reserved.
***
*** This file is part of Catapult.
***
*** Catapult is free software: you can redistribute it and/or modify
*** it under the terms of the GNU Lesser General Public License as published by
*** the Free Software Foundation, either version 3 of the License, or
*** (at your option) any later version.
***
*** Catapult is distributed in the hope that it will be useful,
*** but WITHOUT ANY WARRANTY; without even the implied warranty of
*** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*** GNU Lesser General Public License for more details.
***
*** You should have received a copy of the GNU Lesser General Public License
*** along with Catapult. If not, see <http://www.gnu.org/licenses/>.
**/

#include "Sortition.h"
#include "Vrf.h"
#include <catapult/utils/Logging.h>

#include <boost/serialization/nvp.hpp>// REMOVE

#include <boost/math/distributions/binomial.hpp>
#include <boost/math/policies/policy.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>

namespace bm = boost::math;
namespace bmp = boost::math::policies;
namespace bmu = boost::multiprecision;

namespace catapult { namespace crypto {

	using Policy = bmp::policy<bmp::discrete_quantile<bmp::integer_round_up>>;
	using Binomial = bm::binomial_distribution<double, Policy>;

	uint64_t InverseCdf(double n, double successRate, double hit) {
		// note: quantile actually returns integer value.
		return static_cast<uint64_t>(quantile(Binomial(n, successRate), hit));
	}

#ifdef _MSC_VER
#define BSWAP64(VAL) _byteswap_uint64(VAL)
#else
#define BSWAP64(VAL) __builtin_bswap64 (VAL)
#endif

	uint64_t Sortition(const Hash512& sortitionVrfHash, uint64_t tau, Amount stake, Amount totalPower) {
		// 1. calculate hit - bswap, to treat it as big-endian
		uint64_t num;
		std::memcpy(&num, sortitionVrfHash.cbegin(), 8);
		num = BSWAP64(num);
		auto hit = static_cast<double>(num) / std::pow(2.0, 64);

		// 3. calculate number of votes
		auto rate = static_cast<double>(tau) / static_cast<double>(totalPower.unwrap());
		auto numVotes = InverseCdf(static_cast<double>(stake.unwrap()), rate, hit);
		return numVotes;
	}
}}
