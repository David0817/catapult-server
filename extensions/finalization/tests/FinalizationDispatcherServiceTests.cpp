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

#include "finalization/src/FinalizationDispatcherService.h"
#include "finalization/src/FinalizationConfiguration.h"
#include "tests/test/local/ServiceLocatorTestContext.h"
#include "tests/test/local/ServiceTestUtils.h"
#include "tests/TestHarness.h"

namespace catapult { namespace finalization {

#define TEST_CLASS FinalizationDispatcherServiceTests

	namespace {
		constexpr auto Num_Expected_Services = 0u;
		constexpr auto Num_Expected_Counters = 2u;
		constexpr auto Num_Expected_Tasks = 0u;

		constexpr auto Counter_Name = "FIN ELEM TOT";
		constexpr auto Active_Counter_Name = "FIN ELEM ACT";
		constexpr auto Sentinel_Counter_Value = extensions::ServiceLocator::Sentinel_Counter_Value;

		struct FinalizationDispatcherServiceTraits {
			static auto CreateRegistrar() {
				return CreateFinalizationDispatcherServiceRegistrar(FinalizationConfiguration::Uninitialized());
			}
		};

		using TestContext = test::ServiceLocatorTestContext<FinalizationDispatcherServiceTraits>;
	}

	ADD_SERVICE_REGISTRAR_INFO_TEST(FinalizationDispatcher, Post_Range_Consumers)

	TEST(TEST_CLASS, CanBootService) {
		// Arrange:
		TestContext context;

		// Act:
		context.boot();

		// Assert:
		EXPECT_EQ(Num_Expected_Services, context.locator().numServices());
		EXPECT_EQ(Num_Expected_Counters, context.locator().counters().size());
		EXPECT_EQ(Num_Expected_Tasks, context.testState().state().tasks().size());

		// - all counters should exist
		EXPECT_EQ(Sentinel_Counter_Value, context.counter(Counter_Name));
		EXPECT_EQ(Sentinel_Counter_Value, context.counter(Active_Counter_Name));
	}

	TEST(TEST_CLASS, CanShutdownService) {
		// Arrange:
		TestContext context;

		// Act:
		context.boot();
		context.shutdown();

		// Assert:
		EXPECT_EQ(Num_Expected_Services, context.locator().numServices());
		EXPECT_EQ(Num_Expected_Counters, context.locator().counters().size());
		EXPECT_EQ(Num_Expected_Tasks, context.testState().state().tasks().size());

		// - all counters should indicate shutdown
		EXPECT_EQ(Sentinel_Counter_Value, context.counter(Counter_Name));
		EXPECT_EQ(Sentinel_Counter_Value, context.counter(Active_Counter_Name));
	}
}}
