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

#include "FinalizationDispatcherService.h"
#include "catapult/extensions/DispatcherUtils.h"

using namespace catapult::consumers;
using namespace catapult::disruptor;

namespace catapult { namespace finalization {

	namespace {
		constexpr auto Service_Name = "finalization.dispatcher";

		class FinalizationDispatcherServiceRegistrar : public extensions::ServiceRegistrar {
		public:
			extensions::ServiceRegistrarInfo info() const override {
				return { "FinalizationDispatcher", extensions::ServiceRegistrarPhase::Post_Range_Consumers };
			}

			void registerServiceCounters(extensions::ServiceLocator& locator) override {
				extensions::AddDispatcherCounters(locator, Service_Name, "FIN");
			}

			void registerServices(extensions::ServiceLocator&, extensions::ServiceState&) override {
				// TODO: placeholder
			}
		};
	}

	std::unique_ptr<extensions::ServiceRegistrar> CreateFinalizationDispatcherServiceRegistrar(const FinalizationConfiguration&) {
		return std::make_unique<FinalizationDispatcherServiceRegistrar>();
	}
}}
