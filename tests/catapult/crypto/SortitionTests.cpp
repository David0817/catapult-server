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

#include "catapult/crypto/Sortition.h"
#include "catapult/utils/Functional.h"
#include "tests/test/nodeps/KeyTestUtils.h"
#include "tests/TestHarness.h"

namespace catapult { namespace crypto {

#define TEST_CLASS SortitionTests

	TEST(TEST_CLASS, InverseCdf_RoundsResultUp) {
		// (10, p) binomial distribution
		// p = 0.3:
		//   CDF | .0282| .1493| .3828| .6496| .8497| .9527| .9894| .9984| .9999| 1.0  | 1.0  |
		//   PMF | .028 | .121 | .233 | .267 | .200 | .103 | .037 | .009 | .001 | .000 | .000 |
		// p = 0.5
		//   CDF | .0010| .0107| .0547| .1719| .3770| .6230| .8281| .9453| .9893| .9990| 1.0  |
		//   PMF | .001 | .010 | .044 | .117 | .205 | .246 | .205 | .117 | .044 | .010 | .001 |
		// p = 0.7:
		//   CDF |~.0000| .0001| .0015| .0015| .0473| .1502| .3503| .6172| .8506| .9717| 1.0  |
		//   PMF |~.0000| .0001| .001 | .009 | .036 | .010 | .200 | .266 | .233 | .121 | .028 |
		//       | 0    | 1    | 2    | 3    | 4    | 5    | 6    | 7    | 8    | 9    | 10   |
		//
		// Aligned for 0.64, p: 0.3 | 0.5 | 0.7
		//    expected results:   3 |   6 |   8

		// Act + Assert:
		EXPECT_EQ(3, InverseCdf(10, 0.3, 0.64));
		EXPECT_EQ(6, InverseCdf(10, 0.5, 0.64));
		EXPECT_EQ(8, InverseCdf(10, 0.7, 0.64));
	}

	namespace {
		// Genearate random amounts between 100-300k
		auto GenerateAmounts(size_t numAccounts) {
			std::vector<Amount> amounts;
			amounts.resize(numAccounts);

			std::generate(amounts.begin(), amounts.end(), []() {
				return Amount(2'000'000'000 + test::Random() % 1'000'000'000);
			});

			return amounts;
		}
	}

	TEST(TEST_CLASS, Sortition_Sample_Data) {
		// Arrange:
		const auto Cert_Committee_Threshold = 2117u;
		const auto Cert_Committee_Tau = 2990u;

		auto amounts = GenerateAmounts(2000);
		auto totalPower = utils::Sum(amounts, [](const auto& amount) { return amount; });

		// Act:
		auto collectedThreshold = 0u;
		auto votingAccounts = 0u;
		for (const auto& amount : amounts) {
			// for test purposes there is no need to generate vrf hash,
			// generating random hash per user is ok
			auto vrfHash = test::GenerateRandomByteArray<Hash512>();

			auto value = Sortition(vrfHash, Cert_Committee_Tau, amount, totalPower);
			collectedThreshold += value;
			if (value)
				++votingAccounts;
		}

		// Assert:
		EXPECT_GT(collectedThreshold, Cert_Committee_Threshold);
		CATAPULT_LOG(info) << "collected threshold " << collectedThreshold;
		CATAPULT_LOG(info) << "voting accounts " << votingAccounts;
	}
}}
