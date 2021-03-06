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

#pragma once
#include "catapult/model/ContainerTypes.h"
#include "catapult/model/NotificationPublisher.h"

namespace catapult {
	namespace model {
		struct BlockElement;
		struct TransactionElement;
	}
}

namespace catapult { namespace addressextraction {

	/// Utility class for extracting addresses.
	class AddressExtractor {
	public:
		/// Creates an extractor around \a pPublisher.
		explicit AddressExtractor(std::unique_ptr<const model::NotificationPublisher>&& pPublisher);

	public:
		/// Extracts transaction addresses into \a transactionInfo.
		void extract(model::TransactionInfo& transactionInfo) const;

		/// Extracts transaction addresses into \a transactionInfos.
		void extract(model::TransactionInfosSet& transactionInfos) const;

		/// Extracts transaction addresses into \a transactionElement.
		void extract(model::TransactionElement& transactionElement) const;

		/// Extracts transaction addresses into \a blockElement.
		void extract(model::BlockElement& blockElement) const;

	private:
		std::unique_ptr<const model::NotificationPublisher> m_pPublisher;
	};
}}
